#include <SPI.h>
#include "tpi.h"

// represents the current pointer register value
unsigned short adrs = 0x0000;
// type of chip connected 1 = Tiny10, 2 = Tiny20
unsigned char type;

void TPI::start()
{
    SPI.begin();
    SPI.setBitOrder(LSBFIRST);
    SPI.setDataMode(SPI_MODE0);
    SPI.setClockDivider(SPI_CLOCK_DIV32);

    // enter TPI programming mode
    RESET_LOW;

    // digitalWrite(SS, LOW); // assert RESET on tiny
    delay(1); // t_RST min = 400 ns @ Vcc = 5 V

    SPI.transfer(0xff); // activate TPI by emitting
    SPI.transfer(0xff); // 16 or more pulses on TPICLK
    SPI.transfer(0xff); // while holding TPIDATA to "1"

    writeCSS(0x02, 0x04); // TPIPCR, guard time = 8bits (default=128)

    send_skey(NVM_PROGRAM_ENABLE); // enable NVM interface
    // wait for NVM to be enabled
    while ((readCSS(0x00) & 0x02) < 1)
    {
    }

    // initialize memory pointer register
    setPointer(0x0000);

    Serial.println(F("NVM enabled"));
}

void TPI::setLockBits()
{

    Serial.print(F("Locking... Are you sure? Y/N"));
    while (Serial.available() < 1)
        ;
    char yn = Serial.read();
    if (yn == 'n' || yn == 'N')
        return;

    setPointer(0x3F00);
    writeIO(NVMCMD, NVM_WORD_WRITE);
    send_byte(SSTp);
    send_byte(0);

    send_byte(SSTp);
    send_byte(0xFF);

    while ((readIO(NVMCSR) & (1 << 7)) != 0x00)
        ;
    Serial.print(F("Locked..."));
}

//  print the register, SRAM, config and signature memory
void TPI::dumpMemory()
{
    unsigned int len;
    uint8_t i;
    // initialize memory pointer register
    setPointer(0x0000);

    Serial.println(F("Current memory state:"));
    if (type != Tiny4_5)
        len = 0x400 * type; //the memory length for a 10/20/40 is 1024/2048/4096
    else
        len = 0x200; //tiny 4/5 has 512 bytes
    len += 0x4000;

    while (adrs < len)
    {
        uint8_t b;
        // read the byte at the current pointer address
        // and increment address
        send_byte(SLDp);
        b = receive_byte(); // get data byte

        // read all the memory, but only print
        // the register, SRAM, config and signature memory
        if ((0x0000 <= adrs && adrs <= 0x005F)   // register/SRAM
            | (0x3F00 <= adrs && adrs <= 0x3F01) // NVM lock bits
            | (0x3F40 <= adrs && adrs <= 0x3F41) // config
            | (0x3F80 <= adrs && adrs <= 0x3F81) // calibration
            | (0x3FC0 <= adrs && adrs <= 0x3FC3) // ID
            | (0x4000 <= adrs && adrs <= len - 1))
        { // program
            // print +number along the top
            if ((0x00 == adrs) | (0x3f00 == adrs) // NVM lock bits
                | (0x3F40 == adrs)                // config
                | (0x3F80 == adrs)                // calibration
                | (0x3FC0 == adrs)                // ID
                | (0x4000 == adrs))
            {
                Serial.println();
                if (adrs == 0x0000)
                {
                    Serial.print(F("registers, SRAM"));
                }
                if (adrs == 0x3F00)
                {
                    Serial.print(F("NVM lock"));
                }
                if (adrs == 0x3F40)
                {
                    Serial.print(F("configuration"));
                }
                if (adrs == 0x3F80)
                {
                    Serial.print(F("calibration"));
                }
                if (adrs == 0x3FC0)
                {
                    Serial.print(F("device ID"));
                }
                if (adrs == 0x4000)
                {
                    Serial.print(F("program"));
                }
                Serial.println();
                for (i = 0; i < 5; i++)
                    Serial.print(F(" "));
                for (i = 0; i < 16; i++)
                {
                    Serial.print(F(" +"));
                    Serial.print(i, HEX);
                }
            }
            // print number on the left
            if (0 == (0x000f & adrs))
            {
                Serial.println();
                outHex(adrs, 4);
                Serial.print(F(": ")); // delimiter
            }
            outHex(b, 2);
            Serial.print(F(" "));
        }
        adrs++; // increment memory address
        if (adrs == 0x0060)
        {
            // skip reserved memory
            setPointer(0x3F00);
        }
    }
    Serial.println(F(" "));
} // end dumpMemory()

