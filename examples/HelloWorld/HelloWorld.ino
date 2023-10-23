// EXAMPLE: HelloWorld
// Last update: Oct 22, 2023
// contact@PTSolns.com
//
// DESCRIPTION
// This example demonstrates the classic HelloWorld print example. 
// "Hello World!" is printed on the first row. Meanwhile an integer is printed out on the second row and incremented with every loop.
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

void setup() {
  Wire.begin(); // Uses I2C default pins on whatever microcontroller being used.

  LCD.begin();   
  LCD.backlight(ON);
  LCD.setCursor(0, 0);
  LCD.print("Hello World!");
}

int i = 0;

void loop() {
  LCD.setCursor(0, 1);
  LCD.print(i);

  i++; // Increment counter

  delay(1000); // Wait one second
}
