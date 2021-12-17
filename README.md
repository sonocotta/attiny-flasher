# ATtiny Flasher and ATtiny Flasher Kits project files

![Open Source Hardware](/doc/images/open-source-hardware-logo.png)
![Open Source Software](/doc/images/open-source-software-logo.png)

ATtiny Flasher is a handy device that allows you to use your favorite IDEs and debugging tools when developing for the ATtiny microcontroller. Enjoy the classic Arduino development experience while working with bare metal. Whether you’re a design engineer, a hobbyist, or someone who’s just getting started programming MCUs, ATtiny Flasher can help you get from a great idea to a great implementation—without having to port your code to its intended platform when you’re done

After implementing numerous projects using the wonderful ATtiny family of MCUs, we’ve come away with a number of ideas about how to simplify the process. Our goal is to make ATtiny chips as easy to work with as more "grown up" MCUs:
- We want a traditional serial terminal for debugging. Ideally, we want two—one via serial data transferred to the host over USB, and another via the onboard display.
- We want some GPIO LEDs to play with. In fact, why not have LEDs for all GPIOs?
- Speaking of GPIOs, we want to use the RESET pin as a GPIO too, so we'll need an on-board high-voltage programmer.
- In general, we want to upload code to breadboard-connected devices and see that code in action immediately.
- And finally, since our debugger will be connected to a PC via USB, it should handle 3.3 V and 5 V breadboard power rails as well

## How to use it

### Using onboard socket

