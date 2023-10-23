#include "PTSolns_I2CBackpack.h"

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Arduino.h"


//-------------------------------------Display Driver-----------------------------------------------------


// When the display powers up, it is configured as follows:
//
// 1. Display clear
// 2. Function set: 
//    DL = 1; 8-bit I2C_LCD data 
//    N = 0; 1-line display 
//    F = 0; 5x8 dot character font 
// 3. Display on/off control: 
//    D = 0; Display off 
//    C = 0; Cursor off 
//    B = 0; Blinking off 
// 4. Entry mode set: 
//    I/D = 1; Increment by 1 
//    S = 0; No shift 
//
// Note, however, that resetting the Arduino doesn't reset the LCD, so we
// can't assume that it's in that state when a sketch starts (and the
// LiquidCrystal constructor is called).


void I2C_LCD::LiquidCrystal(uint8_t rs, uint8_t rw, uint8_t enable, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3)
{
  init(1, rs, rw, enable, d0, d1, d2, d3, 0, 0, 0, 0);
}


void I2C_LCD::setLCD(bool lcdType)
{
    this->lcdType = lcdType;
    
    if(lcdType == LCD_2004)
        beginLCD(20, 4);
    else
        beginLCD(16, 2);
}


void I2C_LCD::init(uint8_t fourbitmode, uint8_t rs, uint8_t rw, uint8_t enable, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7)
{
  _rs_pin = rs;
  _rw_pin = rw;
  _enable_pin = enable;
  
  _data_pins[0] = d0;
  _data_pins[1] = d1;
  _data_pins[2] = d2;
  _data_pins[3] = d3; 
  _data_pins[4] = d4;
  _data_pins[5] = d5;
  _data_pins[6] = d6;
  _data_pins[7] = d7; 

  if (fourbitmode)
    _displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
  else 
    _displayfunction = LCD_8BITMODE | LCD_1LINE | LCD_5x8DOTS;
  
  beginLCD(16, 1);
}

void I2C_LCD::beginLCD(uint8_t cols, uint8_t lines, uint8_t dotsize) {
  if (lines > 1) {
    _displayfunction |= LCD_2LINE;
  }
  _numlines = lines;

  setRowOffsets(0x00, 0x40, 0x00 + cols, 0x40 + cols);  

  // for some 1 line displays you can select a 10 pixel high font
  if ((dotsize != LCD_5x8DOTS) && (lines == 1)) {
    _displayfunction |= LCD_5x10DOTS;
  }

  this->pinnMode(_rs_pin, OUTPUT);
  // we can save 1 pin by not using RW. Indicate by passing 255 instead of pin#
  if (_rw_pin != 255) { 
    this->pinnMode(_rw_pin, OUTPUT);
  }
  this->pinnMode(_enable_pin, OUTPUT);
  
  // Do these once, instead of every time a character is drawn for speed reasons.
  for (int i=0; i<((_displayfunction & LCD_8BITMODE) ? 8 : 4); ++i)
  {
    this->pinnMode(_data_pins[i], OUTPUT);
   } 

  // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
  // according to datasheet, we need at least 40 ms after power rises above 2.7 V
  // before sending commands. Arduino can turn on way before 4.5 V so we'll wait 50
  delayMicroseconds(50000); 
  // Now we pull both RS and R/W low to begin commands
  this->pinWrite(_rs_pin, LOW);
  this->pinWrite(_enable_pin, LOW);
  if (_rw_pin != 255) { 
    this->pinWrite(_rw_pin, LOW);
  }
  
  //put the LCD into 4 bit or 8 bit mode
  if (! (_displayfunction & LCD_8BITMODE)) {
    // this is according to the Hitachi HD44780 datasheet
    // figure 24, pg 46

    // we start in 8bit mode, try to set 4 bit mode
    write4bits(0x03);
    delayMicroseconds(4500); // wait min 4.1ms

    // second try
    write4bits(0x03);
    delayMicroseconds(4500); // wait min 4.1ms
    
    // third go!
    write4bits(0x03); 
    delayMicroseconds(150);

    // finally, set to 4-bit I2C_LCD
    write4bits(0x02); 
  } else {
    // this is according to the Hitachi HD44780 datasheet
    // page 45 figure 23

    // Send function set command sequence
    command(LCD_FUNCTIONSET | _displayfunction);
    delayMicroseconds(4500);  // wait more than 4.1 ms

    // second try
    command(LCD_FUNCTIONSET | _displayfunction);
    delayMicroseconds(150);

    // third go
    command(LCD_FUNCTIONSET | _displayfunction);
  }

  // finally, set # lines, font size, etc.
  command(LCD_FUNCTIONSET | _displayfunction);  

  // turn the display on with no cursor or blinking default
  _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;  
  display();

  // clear it off
  clear();

  // Initialize to default text direction (for romance languages)
  _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
  // set the entry mode
  command(LCD_ENTRYMODESET | _displaymode);

}