void TPI::eraseChip()
{
    // initialize memory pointer register
    setPointer(0x4001); // need the +1 for chip erase

    // erase the chip
    writeIO(NVMCMD, NVM_CHIP_ERASE);
    send_byte(SSTp);
    send_byte(0xAA);
    send_byte(SSTp);
    send_byte(0xAA);
    send_byte(SSTp);
    send_byte(0xAA);
    send_byte(SSTp);
    send_byte(0xAA);
    while ((readIO(NVMCSR) & (1 << 7)) != 0x00)
    {
        // wait for erasing to finish
    }
    Serial.println(F("chip erased"));
}

void TPI::setConfig(boolean val)
{
    uint8_t b;
    // get current config byte
    setPointer(0x3F40);
    send_byte(SLD);
    b = receive_byte();

    Serial.println(F("input one of these letters"));
    Serial.println(F("c = system clock output"));
    Serial.println(F("w = watchdog timer on"));
    Serial.println(F("r = disable reset"));
    Serial.println(F("x = cancel. don't change anything"));

    while (Serial.available() < 1)
    {
        // wait
    }
    char comnd = Serial.read();
    setPointer(0x3F40);
    writeIO(NVMCMD, (val ? NVM_WORD_WRITE : NVM_SECTION_ERASE));

    if (comnd == 'c')
    {
        send_byte(SSTp);
        if (val)
        {
            send_byte(b & 0b11111011);
        }
        else
        {
            send_byte(b | 0x04);
        }
        send_byte(SSTp);
        send_byte(0xFF);
    }
    else if (comnd == 'w')
    {
        send_byte(SSTp);
        if (val)
        {
            send_byte(b & 0b11111101);
        }
        else
        {
            send_byte(b | 0x02);
        }
        send_byte(SSTp);
        send_byte(0xFF);
    }
    else if (comnd == 'r')
    {
        send_byte(SSTp);
        if (val)
        {
            send_byte(b & 0b11111110);
        }
        else
        {
            send_byte(b | 0x01);
        }
        send_byte(SSTp);
        send_byte(0xFF);
    }
    else if (comnd == 'x')
    {
        // do nothing
    }
    else
    {
        Serial.println(F("received unknown command. Cancelling"));
    }
    while ((readIO(NVMCSR) & (1 << 7)) != 0x00)
    {
        // wait for write to finish
    }
    writeIO(NVMCMD, NVM_NOP);
    SPI.transfer(0xff);
    SPI.transfer(0xff);
    if (comnd != 'x')
    {

        Serial.print(F("\n\nSuccessfully "));
        if (val)
            Serial.print(F("Set "));
        else
            Serial.print(F("Cleared "));

        Serial.print(F("\""));
        if (comnd == 'w')
            Serial.print(F("Watchdog"));
        else if (comnd == 'c')
            Serial.print(F("Clock Output"));
        else if (comnd == 'r')
            Serial.print(F("Reset"));

        Serial.println(F("\" Flag\n"));
    }
}

void TPI::checkID()
{
    // check the device ID
    uint8_t id1, id2, id3;
    setPointer(0x3FC0);

    send_byte(SLDp);
    id1 = receive_byte();
    send_byte(SLDp);
    id2 = receive_byte();
    send_byte(SLDp);
    id3 = receive_byte();
    if (id1 == 0x1E && id2 == 0x8F && id3 == 0x0A)
    {
        Serial.print(F("ATtiny4"));
        type = Tiny4_5;
    }
    else if (id1 == 0x1E && id2 == 0x8F && id3 == 0x09)
    {
        Serial.print(F("ATtiny5"));
        type = Tiny4_5;
    }
    else if (id1 == 0x1E && id2 == 0x90 && id3 == 0x08)
    {
        Serial.print(F("ATtiny9"));
        type = Tiny9;
    }
    else if (id1 == 0x1E && id2 == 0x90 && id3 == 0x03)
    {
        Serial.print(F("ATtiny10"));
        type = Tiny10;
    }
    else if (id1 == 0x1E && id2 == 0x91 && id3 == 0x0f)
    {
        Serial.print(F("ATtiny20"));
        type = Tiny20;
    }
    else if (id1 == 0x1E && id2 == 0x92 && id3 == 0x0e)
    {
        Serial.print(F("ATtiny40"));
        type = Tiny40;
    }
    else
    {
        Serial.print(F("Unknown chip"));
    }
    Serial.println(F(" connected"));
}

// receive and translate the contents of a hex file, Program and verify on the fly
// boolean TPI::writeProgram()
// {
//     boolean correct;
//     unsigned int progSize = 0; //program size in bytes
//     char datlength[] = "00";
//     char addr[] = "0000";
//     char something[] = "00";
//     char chksm[] = "00";
//     unsigned int currentByte = 0;
//     progSize = 0;
//     uint8_t linelength = 0;
//     boolean fileEnd = false;
//     unsigned short tadrs;
//     tadrs = adrs = 0x4000;
//     correct = true;
//     unsigned long pgmStartTime = millis();
//     TPI::eraseChip(); // erase chip
//     unsigned char words = (type != Tiny4_5 ? type : 1);
    
