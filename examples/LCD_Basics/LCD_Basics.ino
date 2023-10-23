// EXAMPLE: LCD Basics
// Last update: Oct 23, 2022
// contact@PTSolns.com
//
// DESCRIPTION
// This example demonstrates basic commands to work with the I2C Backpack.
// Many of the command options are commented. Details in corresponding notes.
//
// HARDWARE CONFIGURATION
// Connect any microcontroller to the I2C Backpack via the regular I2C pins. User can use either the screw terminal or the QWIIC connection method.
// It does not matter if the microcontroller is based on 5V or 3.3V. Either will work!
// Prior assembly of the I2C Backpack is assumed (soldering or otherwise connecting the 16 pins of the I2C Backpack with either a 1602 or 2004 LCD.).

#include <PTSolns_I2CBackpack.h>

I2C_LCD LCD;

    
void setup(){
  // Wire library already included in the PTSolns_I2CBackpack.h. But we need to initialize it here.
  Wire.begin();     // This uses I2C default pins on whatever microcontroller being used.
  //Wire.begin(SDA_pin, SCL_pin); // This explicitly specifies the I2C pins the user wants to use. User needs to specify what SDA_pin and SCL_pin are.

  LCD.begin();  // Use the default address (0x3F) by leaving argument blank. See "IO Expander Address" example for details.

  LCD.backlight(ON); // Turn ON the LCD backlight (or turn it OFF instead).

  // Change communication frequency (NOT RECOMMENDED). Default speed is 100000L (100kHz)
  // LCD.setClock(frequency);
  // The I/O expander can go up to 400000L (400kHz), but it recommeneded for most application to keep it at 100kHz.

  // Specify the LCD type. By default the 1602 is selected.
  //LCD.setLCD(LCD_1602); // default even without specifying.
  //LCD.setLCD(LCD_2004);
}

void loop(){
  // All the standard LCD commands apply.
  LCD.setCursor(0,0);
  LCD.print("Hello World!");
  delay(2000);
  
  LCD.clear(); 
  LCD.setCursor(0,1);
  LCD.print("Hello Universe!");
  delay(2000);

  LCD.clear();
  LCD.backlight(OFF);
  delay(2000);

  LCD.backlight(ON);
  LCD.setCursor(0,0);
  LCD.print("...");
  delay(2000);

  // All LCD commands include:
  // LCD.setCurser(0,0);
  // LCD.print("Hello World");
  // LCD.autoscroll();
  // LCD.noAutoscroll();
  // LCD.clear();
  // LCD.noBlink();
  // LCD.blink();
  // LCD.noCursor();
  // LCD.cursor();
  // LCD.noDisplay();
  // LCD.Display();
  // LCD.scrollDisplayLeft();
  // LCD.scrollDisplayRight();
}
