<a id="library"></a>
# LiquidCrystal_I2C
It is the reimplementation of the standard Arduino LCD library, configured to work with parallel HD44780 compatible LCDs, and interfaced via a Chinese PCF8574 I2C serial extender.

<a id="examples"></a>
## Examples
##### Autoscroll
Demonstrates autoscroll function.

##### CompleteTest
Demo sketch for complete printing test of the LCD. The test suit consist of 9 tests each labeled in the first row.

##### CustomChars
Creation of 8 custom characters in form of graphical symbols.

##### HelloWorld
Standard initial example. Use it for checking basic functionality and address of the LCD.

##### Histogram
Demo sketch for several histograms composed of vertical bar graphs. The values for graphs are generated randomly.
1. The first demo histogram is displayed in one row only.
1. The second demo histogram display across all rows of the LCD and uses its entire screen.

##### HorizontalBarGraph
Demo sketch for horizontal bar graphs.
1. The first demo histogram uses full row for continues increasing and decreasing values (breathing graph).
1. The second demo histogram is a breathing graph using just one half of a row.
1. The third demo histogram displays values generated randomly.

##### HorizontalLineGraph
Demo sketch for horizontal line graphs. A value is represented just with a pipe on a row.
1. The first demo histogram uses full row for continues increasing and decreasing values (running graph).
1. The second demo histogram is a running graph using just one half of a row.
1. The third demo histogram displays values generated randomly.

##### MultipleLcd
Using multiple LCD on the same I2C bus but communicating on different addresses.

##### Scroll
Demonstrates scrolling text to the left and right without changing text.

##### SerialDisplay
Sketch receives characters from the serial port and displays them on the LCD one by one.

##### VerticalBarGraph
Demo sketch for vertical bar graph. The graph uses all rows in the last column for continues increasing and decreasing values (breathing graph).