<a id="library"></a>
# LiquidCrystal_I2C
It is the reimplementation of the standard Arduino LCD library, configured to work with parallel HD44780 compatible LCDs, and interfaced via a Chinese PCF8574 I2C serial extender.


<a id="credit"></a>
## Credit
The reimplementation has been inspired by and credit goes to:

- Mario H. atmega@xs4all.nl LiquidCrystal_I2C V2.0
- Murray R. Van Luyn vanluynm@iinet.net.au Mods for Chinese I2C converter board


<a id="dependency"></a>
## Dependency
The library class extends the system library *Print* and includes following sytem header files.

- **inttypes.h**: Integer type conversions. This header file includes the exact-width integer definitions and extends them with additional facilities provided by the implementation.
- **Print.h**: Base class that provides *print()* and *println()*.
- **Wire.h**: TWI/I2C library for Arduino & Wiring.


<a id="interface"></a>
## Interface
Some of listed functions come out of Arduino [LCD API 1.0](http://playground.arduino.cc/Code/LCDAPI), some of them are specific for this library. It is possible to use functions from the system library [Print](#dependency), which is extended by the *LiquidCrystal_I2C*.

*) The function is an alias of another (usually previous) one.

##### Initialization
- [LiquidCrystal_I2C()](#LiquidCrystal_I2C)
- [begin()](#begin)
- [init()](#init)
- [clear()](#clear)
- [home()](#home)

##### Printing
- [print()](#print)
- [write()](#write)

##### Display control
- [noDisplay()](#noDisplay)
- *[off()](#noDisplay)
- [display()](#display)
- *[on()](#display)
- [scrollDisplayLeft()](#scrollDisplayLeft)
- [scrollDisplayRight()](#scrollDisplayRight)
- [leftToRight()](#leftToRight)
- [rightToLeft()](#rightToLeft)
- [noAutoscroll()](#noAutoscroll)
- [autoscroll()](#autoscroll)
- [noBacklight()](#noBacklight)
- [backlight()](#backlight)
- *[setBacklight()](#backlight)

##### Cursor manipulation
- [noCursor()](#noCursor)
- *[cursor_off()](#noCursor)
- [cursor()](#cursor)
- *[cursor_on()](#cursor)
- [noBlink()](#noBlink)
- *[blink_off()](#noBlink)
- [blink()](#blink)
- *[blink_on()](#blink)
- [setCursor()](#setCursor)

##### Graphs
- [init_bargraph()](#init_bargraph)
- [draw_horizontal_graph()](#draw_horizontal_graph)
- [draw_vertical_graph()](#draw_vertical_graph)

##### Utilities
- [createChar()](#createChar)
- *[load_custom_character()](#createChar)
- [command()](#command)


<a id="LiquidCrystal_I2C"></a>
## LiquidCrystal_I2C()
#### Description
Constructor of the object controlling an LCD. It defines address of the LCD and its geometry.

- More LCDs can be connected to the same I2C bus if they are hardware configured for different addresses.
- For each of LCDs the separate object has to be created. 
- When the display powers up, it is configured as follows:

1. Display clear
1. Function set: 
	- DL = 1; 8-bit interface data 
	- N = 0; 1-line display 
	- F = 0; 5x8 dot character font 
1. Display on/off control: 
	- D = 0; Display off 
	- C = 0; Cursor off 
	- B = 0; Blinking off 
1. Entry mode set: 
	- I/D = 1; Increment by 1
	- S = 0; No shift 

- Note, however, that resetting the Arduino does not reset the LCD, so we cannot assume that it is in that state when a sketch starts (and the constructor is called).

#### Syntax
	LiquidCrystal_I2C(uint8_t addr, uint8_t cols, uint8_t rows);

#### Parameters
- **addr**: I2C addres of the LCD predefined by the serial extender.
	- *Valid values*: unsigned byte
	- *Default value*: none
	- *Usual values*:
		- **0x3F** for LCDs *2004* with 20 columns and 4 rows.
		- **0x27** for LCDs *1602* with 16 columns and 2 rows.


<a id="prm_cols"></a>
- **cols**: Number of characters in a row defined by the hardware construction of the LCD screen.
	- *Valid values*: unsigned byte
	- *Default value*: none
	- *Usual values*: 20, 16, 8


<a id="prm_rows"></a>
- **rows**: Number of rows in the LCD screen defined by the hardware construction of the LCD.
	- *Valid values*: unsigned byte
	- *Default value*: none
	- *Usual values*: 4, 2, 1

#### Returns
- **LCD object**: Object controlling the LCD communicating at defined address.

#### Example

``` cpp
lcd = LiquidCrystal_I2C(0x27, 16, 2);
```

[Back to interface](#interface)


<a id="begin"></a>
## begin()
#### Description
Initialize the LCD with its specific geometry parameters.

#### Syntax
	void begin(uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5x8DOTS);

#### Parameters
- **cols**: Number of characters in a row defined by the hardware construction of the LCD screen.
	- *Valid values*: unsigned byte
	- *Default value*: none
	- *Usual values*: 20, 16, 8


- **rows**: Number of rows in the LCD screen defined by the hardware construction of the LCD.
	- *Valid values*: unsigned byte
	- *Default value*: none
	- *Usual values*: 4, 2, 1


- **charsize**: Geometry of the LCD's character defined by a library constant. 
	- *Valid values*: unsigned byte LCD_5x8DOTS, LCD_5x10DOT 
	- *Default value*: LCD_5x8DOTS

#### Returns
None

#### See also
[LiquidCrystal_I2C()](#LiquidCrystal_I2C)

[init()](#init)

[Back to interface](#interface)


<a id="init"></a>
## init()
#### Description
Initializes the display with values put to the [constructor](LiquidCrystal_I2C), clears the screen, and puts cursor to the upper left corner of the screen, i.e., to the home position 0,0. It is a wrapper function for function [begin()](#begin) with forgoing initialization of [Wire](#dependecy) library.

#### Syntax
	void init();

#### Parameters
None

#### Returns
None

#### See also
[LiquidCrystal_I2C()](#LiquidCrystal_I2C)

[begin()](#begin)

[Back to interface](#interface)


<a id="clear"></a>
## clear()
#### Description
Overloaded function for clearing the entire LCD screen or just a part of a row.
- Using the function without any parameters clears the entire srceen.
- For clearing the entire row use the function just with the first parameter.
- The functions sets the cursor to the start column and row after clearing, i.e., after calling without parameters to the home position (0, 0), or after calling with parameters to the starts of cleared row segment.

#### Syntax
	void clear();
	void clear(uint8_t rowStart, uint8_t colStart = 0, uint8_t colCnt = 255);

#### Parameters
- **rowStart**: Number of a row to be cleared counting from 0.
	- *Valid values*: unsigned byte 0 to [rows - 1](#prm_rows) of the [constructor](#LiquidCrystal_I2C)
	- *Default value*: none


- **colStart**: Order number of the first character in a cleared row counting from 0, which the cleared segment starts from.
	- *Valid values*: unsigned byte 0 to [cols - 1](#prm_cols) of the [constructor](#LiquidCrystal_I2C)
	- *Default value*: 0 (start of a row)


- **colCnt**: Number of cleared characters in a cleard row. 
	- *Valid values*: unsigned byte 0 to [cols](#prm_cols) of the [constructor](#LiquidCrystal_I2C) 
	- *Default value*: 255, but internally limited to (*cols* - *colStart*)

#### Returns
None

#### See also
[LiquidCrystal_I2C()](#LiquidCrystal_I2C)

[Back to interface](#interface)


<a id="home"></a>
## home()
#### Description
Places the cursor to the home position (0, 0) and leaves displayed characters.

#### Syntax
	void init();

#### Parameters
None

#### Returns
None

#### See also
[LiquidCrystal_I2C()](#LiquidCrystal_I2C)

[clear()](#clear)

[Back to interface](#interface)


<a id="print"></a>
## print()
#### Description
Prints text or number to the LCD. It is inhereted function from the parent system one. The function is overloaded and acts according the data type of the input data to be printed.

#### Syntax
	byte print(char|byte|int|long|string data, int base);

#### Parameters
- **data**: String or number that should be printed on the LCD from current cursor position.
	- *Valid values*: arbitrary
	- *Default value*: none


- **base**: Optional base in which to print numbers.
	- *Valid values*: integer in form of preprocesor constants
	    - BIN: binary base 2
	    - DEC: decimal base 10
	    - OCT: octal base 8
	    - HEX hexadecimal base 16
	- *Default value*: string

#### Returns
- **ProcessBytes**: Number of successfully printed bytes.

#### Example

``` cpp
lcd = LiquidCrystal_I2C(0x27, 16, 2);
void setup()
{
  lcd.print("Hello, world!");
  lcd.setCursor(0, 1);
  lcd.print(128, HEX);
}

void loop() {}
```
	>Hello, world!
	>80


#### See also
[write()](#write)

[setCursor()](#setCursor)

[LiquidCrystal_I2C()](#LiquidCrystal_I2C)

[Back to interface](#interface)


<a id="write"></a>
## write()
#### Description
Writes a raw value to the display.

#### Syntax
	size_t write(uint8_t value);

#### Parameters
- **value**: Value that should be write to the LCD at address set before.
	- *Valid values*: unsigned byte
	- *Default value*: none

#### Returns
- **ProcessBytes**: Number of successfully processed bytes; always 1.

#### See also
[print()](#print)

[command()](#command)

[Back to interface](#interface)


<a id="noDisplay"></a>
## noDisplay()
#### Description
Turns the display off quickly. If the display does not have an option to turn on the display, the function just turns backlight on.

#### Syntax
	void noDisplay();

#### Alias
	void off();

#### Parameters
None

#### Returns
None

#### See also
[display()](#display)

[Back to interface](#interface)


<a id="display"></a>
## display()
#### Description
Turns the display on quickly. If the display does not have an option to turn off the display, the function just turns backlight off.

#### Syntax
	void display();

#### Alias
	void on();

#### Parameters
None

#### Returns
None

#### See also
[noDisplay()](#noDisplay)

[Back to interface](#interface)


<a id="scrollDisplayLeft"></a>
## scrollDisplayLeft()
#### Description
Scrolls the display text to the left without changing the RAM. The function scrolls entire 40 character buffer. If you print 40 characters to a row and start scrolling, you get continuous moving banner in a row especially on 1602 LCDs.

#### Syntax
	void scrollDisplayLeft();

#### Parameters
None

#### Returns
None

#### See also
[scrollDisplayRight()](#scrollDisplayRight)

[Back to interface](#interface)


<a id="scrollDisplayRight"></a>
## scrollDisplayRight()
#### Description
Scrolls the display text to the right without changing the RAM. The function scrolls entire 40 character buffer. If you print 40 characters to a row and start scrolling, you get continuous moving banner in a row especially on 1602 LCDs.

#### Syntax
	void scrollDisplayRight();

#### Parameters
None

#### Returns
None

#### See also
[scrollDisplayLeft()](#scrollDisplayLeft)

[Back to interface](#interface)


<a id="leftToRight"></a>
## leftToRight()
#### Description
Sets the flow of text from left to right as it is normal for Latin languages.

#### Syntax
	void leftToRight();

#### Parameters
None

#### Returns
None

#### See also
[rightToLeft()](#rightToLeft)

[Back to interface](#interface)


<a id="rightToLeft"></a>
## rightToLeft()
#### Description
Sets the flow of text from right to left as it is normal for Arabic languages.

#### Syntax
	void rightToLeft();

#### Parameters
None

#### Returns
None

#### See also
[leftToRight()](#leftToRight)

[Back to interface](#interface)


<a id="noAutoscroll"></a>
## noAutoscroll()
#### Description
Justifies the text from the cursor to the left.

#### Syntax
	void noAutoscroll();

#### Parameters
None

#### Returns
None

#### See also
[autoscroll()](#autoscroll)

[Back to interface](#interface)


<a id="autoscroll"></a>
## autoscroll()
#### Description
Justifies the text from the cursor to the right.

#### Syntax
	void autoscroll();

#### Parameters
None

#### Returns
None

#### See also
[noAutoscroll()](#noAutoscroll)

[Back to interface](#interface)


<a id="noBacklight"></a>
## noBacklight()
#### Description
Turns the backlight off.

#### Syntax
	void noBacklight();

#### Alias
  void setBacklight(0);

#### Parameters
None

#### Returns
None

#### See also
[backlight()](#backlight)

[Back to interface](#interface)


<a id="backlight"></a>
## backlight()
#### Description
Turns the backlight on.

#### Syntax
	void backlight();

#### Alias
  void setBacklight(1);

#### Parameters
None

#### Returns
None

#### See also
[noBacklight()](#noBacklight)

[Back to interface](#interface)


<a id="noCursor"></a>
## noCursor()
#### Description
Turns the block cursor off.

#### Syntax
	void noCursor();

#### Alias
	void cursor_off();

#### Parameters
None

#### Returns
None

#### See also
[cursor()](#cursor)

[Back to interface](#interface)


<a id="cursor"></a>
## cursor()
#### Description
Turns the block cursor on.

#### Syntax
	void cursor();

#### Alias
	void cursor_on();

#### Parameters
None

#### Returns
None

#### See also
[noCursor()](#noCursor)

[Back to interface](#interface)


<a id="noBlink"></a>
## noBlink()
#### Description
Turns on the blinking underline cursor.

#### Syntax
	void noBlink();

#### Alias
	void blink_off();

#### Parameters
None

#### Returns
None

#### See also
[blink()](#blink)

[Back to interface](#interface)


<a id="blink"></a>
## blink()
#### Description
Turns off the blinking underline cursor.

#### Syntax
	void blink();

#### Alias
	void blink_on();

#### Parameters
None

#### Returns
None

#### See also
[noBlink()](#noBlink)

[Back to interface](#interface)


<a id="setCursor"></a>
## setCursor()
#### Description
Fills the first 8 character generator RAM (CGRAM) locations with custom characters.

#### Syntax
	void setCursor(uint8_t col, uint8_t row);

#### Parameters
- **col**: Number of a column where the cursor will be located counting from 0.
	- *Valid values*: unsigned byte 0 to [cols - 1](#prm_cols) of the [constructor](#LiquidCrystal_I2C)
	- *Default value*: none

	
- **row**: Number of a row where the cursor will be located counting from 0.
	- *Valid values*: unsigned byte 0 to [rows - 1](#prm_rows) of the [constructor](#LiquidCrystal_I2C) 
	- *Default value*: none

#### Returns
None

#### See also
[home()](#home)

[Back to interface](#interface)


<a id="init_bargraph"></a>
## init_bargraph()
#### Description
Initializes particular bar graph. The function creates a set of custom
characters for displaying bar graphs. Some number of first current custom
characters (5 or 8) will be overwritten according to the type of graph.

#### Syntax
	uint8_t init_bargraph(uint8_t graphtype);

#### Parameters
- **graphtype**: Type of a graph.
	- *Valid values*: unsigned integer
		- LCDI2C_VERTICAL_BAR_GRAPH    - rewrites all 8 custom characters
		- LCDI2C_HORIZONTAL_BAR_GRAPH  - rewrites first 5 custom characters
		- LCDI2C_HORIZONTAL_LINE_GRAPH - rewrites first 5 custom characters
	- *Default value*: none

#### Returns
- **ResultCode**: Numeric code determining processing of the initialization.
	- 0: success
	- 1: failure, e.g., not recognized graph type

#### See also
[draw_horizontal_graph()](#draw_horizontal_graph)

[draw_vertical_graph()](#draw_vertical_graph)

[Back to interface](#interface)


<a id="draw_horizontal_graph"></a>
## draw_horizontal_graph()
#### Description
Displays horizontal graph from desired cursor position with input value.
- The bar graph is composed of solid, full rectangle characters eventually except final character with reduced vertical pipes. Value of the bar graph is displayed as equivalent number of pipes in the graph segment.
- The line graph is composed of one pipe running across a LCD row. Value
  of the bar graph is displayed as a pipe at equivalent dot position in the graph segment.
- The function is overloaded by data type of a displayed graph value, which
  determines its form.
- Zero value of the graph is displayed as the very left pipe in the	graph segment due to counting from 0, so that the graph always displays something.
  
#### Syntax
	void draw_horizontal_graph(uint8_t row, uint8_t column, uint8_t len, uint8_t pixel_col_end);
	void draw_horizontal_graph(uint8_t row, uint8_t column, uint8_t len, uint16_t percentage);
	void draw_horizontal_graph(uint8_t row, uint8_t column, uint8_t len, float ratio);

#### Parameters
- **row**: Row positon of graph segment counting from 0 to physical number of rows.
	- *Valid values*: non-negative integer 0 to [rows - 1](#prm_rows) of the [constructor](#LiquidCrystal_I2C) 
	- *Default value*: none

	
- **col**: Column position of graph segment counting from 0 physical number of columns on a row.
	- *Valid values*: non-negative integer 0 to [cols - 1](#prm_cols) of the [constructor](#LiquidCrystal_I2C)
	- *Default value*: none

	
- **len**: Length of a graph segment in characters limited to remaining	physical columns from starting *col* position.
	- *Valid values*: non-negative integer 0 to [cols - col](#prm_cols) of the [constructor](#LiquidCrystal_I2C)
	- *Default value*: none

	
- **pixel_col_end**: Displayed value in pipes (horizontal dots) counting from 0 to number of pipes of the graph segment. A sketch should calculate the number of segment pipes in order to map an application value to displayed value.
	- *Valid values*: non-negative integer 0 to 5 * *len*
	- *Default value*: none

	
- **percentage**: Displayed value in percentage of a graph segment length. Accepted value is rounded to integer per cents.
	- *Valid values*: non-negative integer 0 to 100
	- *Default value*: none

	
- **ratio**: Displayed value as a fragment of a graph segment length.
	- *Valid values*: non-negative decimal 0. to 1.
	- *Default value*: none
	
#### Returns
None

#### See also
[init_bargraph()](#init_bargraph)

[draw_vertical_graph()](#draw_vertical_graph)

[Back to interface](#interface)


<a id="draw_vertical_graph"></a>
## draw_vertical_graph()
#### Description
Displays vertical bar from desired cursor position with input value.
- The bar graph is composed of solid, full rectangle characters eventually except final character with reduced horizontal dashes. Value of the bar	graph is displayed as equivalent number of dashes in the graph segment.
- The function is overloaded by data type of a displayed graph value, which
  determines its form.
  
#### Syntax
	void draw_vertical_graph(uint8_t row, uint8_t column, uint8_t len,  uint8_t pixel_row_end);
	void draw_vertical_graph(uint8_t row, uint8_t column, uint8_t len,  uint16_t percentage);
	void draw_vertical_graph(uint8_t row, uint8_t column, uint8_t len,  float ratio);

#### Parameters
- **row**: Row positon of graph segment counting from 0 to physical limit.
	- *Valid values*: non-negative integer 0 to [rows - 1](#prm_rows) of the [constructor](#LiquidCrystal_I2C) 
	- *Default value*: none

	
- **col**: Column position of graph segment counting from 0 physical limit.
	- *Valid values*: non-negative integer 0 to [cols - 1](#prm_cols) of the [constructor](#LiquidCrystal_I2C)
	- *Default value*: none

	
- **len**: Length of a graph segment in rows limited to remaining physical rows from starting *row* position.
	- *Valid values*: non-negative integer 0 to [row + 1](#prm_rows) of the [constructor](#LiquidCrystal_I2C)
	- *Default value*: none

	
- **pixel_col_end**: Displayed value in dashes (vertical dots) counting	from 0 to	number of dashes of the graph segment. A sketch should calculate the number of segment dashes in order to map an application value to displayed value.
	- *Valid values*: non-negative integer 0 to 8 * *len* or 10 * *len*
	- *Default value*: none

	
- **percentage**: Displayed value in percentage of a graph segment length. Accepted value is rounded to integer per cents.
	- *Valid values*: non-negative integer 0 to 100
	- *Default value*: none

	
- **ratio**: Displayed value as a fragment of a graph segment length.
	- *Valid values*: non-negative decimal 0. to 1.
	- *Default value*: none
	
#### Returns
None

#### See also
[init_bargraph()](#init_bargraph)

[draw_horizontal_graph()](#draw_horizontal_graph)

[Back to interface](#interface)


<a id="createChar"></a>
## createChar()
#### Description
Fills the first 8 character generator RAM (CGRAM) locations with custom characters.

#### Syntax
	void createChar(uint8_t, uint8_t[]);

#### Alias
  void load_custom_character(uint8_t char_num, uint8_t *rows);

#### Parameters
- **char_num**: Position of a custom character in CGRAM for custom chars.
	- *Valid values*: 0 - 7
	- *Default value*: none

	
- **uint8_t[]**: Array of custom characters definitions.
	- *Valid values*: Character row byte patterns from the top of the char.
		- Array length 8 bytes for 5x8 characters.
		- Array length 10 bytes for 5x10 characters.
	- *Default value*: none

	
- **rows**: Pointer to the array of custom characters definitions.

#### Returns
None

#### See also
[init_bargraph()](#init_bargraph)

[Back to interface](#interface)


<a id="command"></a>
## command()
#### Description
Sends a command to the display. It is useful for commands not supported
by the library.

#### Syntax
	void command(uint8_t value);

#### Parameters
- **value**: Command code that should be send to the LCD.
	- *Valid values*: unsigned byte
	- *Default value*: none

#### Returns
None

#### See also
[write()](#write)

[Back to interface](#interface)
