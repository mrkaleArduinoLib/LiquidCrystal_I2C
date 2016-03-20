/*
  NAME:
  Demonstration scrolling text to the left and right without changing text.

  DESCRIPTION:
  This sketch demonstrates the use of the scrollDisplayLeft() and
  scrollDisplayRight() functions to make new text scroll to the left and right.
  * The sketch is intended preferrably for 16x2 LCD, but can be configured
    for 20x4 LCDs just by uncommenting and commenting related sections.
  * The sketch is just for demonstration purposes, so that it is not
    optimized for memory usage.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the MIT License (MIT).
    
  CREDENTIALS:
  Author: Libor Gabaj
  Version: 1.0.0
  Updated: 20.03.2016
  
  CREDIT:
  The example taken and rewritten for I2C from official Arduino standard library
  (https://github.com/arduino/Arduino/tree/master/libraries/LiquidCrystal)
*/
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD address and geometry and library initialization
const byte lcdAddr = 0x27;  // Address of I2C backpack
const byte lcdCols = 16;    // Number of character in a row
const byte lcdRows = 2;     // Number of lines
//const byte lcdAddr = 0x3F;  // Address of I2C backpack
//const byte lcdCols = 20;    // Number of character in a row
//const byte lcdRows = 4;     // Number of lines

LiquidCrystal_I2C lcd(lcdAddr, lcdCols, lcdRows);

// Demo parameters
const char demoText[]= "Hello World!";
const unsigned int scrollDelay = 500;   // Miliseconds before scrolling next char
const unsigned int demoDelay = 2000;    // Miliseconds between demo loops
byte textLen;                           // Number of visible characters in the text

void setup() {
  textLen = sizeof(demoText) - 1;
  lcd.init();
  lcd.backlight();
  lcd.print(demoText);
  delay(demoDelay);
}

void loop() {
  // Scroll entire text in a row to the left outside the screen
  for (byte positionCounter = 0; positionCounter < textLen; positionCounter++) {
    lcd.scrollDisplayLeft();
    delay(scrollDelay);
  }
  // Scroll hidden text through entire row to the right outside the screen
  for (byte positionCounter = 0; positionCounter < textLen + lcdCols; positionCounter++) {
    lcd.scrollDisplayRight();
    delay(scrollDelay);
  }
  // Scroll text to the right back to original position
  for (byte positionCounter = 0; positionCounter < lcdCols; positionCounter++) {
    lcd.scrollDisplayLeft();
    delay(scrollDelay);
  }
  delay(demoDelay);
}

