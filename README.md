# LiquidCrystal_I2C
A reimplementation of the standard Arduino LCD library, configured to work
with parallel HD44780 compatible LCDs, and interfaced via a Chinese
PCF8574 I2C serial extender.

Version 2.2.0
--
- Implemented function for horizontal bar and line graphs from official API with help of custom characters

	`void draw_horizontal_graph(uint8_t row, uint8_t column, uint8_t len, uint8_t pixel_col_end)`

	- The function uses first **5 custom characters**.
	- Parameters **row** and **column** are position of a graph segment (part of a display row) counting from 0. They are limited to physical geometry of a display.
	- Parameter **len** is a length of a graph segment in characters. It is limited to remaining physical characters to the end of a display row .
	- Parameter **pixel\_col\_end** is a drawn value of a graph in horizontal pixels counting from 0. It is limited to physical pixels of the graph segment.
	- **Bar graph** mimics a progress bar. Value represents the length of the bar in pixels from the left.
	- **Line graph** mimics a tachograph. Value represents the position of pipe in pixels from the left.

- Implemented function for vertical bar graph from official API with help of custom characters

	`draw_vertical_graph(uint8_t row, uint8_t column, uint8_t len,  uint8_t pixel_row_end)`

	- The function uses all **8 custom characters**.
	- Parameters **row** and **column** are position of a graph segment (part of a display column) counting from 0. They are limited to physical geometry of a display.
	- Parameter **len** is a length of a graph segment in rows. It is limited to remaining physical rows to the top of a display.
	- Parameter **pixel\_row\_end** is a drawn value of a graph in vertical pixels counting from 0. It is limited to physical pixels of the graph segment.
	- **Vertical graph** mimics histogram or audio level monitor bar. Value represents the hight of the bar in pixels from bottom to top of the graph segment.

- Added overloaded graph functions for horizontal and vertical bar graphs

	`void draw_horizontal_graph(uint8_t row, uint8_t column, uint8_t len, float percentage)`
	`void draw_vertical_graph(uint8_t row, uint8_t column, uint8_t len,  float percentage)`

	- Difference is in the graph value, which is the **percentage of the graph segment** length. It avoids to express the graph value in physical units.

- Implemented graph initialization function from official API

	`uint8_t init_bargraph(uint8_t graphtype)`

	- Type of graph is determined by macros from official API
      - LCDI2C\_VERTICAL\_BAR_GRAPH
    	- LCDI2C\_HORIZONTAL\_BAR_GRAPH
    	- LCDI2C\_HORIZONTAL\_LINE_GRAPH

- Added private functions for creating graph custom characters

	`uint8_t graphHorizontalChars(uint8_t rowPattern)`
	`uint8_t graphVerticalChars(uint8_t rowPattern)`  

- Added macros for display character geometry
	- LCD\_CHARACTER\_HORIZONTAL\_DOTS
	- LCD\_CHARACTER\_VERTICAL\_DOTS


Version 2.1.0
--
- Added overloaded function for clearing particular sergment of a row

	`void clear(uint8_t rowStart, uint8_t colStart = 0, uint8_t colCnt = 255)`

	- All parameters are limited to physical geometry of a display.
	- For clearing the entire row, use function with the first parameter only

		`clear(rowStart)`  