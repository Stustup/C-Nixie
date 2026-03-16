<# CNIX - Nixie Tube Clock ->WORK IN PROGRESS

CNIX is a nixie tube clock which accepts many diffrent nixie tubes with only slight modifications to the tube carrier board. 

![Nixie Clock tube board](./Docs/Nixie%20Clock%20combined%203D.png)

The motivation was, that there are not many clocks out there that let you use almost any nixie tube you like. Most have fixed power supplies, 
underdesigned driver stages or inflexible microcontrollers. I hope to tackle these problems with an adjustable boost converter PSU, extremely 
flexible (and electrically protected) driver stages and an open source firmware written with free tools.

## Schematic

The control board consists of 3 parts:
- Power Supply
- Microcontroller
- I/O
- GPIO header for future ideas and feature expansion

![Control board schematic](./Docs/CNIX%20V2%20control%20board%20schematic.png)

Power comes from a simple USB-C plug configured for 5V 3A max with the 5k1 CC line resistors. I thought about adding a PD controller for higher input voltages,
but this would just have added complexity and locked you into more expensive USB PD power supplies.

The µC is a STM32G051, because it has an included RTC *with* added Vbat pin for an external battery, so that the RTC remembers time even when powered off. 
The unused GPIOs are broken out to a connector, so that in the future one could add cool stuff like a radio time reciever, a display or wifi.
Time is set manually with the push buttons on top. 

The PSU is heavily inspired by [this site](https://surfncircuits.com/2018/02/03/optimizing-the-5v-to-170v-nixie-tube-power-supply-design-part-2/). I highly advise you giving it a read. The article goes in depth with boost converter design and its challanges.

![Control board 3d view](./Docs//CNIX%20V2%20control%20board%20front.png)

## Neat features

### Addon Boards
I designed an addon-board which just plugs into the broken out pins on the control board, to host all kinds of stuff. The one i already made has an DCF77 antenna for automatically getting the exact time OTA in germany. It also holds a HTU21 temperature and humidity sensor from TE, which can be accessed via a single press on the "menu" button on the control board. 
The addon-boards get detected via two pins (D2 a& D3), which form a 2 bit id system. so 0b11 means no board is connected, because i used internal pullups. the DCF77 board has the id 0b00, which deactivates the manual time setting menu and adds the temperature and humidity menu. 
With this all kinds of boards are possible, for example an ESP32 addon board with ESP-Home installed to integrate the clock and a sensor into homeassistant.

On the DCF77 Addon board there is a single pin to give structural strength to a simple perfboard on the main breakout bus. One can use this in tandem with the A1, A2, A3, A4, F0 and F1 pins.

### Turn on/off timers
You can set power on and off times in the clock natively, so that the nixies dont always glow. With this the HT-power supply also gets deactivated to save power and lifespan of the parts. More advanced features like weekday dependant times have to be programmed directly into the µC, except if you want to code a big menu with just 3 buttons and 2 LEDs.

## TODO

- Change to new MCU. STM32H5 or ESP32. ESP32 for Wifi stuff like syncing time, Homeassistant integration etc. Could be done as module for the pinheader. STM32H5 for USBC pd
    - (DONE) Not necessary, BUT-> Create Plugin board for a DCF77 reciever conencting via GPIO
    - Addon board for Homeassistant Integration?
- (DONE) lower value caps for the LSE
- USB-PD (only with STM32H5) (Not neccessary)
- (DONE) HTU21 Temp and relative humidity sensor