void I2C_LCD::setRowOffsets(int row0, int row1, int row2, int row3)
{
  _row_offsets[0] = row0;
  _row_offsets[1] = row1;
  _row_offsets[2] = row2;
  _row_offsets[3] = row3;
}

/********** high level commands, for the user! */
void I2C_LCD::clear()
{
  command(LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
  delayMicroseconds(2000);  // this command takes a long time!
}

void I2C_LCD::home()
{
  command(LCD_RETURNHOME);  // set cursor position to zero
  delayMicroseconds(2000);  // this command takes a long time!
}

void I2C_LCD::setCursor(uint8_t col, uint8_t row)
{
  const size_t max_lines = sizeof(_row_offsets) / sizeof(*_row_offsets);
  if ( row >= max_lines ) {
    row = max_lines - 1;    // we count rows starting w/ 0
  }
  if ( row >= _numlines ) {
    row = _numlines - 1;    // we count rows starting w/ 0
  }
  
  command(LCD_SETDDRAMADDR | (col + _row_offsets[row]));
}

// Turn the display on/off (quickly)
void I2C_LCD::noDisplay() {
  _displaycontrol &= ~LCD_DISPLAYON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void I2C_LCD::display() {
  _displaycontrol |= LCD_DISPLAYON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turns the underline cursor on/off
void I2C_LCD::noCursor() {
  _displaycontrol &= ~LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void I2C_LCD::cursor() {
  _displaycontrol |= LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turn on and off the blinking cursor
void I2C_LCD::noBlink() {
  _displaycontrol &= ~LCD_BLINKON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void I2C_LCD::blink() {
  _displaycontrol |= LCD_BLINKON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// These commands scroll the display without changing the RAM
void I2C_LCD::scrollDisplayLeft(void) {
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void I2C_LCD::scrollDisplayRight(void) {
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void I2C_LCD::leftToRight(void) {
  _displaymode |= LCD_ENTRYLEFT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// This is for text that flows Right to Left
void I2C_LCD::rightToLeft(void) {
  _displaymode &= ~LCD_ENTRYLEFT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'right justify' text from the cursor
void I2C_LCD::autoscroll(void) {
  _displaymode |= LCD_ENTRYSHIFTINCREMENT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'left justify' text from the cursor
void I2C_LCD::noAutoscroll(void) {
  _displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void I2C_LCD::createChar(uint8_t location, uint8_t charmap[]) {
  location &= 0x7; // we only have 8 locations 0-7
  command(LCD_SETCGRAMADDR | (location << 3));
  for (int i=0; i<8; i++) {
    write(charmap[i]);
  }
}

/*********** mid level commands, for sending data/cmds */

inline void I2C_LCD::command(uint8_t value) {
  send(value, LOW);
}

inline size_t I2C_LCD::write(uint8_t value) {
  send(value, HIGH);
  return 1; // assume success
}

/************ low level data pushing commands **********/

// write either command or data, with automatic 4/8-bit selection
void I2C_LCD::send(uint8_t value, uint8_t mode) {
  this->pinWrite(_rs_pin, mode);

  // if there is a RW pin indicated, set it low to Write
  if (_rw_pin != 255) { 
    this->pinWrite(_rw_pin, LOW);
  }
  
  if (_displayfunction & LCD_8BITMODE) {
    write8bits(value); 
  } else {
    write4bits(value>>4);
    write4bits(value);
  }
}

void I2C_LCD::pulseEnable(void) {
  this->pinWrite(_enable_pin, LOW);
  delayMicroseconds(1);    
  this->pinWrite(_enable_pin, HIGH);
  delayMicroseconds(1);    // enable pulse must be >450 ns
  this->pinWrite(_enable_pin, LOW);
  delayMicroseconds(100);   // commands need >37 us to settle
}

void I2C_LCD::write4bits(uint8_t value) {
  for (int i = 0; i < 4; i++) {
    this->pinWrite(_data_pins[i], (value >> i) & 0x01);
  }

  pulseEnable();
}

void I2C_LCD::write8bits(uint8_t value) {
  for (int i = 0; i < 8; i++) {
    this->pinWrite(_data_pins[i], (value >> i) & 0x01);
  }
  
  pulseEnable();
}


//-------------------------------------PCA9554A Instruction Set-----------------------------------------------------


uint8_t I2C_LCD::begin()
{
    return InitialSetup();
}

uint8_t I2C_LCD::begin(uint8_t addr)
{
    if(0x30 <= addr && 0X3F >= addr)
        this->addr = addr;
    else
        return 2;
    return InitialSetup();
}

void I2C_LCD::setClock(unsigned long speed)
{
    Wire.setClock(speed);
}

uint8_t I2C_LCD::backlight(bool state)
{
    int err = 0;
    err += this->pinnMode(3, OUTPUT);
    err += pinWrite(3, state);
    return (err > 0) * 10;
}

uint8_t I2C_LCD::InitialSetup()
{
    setClock(100000L);
    LiquidCrystal(0, 1, 2, 4, 5, 6, 7);
    
    if(lcdType == LCD_2004)
      beginLCD(20, 4);
    else
      beginLCD(16, 2);
    return 0;
}

bool I2C_LCD::twiRead(byte &registerAddress)
{
	Wire.beginTransmission(addr);
  	Wire.write(registerAddress);

  	if(Wire.endTransmission() == 0)
  	{
    		delay(15);
    		Wire.requestFrom(addr, 1, true);
    		while(Wire.available() < 1);
    		registerAddress = Wire.read();
    		return true;
  	}
  	return false;
}

bool I2C_LCD::twiWrite(byte registerAddress, byte dataWrite)
{
	Wire.beginTransmission(addr);
  	Wire.write(registerAddress);
  	Wire.write(dataWrite);

  	if(Wire.endTransmission() == 0)
    		return true;
  	return false;
}

bool I2C_LCD::pinnMode(byte pinNumber, bool state)
{
	byte oldValue = CONFIGPORT;
	if(this->twiRead(oldValue) && (pinNumber <= 7))
	{
		if(!state)
		{
			oldValue |= (1 << pinNumber);
			if(this->portMode(oldValue))
				return true;
			return false;
		}
		else if(state)
		{
			oldValue &= ~(1 << pinNumber);
			if(this->portMode(oldValue))
				return true;
			return false;
		}
	}
	return false;
}

bool I2C_LCD::portMode(byte value)
{
	if(this->twiWrite(CONFIGPORT, value))
		return true;
	return false;
}


bool I2C_LCD::pinWrite(byte pinNumber, bool state)
{
	byte oldValue = OUTPUTPORT;
	if(pinNumber <= 7)
	{
		if(state)
		{
			portsOutput |= (1 << pinNumber);
			if(this->digitalWritePort(portsOutput))
				return true;
			return false;
		}
		else if(!state)
		{
			portsOutput &= ~(1 << pinNumber);
			if(this->digitalWritePort(portsOutput))
				return true;
			return false;
		}
	}
	return false;
}



bool I2C_LCD::digitalWritePort(byte value)
{
	if(this->twiWrite(OUTPUTPORT, value))
		return true;
	return false;
}



bool I2C_LCD::pinRead(byte &pinNumber)
{
	byte oldValue = INPUTPORT;
	if(this->twiRead(oldValue) && (pinNumber <= 7))
	{
		oldValue &= (1 << pinNumber);
		if(oldValue > 0) pinNumber = 1;
		else pinNumber = 0;
		return true;
	}
	return false;
}


bool I2C_LCD::digitalReadPort(byte &value)
{
	value = INPUTPORT;
	if(this->twiRead(value))
		return true;
	return false;
}
