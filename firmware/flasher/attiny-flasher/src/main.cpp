// ArduinoISP
// Copyright (c) 2008-2011 Randall Bohn
// If you require a license, see
// http://www.opensource.org/licenses/bsd-license.php
//
// This sketch turns the Arduino into a AVRISP:

#include "main.h"
#include "logo.h"

static bool rst_active_high = true;

void setup() {
  SERIAL.begin(BAUDRATE_IN);
  #ifdef SW_SERIAL_ENABLE
  SSerial.begin(BAUDRATE_OUT);
  #endif
    
  pinMode(PIN_BUFEN, OUTPUT);
  BUFFER_OFF;

  pinMode(LED_PMODE, OUTPUT);
  pulse(LED_PMODE, 2);

  pinMode(LED_ERR, OUTPUT);
  pulse(LED_ERR, 2);

  pinMode(LED_HB, OUTPUT);
  pulse(LED_HB, 2);

  #ifdef OLED_ENABLE
  
    #ifdef OLED_LIB_ADAFRUIT
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
      SERIAL.println(F("SSD1306 allocation failed"));
        for (;;) ;
    }
    
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.cp437(true);
    #endif

    #ifdef OLED_LIB_TINY
    oled.begin(SCREEN_WIDTH, SCREEN_HEIGHT, sizeof(tiny4koled_init_128x64br), tiny4koled_init_128x64br);
    #endif

    display_logo();

    #ifdef OLED_LIB_TINY
    //oled.setFont(FONT6X8);
    oled.on();
    #endif
  #endif

  #ifdef SW_SERIAL_ENABLE
  SSerial.listen();
  #endif

  RESET_INIT;
}

int error = 0;
int pmode = 0;
int serial_bridge = 1;
// address for reading and writing, set by 'U' command
unsigned int here;
#define buff_size 128 // 256
uint8_t buff[buff_size]; // global block storage

#define beget16(addr) (*addr * 256 + *(addr+1) )

void reset_target(bool reset) {
  if (reset && rst_active_high) {
    RESET_HIGH;
  } else if (!reset && !rst_active_high) {
    RESET_HIGH;
  } else {
    RESET_LOW;
  }
}

void loop(void) {
  // is pmode active?
  if (pmode) {
    digitalWrite(LED_PMODE, HIGH);
  } else {
    digitalWrite(LED_PMODE, LOW);
  }

  // is there an error?
  if (error) {
    digitalWrite(LED_ERR, HIGH);
  } else {
    digitalWrite(LED_ERR, LOW);
  }

  // light the heartbeat LED
  digitalWrite(LED_HB, millis() % 1000 < 10 ? HIGH : LOW);

  if (SERIAL.available())
    avrisp();

  serialToScreen();
  check_logo();
}

uint8_t getch() {
  while (!SERIAL.available());
  return SERIAL.read();
}

void fill(int n) {
  for (int x = 0; x < n; x++) {
    buff[x] = getch();
  }
}

#define PTIME 30
void pulse(int pin, int times) {
  do {
    digitalWrite(pin, HIGH);
    delay(PTIME);
    digitalWrite(pin, LOW);
    delay(PTIME);
  } while (times--);
}

void prog_lamp(int state) {
  if (PROG_FLICKER) {
    digitalWrite(LED_PMODE, state);
  }
}

uint8_t spi_transaction(uint8_t a, uint8_t b, uint8_t c, uint8_t d) {
  SPI.transfer(a);
  SPI.transfer(b);
  SPI.transfer(c);
  return SPI.transfer(d);
}

void empty_reply() {
  if (CRC_EOP == getch()) {
    SERIAL.print((char)STK_INSYNC);
    SERIAL.print((char)STK_OK);
  } else {
    error++;
    SERIAL.print((char)STK_NOSYNC);
  }
}

void breply(uint8_t b) {
  if (CRC_EOP == getch()) {
    SERIAL.print((char)STK_INSYNC);
    SERIAL.print((char)b);
    SERIAL.print((char)STK_OK);
  } else {
    error++;
    SERIAL.print((char)STK_NOSYNC);
  }
}

void get_version(uint8_t c) {
  switch (c) {
    case 0x80:
      breply(HWVER);
      break;
    case 0x81:
      breply(SWMAJ);
      break;
    case 0x82:
      breply(SWMIN);
      break;
    case 0x93:
      breply('S'); // serial programmer
      break;
    default:
      breply(0);
  }
}

