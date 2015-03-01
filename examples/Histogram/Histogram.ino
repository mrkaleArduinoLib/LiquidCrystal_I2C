/*
  NAME:
  Demo sketch for Histogram composed of Vertical Bar Graphs

  DESCRIPTION:
  The sketch demonstrates usage of LiquidCrystal_I2C library version 2.x
  for programing histograms with help of vertical graphs.
  * The sketch is intended preferrably for 16x2 LCD, but can be configured
    for 20x4 LCDs just by uncommenting and commenting related sections.
  * All graph values are displayed in number of vertical pixels.
  * The sketch demostrates a histogram
    - in second row with one row height
    - in full display are with
    with values changed randomly.
  * The sketch is just for demonstration purposes, so that it is not
    optimized for memory usage.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the MIT License (MIT).
    
  CREDENTIALS:
  Author: Libor Gabaj
  Version: 1.0.0
  Updated: 01.03.2015
*/

/*  Needed libraries
    Dispite the LCD library includes Wire library, the ArduinoIDE does not
    includes nested libraries, if they are not in the same folder.
*/
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD address and geometry for LCD 1602
const byte lcdAddr = 0x27;  // Typical address of I2C backpack for 1602
const byte lcdCols = 16;    // Number of characters in a row of display
const byte lcdRows = 2;     // Number of lines of display

// LCD address and geometry for LCD 2004
//const byte lcdAddr = 0x3F;  // Typical address of I2C backpack for 2004
//const byte lcdCols = 20;    // Number of characters in a row of display
//const byte lcdRows = 4;     // Number of lines of display

// Initialize library and setting LCD geometry
LiquidCrystal_I2C lcd(lcdAddr, lcdCols, lcdRows);

// Demo constants
const int graphDelay = 100;  // Delay between histograms
const int demoTime   = 5000; // Showing time of a demo

// Demo variables
byte graphPixelsCur, graphPixelsMax;
unsigned long demoStart;

void setup()
{
  // Initialize LCD
  lcd.init();
  lcd.backlight();  // Switch on the backlight LED, if any or wired
  
  /*  Initialize graph
      * Macro is defined in LiquidCrystal_I2C library.
      * Function uses all 8 custom character positions (0-7)
        and creates custom characters for displaying vertical bar.
  */
  lcd.init_bargraph(LCDI2C_VERTICAL_BAR_GRAPH);
}

void loop()
{
  // Demo 1: One row histogram
  lcd.clear();
  lcd.print("Histogram");
  graphPixelsMax = LCD_CHARACTER_VERTICAL_DOTS;
  demoStart = millis();
  while(millis() - demoStart < demoTime) {
    for(byte graphCol = 0; graphCol < lcdCols; graphCol++) {
      graphPixelsCur = random(0, graphPixelsMax);
      lcd.draw_vertical_graph(1, graphCol, 1, graphPixelsCur);
    }
    delay(graphDelay);
  }
  // Demo 2: Full display histogram
  lcd.clear();
  graphPixelsMax = lcdRows * LCD_CHARACTER_VERTICAL_DOTS;
  demoStart = millis();
  while(millis() - demoStart < demoTime) {
    for(byte graphCol = 0; graphCol < lcdCols; graphCol++) {
      graphPixelsCur = random(0, graphPixelsMax);
      lcd.draw_vertical_graph(lcdRows - 1, graphCol, lcdRows, graphPixelsCur);
    }
    delay(graphDelay);
  }
}
