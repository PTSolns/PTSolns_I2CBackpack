# PTSolns I2C Backpack

Library for PTSolns I2C Backpack

The I2C Backpack interfaces with the common 1602 and 2005 LCDs via I2C protocol. Onboard the I2C Backpack is a boost converter as well as a logic level shifter. This allows the user to input I2C signals at both 3.3V as well as 5V, including the input voltage. The following are the main features of the I2C Backpack:

- Interface via I2C protocol using either 3.3V or 5V, with automatic 5V logic level and main voltage conversion.
- Onboard main power indicator LED.
- Onboard LCD contrast potentiometer.
- Onboard LCD backlight potentiometer.
- 4-pin screw terminal for I2C signals.
- QWIIC connector for I2C signals.
- Eight configurable addresses, with default address 0x3F.
- Large I2C testing pads for oscilloscope connection.

Various examples are included in this package. For any comments or questions please contact us at contact@PTSolns.com.

## Acknowledgements
The Interface-Shield library incorporates large parts of the following libraries. We wish to give credit to their respective authors.
- PCA9554.h by AD0ND, GPL-3.0 license, https://github.com/AD0ND/PCA9554
- LiquidCrystal.h by Hans-Christoph Steiner, GNU Lesser General Public License (LGPL), https://github.com/arduino-libraries/LiquidCrystal
