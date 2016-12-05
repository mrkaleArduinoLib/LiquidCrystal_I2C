/*
  NAME:
  LiquidCrystal_I2C

  DESCRIPTION:
  Library for parallel HD44780 compatible LCDs interfaced via a Chinese
  PCF8574 I2C serial extender.
  - Library implements LCD API 1.0 to the extend the appropriate LCDs
    support functionality expected by the API.
  - Library adds overloaded clear() function for clearing particular
    segment of an input row.
  - Library implements extended graph functions with help of custom 
    characters, so that do not use your custom characters concurrently
    with graph function. Your custom characters will be overwritten by
    graph initialization.
  - Library adds overloaded graph functions for expression graph value
    in percentage instead of pixels.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the MIT License (MIT).
    
  CREDENTIALS:
  Author: Libor Gabaj
  GitHub: https://github.com/mrkaleArduinoLib/LiquidCrystal_I2C.git
  
  CREDITS:
  Mario H. atmega@xs4all.nl - LiquidCrystal_I2C V2.0
  Murray R. Van Luyn. vanluynm@iinet.net.au - Mods for Chinese I2C converter board
 */
#ifndef LIQUIDCRYSTAL_I2C_H
#define LIQUIDCRYSTAL_I2C_H
#define LIQUIDCRYSTAL_I2C_VERSION "LiquidCrystal_I2C 2.6.1"

#ifdef ARDUINO_ARCH_AVR
  #if ARDUINO >= 100
    #include "Arduino.h"
  #else
    #include "WProgram.h"
  #endif
  #include <inttypes.h>
#endif

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
  LiquidCrystal_I2C(uint8_t addr, uint8_t cols, uint8_t rows);
  void begin(uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5x8DOTS);
  void init();
/*
  Clear particular segment of a row
  
  DESCRIPTION:
  Overloaded original function clear().
  * Thanks to default  parameters, for clearing the entire row
    use just clear(rowStart).
  * The functions sets the cursor to start column and row after clearing.
  
  PARAMETERS:
  rowStart  - row number to be cleared counting from 0.
              Limited to the last row.
  colStart  - column number of the cleared segment counting from 0.
              Defaulted to the very begining of the row.
              Limited to the last character.
  colCnt    - number of cleared characters.
              Defaulted to 255.
              Limited to remaining characters on the row.
  
  RETURN:	none
*/
  void clear();
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
  void leftToRight();
  void rightToLeft();
  void noBacklight();
  void backlight();
  void noAutoscroll(); 
  void autoscroll();
  void createChar(uint8_t location, uint8_t charmap[]);
  void setCursor(uint8_t col, uint8_t row); 
  virtual size_t write(uint8_t value);
  void command(uint8_t value);

/*
  Initialize particular bar graph
  
  DESCRIPTION:
  Creates a set of custom characters for displaying bar graphs.
  Some number of first current custom characters will be overwritten
  according to the type of graph.

  PARAMETERS:
  uint8_t graphtype - type of graph
    LCDI2C_VERTICAL_BAR_GRAPH    - rewrites all 8 custom characters
    LCDI2C_HORIZONTAL_BAR_GRAPH  - rewrites first 5 custom characters
    LCDI2C_HORIZONTAL_LINE_GRAPH - rewrites first 5 custom characters

  RETURN:	error code
          0 - at success
          1 - at failure, e.g., graph type not recognized
*/
uint8_t init_bargraph(uint8_t graphtype);

/*
  Display horizontal graph from desired cursor position with input value
  
  DESCRIPTION:
  Displays horizontal bar or running pipe starting at input cursor position
  composed of custom characters.
  * For graph is reserved "len" characters long segment on the "row" starting
    on "column".
  * Current value of the bar graph is displayed as "pixel_col_end" pipes
    in the graph segment.
  * Current value of the line graph is displayed as pipe on "pixel_col_end"
    dot position in the graph segment.
  * Zero value of the graph is displayed as the very left pipe in the
    graph segment, so that the graph always displays something.
  
  PARAMETERS:
  uint8_t row           - row position of graph segment counting from 0
                          Limited to physical rows.
  uint8_t column        - column position of graph segment counting from 0
                          Limited to physical columns.
  uint8_t len           - length of graph segment in characters
                          Limited to remaining physical columns from col position.
  uint8_t pixel_col_end - value of graph in pipes counting from 0
                          Limited to physical horizontal dots of graph segment.
  
  RETURN:	none
*/
void draw_horizontal_graph(uint8_t row, uint8_t column, uint8_t len, uint8_t pixel_col_end);