![image](https://user-images.githubusercontent.com/5459747/145794310-4a41d491-20d0-4a6e-aec5-e476e50f1972.png)
- Tiny85 to be programmed ![image](https://user-images.githubusercontent.com/5459747/145794452-28c62861-e127-4d99-9208-2541a7b45062.png)
- Breadboard lines connected to Tiny85 pins ![image](https://user-images.githubusercontent.com/5459747/145794470-71d8ed3d-89c2-4ec9-a671-21e9bb452a44.png)
- LEDs connected to Tiny85 pins ![image](https://user-images.githubusercontent.com/5459747/145794488-3a0da93e-0e51-42a2-ac87-59283e536fa3.png)
- Tiny85 Serial monitor displayed on LED as well as sent to host PC ![image](https://user-images.githubusercontent.com/5459747/145794656-6752d589-8a13-49ae-be51-29bedecc82eb.png)
- Host MCU connection to PC ![image](https://user-images.githubusercontent.com/5459747/145794529-9359fdc7-d663-4259-9b59-2cdfbd5ad2c9.png)

### Using breadboard

![image](https://user-images.githubusercontent.com/5459747/145795092-5d8e3d47-a672-4102-960f-d47f2bbc278a.png)
- Tiny85 to be programmed ![image](https://user-images.githubusercontent.com/5459747/145794452-28c62861-e127-4d99-9208-2541a7b45062.png)
- Breadboard lines connected to Tiny85 pins ![image](https://user-images.githubusercontent.com/5459747/145794470-71d8ed3d-89c2-4ec9-a671-21e9bb452a44.png)
- LEDs connected to Tiny85 pins ![image](https://user-images.githubusercontent.com/5459747/145794488-3a0da93e-0e51-42a2-ac87-59283e536fa3.png)
- Tiny85 Serial monitor displayed on LED as well as sent to host PC ![image](https://user-images.githubusercontent.com/5459747/145794656-6752d589-8a13-49ae-be51-29bedecc82eb.png)
- Host MCU connection to PC ![image](https://user-images.githubusercontent.com/5459747/145794529-9359fdc7-d663-4259-9b59-2cdfbd5ad2c9.png)

### Using dedicated ATtiny Flasher Kits

![image](https://user-images.githubusercontent.com/5459747/145795387-35692cc8-04b7-4e2f-94a4-4cc9e8997996.png)
- Tiny85 to be programmed ![image](https://user-images.githubusercontent.com/5459747/145794452-28c62861-e127-4d99-9208-2541a7b45062.png)
- Kit components (addressable leds and 2 push buttons) ![image](https://user-images.githubusercontent.com/5459747/145795479-d2dde429-0848-4ffb-8ffa-03a7a626c1ca.png)
- Breadboard lines connected to Tiny85 pins ![image](https://user-images.githubusercontent.com/5459747/145794470-71d8ed3d-89c2-4ec9-a671-21e9bb452a44.png)
- LEDs connected to Tiny85 pins ![image](https://user-images.githubusercontent.com/5459747/145794488-3a0da93e-0e51-42a2-ac87-59283e536fa3.png)
- Tiny85 Serial monitor displayed on LED as well as sent to host PC ![image](https://user-images.githubusercontent.com/5459747/145794656-6752d589-8a13-49ae-be51-29bedecc82eb.png)
- Host MCU connection to PC ![image](https://user-images.githubusercontent.com/5459747/145794529-9359fdc7-d663-4259-9b59-2cdfbd5ad2c9.png)

### Standalone, using onboard ISP Header 
![image](https://user-images.githubusercontent.com/5459747/145794841-b2c4e7b8-c874-4d39-adbe-7e683b24a473.png)
- Use ATTiny Flasher like regular ISP Programmer

## ATTiny Flasher Features

- Flash ATtiny85, ATtiny45, ATtiny25, and ATtiny13 via breadboard or onboard header
- Flash the entire Atmega MCU family using the ISP header
- Flash using Arduino IDE, Platformio, or the IDE of your choice
- Two configurable power rails, either 3.3 V or 5 V
- Supports high-voltage programming
- Supports UPDI programming with dedicated header
- Supports TPI programming (after certain HW modifications)
- Six GPIO "weak" LEDs
- Three flash-status LEDs
- Onboard OLED screen connected to target MCU as a live terminal
- Streams serial data to the host PC as if it were connected directly to the target MCU. (Yes, serial on ATtiny. Even ATtiny13!)
- A buffer IC between target and host MCUs only connects the two while flashing
- Open hardware driven by open source software

## Educational ATtiny Project Kits

Having built a product that ticks all of those boxes, we wanted to put ATtiny through its paces, so we designed a collection of simple, affordable, breadboard-compatible project kits that are easy to understand, fun to build, satisfying to use, and well suited to extension and modification. There are five of them in all, and they are a great way to learn about ATtiny programming:

### Traffic Light: ATtiny85-driven "traffic light"

![image](https://user-images.githubusercontent.com/5459747/145796683-79122025-ec81-48d7-9f73-33477eac7129.png)

- Onboard ATTiny85 ![image](https://user-images.githubusercontent.com/5459747/145796865-d5a6c178-560d-44e9-987f-3b413a4d1c9d.png)
- 3 bright LEDs: green, yellow, red (PWM controllable) ![image](https://user-images.githubusercontent.com/5459747/145796892-a14c619c-7834-4a60-9213-c4971d18646f.png)
- User-programmable button ![image](https://user-images.githubusercontent.com/5459747/145796912-b3854fe7-b8bb-4fbb-8795-e396ad15a813.png)
- External input or output ![image](https://user-images.githubusercontent.com/5459747/145796941-0d2c1932-92d3-4145-9b01-5af8f0ee9c48.png)

### Rubber Ducky: ATtiny85-driven "USB rubber ducky" tool capable of emulating keyboard & mouse devices

![image](https://user-images.githubusercontent.com/5459747/145797260-2f965c6e-cff2-4b7d-b6cc-6f5a37b34abe.png)

- Onboard ATTiny85 ![image](https://user-images.githubusercontent.com/5459747/145796865-d5a6c178-560d-44e9-987f-3b413a4d1c9d.png)
- 3 user-programmable buttons ![image](https://user-images.githubusercontent.com/5459747/145796912-b3854fe7-b8bb-4fbb-8795-e396ad15a813.png)
- USB connector with possibility act as PC keyboard or mouse ![image](https://user-images.githubusercontent.com/5459747/145796941-0d2c1932-92d3-4145-9b01-5af8f0ee9c48.png)

### IR Messenger: ATtiny85-driven IR remote transmitter & receiver

![image](https://user-images.githubusercontent.com/5459747/145797635-f74de5e0-80a9-4919-959b-2ff164ff9299.png)

- Onboard ATTiny85 ![image](https://user-images.githubusercontent.com/5459747/145796865-d5a6c178-560d-44e9-987f-3b413a4d1c9d.png)
- 3 user-programmable buttons ![image](https://user-images.githubusercontent.com/5459747/145796912-b3854fe7-b8bb-4fbb-8795-e396ad15a813.png)
- IR recevier (940nm) ![image](https://user-images.githubusercontent.com/5459747/145796941-0d2c1932-92d3-4145-9b01-5af8f0ee9c48.png)
- IR sender (dual 940nm IR LED) ![image](https://user-images.githubusercontent.com/5459747/145797786-d6326a19-1ce1-4668-8aad-e209e317f274.png)

### Rainbow LED: ATtiny85-driven circle of addressable LEDs

![image](https://user-images.githubusercontent.com/5459747/145797962-efee1b16-2bdd-4cac-9596-0881f9976e6b.png)

- Onboard ATTiny85 ![image](https://user-images.githubusercontent.com/5459747/145796865-d5a6c178-560d-44e9-987f-3b413a4d1c9d.png)
- 2 user-programmable buttons ![image](https://user-images.githubusercontent.com/5459747/145796912-b3854fe7-b8bb-4fbb-8795-e396ad15a813.png)
- 8 individually addressable LEDs (WS2820) ![image](https://user-images.githubusercontent.com/5459747/145796941-0d2c1932-92d3-4145-9b01-5af8f0ee9c48.png)
- External input or output ![image](https://user-images.githubusercontent.com/5459747/145797786-d6326a19-1ce1-4668-8aad-e209e317f274.png)

### Digital Clock: ATtiny85-driven digital clock

![image](https://user-images.githubusercontent.com/5459747/145798488-0826000f-f39c-4c39-b183-5cd802265cf4.png)
- Onboard ATTiny85 ![image](https://user-images.githubusercontent.com/5459747/145796865-d5a6c178-560d-44e9-987f-3b413a4d1c9d.png)
- 2 user-programmable buttons ![image](https://user-images.githubusercontent.com/5459747/145796912-b3854fe7-b8bb-4fbb-8795-e396ad15a813.png)
- SPI RTC clock (DS1302) with CR1220 battery ![image](https://user-images.githubusercontent.com/5459747/145798700-1908f86c-6ebe-497f-a6d1-646ad6136e34.png)
- 128 individually controllable LEDs (forms matrix, stacks on top) ![image](https://user-images.githubusercontent.com/5459747/145798776-0b706323-819d-4981-a2d9-117fb646b131.png)

## Hardware

Please find more information in the [hardware](/hardware) section

## Firmware

Please find more information in the [software](/hardware) section

## Where to buy

Shortly will be available to order from [Mouser](https://mouser.com).

Later on will be shared on [Tindie](https://www.tindie.com/) store.

## Links

- [Crowd Supply campaign](https://www.crowdsupply.com/sonocotta/attiny-flasher)
- [ATtiny Flasher - Product site](https://sonocotta.com/attiny-flasher/)
- [ATtiny Flasher Kit - Product site](https://sonocotta.com/attiny-flasher-kit/)
