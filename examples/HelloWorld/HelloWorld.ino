#include <PTSolns_I2CBackpack.h>

Interface interface;

void setup()
{
    interface.begin();   // default 0x3F, comment to change address
    // interface.begin(0x3F);   // uncomment to change address
    
    interface.backlight(ON);
    interface.clear();
    interface.setCursor(0, 0);
    interface.print("Hello World");
    interface.setCursor(0, 1);
    interface.print("PTSolns");
    interface.setCursor(0, 2);
    interface.print("I2CBackpack");
}

int i = 0;

void loop()
{
    delay(1000);
    interface.setCursor(0, 3);
    interface.print(i);
    i++;
}