/*
  Display vertical graph from desired cursor position with input value
  
  DESCRIPTION:
  Displays vertical bar starting at "row", "column" position composed
  of custom characters.
  For bar is reserved "len" rows long segment on the "column" starting on
  "row".
  Current value of the bar graph is displayed as "pixel_row_end" dashes
  from bottom to top of the graph segment.
  
  PARAMETERS:
  uint8_t row           - row positon of the bottom of a graph segment
													counting from 0
                          Limited to physical rows.
  uint8_t column        - column position of graph segment counting from 0
                          Limited to physical columns.
  uint8_t len           - length of graph segment in rows
                          Limited to remaining physical rows from row position.
  uint8_t pixel_row_end - value of graph in dashes counting from 0
                          Limited to physical vertical dots of graph segment.
  
  RETURN:	none
*/
void draw_vertical_graph(uint8_t row, uint8_t column, uint8_t len,  uint8_t pixel_row_end);

/*
  Overloaded methods with type difference of graph value
  
  PARAMETERS:
  uint16_t percentage - percentage of graph segment as graph value
    Although expected range is 0 to 100, uint8_t has been reserved
    by official API already.
  float ratio - fraction of graph segment as graph value
    Expected range is 0.0 to 1.0
*/
void draw_horizontal_graph(uint8_t row, uint8_t column, uint8_t len, uint16_t percentage);
void draw_horizontal_graph(uint8_t row, uint8_t column, uint8_t len, float ratio);
void draw_vertical_graph(uint8_t row, uint8_t column, uint8_t len,  uint16_t percentage);
void draw_vertical_graph(uint8_t row, uint8_t column, uint8_t len,  float ratio);

////compatibility API function aliases
void on();                          // alias for display()
void off();                         // alias for noDisplay()
void blink_on();						        // alias for blink()
void blink_off();       					  // alias for noBlink()
void cursor_on();      	 					  // alias for cursor()
void cursor_off();      					  // alias for noCursor()
void setBacklight(uint8_t new_val);	// alias for backlight() and nobacklight()
void load_custom_character(uint8_t char_num, uint8_t *rows);	// alias for createChar()
void printstr(const char[]);

/* Unsupported API functions (not implemented in this library)
uint8_t status();
void setContrast(uint8_t new_val);
uint8_t keypad();
void setDelay(int, int);
*/

private:
  void init_priv();
  void send(uint8_t, uint8_t);
  void write4bits(uint8_t);
  void expanderWrite(uint8_t);
  void pulseEnable(uint8_t);

/*
  Create custom characters for horizontal graphs
  
  DESCRIPTION:
  Creates the set of custom characters for displaying horizontal graphs.
  The first 5 current custom characters will be overwritten.
  Particular custom characters are filled by bit shifting fullCharRowPattern
  from the right to the left.
  
  PARAMETERS:
  uint8_t rowPattern - row pattern of the full character

  RETURN:	uint8_t - number of created custom characters
*/
  uint8_t graphHorizontalChars(uint8_t rowPattern);

/*
  Create custom characters for vertical graphs
  
  DESCRIPTION:
  Creates the set of custom characters for displaying vertical graphs.
  All 8 current custom characters will be overwritten.
  Particular custom characters are filled with rowPattern from the very
  bottom pixel line.
  
  PARAMETERS:
  uint8_t rowPattern - row pattern of the pixel line

  RETURN:	uint8_t - number of created custom characters
*/
  uint8_t graphVerticalChars(uint8_t rowPattern);

  // Private attributes
  uint8_t _Addr;
  uint8_t _displayfunction;
  uint8_t _displaycontrol;
  uint8_t _displaymode;
  uint8_t _numlines;
  uint8_t _cols;
  uint8_t _rows;
  uint8_t _backlightval;
  uint8_t _graphtype;   // Internal code for graph type
  uint8_t _graphstate[20];  // Internal last graph column/row state
};

#endif
