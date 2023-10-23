// EXAMPLE: LCD Backlight Flashing
// Last update: Oct 23, 2022
// contact@PTSolns.com
//
// DESCRIPTION
// This example demonstrates how to flash the backlight of an LCD using the I2C Backpack.
// This will work equally well for the 1602 and the 2004 LCD.
//
// Many important settings are not demonstrated in this basic example. 
// For setting address, I2C frequency, I2C pins (SDA and SCL pins) if they are not defaulted, etc, the user is encouraged to review the other examples in this package.
//
// HARDWARE CONFIGURATION
// Connect any microcontroller to the I2C Backpack via the regular I2C pins. User can use either the screw terminal or the QWIIC connection method.
// It does not matter if the microcontroller is based on 5V or 3.3V. Either will work!
// Prior assembly of the I2C Backpack is assumed (soldering or otherwise connecting the 16 pins of the I2C Backpack with either a 1602 or 2004 LCD.).

#include <PTSolns_I2CBackpack.h>

I2C_LCD LCD;
   
void setup(){
  Wire.begin(); // Uses I2C default pins on whatever microcontroller being used.

  LCD.begin();   
  LCD.backlight(ON);
  LCD.setCursor(0, 0);
  LCD.print("Hello World!");
}

void loop(){
  LCD.backlight(OFF);

  delay(200);

  LCD.backlight(ON);

  delay(200);
}
