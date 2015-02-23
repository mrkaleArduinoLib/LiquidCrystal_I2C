/*
  NAME:
  LiquidCrystal_I2C

  DESCRIPTION:
  Library for parallel HD44780 compatible LCDs interfaced via a Chinese
  PCF8574 I2C serial extender.
  - Library adds (overloads) clear() function for clearing particular
    segment of an input row.
  - Library implements extended graph functions with help of custom 
    characters, so that do not user your custom characters concurrently
    with graph function. Your custom characters will be overwritten by
    graph initialization.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the MIT License (MIT).
  
  CREDITS:
  Mario H. atmega@xs4all.nl - LiquidCrystal_I2C V2.0
  Murray R. Van Luyn. vanluynm@iinet.net.au - Mods for Chinese I2C converter board
    
  CREDENTIALS:
  Author: Libor Gabaj
  Version: 2.2.0
  Updated: 23.02.2015
 */
#ifndef LiquidCrystal_I2C_h
#define LiquidCrystal_I2C_h

#include <inttypes.h>
#include "Print.h" 
#include <Wire.h>

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

// flags for backlight control
#define LCD_BACKLIGHT   B00001000
#define LCD_NOBACKLIGHT B00000000

// values for graphtype in calls to init_bargraph and character geometry
#define LCDI2C_VERTICAL_BAR_GRAPH    1
#define LCDI2C_HORIZONTAL_BAR_GRAPH  2
#define LCDI2C_HORIZONTAL_LINE_GRAPH 3
#define LCD_CHARACTER_HORIZONTAL_DOTS 5
#define LCD_CHARACTER_VERTICAL_DOTS   8

#define En B00000100  // Enable bit
#define Rw B00000010  // Read/Write bit
#define Rs B00000001  // Register select bit

class LiquidCrystal_I2C : public Print {
public:
  LiquidCrystal_I2C(uint8_t lcd_Addr,uint8_t lcd_cols,uint8_t lcd_rows);
  void begin(uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5x8DOTS );
  void clear();
/*
  Clear particular segment of a row.
  DESCRIPTION:  Overloaded original function clear(). Thanks to defaulted
                parameters, for clearing the entire row use just
                clear(rowStart).
  PARAMETERS:
  rowStart  - row number to be cleared counting from 0.
              Limited to the last row.
  colStart  - column number of the cleared segment counting from 0.
              Defaulted to the very beginnig of the row.
              Limited to the last character.
  colCnt    - number of cleared characters.
              Defaulted to 255.
              Limited to remaining characters on the row.
  RETURN:	none
*/
  void clear(uint8_t rowStart, uint8_t colStart = 0, uint8_t colCnt = 255);
  void home();
  void noDisplay();
  void display();
  void noBlink();
  void blink();
  void noCursor();
  void cursor();
  void scrollDisplayLeft();
  void scrollDisplayRight();
  void printLeft();
  void printRight();
  void leftToRight();
  void rightToLeft();
  void shiftIncrement();
  void shiftDecrement();
  void noBacklight();
  void backlight();
  void autoscroll();
  void noAutoscroll(); 
  void createChar(uint8_t, uint8_t[]);
  void setCursor(uint8_t, uint8_t); 
  virtual size_t write(uint8_t);
  void command(uint8_t);
  void init();
  /*
  Initialize particular bar graph.
  DESCRIPTION:  Creates a set of custom characters for displaying bar graphs.
                Some number of first current custom characters will be
                overwritten according to the type of graph.
  PARAMETERS:
  graphtype - type of graph defined by the macros
              LCDI2C_VERTICAL_BAR_GRAPH
              LCDI2C_HORIZONTAL_BAR_GRAPH - rewrites 5 custom characters
              LCDI2C_HORIZONTAL_LINE_GRAPH
  RETURN:	error code
          0 - at success
          1 - at failure, e.g., graph type not recognized
*/
uint8_t init_bargraph(uint8_t graphtype);
  /*
  Display bar graph from desired cursor position with input value
  DESCRIPTION:  Displays bar starting at input cursor position composed
                of custom characters. For bar is reserved particular number
                of characters on the row and current displayed characters
                determine the current value of graph.
  PARAMETERS:
  row           - row of segment reserved for graph counting from 0
  column        - column of segment reserved for graph counting from 0
  len           - number of characters reserved for graph
  pixel_col_end - positiion of horizontal dots representing displayed bar
                  (current value of graph) counting from 0.
                  Limited to dots of reserved segment of the graph.
  RETURN:	none
*/
void draw_horizontal_graph(uint8_t row, uint8_t column, uint8_t len, uint8_t pixel_col_end);
  
 


////compatibility API function aliases
void blink_on();						// alias for blink()
void blink_off();       					// alias for noBlink()
void cursor_on();      	 					// alias for cursor()
void cursor_off();      					// alias for noCursor()
void setBacklight(uint8_t new_val);				// alias for backlight() and nobacklight()
void load_custom_character(uint8_t char_num, uint8_t *rows);	// alias for createChar()
void printstr(const char[]);

////Unsupported API functions (not implemented in this library)
uint8_t status();
void setContrast(uint8_t new_val);
uint8_t keypad();
void setDelay(int,int);
void on();
void off();
void draw_vertical_graph(uint8_t row, uint8_t column, uint8_t len,  uint8_t pixel_col_end);
	 

private:
  void init_priv();
  void send(uint8_t, uint8_t);
  void write4bits(uint8_t);
  void expanderWrite(uint8_t);
  void pulseEnable(uint8_t);
  uint8_t _Addr;
  uint8_t _displayfunction;
  uint8_t _displaycontrol;
  uint8_t _displaymode;
  uint8_t _numlines;
  uint8_t _cols;
  uint8_t _rows;
  uint8_t _backlightval;
  uint8_t _graphtype;
};

#endif
