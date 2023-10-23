// EXAMPLE: IO Expander Address
// Last update: Oct 22, 2023
// contact@PTSolns.com
//
// DESCRIPTION
// This example shows how to set the address for the IO Expander onboard the I2C Backpack.
// The IO Expander is started with the command LCD.begin(). If no argument is given, the default address of 0x3F is used.
// The I/O Expander onboard the I2C Backpack has eight available addresses. 
// The default address is 0x3F, which is when jumper bridges for A0, A1 and A2 on the I2C Backpack are left open.
//
// Address Legend:
// A0=0, A1=0, A2=0 -> 0x3F (default)
// A0=1, A1=0, A2=0 -> 0x3E
// A0=0, A1=1, A2=0 -> 0x3D
// A0=1, A1=1, A2=0 -> 0x3C
// A0=0, A1=0, A2=1 -> 0x3B
// A0=1, A1=0, A2=1 -> 0x3A
// A0=0, A1=1, A2=1 -> 0x39
// A0=1, A1=1, A2=1 -> 0x38
// Where 0=solder bridge open (pulled high), 1=solder bridge closed (pulled low).
//
// Only this range of available addresses are allowed. 
// If LCD reads "Address worked", then you've set the correct address and all is working as it should.
// Otherwise, check on the I2C Backpack if any of the three jumper bridges for A0, A1, A2 are closed. If so, change the address accordingly.
// Otherwise run example "I2C_scanner" and check Serial Monitor for address output.
//
// HARDWARE CONFIGURATION
// Connect any microcontroller to the I2C Backpack via the regular I2C pins. User can use either the screw terminal or the QWIIC connection method.
// It does not matter if the microcontroller is based on 5V or 3.3V. Either will work!
// Prior assembly of the I2C Backpack is assumed (soldering or otherwise connecting the 16 pins of the I2C Backpack with either a 1602 or 2004 LCD.).

#include <PTSolns_I2CBackpack.h>

I2C_LCD LCD;

void setup() {
  Wire.begin(); // Uses I2C default pins on whatever microcontroller being used.

  LCD.begin(0x3F);   // Use the default address by leaving argument blank, or by specifying 0x3F.

  LCD.backlight(ON);
  LCD.setCursor(0, 0);
  LCD.print("Address worked"); // If this gets written to LCD, then address worked!
}

void loop() {
  // Nothing to do in the main loop for this example.
}
