/*
  NAME:
  Demonstration of Autoscroll function

  DESCRIPTION:
  This sketch demonstrates the use of the autoscroll() and noAutoscroll()
  functions to make new text scroll or not.
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
const byte lcdScrollRow = 0;          // Number of a demo row counting from 0
const unsigned int digitDelay = 500;  // Miliseconds before displaying next digit

// Function for displaying demo digits
void printDigits() {
  for (byte thisChar = 0; thisChar < 10; thisChar++) {
    lcd.print(thisChar);
    delay(digitDelay);
  }
}

void setup() {
  lcd.init();
  lcd.backlight();
}

void loop() {
  lcd.setCursor(0, lcdScrollRow);
  printDigits();

  // Set the cursor to the last column of the demo row and turn on autoscroll
  lcd.setCursor(lcdCols, lcdScrollRow);
  lcd.autoscroll();
  printDigits();
  lcd.noAutoscroll();
  lcd.clear();
}

