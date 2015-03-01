/*
  NAME:
  Demo sketch for Vertical Bar Graph

  DESCRIPTION:
  The sketch demonstrates usage of LiquidCrystal_I2C library version 2.x
  for programing vertical graphs, which mimics histogram.
  * The sketch is intended preferrably for 16x2 LCD, but can be configured
    for 20x4 LCDs just by uncommenting and commenting related sections.
  * All graph values are displayed in number of vertical pixels.
  * The sketch demostrates vertical graphs from one row graph to full rows
    graph of the display.
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

// Cursor coordinates and character row pattern for progress bar
const byte graphCol = lcdCols - 1;  // In this column the graph is displayed
const int graphDelay = 200;  // Delay between graph values in ms
const int demoDelay  = 3000; // Delay between demos in ms

// Demo parameters
const char  graphType[] = " Col(s) Graph";

// Demo variables
byte graphPixelsCur, graphPixelsMax;

// Function for displaying graph label
void printLabel(byte rows) {
  const byte labelCol = 0;
  const byte labelRow = 0;
  // Create label
  char labelText[graphCol];
  sprintf(labelText, "%1u%s", rows, graphType);
  // Display label on clear display
  lcd.clear();
  lcd.setCursor(labelCol, labelRow);
  lcd.print(labelText);
  
}

// Function for displaying graph value
void printValue(byte value) {
  const byte valueWidth = 2; // Max. digits in value
  const byte valueCol = graphCol - valueWidth - 1;
  const byte valueRow = 1;
  // Create value
  char valueFormat[4], valueText[valueWidth + 1];
  sprintf(valueFormat, "%%%1uu", valueWidth);
  sprintf(valueText, valueFormat, value);
  // Display label on clear display
  lcd.clear(valueRow, valueCol, valueWidth);
  lcd.setCursor(valueCol, valueRow);
  lcd.print(valueText);
}

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
  for(byte graphHight = 1; graphHight <= lcdRows; graphHight++) {
    graphPixelsMax = graphHight * LCD_CHARACTER_VERTICAL_DOTS;
    printLabel(graphHight);
    for (byte i = graphPixelsMax; i > 0; i--) {
      graphPixelsCur = i - 1;
      printValue(graphPixelsCur);
      lcd.draw_vertical_graph(graphHight - 1, graphCol, graphHight, graphPixelsCur);
      delay(graphDelay);
    }
    // Ascending graph values
    for (byte i = 0; i < graphPixelsMax; i++) {
      graphPixelsCur = i;
      printValue(graphPixelsCur);
      lcd.draw_vertical_graph(graphHight - 1, graphCol, graphHight, graphPixelsCur);
      delay(graphDelay);
    }
    delay(demoDelay);
  }
}