//     char b1, b2;
//     // read in the data and
//     while (!fileEnd)
//     {
//         startTime = millis();
//         while (Serial.available() < 1)
//         {
//             if (millis() - startTime > timeout)
//             {
//                 ERROR_data(TimeOut);
//                 return false;
//             }
//             if (pgmStartTime == 0)
//                 pgmStartTime = millis();
//         }

//         if (Sread() != ':')
//         { // maybe it was a newline??
//             if (Sread() != ':')
//             {
//                 ERROR_data(HexError);
//                 return false;
//             }
//         }
//         // read data length

//         datlength[0] = Sread();
//         datlength[1] = Sread();
//         linelength = byteval(datlength[0], datlength[1]);

//         // read address. if "0000" currentByte = 0
//         addr[0] = Sread();
//         addr[1] = Sread();
//         addr[2] = Sread();
//         addr[3] = Sread();
//         if (linelength != 0x00 && addr[0] == '0' && addr[1] == '0' && addr[2] == '0' && addr[3] == '0')
//             currentByte = 0;

//         // read type thingy. "01" means end of file
//         something[0] = Sread();
//         something[1] = Sread();
//         if (something[1] == '1')
//         {
//             fileEnd = true;
//         }

//         if (something[1] == '2')
//         {
//             for (int i = 0; i <= linelength; i++)
//             {
//                 Sread();
//                 Sread();
//             }
//         }
//         else
//         {
//             // read in the data
//             for (int k = 0; k < linelength; k++)
//             {
//                 while (Serial.available() < 1)
//                 {
//                     if (millis() - startTime > timeout)
//                     {
//                         ERROR_data(TimeOut);
//                         return false;
//                     }
//                 }
//                 b1 = Sread();
//                 b2 = Sread();
//                 data[currentByte] = byteval(b1, b2);
//                 currentByte++;
//                 progSize++;
//                 if (progSize > (type != Tiny4_5 ? type * 1024 : 512))
//                 {
//                     ERROR_data(TooLarge);
//                     return 0;
//                 }

//                 if (fileEnd) //has the end of the file been reached?
//                     while (currentByte < 2 * words)
//                     { // append zeros to align the word count to program
//                         data[currentByte] = 0;
//                         currentByte++;
//                     }

//                 if (currentByte == 2 * words)
//                 { // is the word/Dword/Qword here?

//                     currentByte = 0;   // yes, reset counter
//                     setPointer(tadrs); // point to the address to program
//                     writeIO(NVMCMD, NVM_WORD_WRITE);
//                     for (int i = 0; i < 2 * words; i += 2)
//                     { // loop for each word size depending on micro

//                         // now write a word to program memory
//                         send_byte(SSTp);
//                         send_byte(data[i]); // LSB first
//                         send_byte(SSTp);
//                         send_byte(data[i + 1]); // then MSB
//                         SPI.transfer(0xff);         //send idle between words
//                         SPI.transfer(0xff);         //send idle between words
//                     }
//                     while ((readIO(NVMCSR) & (1 << 7)) != 0x00)
//                     {
//                     } // wait for write to finish

//                     writeIO(NVMCMD, NVM_NOP);
//                     SPI.transfer(0xff);
//                     SPI.transfer(0xff);

//                     //verify written words
//                     setPointer(tadrs);
//                     for (int c = 0; c < 2 * words; c++)
//                     {
//                         send_byte(SLDp);
//                         b = receive_byte(); // get data byte

//                         if (b != data[c])
//                         {
//                             correct = false;
//                             Serial.println(F("program error:"));
//                             Serial.print(F("byte "));
//                             outHex(adrs, 4);
//                             Serial.print(F(" expected "));
//                             outHex(data[c], 2);
//                             Serial.print(F(" read "));
//                             outHex(b, 2);
//                             Serial.println();

//                             if (!correct)
//                                 return false;
//                         }
//                     }
//                     tadrs += 2 * words;
//                 }
//             }

//             // read in the checksum.
//             startTime = millis();
//             while (Serial.available() == 0)
//             {
//                 if (millis() - startTime > timeout)
//                 {
//                     ERROR_data(TimeOut);
//                     return false;
//                 }
//             }
//             chksm[0] = Sread();
//             chksm[1] = Sread();
//         }
//     }
//     // the program was successfully written
//     Serial.print(F("Successfully wrote program: "));
//     Serial.print(progSize, DEC);
//     Serial.print(F(" of "));
//     if (type != Tiny4_5)
//         Serial.print(1024 * type, DEC);
//     else
//         Serial.print(512, DEC);
//     Serial.print(F(" bytes\n in "));
//     Serial.print((millis() - pgmStartTime) / 1000.0, DEC);
//     Serial.print(F(" Seconds"));
//     //  digitalWrite(SS, HIGH); // release RESET

