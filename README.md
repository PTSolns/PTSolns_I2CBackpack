# PTSolns I2C Backpack

Library for PTSolns I2C Backpack

The I2C Backpack interfaces the common 1602 and 2004 LCDs to a microcontroller via the I2C protocol. It is called a "backpack" as it is intended to be soldered right onto the back of the LCD. Onboard the I2C Backpack is a boost converter as well as a logic level shifter. This allows the user to connect an LCD to either a 3.3V or a 5V microcontroller.

The user can connect the I2C and power wires either directly to the onboard 4-pin screw terminal or via the onboard QWIIC® connector. This makes it convenient to easily daisy-chain other modules and sensors that also use the I2C protocol. The I2C Backpack has LCD contrast and LCD backlight control via onboard potentiometers. The board has eight configurable addresses.

To use the I2C Backpack the user should install the PTSolns_I2CBackpack library. Various examples and documentation can be found in the library package. 

The following are the main features of the I2C Backpack:

- Interface via I2C using either 3.3V or 5V, with automatic 5V logic level and main voltage conversion.
- Onboard main power indicator LED.
- Onboard LCD contrast potentiometer.
- Onboard LCD backlight potentiometer.
- 4-pin screw terminal for I2C signals.
- QWIIC connector for I2C signals.
- Eight configurable addresses, with default address 0x3F.
- Large I2C testing pads for oscilloscope connection.

Various examples are included in this package. For any comments or questions please contact us at contact@PTSolns.com.

## Acknowledgements
The I2C Backpack library incorporates large parts of the following libraries. We wish to give credit to their respective authors.
- PCA9554.h by AD0ND, GPL-3.0 license, https://github.com/AD0ND/PCA9554
- LiquidCrystal.h by Hans-Christoph Steiner, GNU Lesser General Public License (LGPL), https://github.com/arduino-libraries/LiquidCrystal
