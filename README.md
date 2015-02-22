# LiquidCrystal_I2C
A reimplementation of the standard Arduino LCD library, configured to work
with parallel HD44780 compatible LCDs, and interfaced via a Chinese
PCF8574 I2C serial extender.

Version 2.1.0
--
- Added overloaded function for clearing particular sergment of a row

	`void clear(uint8_t rowStart, uint8_t colStart = 0, uint8_t colCnt = 255);`

- For clearing the entire row, use function with only the first parameter

	`clear(rowStart)`  