void set_parameters() {
  // call this after reading parameter packet into buff[]
  param.devicecode = buff[0];
  param.revision   = buff[1];
  param.progtype   = buff[2];
  param.parmode    = buff[3];
  param.polling    = buff[4];
  param.selftimed  = buff[5];
  param.lockbytes  = buff[6];
  param.fusebytes  = buff[7];
  param.flashpoll  = buff[8];
  // ignore buff[9] (= buff[8])
  // following are 16 bits (big endian)
  param.eeprompoll = beget16(&buff[10]);
  param.pagesize   = beget16(&buff[12]);
  param.eepromsize = beget16(&buff[14]);

  // 32 bits flashsize (big endian)
  param.flashsize = buff[16] * 0x01000000
                    + buff[17] * 0x00010000
                    + buff[18] * 0x00000100
                    + buff[19];

  // AVR devices have active low reset, AT89Sx are active high
  rst_active_high = (param.devicecode >= 0xe0);
}

void start_pmode() {

  // Reset target before driving PIN_SCK or PIN_MOSI
  BUFFER_ON;
  // SPI.begin() will configure SS as output, so SPI master mode is selected.
  // We have defined RESET as pin 10, which for many Arduinos is not the SS pin.
  // So we have to configure RESET as output here,
  // (reset_target() first sets the correct level)
  reset_target(true);
  /// pinMode(RESET, OUTPUT);
  SPI.begin();
  SPI.beginTransaction(SPISettings(SPI_CLOCK, MSBFIRST, SPI_MODE0));

  // See AVR datasheets, chapter "SERIAL_PRG Programming Algorithm":

  // Pulse RESET after PIN_SCK is low:
  digitalWrite(PIN_SCK, LOW);
  delay(20); // discharge PIN_SCK, value arbitrarily chosen
  reset_target(false);
  // Pulse must be minimum 2 target CPU clock cycles so 100 usec is ok for CPU
  // speeds above 20 KHz
  delayMicroseconds(100);
  reset_target(true);

  // Send the enable programming command:
  delay(50); // datasheet: must be > 20 msec
  spi_transaction(0xAC, 0x53, 0x00, 0x00);
  pmode = 1;
}

void end_pmode() {
  SPI.end();
  // We're about to take the target out of reset so configure SPI pins as input
  pinMode(PIN_MOSI, INPUT);
  pinMode(PIN_SCK, INPUT);
  reset_target(false);
  ///pinMode(RESET, INPUT);
  RESET_Z;
  pmode = 0;

  BUFFER_OFF;
}

void universal() {
  uint8_t ch;

  fill(4);
  ch = spi_transaction(buff[0], buff[1], buff[2], buff[3]);
  breply(ch);
}

void flash(uint8_t hilo, unsigned int addr, uint8_t data) {
  spi_transaction(0x40 + 8 * hilo,
                  addr >> 8 & 0xFF,
                  addr & 0xFF,
                  data);
}

void commit(unsigned int addr) {
  if (PROG_FLICKER) {
    prog_lamp(LOW);
  }
  spi_transaction(0x4C, (addr >> 8) & 0xFF, addr & 0xFF, 0);
  if (PROG_FLICKER) {
    delay(PTIME);
    prog_lamp(HIGH);
  }
}

unsigned int current_page() {
  if (param.pagesize == 32) {
    return here & 0xFFFFFFF0;
  }
  if (param.pagesize == 64) {
    return here & 0xFFFFFFE0;
  }
  if (param.pagesize == 128) {
    return here & 0xFFFFFFC0;
  }
  if (param.pagesize == 256) {
    return here & 0xFFFFFF80;
  }
  return here;
}

void write_flash(int length) {
  fill(length);
  if (CRC_EOP == getch()) {
    SERIAL.print((char) STK_INSYNC);
    SERIAL.print((char) write_flash_pages(length));
  } else {
    error++;
    SERIAL.print((char) STK_NOSYNC);
  }
}

uint8_t write_flash_pages(int length) {
  int x = 0;
  unsigned int page = current_page();
  while (x < length) {
    if (page != current_page()) {
      commit(page);
      page = current_page();
    }
    flash(LOW, here, buff[x++]);
    flash(HIGH, here, buff[x++]);
    here++;
  }

  commit(page);

  return STK_OK;
}

