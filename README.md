# MASA LoRa Communication
Contains communication source files and setup instructions for the MASA project

## Requirements

- STM32F103Cx "Blue Pill" Board
    - Don't really know how library support looks for other boards, so some modifications may be required if you use something else.
    - You'll need some way to flash the Blue Pill.
        - I'm using the ST-Link v2.
        - Using an FTDI programmer is also easy.
        - Can flash a bootloader (will need one of the above) and program via the onboard micro USB port.
- Semtech SX1276/77/78/79
    - This project uses the Adafruit RFM96W LoRa Radio Transceiver Breakout 433 MHz. Some small modifications will be required if you're using a different module.
    - Should also work with HopeRF modules [as described here.](https://alexbirkett.github.io/microcontroller/2018/12/15/blue_pill_hoperf_lora_tx_rx_with_arduino.html#:~:text=Blue%20Pill%20Lora%20Transmitter%20Receiver%20with%20HopeRF%20module%20and%20Arduino%20IDE,-I%20became%20interested&text=The%20Blue%20Pill%20is%20not,racing%20quads%20and%20RC%20planes.)
- (Optional) It may be helpful to have a known-working system. I also purchased the RFM9x Raspberry Pi bonnet from Adafruit to reduce the number of unknown variables. They have an easy-to-use Python library, and the setup is very easy.

## Libraries

Huge thanks to the maintainers of the following projects and libraries used in this project.

- [Arduino LoRa For STM32F103CB](https://github.com/armtronix/arduino-LoRa-STM32)
    - [Arduino LoRa](https://github.com/sandeepmistry/arduino-LoRa)
- [Arduino STM32](https://github.com/rogerclarkmelbourne/Arduino_STM32.git)
- [LibTomCrypt](https://github.com/libtom/libtomcrypt)

## Setup

I set up the wiring up as follows. Depending on the module you're using, `VIN` may be labeled as `VCC`, `CS` may be labeled as `NSS`, and `Gx` may be labeled as `DIOx`.

| Adafruit RFM96W (SX1276) | STM32F103C8T6 Blue Pill |
| :---: | :---: |
| VIN | 3.3V |
| GND | GND |
| SCK | SCK |
| MISO | MISO (PA6) |
| MOSI | MOSI (PA7) |
| CS | PA4 |
| RST | PC14 |
| G0 | PA1 |
| G1 | PB13 |
| G2 | PB12 |

You'll need to solder an antenna to your radio module as well. Currently I'm using some copper wire out of an RJ11 cable and the performance is pretty bad, so maybe don't do that.

### Arduino IDE (Not Recommended)

This assumes you already have the Arduino IDE installed.

#### STM32F103 Board Setup

You'll need to add support for STM32 based board on the Arduino IDE. Navigate to your Arduino Hardware directory. On Windows, this is located at `C:\Users\[USER]\Documents\Arduino\hardware\`. If the hardware directory isn't already there, create it. Now you can clone Roger Clark's repository of hardware files to support STM32F103 boards.

```
cd ~\Documents\Arduino\hardware
git clone https://github.com/rogerclarkmelbourne/Arduino_STM32.git
```

You will also need to use the Arduino Board Manager to add support for the 32-bit ARM Cortex-M3. Open the IDE, go to Tools > Board > Boards Manager, search for "ARM Cortex-M3" and install the board. 

Relaunch the IDE.

Open the Tools menu and select the appropriate board and settings for your case. I'm using the STM32F103C8T6 and writing to it with the ST-Link v2, so my settings look like this:

![Image of My Settings](./assets/Board_Settings.png)

#### LoRa Library Setup

You'll need to first add the library we're using for LoRa communication, and then you'll need to make a small modification to it.  

First, navigate into your Arduino Library directory. On Windows, this is located at `C:\Users\[USER]\Documents\Arduino\libraries\`. If that directory isn't already there, create it. Now you can clone the Arduino LoRa for STM32103C.

```
cd ~\Documents\Arduino\libraries
git clone https://github.com/armtronix/arduino-LoRa-STM32 LoRa-STM32
```

Now for the modification. The LoRa STM32F103C library uses PC13 for the default reset pin, but on the STM32F103C8T6, PC13 is an LED. Go into the `src` directory of the repository you just cloned and open up the file `LoRa_STM32.h` for editing. Change `#define LORA_DEFAULT_RESET_PIN PC13` to `#define LORA_DEFAULT_RESET_PIN PC14`.

Restart the Arduino IDE, and you should be able to use the functions made available by this library.

### LibTomCrypt

You'll need to clone the libtomcrypt repository into the Arduino libraries directory. Currently we're using the `develop` branch. We may switch to the `master` branch for stability.

```
cd ~\Documents\Arduino\libraries
git clone https://github.com/libtom/libtomcrypt
```

Go into the directory of the repository you just cloned, and in the top level directory, create a `library.properties` with data like this.

```
name=TomCrypt
version=1.0.0
author=Tom
maintainer=xxx
sentence=xxx
paragraph=xxx
category=Device Control
url=xxx
architectures=*
include=tomcrypt.h
```

Copy all of the header files from the `src/header` directory into the `src` directory. The Arduino IDE expects headers to be there.

Edit `tomcrypt_custom.h`. After the include guard (`#include <pthread.h>`) add a new line with `#define LTC_SOURCE`.

Thank you to member arduarn of the Arduino forum for the [detailed help](https://forum.arduino.cc/index.php?topic=513656.0) getting this set up.

Restart the Arduino IDE, and you should be able to use the functions made available by this library.

#### Running the MASA Code

Create a new Arduino project and copy the contents of `src/main.cpp` into your main file. Comment out the `#include <Arduino.h>` line. Then, you'll need to add project dependencies from the `lib` directory (that aren't listed above) into your project with `Sketch > Add File` and modify imports accordingly. 

If you've done the above steps correctly, you should be able to run the code on your Blue Pill.
