# ATtiny Flasher and ATtiny Flasher Kits hardware 

![Open Source Hardware](/doc/images/open-source-hardware-logo.png)
![Open Source Software](/doc/images/open-source-software-logo.png)

This is hardware design files for ATtiny Flasher project

![image](https://user-images.githubusercontent.com/5459747/144618748-68b3735c-2a7d-4506-89f1-b8aede229ed4.png)

## Features

![image](https://user-images.githubusercontent.com/5459747/144621288-b737dde0-0a01-4a6c-9c53-77841b7465e4.png)

1. Host MCU - Atmega328P ![image](https://user-images.githubusercontent.com/5459747/144620501-3eebf87d-a2e3-4973-979c-14a5235b6e9d.png)
2. Host RESET button ![image](https://user-images.githubusercontent.com/5459747/144620579-f351031a-4d71-4658-b6de-6a8b4fa7adbd.png)
3. Flashing status indicators ![image](https://user-images.githubusercontent.com/5459747/144620630-7254bd69-e4e1-44ad-8c67-ce9246c346d1.png)
4. Host USB port - for flashing both TARGET and HOST MCUs ![image](https://user-images.githubusercontent.com/5459747/144620654-f74ba596-8e9c-4c00-a473-980286ec46ca.png)
5. Host ISP header - for direct hardware flashing ![image](https://user-images.githubusercontent.com/5459747/144620693-db325348-32a3-4cdc-a829-4776d43fbdd9.png)
6. OLED screen - connected to Host, when used with stock firmware will output TARGET serial data ![image](https://user-images.githubusercontent.com/5459747/144620734-1d3f31e1-3e3d-44f0-a6ca-3ded4d74dbac.png)
7. Programming mode switches ![image](https://user-images.githubusercontent.com/5459747/144620763-7862cfda-a0ef-4106-bcda-344fa1addb71.png)
8. Target ISP flashing header. Compartible with Atmega and ATTiny Family MCUs ![image](https://user-images.githubusercontent.com/5459747/144620789-f467a5af-4967-4e95-8672-46a64e37ee50.png)
9. Target socket - alternative to flashing within breadboard. Compartible with Tinyx5 and Tiny13 series. ![image](https://user-images.githubusercontent.com/5459747/144620807-819f44b0-f6bb-4140-9323-e2c2a7acecb6.png)
10. Power Rails and voltage selectors ![image](https://user-images.githubusercontent.com/5459747/144620828-ca83b515-78db-4285-afc8-d21dff26b321.png)
11. TARGET port LED indicators ![image](https://user-images.githubusercontent.com/5459747/144620918-e896c50e-8a21-4bfe-b8f0-9d55a106833a.png)
12. TARGET breadboard connectors ![image](https://user-images.githubusercontent.com/5459747/144620955-2956ef2e-7ad2-4276-a696-9ef467ca4d5d.png)

## Mode Switches

![image](https://user-images.githubusercontent.com/5459747/144621344-74cea5d4-79c8-4450-965c-38f512137482.png)

| Num | Function | ON | OFF |
|----|----|----|----|
| 1 | Serial Bridge between Host and Target. When enabled Serial Data from TARGET MCU will be captured on the HOST MCU (115200 baud rate by default). It will be displayed on the OLED screen, also translated to PC via on-boarde Serial-to-USB chip. When enalbed it will pullup PB3 and PB4 ports on the TARGET, therefore might be required to be off | Enabled | Disabled |
| 2 | High voltage programming. When ON it will use built in 12V boost converter to reset HOST MCU into high voltage programming mode. Using this method you can use PB5 port of ATTINY (normally not usable), however make sure that other components of your device can handle 12V during flashing process. | Low voltage, green status LED blink slowly. | High voltage, green status LED blink rapidly |
| 3 | Programming target. When ON incoming Serial data will be interpretet as STK500 commands and translatred accordingly to TARGET MCU. When OFF it will allow to flash HOST MCU using Arduino bootloader (in other words like normal Atmega) | Flash HOST | Flash TARGET |


## Board Revisions

This comes in multiple revisions, normally revision of the specific board will be clearly visible on the back side of the PCB.

![image](https://user-images.githubusercontent.com/5459747/144602248-bd2b6d16-f4cc-40ab-b462-3590b953f42f.png)

Customers that have participated in the [Crowd Supply campaign](https://www.crowdsupply.com/sonocotta/attiny-flasher)

Current distribution of board revisions looks like this

| Revision | ATtiny Flasher | Revision | ATtiny Flasher Kits  |
|----|----|----|----|
| A | First idea implementation <br /> ![image](https://user-images.githubusercontent.com/5459747/144602782-9e941118-c013-4089-91c4-6486791c099f.png) | | |
| B | Early production run, first one that is breadboard compatible <br /> ![image](https://user-images.githubusercontent.com/5459747/144602972-c0f9ad33-1639-4d04-9ae2-39565d194542.png) | | |
| C | Version which started Crowd Supply campaign, still lacking many features however <br /> ![image](https://user-images.githubusercontent.com/5459747/144603188-4f347438-0aa2-4e5a-9e6b-6ba8fc3b09ea.png) | A | First run of the Kits <br /> ![image](https://user-images.githubusercontent.com/5459747/144603445-d9c43ef4-f532-412a-bbb9-6d58c0400823.png) |
| D | Deep revision of the board with High Voltage programming mode added, however not without mistakes :) <br /> ![image](https://user-images.githubusercontent.com/5459747/144603551-806e602c-6c21-4f8f-b07d-2bd1e7f7b7cc.png) | | |
| E | Home work was done to make HVSP work, few improvements <br /> ![image](https://user-images.githubusercontent.com/5459747/144603716-3f641c8f-f801-48f6-8c3e-dd20a97f6272.png) |  | |
| F | Pre-production version of the board, first having UPDI and TPI hardware support. <br /> ![image](https://user-images.githubusercontent.com/5459747/144605016-f7bcbdd5-9535-4958-b31f-0bd34779a3a9.png) | B | Fixups after the first run validation <br /> ![image](https://user-images.githubusercontent.com/5459747/144605058-0ced99ef-341b-4db1-baac-79fab3ce00fe.png) |
| G | **First batch of boards delivered to backers**. Tested with both UPDI and TPI, however require soldering to run in TPI mode. <br /> ![image](https://user-images.githubusercontent.com/5459747/144605492-b99d7d6a-2a99-4355-9d19-97316e5c4ad2.png) | C | **First batch of boards delivered to backers**. One issue is still present on the board, that was fixed using bodge resistor <br /> ![image](https://user-images.githubusercontent.com/5459747/144605459-64c80f76-ce4d-4cde-99bc-5a9d79df57d0.png) |
| H | Work in progress. Had to replace CH340E chip with CH340G due to inavailability of the first. **Second batch of boards delivered to backers** | D | Work in progress. Issue with Clock kit was found and fixed |
| J | Work in progress. Based on ATMega32u4 host MCU. | | 

## Pin Definitions

### ATtiny Flasher

Generally pins will differ slightly from revision to revision, and will be defined in [boards.h](/firmware/flasher/attiny-flasher/src/boards.h) file Most essential ones are listed below

| PIN DEF | IO | G, H | J
|----|----|----|----|
| OLED SDA,SCL | IO | PC4,PC5 | PD1,PD0 |
| HOST USB | IO | NA | D+,D- |
| HOST SERIAL | IO | PD0,PD1 | NA | 
| TINY SERIAL TX,RX | IO | PD4,PD5 | PD3,PD3 |
| SPI (TINY PROGRAMMING, SELF PROGRAMMING) CLK,MOSI,MISO | IO | PB5,PB3,PB4 | PB1,PB2,PB3 |
| +12V RESET ENABLE | IO | PD2 | PF1
| TARGET RESET PULL UP,DOWN | O | PB2,PC2 | PF0,PF4
| BUFFER ENABLE | O | PC1 (HIGH for ISP, LOW for HVSP) | PB7 (ISP), PE6 (HVSP) |
| STATUS LEDs | O | PD7,PB0,PB1 | NA |
| RGB LED | O | NA | PC6 |
| UPDI IO | IO | PD6 | PD6 | 
| SERIAL ENABLE INPUT | I | PC3 | PF5 

### ATtiny Flasher Kits 

Each usable pin is subscribed directly on the PCB, like on exmaple below

![image](https://user-images.githubusercontent.com/5459747/145799344-505a6c2e-4c5e-4924-b312-86ae6cd9acf6.png)

## Links

- [Crowd Supply campaign](https://www.crowdsupply.com/sonocotta/attiny-flasher)
- [ATtiny Flasher - Product site](https://sonocotta.com/attiny-flasher/)
- [ATtiny Flasher Kit - Product site](https://sonocotta.com/attiny-flasher-kit/)
