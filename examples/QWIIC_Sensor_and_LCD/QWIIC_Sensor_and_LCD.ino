// EXAMPLE: QWIIC Sensor and LCD
// Last update: Oct 23, 2022
// contact@PTSolns.com
//
// DESCRIPTION
// This example shows how to connect a sensor via tha QWIIC conector onboard the I2C Backpack.
// This will work equally well for the 1602 and the 2004 LCD.
//
// Many important settings are not demonstrated in this basic example. 
// For setting address, I2C frequency, I2C pins (SDA and SCL pins) if they are not defaulted, etc, the user is encouraged to review the other examples in this package.
//
// HARDWARE CONFIGURATION
// Connect any microcontroller to the I2C Backpack via the regular I2C pins. In this example, the screw terminal connection method is used as the QWIIC connector is used to plug in Sparkfun BME280 module.
// Details on the Sparkfun BME280 can be found here: https://github.com/sparkfun/SparkFun_BME280_Arduino_Library or here: https://www.sparkfun.com/products/14348 
// This example can easily be adapted to any sensor, so use whatever sensor module you have.
// In this case, the Sparkfun module operates only on 3.3V, therefore ensure you are using a microcontroller that is at 3.3V. Although the I2C Backpack can handle both 3.3V and 5V.
// Prior assembly of the I2C Backpack is assumed (soldering or otherwise connecting the 16 pins of the I2C Backpack with either a 1602 or 2004 LCD.).

#include <PTSolns_I2CBackpack.h>
#include "SparkFunBME280.h"

BME280 mySensor; 

I2C_LCD LCD;

void setup() {
  Wire.begin();

  mySensor.setI2CAddress(0x77); // The address on the Sparkfun module.
  mySensor.beginI2C();

  LCD.begin();

  LCD.backlight(ON);

  LCD.clear();
  LCD.setCursor(0, 0);
  LCD.print("Temperature:");
}

void loop() {
  LCD.setCursor(0, 1);
  LCD.print("                "); // Clean the second row without using LCD.clear();
  LCD.setCursor(0, 1);
  LCD.print(mySensor.readTempC());

  delay(1000);
}