//     return true;
// }

void TPI::finish()
{
    writeCSS(0x00, 0x00);
    SPI.transfer(0xff);
    SPI.transfer(0xff);
    RESET_HIGH;
    //  digitalWrite(SS, HIGH); // release RESET
    delay(1); // t_RST min = 400 ns @ Vcc = 5 V
    SPI.end();
    DDRB &= 0b11000011; //tri-state spi so target can be tested
    PORTB &= 0b11000011;
}

// writes to CSS
void TPI::writeCSS(uint8_t address, uint8_t value)
{
    send_byte(0xC0 | address);
    send_byte(value);
}

// reads from CSS
uint8_t TPI::readCSS(uint8_t address)
{
    send_byte(0x80 | address);
    return receive_byte();
}

// send the 64 bit NVM key
void TPI::send_skey(uint64_t nvm_key)
{
    send_byte(SKEY);
    while (nvm_key)
    {
        send_byte(nvm_key & 0xFF);
        nvm_key >>= 8;
    }
}

/*
* send a byte in one TPI frame (12 bits)
* (1 start + 8 data + 1 parity + 2 stop)
* using 2 SPI data bytes (2 x 8 = 16 clocks)
* (with 4 extra idle bits)
*/
void TPI::send_byte(uint8_t data)
{
    // compute partiy bit
    uint8_t par = data;
    par ^= (par >> 4); // b[7:4] (+) b[3:0]
    par ^= (par >> 2); // b[3:2] (+) b[1:0]
    par ^= (par >> 1); // b[1] (+) b[0]

    // REMEMBER: this is in LSBfirst mode and idle is high
    // (2 idle) + (1 start bit) + (data[4:0])
    SPI.transfer(0x03 | (data << 3));
    // (data[7:5]) + (1 parity) + (2 stop bits) + (2 idle)
    SPI.transfer(0xf0 | (par << 3) | (data >> 5));
} // end send_byte()

/*
* receive TPI 12-bit format byte data
* via SPI 2 bytes (16 clocks) or 3 bytes (24 clocks)
*/
uint8_t TPI::receive_byte(void)
{
    uint8_t b1, b2, b3;
    // keep transmitting high(idle) while waiting for a start bit
    do
    {
        b1 = SPI.transfer(0xff);
    } while (0xff == b1);
    // get (partial) data bits
    b2 = SPI.transfer(0xff);
    // if the first byte(b1) contains less than 4 data bits
    // we need to get a third byte to get the parity and stop bits
    if (0x0f == (0x0f & b1))
    {
        b3 = SPI.transfer(0xff);
    }

    // now shift the bits into the right positions
    // b1 should hold only idle and start bits = 0b01111111
    while (0x7f != b1)
    {             // data not aligned
        b2 <<= 1; // shift left data bits
        if (0x80 & b1)
        {            // carry from 1st byte
            b2 |= 1; // set bit
        }
        b1 <<= 1;
        b1 |= 0x01; // fill with idle bit (1)
    }
    // now the data byte is stored in b2
    return (b2);
} // end receive_byte()

// writes using SOUT
void TPI::writeIO(uint8_t address, uint8_t value)
{
    //  SOUT 0b1aa1aaaa replace a with 6 address bits
    send_byte(0x90 | (address & 0x0F) | ((address & 0x30) << 1));
    send_byte(value);
}

// reads using SIN
uint8_t TPI::readIO(uint8_t address)
{
    //  SIN 0b0aa1aaaa replace a with 6 address bits
    send_byte(0x10 | (address & 0x0F) | ((address & 0x30) << 1));
    return receive_byte();
}

// sets the pointer address
void TPI::setPointer(unsigned short address)
{
    adrs = address;
    send_byte(SSTPRL);
    send_byte(address & 0xff);
    send_byte(SSTPRH);
    send_byte((address >> 8) & 0xff);
}

void TPI::outHex(unsigned int n, char l)
{ // call with the number to be printed, and # of nibbles expected.
    for (char count = l - 1; count > 0; count--)
    {                                         // quick and dirty to add zeros to the hex value
        if (((n >> (count * 4)) & 0x0f) == 0) // if MSB is 0
            Serial.print(F("0"));             //prepend a 0
        else
            break; //exit the for loop
    }
    Serial.print(n, HEX);
}