#define EECHUNK (32)
uint8_t write_eeprom(unsigned int length) {
  // here is a word address, get the byte address
  unsigned int start = here * 2;
  unsigned int remaining = length;
  if (length > param.eepromsize) {
    error++;
    return STK_FAILED;
  }
  while (remaining > EECHUNK) {
    write_eeprom_chunk(start, EECHUNK);
    start += EECHUNK;
    remaining -= EECHUNK;
  }
  write_eeprom_chunk(start, remaining);
  return STK_OK;
}

// write (length) bytes, (start) is a byte address
uint8_t write_eeprom_chunk(unsigned int start, unsigned int length) {
  // this writes byte-by-byte, page writing may be faster (4 bytes at a time)
  fill(length);
  prog_lamp(LOW);
  for (unsigned int x = 0; x < length; x++) {
    unsigned int addr = start + x;
    spi_transaction(0xC0, (addr >> 8) & 0xFF, addr & 0xFF, buff[x]);
    delay(45);
  }
  prog_lamp(HIGH);
  return STK_OK;
}

void program_page() {
  char result = (char) STK_FAILED;
  unsigned int length = 256 * getch();
  length += getch();
  char memtype = getch();
  // flash memory @here, (length) bytes
  if (memtype == 'F') {
    write_flash(length);
    return;
  }
  if (memtype == 'E') {
    result = (char)write_eeprom(length);
    if (CRC_EOP == getch()) {
      SERIAL.print((char) STK_INSYNC);
      SERIAL.print(result);
    } else {
      error++;
      SERIAL.print((char) STK_NOSYNC);
    }
    return;
  }
  SERIAL.print((char)STK_FAILED);
  return;
}

uint8_t flash_read(uint8_t hilo, unsigned int addr) {
  return spi_transaction(0x20 + hilo * 8,
                         (addr >> 8) & 0xFF,
                         addr & 0xFF,
                         0);
}

char flash_read_page(int length) {
  for (int x = 0; x < length; x += 2) {
    uint8_t low = flash_read(LOW, here);
    SERIAL.print((char) low);
    uint8_t high = flash_read(HIGH, here);
    SERIAL.print((char) high);
    here++;
  }
  return STK_OK;
}

char eeprom_read_page(int length) {
  // here again we have a word address
  int start = here * 2;
  for (int x = 0; x < length; x++) {
    int addr = start + x;
    uint8_t ee = spi_transaction(0xA0, (addr >> 8) & 0xFF, addr & 0xFF, 0xFF);
    SERIAL.print((char) ee);
  }
  return STK_OK;
}

void read_page() {
  char result = (char)STK_FAILED;
  int length = 256 * getch();
  length += getch();
  char memtype = getch();
  if (CRC_EOP != getch()) {
    error++;
    SERIAL.print((char) STK_NOSYNC);
    return;
  }
  SERIAL.print((char) STK_INSYNC);
  if (memtype == 'F') result = flash_read_page(length);
  if (memtype == 'E') result = eeprom_read_page(length);
  SERIAL.print(result);
}

void read_signature() {
  if (CRC_EOP != getch()) {
    error++;
    SERIAL.print((char) STK_NOSYNC);
    return;
  }
  SERIAL.print((char) STK_INSYNC);
  uint8_t high = spi_transaction(0x30, 0x00, 0x00, 0x00);
  SERIAL.print((char) high);
  uint8_t middle = spi_transaction(0x30, 0x00, 0x01, 0x00);
  SERIAL.print((char) middle);
  uint8_t low = spi_transaction(0x30, 0x00, 0x02, 0x00);
  SERIAL.print((char) low);
  SERIAL.print((char) STK_OK);
}

void avrisp() {
  uint8_t ch = getch();
  switch (ch) {
    case '0': // signon
      error = 0;
      serial_bridge = 0;
      empty_reply();
      break;
    case '1':
      if (getch() == CRC_EOP) {
        SERIAL.print((char) STK_INSYNC);
        SERIAL.print("AVR ISP");
        SERIAL.print((char) STK_OK);
      }
      else {
        error++;
        SERIAL.print((char) STK_NOSYNC);
      }
      break;
    case 'A':
      get_version(getch());
      break;
    case 'B':
      fill(20);
      set_parameters();
      empty_reply();
      break;
    case 'E': // extended parameters - ignore for now
      fill(5);
      empty_reply();
      break;
    case 'P':
      if (!pmode)
        start_pmode();
      empty_reply();
      break;
    case 'U': // set address (word)
      here = getch();
      here += 256 * getch();
      empty_reply();
      break;

    case 0x60: //STK_PROG_FLASH
      getch(); // low addr
      getch(); // high addr
      empty_reply();
      break;
    case 0x61: //STK_PROG_DATA
      getch(); // data
      empty_reply();
      break;

    case 0x64: //STK_PROG_PAGE
      program_page();
      break;

    case 0x74: //STK_READ_PAGE 't'
      read_page();
      break;

    case 'V': //0x56
      universal();
      break;
    case 'Q': //0x51
      error = 0;
      end_pmode();
      empty_reply();
      serial_bridge = 1;
      break;

    case 0x75: //STK_READ_SIGN 'u'
      read_signature();
      break;

    // expecting a command, not CRC_EOP
    // this is how we can get back in sync
    case CRC_EOP:
      error++;
      SERIAL.print((char) STK_NOSYNC);
      break;

    // anything else we will return STK_UNKNOWN
    default:
      error++;
      if (CRC_EOP == getch())
        SERIAL.print((char)STK_UNKNOWN);
      else
        SERIAL.print((char)STK_NOSYNC);
  }
}

