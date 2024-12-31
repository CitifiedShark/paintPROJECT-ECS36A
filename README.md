# paintPROJECT-ECS36A

Paint a canvas via commandline
Type the following letters 
1.      q - quit
2.      h - help
3.      w - write
4.      a - add
5.      d - delete
6.      r - resize
7.      s - save
8.      l - load
9.      e - erase

* quit - closes the program and frees any dynamically allocated memory used
* help - displays help information
* write start_row start_column end_row end_column - Draws a line from start_row, start_column to end_row, end_column. Must be a straight line
* erase row col - erases the character at row col
* resize num_rows num_cols - resizes the canvas to be num_rows and num_cols in size
* add [r or c] position - adds a new row or column at the position given
* delete [r or c] position - deletes a row or column at the position given
* save file_name - saves the canvas to a file named file_name
* load file_name - loads the canvas from the file named file_name
