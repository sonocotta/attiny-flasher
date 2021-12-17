# ATtiny Flasher firmware

![Open Source Hardware](/doc/images/open-source-hardware-logo.png)
![Open Source Software](/doc/images/open-source-software-logo.png)

## Introduction

All repos are [Platformio](https://platformio.org/) projects, all tested under Ubuntu 20.04 LTS. 

Most of the repos will utilize multiple environments with different settings for different board revisions. Please refer to **platformio.ini** file within project folder.

For each project please find detailed description using links below

## ATtiny Flasher repos

- [attiny-flasher](/firmware/flasher/attiny-flasher) - main ATtiny Flasher firmware repo
- (WIP) [attiny-flasher-updi](/firmware/flasher/attiny-flasher-updi) - adaptation of [jtag2updi](https://github.com/ElTangas/jtag2updi) repo that can be used with Flasher to programm UPDI enabled MCUs
- (WIP) [attiny-flasher-tpi](/firmware/flasher/attiny-flasher-tpi) - TPI communication testing, requires soldering 0-Ohm resistor to be enabled on tyhe hardware level
- (DEBUGGING) [attiny-flasher-arduino-isp](/firmware/flasher/attiny-flasher-arduino-isp) - Original [ArduinoISP](https://www.arduino.cc/en/Tutorial/BuiltInExamples/ArduinoISP) repo adopted to be used with ATtiny Flasher
- (DEBUGGING) [attiny-flasher-test-reset-states](/firmware/flasher/attiny-flasher-test-reset-states) - Firmware that cycle through RESET modes (for HW testing and debuging)
- (DEBUGGING) [attiny-flasher-test-reset-fuses](/firmware/flasher/attiny-flasher-test-reset-fuses) - Firmware that clears out fuse settings on RESET-disabled Tinys (for HWSP testing and debuging)
- (DEBUGGING) [attiny-serial-demo](/firmware/flasher/attiny-serial-demo) - Demo formware for TARGET MCUs that blinks all available LEDs and sends out Serial communications (for HW testing and debuging) 
- (DEBUGGING) [attiny-serial-demo-naked](/firmware/flasher/attiny-serial-demo-naked) - same as above but Framework-less

## ATtiny Flasher Kits repos

- [attiny-bme280-vs-oled](/firmware/kits/attiny-bme280-vs-oled) - Demo firmware that reads BME280 sensor data and sends it to TINY-connected OLED screen
- [attiny-ir-messenger-send-receive](/firmware/kits/attiny-ir-messenger-send-receive) - Receive and send back IR communications (IR Messenger project Kit)
- [attiny-ir-messenger-send](/firmware/kits/attiny-ir-messenger-send) - Send IR communications (IR Messenger project Kit)
- [attiny-led-matrix-demo](/firmware/kits/attiny-led-matrix) - Demo of LED Matrix (Digital Clock kit)
- [attiny-led-matrix-game-of-life](/firmware/kits/attiny-led-matrix-game-of-life) - Demo of LED Matrix [game of life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life) (Digital Clock kit)
- [attiny-led-matrix-rtc-clock](/firmware/kits/attiny-led-matrix-rtc-clock) - RTC clock demo firmware (Digital Clock kit)
- [attiny-led-matrix-rtc-clock-set](/firmware/kits/attiny-led-matrix-rtc-clock-set) - 'Set the time' firmware (Digital Clock kit)
- [attiny-led-matrix-text](/firmware/kits/attiny-led-matrix-text) - Demo of the text mode (Digital Clock kit)
- [attiny-rainbow-led](/firmware/kits/attiny-rainbow-led) - Demo firmware of different RGB LED modes (Rainbow Led project kit)
- [attiny-rainbow-led-demo](/firmware/kits/attiny-rainbow-led-demo) - Spinning rainbow demo (Rainbow Led project kit)
- [attiny-rainbow-led-light](/firmware/kits/attiny-rainbow-led-light) - Lightweight library sample that works with Tiny13 (Rainbow Led project kit)
- [attiny-rubber-ducky-keyboard](/firmware/kits/attiny-rubber-ducky-keyboard) - Keyboard emulator demo (Rubber Ducky project kit)
- [attiny-rubber-ducky-mouse](/firmware/kits/attiny-rubber-ducky-mouse) - Mouse emulator demo  (Rubber Ducky project kit)
- [attiny-traffic-light](/firmware/kits/attiny-traffic-light) - Traffic light demo firmware (Traffic Light project kit)

## Links

- [Crowd Supply campaign](https://www.crowdsupply.com/sonocotta/attiny-flasher)
- [ATtiny Flasher - Product site](https://sonocotta.com/attiny-flasher/)
- [ATtiny Flasher Kit - Product site](https://sonocotta.com/attiny-flasher-kit/)