#define bufferSize 64
uint8_t buf[bufferSize];
uint8_t i = 0;

void serialToScreen()
{
  if (pmode == 1 || !serial_bridge)
    return;

  #ifdef SW_SERIAL_ENABLE
  if (SSerial.available())
  {
    while (1)
    {
      if (SSerial.available())
      {
        buf[i] = (char)SSerial.read();
        if (i < bufferSize - 1)
          i++;
      }
      else
      {
        if (!SSerial.available())
        {
          break;
        }
      }
    }

    for (uint8_t j = 0; j < i; j++)
    {
      #ifdef SERAIL_BRIDGE_ENABLE
      if (!pmode) {
        SERIAL.print((char)buf[j]);
      }
      #endif
      
      #ifdef OLED_ENABLE
        #ifdef OLED_LIB_ADAFRUIT
          display.print((char)buf[j]);
        #endif

        #ifdef OLED_LIB_TINY
        if (buf[j] == '\n') {
          if (display.getCursorY() << 3 >= SCREEN_HEIGHT - SCREEN_ROW_HEIGHT) {
            display.clear();
            display.setCursor(0, 0);
          } else {
            display.print((char)buf[j]);  
          }
        } else {
          display.print((char)buf[j]);
        }
        #endif
        
      #endif
    }
    
    #ifdef OLED_ENABLE
      #ifdef OLED_LIB_ADAFRUIT
      display.display();
      #endif
      #ifdef OLED_LIB_TINY
      
      #endif
    #endif

    #ifdef OLED_ENABLE
      #ifdef OLED_LIB_ADAFRUIT
      if (display.getCursorY() > SCREEN_HEIGHT - SCREEN_ROW_HEIGHT) {
        display.clearDisplay();
        display.setCursor(0, 0);
      }
      #endif
    i = 0;
    #endif
  }
  #endif
}

bool logo_hidden = true;

void display_logo() {
  
  #ifdef OLED_LIB_ADAFRUIT
  display.clearDisplay();
  display.drawBitmap(0, 0, bitmap_logo, LOGO_BMPWIDTH, LOGO_BMPHEIGHT, SSD1306_WHITE);

  display.setTextSize(1);  
  display.setCursor(72, 0);
  display.print(F("ATTINY"));
  display.setCursor(72, 16);
  display.print(F("FLASHER"));
  display.setCursor(72, 32);
  display.print(F("Rev.E by"));
  display.setCursor(72, 48);
  display.print(F("SONOCOTTA"));
  display.display();
  logo_hidden = false;
  #endif

  #ifdef OLED_LIB_TINY
  display.setFont(FONT8X16);
  display.clear();
  display.bitmap(0, 0, LOGO_BMPWIDTH, 8, bitmap_logo);
  display.setCursor(72, 0);
  display.print(F("ATTINY"));
  display.setCursor(72, 2);
  display.print(F("FLASHER"));
  display.setCursor(72, 4);
  display.print(F("rv.E by"));
  display.setCursor(56, 6);
  display.print(F("SONOCOTTA"));
  display.on();
  logo_hidden = false;
  #endif 
}

void check_logo() {
  
  #ifdef OLED_LIB_ADAFRUIT
  if (!logo_hidden)
    if (millis() > LOGO_SHOW_MSEC) {
      display.clearDisplay();
      display.display();
      logo_hidden = true;
    }
  #endif 

  #ifdef OLED_LIB_TINY
  if (!logo_hidden)
    if (millis() > LOGO_SHOW_MSEC) {
      display.clear();
      logo_hidden = true;
    }
  #endif 
}