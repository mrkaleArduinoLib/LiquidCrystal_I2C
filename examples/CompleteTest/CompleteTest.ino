/*
  NAME:
  Demo sketch for complete printing test of LCD

  DESCRIPTION:
  The sketch demonstrates capabalities of the LCD by displaying several
  test.
  * The sketch is intended preferrably for 16x2 LCD, but can be configured
    for 20x4 LCDs just by uncommenting and commenting related sections.
  * The sketch is just for demonstration purposes, so that it is not
    optimized for memory usage.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the MIT License (MIT).
  
  CREDITS:
  Inspired by the example LCD_Test in the library LCDi2cW
  from "4-2-2009 dale@wentztech.com".
    
  CREDENTIALS:
  Author: Libor Gabaj
  Version: 1.1.0
  Updated: 04.03.2015
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
const int testDelay = 500;  // Delay between tests in ms
const int demoDelay = 3000;  // Delay between demos in ms
const byte demoNumMin = 1;    // Range of demo tests
const byte demoNumMax = 255;
const byte charNumMin = 0;    // Code of first displayed character
const byte charNumMax = 255;  // Code of last displayed character

// Demo variables
byte col, row;
unsigned int demoNum, charNum;
char buffer[lcdCols + 1];

void setup() { 
  // Initialize LCD
  lcd.init();
  lcd.backlight();  // Switch on the backlight LED, if any or wired
}

void loop()
{
  demoNum = max(demoNumMin, 1);
  while(demoNum >= demoNumMin && demoNum <= demoNumMax) {
    lcd.clear();
    lcd.cursor_off();
    lcd.blink_off();
    sprintf(buffer, "%u.", demoNum);
    lcd.print(buffer);
    
    switch (demoNum) {
      case 1:
        lcd.print(F("Hello World!"));
        break;
      case 2:
        lcd.print(F("Dash Cursor"));
        lcd.setCursor(0, 1);
        lcd.cursor_on();
        break;
      case 3:
        lcd.print (F("Block Cursor"));
        lcd.setCursor(0, 1);
        lcd.blink_on();
        break;
      case 4:
        lcd.print(F("No Cursor"));
        lcd.setCursor(0, 1);
        break;
      case 5:
        lcd.print(F("Characters"));
        lcd.cursor_on();
        charNum = charNumMin;
        while(charNum <= charNumMax) {
          row = 1;
          do {
            lcd.clear(row);
            col = 0;
            lcd.setCursor(col, row);
            do {
              lcd.write(char(charNum++));
              delay(testDelay);
            } while(++col < lcdCols && charNum <= charNumMax);
          } while(++row < lcdRows && charNum <= charNumMax);
        }        
        lcd.cursor_off();
        break;
      case 6:
        for (byte row=0; row < lcdRows; row++) {
          lcd.print(F("Line "));
          lcd.print(row);
          delay(testDelay);
          lcd.setCursor(0, row + 1);
        }
        break;
      case 7:
        lcd.print(F("Count to 255"));
        for (unsigned int i = 0; i < 256; i++) {
          lcd.clear(1);
          sprintf(buffer, "%03u 0x%02X %c", i, i, i);
          lcd.print(buffer);
          if (lcdCols >= 20) {
            lcd.print(" B");
            lcd.print(i, BIN);
          }
          delay(testDelay);
        }
        break;
      case 8:
        lcd.print(F("Positions"));
        lcd.setCursor(0, 1);
        for (byte col = 0; col < lcdCols; col++) {
          lcd.write(col%10 + char('0'));
          delay(testDelay);
        }
        break;
      case 9:
        lcd.print(F("Clearing"));
        for (byte row = 1; row < lcdRows; row++) {
          // Fill row
          lcd.setCursor(0, row);
          for (byte col = 0; col < lcdCols; col++) {
            lcd.write(col%10 + char('0'));
          }
          delay(testDelay);
          // Clear row
          for (byte i = 0; i < lcdCols / 2; i++) {
            lcd.setCursor(lcdCols / 2 - i - 1, row);
            lcd.write(' ');
            lcd.setCursor(lcdCols / 2 + i, row);
            lcd.write(' ');
            delay(testDelay);
          }
        }
        break;
      default:
        demoNum = 0;
        continue;
    }
    delay(demoDelay);
    demoNum++;
  }
}
