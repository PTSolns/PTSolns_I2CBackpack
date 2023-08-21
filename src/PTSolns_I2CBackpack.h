#ifndef PTSolns_InterfaceShield_h
#define PTSolns_InterfaceShield_h
#endif
#include <Arduino.h>
#include <Wire.h>
#include <inttypes.h>
#include "Print.h"

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// user input / output
#define LED1 0
#define LED2 1
#define LED3 2
#define LED4 3

#define B1 0
#define B2 1
#define B3 2
#define B4 3

#define ON 1
#define OFF 0



namespace Reg 
{
    enum : uint8_t 
    {
        INPUT_PORT_0,
        INPUT_PORT_1,
        OUTPUT_PORT_0,
        OUTPUT_PORT_1,
        POLARITY_INVERSION_PORT_0,
        POLARITY_INVERSION_PORT_1,
        CONFIGURATION_PORT_0,
        CONFIGURATION_PORT_1,
    };
}

namespace Port 
{
    enum Port : uint8_t 
    {P00, P01, P02, P03, P04, P05, P06, P07, P10, P11, P12, P13, P14, P15, P16, P17,};
}  // namespace Port

namespace Level 
{
    enum Level : uint8_t { L, H };
    enum LevelAll : uint16_t { L_ALL = 0x0000, H_ALL = 0xFFFF };
}  // namespace Level

namespace Polarity 
{
    enum Polarity : uint8_t { ORIGINAL, INVERTED };
    enum PolarityAll : uint16_t { ORIGINAL_ALL = 0x0000, INVERTED_ALL = 0xFFFF };
}  // namespace Polarity

namespace Direction 
{
    enum Direction : uint8_t { OUT, IN };
    enum DirectionAll : uint16_t { OUT_ALL = 0x0000, IN_ALL = 0xFFFF };
}  // namespace Direction


class Interface : public Print
{

    public:
    
    
    uint8_t begin();
    uint8_t begin(uint8_t addr);
    uint8_t backlight(bool state);
    void interrupt(bool state);
    uint8_t digitalWrite(uint8_t pin, bool value);
    bool digitalRead(uint8_t pin);
    void debounce(bool enabled, int ms);
    void debounce(bool enabled);
    void setClock(unsigned long speed);

    void clear();
    void home();
    void noDisplay();
    void display();
    void noBlink();
    void blink();
    void noCursor();
    void cursor();
    void scrollDisplayLeft();
    void scrollDisplayRight();
    void leftToRight();
    void rightToLeft();
    void autoscroll();
    void noAutoscroll();

    void setRowOffsets(int row1, int row2, int row3, int row4);
    void createChar(uint8_t, uint8_t[]);
    void setCursor(uint8_t, uint8_t); 
    virtual size_t write(uint8_t);
    void command(uint8_t);

    using Print::write;
    

    private:

    unsigned long btnTimes[4] = {0, 0, 0, 0};
    bool btnStates[4] = {LOW, LOW, LOW, LOW};
    int debounceTime = 100;
    bool debounceEnabled = false;

    union Ports 
    {
        uint16_t w;
        uint8_t b[2];
    };

    uint8_t addr = 0x27;
    Ports input {0x0000};
    Ports output {0xFFFF};
    Ports pol {0x0000};
    Ports dir {0xFFFF};
    uint8_t status {0x00};
    
    
    uint8_t _rs_pin; // LOW: command. HIGH: character.
    uint8_t _rw_pin; // LOW: write to LCD. HIGH: read from LCD.
    uint8_t _enable_pin; // activated by a HIGH pulse.
    uint8_t _data_pins[8];

    uint8_t _displayfunction;
    uint8_t _displaycontrol;
    uint8_t _displaymode;

    uint8_t _initialized;

    uint8_t _numlines;
    uint8_t _row_offsets[4];
    
    
    Level::Level read(const Port::Port port) 
    {
        uint16_t v = read();
        return (v & (1 << port)) ? Level::H : Level::L;
    }
    
    
    bool write_impl();
    bool polarity_impl();
    bool direction_impl();

    int8_t read_bytes(const uint8_t dev, const uint8_t reg, uint8_t* data, const uint8_t size);
    bool write_bytes(const uint8_t dev, const uint8_t reg, const uint8_t* data, const uint8_t size);
    
    void send(uint8_t, uint8_t);
    void write4bits(uint8_t);
    void write8bits(uint8_t);
    void pulseEnable();
    
    
    uint8_t InitialSetup();    
    bool ExpanderPinMode(uint8_t pin, uint8_t mode);    
    uint8_t PinWrite(uint8_t pin, bool value);
    uint16_t read();

    bool write(const uint16_t value);
    bool write(const Port::Port port, const Level::Level level);

    bool polarity(const uint16_t value);
    bool polarity(const Port::Port port, const Polarity::Polarity pol);

    bool direction(const uint16_t value);
    bool direction(const Port::Port port, const Direction::Direction dir);
    
    void init(uint8_t fourbitmode, uint8_t rs, uint8_t rw, uint8_t enable,
	    uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
	    uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7);
    
    void beginLCD(uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5x8DOTS);

    
    
    
};
