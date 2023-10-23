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
// There is an extra function, start_up(), call made in the setup() function. start_up() is not requred. It simply adds three quick LED blinks upon startup or restart. 
// If the user's micrcontroller does not have a LED_builtIn, simply comment the start_up() function out.
//
// HARDWARE CONFIGURATION
// Connect any microcontroller to the I2C Backpack via the regular I2C pins. User can use either the screw terminal or the QWIIC connection method.
// It does not matter if the microcontroller is based on 5V or 3.3V. Either will work!
// Prior assembly of the I2C Backpack is assumed (soldering or otherwise connecting the 16 pins of the I2C Backpack with either a 1602 or 2004 LCD.).

#include <PTSolns_I2CBackpack.h>
#include <Wire.h>

I2C_LCD LCD;

int D9 = 9; // The Interface-Shield hardware selection dictates this to be pin D9.
int dimLevel = 0;
int dimChange = 5;
   
void setup(){
  // Not requried. Called during startup or restart. Blinks onboard LED.
  startup_blink(13, 4, 50); // (LED_pin, number_of_blink, time_between_blink). 

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

void startup_blink(int const LED_builtIn, int number_of_blink, int time_between_blink) {

  pinMode(LED_builtIn, OUTPUT);

  for (int i = 1; i <= number_of_blink; ++i){
    digitalWrite(LED_builtIn, HIGH);
    delay(time_between_blink); 
    digitalWrite(LED_builtIn, LOW);
    delay(time_between_blink); 
  }
}
