#include "LiquidCrystal_I2C.h"

// When the display powers up, it is configured as follows:
//
// 1. Display clear
// 2. Function set: 
//    DL = 1; 8-bit interface data 
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
// can't assume that its in that state when a sketch starts (and the
// LiquidCrystal constructor is called).
LiquidCrystal_I2C::LiquidCrystal_I2C(uint8_t addr, uint8_t cols, uint8_t rows)
{
  _Addr = addr;
  _cols = cols;
  _rows = rows;
  _backlightval = LCD_NOBACKLIGHT;
}

void LiquidCrystal_I2C::init(){
	init_priv();
}

void LiquidCrystal_I2C::init_priv()
{
	Wire.begin();
	_displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
	begin(_cols, _rows);  
}

void LiquidCrystal_I2C::begin(uint8_t cols, uint8_t lines, uint8_t charsize) {
	if (lines > 1) {
		_displayfunction |= LCD_2LINE;
	}
	_numlines = lines;

	// for some 1 line displays you can select a 10 pixel high font
	if ((charsize != 0) && (lines == 1)) {
		_displayfunction |= LCD_5x10DOTS;
	}

	// SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
	// according to datasheet, we need at least 40ms after power rises above 2.7V
	// before sending commands. Arduino can turn on way before 4.5V so we'll wait 50
	delayMicroseconds(50000); 
  
	// Now we pull both RS and R/W low to begin commands
	expanderWrite(_backlightval);	// reset expanderand turn backlight off (Bit 8 =1)
	delay(1000);

  // put the LCD into 4 bit mode
	// this is according to the hitachi HD44780 datasheet
	// figure 24, pg 46
	
	// we start in 8bit mode, try to set 4 bit mode
	write4bits(0x30);
	delayMicroseconds(4500); // wait min 4.1ms
	
	// second try
	write4bits(0x30);
	delayMicroseconds(4500); // wait min 4.1ms
	
	// third go!
	write4bits(0x30); 
	delayMicroseconds(150);
	
	// finally, set to 4-bit interface
	write4bits(0x20); 


	// set # lines, font size, etc.
	command(LCD_FUNCTIONSET | _displayfunction);  
	
	// turn the display on with no cursor or blinking default
	_displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
	display();
	
	// clear it off
	clear();
	
	// Initialize to default text direction (for roman languages)
	_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
	
	// set the entry mode
	command(LCD_ENTRYMODESET | _displaymode);
	
	home();
  
}



