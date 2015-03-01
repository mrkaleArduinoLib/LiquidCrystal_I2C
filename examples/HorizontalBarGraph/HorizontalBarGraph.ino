/*
  NAME:
  Demo sketch for Horizontal Bar Graph

  DESCRIPTION:
  The sketch demonstrates usage of LiquidCrystal_I2C library version 2.x
  for programing horizontal graphs, which mimics progress bar.
  * The sketch is intended preferrably for 16x2 LCD, but can be configured
    for 20x4 LCDs just by uncommenting and commenting related sections.
  * All graph values are displayed in number of horizontal pixels.
  * The sketch demostrates
    - 1. Continues full row progress bar
    - 2. Continues half row central progress bar
    - 3. Random full row progress bar
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
const byte graphRow = 1;  // In this row the graph is displayed
const byte labelRow = 0;  // In this row the label is displayed
const byte valueCol = lcdCols - 2; // In this column starts graph value
const int graphDelay = 100;  // Delay between graph values in ms
const int demoDelay  = 3000; // Delay between demos in ms

// Demo parameters
const char  graphType[] = " Bar";
const char* graphLbls[] = {"Full", "Half", "Random"};
const byte  graphLens[] = {lcdCols, lcdCols / 2, lcdCols};
const byte  graphCols[] = {0, lcdCols / 4, 0};

// Demo variables
byte graphPixelsCur, graphPixelsMax;

// Function for displaying graph label
void printLabel(byte demo) {
  // Create label
  char labelText[valueCol];
  sprintf(labelText, "%1u.%s%s", demo + 1, graphLbls[demo], graphType);
  // Display label on clear display
  lcd.clear();
  lcd.setCursor(0, labelRow);
  lcd.print(labelText);
  
}

// Function for displaying graph value
void printValue(byte value) {
  lcd.clear(labelRow, valueCol);      // Clear value space
  lcd.setCursor(valueCol, labelRow);
  lcd.print(value);
}

void setup()
{
  // Initialize LCD
  lcd.init();
  lcd.backlight();  // Switch on the backlight LED, if any or wired
  
  /*  Initialize graph
      * Macro is defined in LiquidCrystal_I2C library.
      * Function uses the first 5 custom character positions (0-4)
        and creates custom characters for displaying progress bar.
  */
  lcd.init_bargraph(LCDI2C_HORIZONTAL_BAR_GRAPH);
}

void loop()
{
  for(byte demoNum = 0; demoNum < sizeof(graphLens)/sizeof(graphLens[0]); demoNum++) {
    graphPixelsMax = graphLens[demoNum] * LCD_CHARACTER_HORIZONTAL_DOTS;
    printLabel(demoNum);
    switch (demoNum) {
      case 0:
      case 1:
        // Demo 1: Graph in full row with sequence values
        // Demo 2: Graph in half row with sequence values
        // Descending graph values
        for (byte i = graphPixelsMax; i > 0; i--) {
          graphPixelsCur = i - 1;
          printValue(graphPixelsCur);
          lcd.draw_horizontal_graph(graphRow, graphCols[demoNum], graphLens[demoNum], graphPixelsCur);
          delay(graphDelay);
        }
        // Ascending graph values
        for (byte i = 0; i < graphPixelsMax; i++) {
          graphPixelsCur = i;
          printValue(graphPixelsCur);
          lcd.draw_horizontal_graph(graphRow, graphCols[demoNum], graphLens[demoNum], graphPixelsCur);
          delay(graphDelay);
        }
        break;
      case 2:
        // Demo 3: Graph in full row with random values
        for (byte i = 0; i < graphPixelsMax; i++) {
          graphPixelsCur = random(0, graphPixelsMax);
          printValue(graphPixelsCur);
          lcd.draw_horizontal_graph(graphRow, graphCols[demoNum], graphLens[demoNum], graphPixelsCur);
          delay(graphDelay);
        }
        break;
    }
    delay(demoDelay);
  }
}