/********** high level commands, for the user! */
void LiquidCrystal_I2C::clear(){
	command(LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
	delayMicroseconds(2000);    // this command takes a long time!
}

// Clear particular segment of a row
void LiquidCrystal_I2C::clear(uint8_t rowStart, uint8_t colStart, uint8_t colCnt) {
  // Maintain input parameters
  rowStart = constrain(rowStart, 0, _rows - 1);
  colStart = constrain(colStart, 0, _cols - 1);
  colCnt   = constrain(colCnt,   0, _cols - colStart);
  // Clear segment
  setCursor(colStart, rowStart);
  for (uint8_t i = 0; i < colCnt; i++) write(' ');
  // Go to segment start
  setCursor(colStart, rowStart);
}


void LiquidCrystal_I2C::home(){
	command(LCD_RETURNHOME);  // set cursor position to zero
	delayMicroseconds(2000);  // this command takes a long time!
}

void LiquidCrystal_I2C::setCursor(uint8_t col, uint8_t row){
	int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
	if ( row > _numlines ) {
		row = _numlines-1;    // we count rows starting w/0
	}
	command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

// Turn the display on/off (quickly)
void LiquidCrystal_I2C::noDisplay() {
	_displaycontrol &= ~LCD_DISPLAYON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LiquidCrystal_I2C::display() {
	_displaycontrol |= LCD_DISPLAYON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turns the underline cursor on/off
void LiquidCrystal_I2C::noCursor() {
	_displaycontrol &= ~LCD_CURSORON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LiquidCrystal_I2C::cursor() {
	_displaycontrol |= LCD_CURSORON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turn on and off the blinking cursor
void LiquidCrystal_I2C::noBlink() {
	_displaycontrol &= ~LCD_BLINKON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LiquidCrystal_I2C::blink() {
	_displaycontrol |= LCD_BLINKON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// These commands scroll the display without changing the RAM
void LiquidCrystal_I2C::scrollDisplayLeft(void) {
	command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void LiquidCrystal_I2C::scrollDisplayRight(void) {
	command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void LiquidCrystal_I2C::leftToRight(void) {
	_displaymode |= LCD_ENTRYLEFT;
	command(LCD_ENTRYMODESET | _displaymode);
}

// This is for text that flows Right to Left
void LiquidCrystal_I2C::rightToLeft(void) {
	_displaymode &= ~LCD_ENTRYLEFT;
	command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'right justify' text from the cursor
void LiquidCrystal_I2C::autoscroll(void) {
	_displaymode |= LCD_ENTRYSHIFTINCREMENT;
	command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'left justify' text from the cursor
void LiquidCrystal_I2C::noAutoscroll(void) {
	_displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
	command(LCD_ENTRYMODESET | _displaymode);
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void LiquidCrystal_I2C::createChar(uint8_t location, uint8_t charmap[]) {
	location &= 0x7; // we only have 8 locations 0-7
	command(LCD_SETCGRAMADDR | (location << 3));
	for (int i=0; i<8; i++) {
		write(charmap[i]);
	}
}

// Turn the (optional) backlight off/on
void LiquidCrystal_I2C::noBacklight(void) {
	_backlightval=LCD_NOBACKLIGHT;
	expanderWrite(0);
}

void LiquidCrystal_I2C::backlight(void) {
	_backlightval=LCD_BACKLIGHT;
	expanderWrite(0);
}



/*********** mid level commands, for sending data/cmds */

inline void LiquidCrystal_I2C::command(uint8_t value) {
	send(value, 0);
}

inline size_t LiquidCrystal_I2C::write(uint8_t value) {
	send(value, Rs);
	return 1; // Number of processed bytes
}



/************ low level data pushing commands **********/

// write either command or data
void LiquidCrystal_I2C::send(uint8_t value, uint8_t mode) {
	uint8_t highnib = value & 0xF0;
	uint8_t lownib = value << 4;
	write4bits((highnib)|mode);
	write4bits((lownib)|mode);
}

void LiquidCrystal_I2C::write4bits(uint8_t value) {
	expanderWrite(value);
	pulseEnable(value);
}

void LiquidCrystal_I2C::expanderWrite(uint8_t _data){                                        
	Wire.beginTransmission(_Addr);
	Wire.write((int)(_data) | _backlightval);
	Wire.endTransmission();   
}

void LiquidCrystal_I2C::pulseEnable(uint8_t _data){
	expanderWrite(_data | En);	// En high
	delayMicroseconds(1);		// enable pulse must be >450ns
	
	expanderWrite(_data & ~En);	// En low
	delayMicroseconds(50);		// commands need > 37us to settle
} 

// Create custom characters for horizontal graphs
uint8_t LiquidCrystal_I2C::graphHorizontalChars(uint8_t rowPattern) {
  uint8_t cc[LCD_CHARACTER_VERTICAL_DOTS];
  for (uint8_t idxCol = 0; idxCol < LCD_CHARACTER_HORIZONTAL_DOTS; idxCol++) {
    for (uint8_t idxRow = 0; idxRow < LCD_CHARACTER_VERTICAL_DOTS; idxRow++) {
      cc[idxRow] = rowPattern << (LCD_CHARACTER_HORIZONTAL_DOTS - 1 - idxCol);
    }
    createChar(idxCol, cc);
  }
  return LCD_CHARACTER_HORIZONTAL_DOTS;
}

// Create custom characters for vertical graphs
uint8_t LiquidCrystal_I2C::graphVerticalChars(uint8_t rowPattern) {
  uint8_t cc[LCD_CHARACTER_VERTICAL_DOTS];
  for (uint8_t idxChr = 0; idxChr < LCD_CHARACTER_VERTICAL_DOTS; idxChr++) {
    for (uint8_t idxRow = 0; idxRow < LCD_CHARACTER_VERTICAL_DOTS; idxRow++) {
      cc[LCD_CHARACTER_VERTICAL_DOTS - idxRow - 1] = idxRow > idxChr ? B00000 : rowPattern;
    }
    createChar(idxChr, cc);
  }
  return LCD_CHARACTER_VERTICAL_DOTS;
}

// Initializes custom characters for input graph type
uint8_t LiquidCrystal_I2C::init_bargraph(uint8_t graphtype) {
  // Initialize row state vector
  for(byte i = 0; i < _rows; i++) {
    _graphstate[i] = 255;
  }
	switch (graphtype) {
		case LCDI2C_VERTICAL_BAR_GRAPH:
      graphVerticalChars(B11111);
      // Initialize column state vector
      for(byte i = _rows; i < _cols; i++) {
        _graphstate[i] = 255;
      }
			break;
		case LCDI2C_HORIZONTAL_BAR_GRAPH:
      graphHorizontalChars(B11111);
			break;
		case LCDI2C_HORIZONTAL_LINE_GRAPH:
      graphHorizontalChars(B00001);
			break;
		default:
			return 1;
	}
  _graphtype = graphtype;
	return 0;
}

// Display horizontal graph from desired cursor position with input value
void LiquidCrystal_I2C::draw_horizontal_graph(uint8_t row, uint8_t column, uint8_t len, uint8_t pixel_col_end) {
  // Maintain input parameters
  row = constrain(row, 0, _rows - 1);
  column = constrain(column, 0, _cols - 1);
  len = constrain(len, 0, _cols - column);
  pixel_col_end = constrain(pixel_col_end, 0, (len * LCD_CHARACTER_HORIZONTAL_DOTS) - 1);
  _graphstate[row] = constrain(_graphstate[row], column, column + len - 1);
  // Display graph
  switch (_graphtype) {
    case LCDI2C_HORIZONTAL_BAR_GRAPH:
      setCursor(column, row);
      // Display full characters
      for (uint8_t i = 0; i < pixel_col_end / LCD_CHARACTER_HORIZONTAL_DOTS; i++) {
        write(LCD_CHARACTER_HORIZONTAL_DOTS - 1);
        column++;
      }
      // Display last character
      write(pixel_col_end % LCD_CHARACTER_HORIZONTAL_DOTS);
      // Clear remaining chars in segment
      for (uint8_t i = column; i < _graphstate[row]; i++) write(' ');
      // Last drawn column as graph state
      _graphstate[row] = column;
      break;
    case LCDI2C_HORIZONTAL_LINE_GRAPH:
      // Drawn column as graph state
      column += pixel_col_end / LCD_CHARACTER_HORIZONTAL_DOTS;
      // Clear previous drawn character if differs from new one
      if (_graphstate[row] != column) {
        setCursor(_graphstate[row], row);
        write(' ');
        _graphstate[row] = column;
      }
      // Display graph character
      setCursor(column, row);
      write(pixel_col_end % LCD_CHARACTER_HORIZONTAL_DOTS);
      break;
		default:
			return;
  }
}
// Display horizontal graph from desired cursor position with input value
void LiquidCrystal_I2C::draw_vertical_graph(uint8_t row, uint8_t column, uint8_t len,  uint8_t pixel_row_end) {
  // Maintain input parameters
  row = constrain(row, 0, _rows - 1);
  column = constrain(column, 0, _cols - 1);
  len = constrain(len, 0, row + 1);
  pixel_row_end = constrain(pixel_row_end, 0, (len * LCD_CHARACTER_VERTICAL_DOTS) - 1);
  _graphstate[column] = constrain(_graphstate[column], row - len + 1, row);
  // Display graph
	switch (_graphtype) {
    case LCDI2C_VERTICAL_BAR_GRAPH:
      // Display full characters
      for (uint8_t i = 0; i < pixel_row_end / LCD_CHARACTER_VERTICAL_DOTS; i++) {
        setCursor(column, row--);
        write(LCD_CHARACTER_VERTICAL_DOTS - 1);
      }
      // Display the highest character
      setCursor(column, row);
      write(pixel_row_end % LCD_CHARACTER_VERTICAL_DOTS);
      // Clear remaining top chars in column
      for (uint8_t i = _graphstate[column]; i < row; i++) {
        setCursor(column, i);
        write(' ');
      }
      _graphstate[column] = row; // Last drawn row as its state
      break;
		default:
			return;
  }
}
// Overloaded methods
void LiquidCrystal_I2C::draw_horizontal_graph(uint8_t row, uint8_t column, uint8_t len, uint16_t percentage) {
  percentage = (percentage * len * LCD_CHARACTER_HORIZONTAL_DOTS / 100) - 1;
  draw_horizontal_graph(row, column, len, (uint8_t) percentage);
}
void LiquidCrystal_I2C::draw_horizontal_graph(uint8_t row, uint8_t column, uint8_t len, float ratio) {
  ratio = (ratio * len * LCD_CHARACTER_HORIZONTAL_DOTS) - 1;
  draw_horizontal_graph(row, column, len, (uint8_t) ratio);
}
void LiquidCrystal_I2C::draw_vertical_graph(uint8_t row, uint8_t column, uint8_t len,  uint16_t percentage) {
  percentage = (percentage * len * LCD_CHARACTER_VERTICAL_DOTS / 100) - 1;
  draw_vertical_graph(row, column, len, (uint8_t) percentage);
}
void LiquidCrystal_I2C::draw_vertical_graph(uint8_t row, uint8_t column, uint8_t len,  float ratio) {
  ratio = (ratio * len * LCD_CHARACTER_VERTICAL_DOTS) - 1;
  draw_vertical_graph(row, column, len, (uint8_t) ratio);
}

// Alias functions

void LiquidCrystal_I2C::on(){
	display();
}

void LiquidCrystal_I2C::off(){
	noDisplay();
}

void LiquidCrystal_I2C::cursor_on(){
	cursor();
}

void LiquidCrystal_I2C::cursor_off(){
	noCursor();
}

void LiquidCrystal_I2C::blink_on(){
	blink();
}

void LiquidCrystal_I2C::blink_off(){
	noBlink();
}

void LiquidCrystal_I2C::load_custom_character(uint8_t char_num, uint8_t *rows){
		createChar(char_num, rows);
}

void LiquidCrystal_I2C::setBacklight(uint8_t new_val){
	if(new_val){
		backlight();		// turn backlight on
	}else{
		noBacklight();		// turn backlight off
	}
}

void LiquidCrystal_I2C::printstr(const char c[]){
	//This function is not identical to the function used for "real" I2C displays
	//it's here so the user sketch doesn't have to be changed 
	print(c);
}
