#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef struct Canvas{
    // This struct contains the canvas and the size of it
    char** board;
    int num_rows;
    int num_cols;
}Canvas;

void print_canvas(Canvas* canvas) {
    int rows = canvas->num_rows;
    int cols = canvas->num_cols;
/*
This function shows the canvas to the user
@param canvas: current canvas
*/
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%c", canvas->board[i][j]);
        }
        printf("\n");
    }
}

void quit(char* command, Canvas* canvas) {
/*
This functions ceases the execution of the program and frees memory
@param command: gives string of command from user input
@param canvas: gives canvas (matrix) that we need to free up
*/
    // TODO: free memory
    free(command);

    int rows = canvas->num_rows;
    for (int i = 0; i < rows; i++) {
        free((canvas->board)[i]);
    }
    free(canvas->board);

    // Cease execution of the program 
    exit(1);
}

void print_help(){
/*
This function acts for the print help command
*/
  printf("Commands:\n");
  printf("Help: h\n");
  printf("Quit: q\n");
  printf("Draw line: w row_start col_start row_end col_end\n");
  printf("Resize: r num_rows num_cols\n");
  printf("Add row or column: a [r | c] pos\n");
  printf("Delete row or column: d [r | c] pos\n");
  printf("Erase: e row col\n");
  printf("Save: s file_name\n");
  printf("Load: l file_name\n");
}

int write_lines(Canvas* canvas, int start_row, int start_column, int end_row, int end_column) {
/*
This function draws the lines given inputs from write function
Writes from start to end points
@param canvas: gives the current canvas
@param start_row: starting row number
@param start_column: starting column number
@param end_row: ending row number
@param end_column: ending column number
@returns 0 to end function
*/

    // Draw only a horizontal line
    if (start_row == end_row) { // We know it's on the same row when they are equal to each other
        if (start_column > end_column) { // If the end column is greater, then we have to count backwards
            for (int i = end_column; i < start_column; i++) {
                if ((canvas->board)[start_row][i] == '|') {
                    (canvas->board)[start_row][i] = '+';
                } else {
                    (canvas->board)[start_row][i] = '-';
                }
            }
        } else {
            for (int i = start_column; i < end_column; i++) {
                // If there is already a vertical line, turn it into an intersection
                if ((canvas->board)[start_row][i] == '|') {
                    (canvas->board)[start_row][i] = '+';
                } else {
                    (canvas->board)[start_row][i] = '-';
                }
            }
        }
    // Draw only a vertical line
    } else if (start_column == end_column) {
        if (start_row > end_row) {
            for (int j = end_row; j < start_row; j++) {
                if ((canvas->board)[j][start_column] == '-') { // Create an intersection
                    (canvas->board)[j][start_column] = '+';
                } else {
                    (canvas->board)[j][start_column] = '|';
                }
            }
        } else {
            for (int j = start_row; j < end_row; j++) {
                // If there is already a horizontal line, turn it into an intersection
                if ((canvas->board)[j][start_column] == '-') {
                    (canvas->board)[j][start_column] = '+';
                } else {
                    (canvas->board)[j][start_column] = '|';
                }
            } 
        }
    // Draw a right diagonal line /
    // Use rate of change equation
    } else {
        // Checks if it's a straight line
        int is_straight = (end_row - start_row) % (end_column - start_column);
        // Checks if it's an increasing line or a decreasing line
        int is_increasing = (end_row - start_row) / (end_column - start_column);
        if (is_straight == 0) { // If it is a straight line
            if (is_increasing > 0) { // If line is increasing, we put the right diagonal
                for (int j = end_column ; j >= start_column; j--) {
                    if ((canvas->board)[j][end_column - (j + 1)] == '\\') { // Create an intersection
                        (canvas->board)[j][j] = 'X';
                    } else {
                        (canvas->board)[j][end_column - (j + 1)] = '/';
                    }
                }
            } else if (is_increasing < 0) { // If line is decreasing, we put the left diagonal
                for (int i = start_column; i < end_column; i++) {
                    if ((canvas->board[i][i] == '/')) { // Create an intersection
                        (canvas->board)[i][i] = 'X';
                    } else {
                        (canvas->board)[i][i] = '\\';
                    }
                }
            }
        } else { // Else, tell the user it is not a straight line and that we won't be drawing it
            printf("Coordinates do not form a straight line, not drawing line...\n");
            return 0;
        }
    }

    return 0;
}

bool write_valid_input(Canvas* canvas, int start_row, int start_column, int end_row, int end_column) {
/*
This function checks for valid input for the write function
@param canvas: Gives current canvas
@param start_row: starting row number
@param start_column: starting column number
@param end_row: ending row number
@param end_column: ending column number
@returns true if valid input, false otherwise
*/
    int rows = canvas->num_rows;
    int cols = canvas->num_cols;

    // If the number of rows given by the user are less than 0 or greater than the number of rows in the canvas
    if (start_row > rows || start_row < 0) { 
        printf("Invalid command\n");
        return false;
    } else if (start_column > cols || start_column < 0) {
        printf("Invalid command\n");
        return false;
    } else if (end_row > rows || end_row < 0) {
        printf("Invalid command\n");
        return false;
    } else if (end_column > cols || end_column < 0) {
        printf("Invalid command\n");
        return false;
    }

    return true;
}

int write_find_number(char* command, int* index) {
/*
This function reads the command given by the line
and returns the value to the variable for row or column
@param command: the command inputted by the user
@param index: the index of the command we are at
@returns number: the number at that index of the command
*/
    int number = 0;

    // Skip over whitespace
    while (command[*index] && !isdigit(command[*index])) {
        (*index)++;
    }

    // Assign the number and iterate through
    while (command[*index] && isdigit(command[*index])) {
        number = number * 10 + (command[*index] - '0'); \
        // When a single digit, number is 0 and multiplies by 10
        // Thus, we only set number as the number at that index
        (*index)++;
    }

    return number;
}

int write(char* command, int length, Canvas* canvas) {
/*
This function draws a line from start_row, start_column to end_row, end_column 
and does this by reading the command given by the line
@param command: command inputted by the user
@param length: length of command string given by user
@param canvas: the current canvas
@return 0: to end the program
*/   
    // Assigning unused values so we know which variables don't have an input assigned to them yet
    int start_row = -1;
    int start_column = -1; 
    int end_row = -1;
    int end_column = -1;
    // FORMAT: command start_row start_column end_row end_column

    // Assign the values from the command
    int index = 0;

    start_row = write_find_number(command, &index);
    start_column = write_find_number(command, &index);
    end_row = write_find_number(command, &index);
    end_column = write_find_number(command, &index);

    // Check for valid input
    bool valid_input;
    valid_input = write_valid_input(canvas, start_row, start_column, end_row, end_column);
    if (valid_input == false) { // If not valid input, end function
        return 0;
    }

    // Draw the lines
    write_lines(canvas, start_row, start_column, end_row, end_column);

    return 0;
}

int erase_find_number(char* command, int* index) {
/*
This function reads the command and assigns the value
to find where we will erase
@param command: the command inputted by the usre
@param index: the position of the command we are at
@returns number: for the value of the row or col
*/
    int number = 0;

    // Skip over whitespace
    while (command[*index] && !isdigit(command[*index])) {
        (*index)++;
    }

    // Assign the number from the command
    while (command[*index] && isdigit(command[*index])) {
        number = number * 10 + (command[*index] - '0');
        (*index)++;
    }

    return number;
}

void erase(char* command, int length, Canvas* canvas) {
/*
This function helps erase what is on the canvas
@param command: the command inputted by the user
@param length: the length of the command string
@param canvas: the current canvas
*/
    // The index of the command we are at
    int index = 0;

    // Obtain values for row and column
    int row = erase_find_number(command, &index);
    int col = erase_find_number(command, &index);

    // Set to erased character
    (canvas->board)[row][col] = '*';
} 

void add_row_or_col(Canvas* canvas, char row_or_col, int position) {
/*
This function adds the row or column to the canvas
@param canvas: is the current canvas
@param row_or_col: is the character to tell us if we are adding a row or column
@param position: is the integer that tells us at what position we are adding the row or column
*/   
    // Add a row
    if (row_or_col == 'r') {
        // Increase row size to be 1 bigger
        char** temp = realloc((canvas->board), (canvas->num_rows + 1) * sizeof(char*));
        canvas->board = temp;

        // Shift every row forward that is past the position
        for (int i = canvas->num_rows; i > position; i--) {
            (canvas->board)[i] = (canvas->board)[i - 1];
        }

        // Give the size of the new row
        (canvas->board)[position] = malloc(canvas->num_cols * sizeof(char));

        // Give an empty character to the new row
        for (int i = 0; i < canvas->num_cols; i++) {
            (canvas->board)[position][i] = '*';
        }

        // Update canvas struct values
        canvas->num_rows++;
    // Add a column
    } else if (row_or_col == 'c') {
        // Create the temporary matrix
        char** temp = (canvas->board);
        // Iterating through every row
        for (int i = 0; i < canvas->num_rows; i++) {
            // Increase column size to be 1 bigger
            temp[i] = realloc(temp[i], (canvas->num_cols + 1) * sizeof(char));

            // Shift every column forward that is past the position
            for (int j = canvas->num_cols; j > position; j--) {
                temp[i][j] = temp[i][j - 1];
            }

            // Give an empty character to the new column
            temp[i][position] = '*';
        }
        // Update canvas struct values
        canvas->num_cols++;
        canvas->board = temp;
    }
}

bool add_valid_input(Canvas* canvas, char row_or_col, int position) {
/*
This function determines if row_or_col and position has valid input by not going out of bounds
@param canvas: is the current canvas
@param row_or_col: is a character for whether we are inserting a row or column
@param position: is the integer for where we are inserting a new row or column
@returns true or false depending on if row_or_col is valid input or not
*/
    if ((row_or_col == 'r' && position > canvas->num_rows) ||
        (row_or_col == 'c' && position > canvas->num_cols)) {
            printf("Error: Position out of bounds.\n");
            return false;
    } else {
        return true;
    }
}

void add_find_input(char* command, int* index, char* row_or_col, int* position) {
/*
This function finds the values for row_or_col and position from the command
@param command: is the command inputted by the user
@param index: is the position that we are reading in the command
@param row_or_col: is the character for whether we are inserting a row or column
@param position: is the integer for where we are inserting a new row or column
*/
    // Skip whitespace
    while (command[*index] && isspace(command[*index])) {
        (*index)++;
    }

    // If it is a letter, take in the value r or c for row or column
    while (command[*index] && isalpha(command[*index])) {
        *row_or_col = command[*index];
        (*index)++;
    }

    // If it is a number, take in the value for the position where we are inserting at
    while (command[*index] && isdigit(command[*index])) {
        *position = *position * 10 + (command[*index] - '0');
        (*index)++;
    }
}

void add(char* command, Canvas* canvas) {
/*
This function adds a new row or column at the specified position
@param command: is the command inputted by the user
@param canvas: is the current canvas
*/
    int index = 1; // The position of the command string that we are reading
    char row_or_col = '\0'; // whether we are adding a row or col
    int position = 0; // The place where we are adding a row or col

    // Find values in command
    add_find_input(command, &index, &row_or_col, &position);

    // Check valid input
    bool valid_input = add_valid_input(canvas, row_or_col, position);
    if (!valid_input) { // End function if not valid input
        return;
    }

    // Add the row or column
    add_row_or_col(canvas, row_or_col, position);
}

bool delete_valid_input(Canvas* canvas, char row_or_col, int position) {
/*
This function checks for valid input for if position is within the boundaries
@param canvas: is the current canvas
@param row_or_col: is the character for whether we are adding a row or column
@param position: is the integer for the position at which we are adding the row or column
*/
    if ((row_or_col == 'r' && position >= 0 && position <= (canvas->num_rows - 1)) ||
        (row_or_col == 'c' && position >= 0 && position <= (canvas->num_cols - 1))) {
            return true;
    } else {
        printf("Error: Position out of bounds\n");
        return false;
    }
    
}

void delete_row_or_col(Canvas* canvas, char row_or_col, int position) {
/*
This function deletes the row or column at the specified position
@param canvas: is the current canvas
@param row_or_col: is the character for whether we are adding a row or column
@param position: is the integer for the position at which we are deleting the row or column
*/
    // Create the temporary matrix
    char** temp = (canvas->board);

    // Delete row
    if (row_or_col == 'r') {
        // Shift everything first so that the don't lose the values when we realloc

        // Shift every column down that is past the position
        for (int i = position; i < canvas->num_rows; i++) {
            temp[i] = temp[i + 1];
        }

        // Decrease row size to be 1 smaller
        temp = realloc((canvas->board), (canvas->num_rows - 1) * sizeof(char*));

        // Update canvas struct
        canvas->num_rows--;
        canvas->board = temp;
        return;
    // Delete column   
    } else if (row_or_col == 'c') {
        // Iterate through every row
        for (int i = 0; i < canvas->num_rows; i++) {
            // Shift every column backward that is past the position
            for (int j = position; j < canvas->num_cols; j++) {
                temp[i][j] = temp[i][j + 1];
            }

            // Decrease column size to be 1 smaller
            temp[i] = realloc(temp[i], (canvas->num_cols - 1) * sizeof(char));
        }

        // Update canvas struct
        canvas->num_cols--;
        canvas->board = temp;
        return;
    }
}

void delete(Canvas* canvas, char* command) {
/*
This function deletes a row or column at the specified position
@param canvas: is the current canvas
@param command: is the string for the command inputted by the user
*/
    int index = 1; // The position of the command string that we are reading
    char row_or_col = '\0'; // whether we are adding a row or col
    int position = 0; // The place where we are adding a row or col

    // Find values in command
    add_find_input(command, &index, &row_or_col, &position);

    // Check valid input
    bool valid_input = delete_valid_input(canvas, row_or_col, position);
    if (!valid_input) {
        return;
    }

    // Delete the row or column
    delete_row_or_col(canvas, row_or_col, position);
}

int resize_find_number(char* command, int* index) {
/*
This function reads the input from the command string and gives back the number of rows or columns we want to resize the canvas by
@param command: is the command inputted by the user
@param index: is the position of the command that we are reading at
@returns the number for the canvas to be num_rows or num_cols big
*/
    int number = 0;

    // Skip whitespace
    while (command[*index] && isspace(command[*index])) {
        (*index)++;
    }

    // If it is an integer, read it and assign it to number
    while (command[*index] && isdigit(command[*index])) {
        number = number * 10 + (command[*index] - '0');
        (*index)++;
    }

    return number;
}

void resize(char* command, Canvas* canvas) {
/*
This function resizes the canvas to be num_rows by num_cols big
@param command: is the string that the user inputted
@param canvas: is the current canvas
*/
    int index = 1;

    // Find num_rows and num_cols to change canvas size
    int num_rows = resize_find_number(command, index);
    int num_cols = resize_find_number(command, index);
    

}

void save_find_file(char* command, char* file_name) {
/*
This function finds the name of the file in the command inputted by the user
and assigns the file name to the string variable
@param command: is the command inputted by the user
@param file_name: is the name of the file that we will assign
*/
    // The position of the command string
    int index = 1;

    // Skip over whitespace
    while (command[index] && isspace(command[index])) {
        index++;
    }

    // Assign a letter to file_name
    while (command[index] && !isspace(command[index])) {
        file_name[index - 2] = command[index];
        index++;
    }
    file_name[strlen(file_name)] = '\0';

}

void save_to_file(Canvas* canvas, char* file_name) {
/*
This function opens the file  and saves the canvas to it
@param canvas: the current canvas to be saved
@param file_name: is the file name given by the user to save to
*/
    // Number of columns and rows in the canvas
    int rows = canvas->num_rows;
    int cols = canvas->num_cols;

    FILE* file = fopen(file_name, "w");

    // Save canvas to file
    fprintf(file, "%d %d\n", rows, cols); // dimensions of canvas
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fprintf(file, "%c", (canvas->board)[i][j]);
        }
        fprintf(file, "\n");
    }
    // Tell user of completion
    printf("file saved as %s\n", file_name);

    // Close the file
    fclose(file);
}

void save(char* command, Canvas* canvas, int length) {
/*
This function saves the current canvas to the file named file_name
@param command: is the command inputted by the user
@param canvas: is the current canvas
@param length: is the length of the command string
*/
    // Allocate memory for the name of the file
    int file_name_length = length - 2;
    char* file_name = malloc((file_name_length) * sizeof(char));

    // Find name of the file
    save_find_file(command, file_name);

    // Save canvas into the file
    save_to_file(canvas, file_name);

    // Free memory for file_name string
    free(file_name);
}

void load_realloc_canvas(Canvas* canvas, int canvas_row, int canvas_col) {
/*
Creates a new canvas with dimensions according to the canvas in file
@param canvas: the current canvas
*/
    // If the canvas from the file has different dimensions, then set the current canvas to those dimensions
    // Else, just leave it as the same dimensions
    (canvas->num_rows) = (canvas->num_rows != canvas_row) ? canvas_row : canvas->num_rows;
    (canvas->num_cols) = (canvas->num_cols != canvas_col) ? canvas_col : canvas->num_cols;

    // make a new matrix
    char** new_matrix = (char**)malloc(canvas->num_rows * sizeof(char*));
    for (int i = 0 ; i < canvas->num_rows; i++) {
        new_matrix[i] = (char*)malloc(canvas->num_cols * sizeof(char));
    }

    // assign it to the canvas
    (canvas->board) = new_matrix;
}

void load_from_file(Canvas* canvas, char* file_name) {
/*
Load the canvas from the file onto the current canvas
@param canvas: is the current canvas that we will load into
@param file_name: is the name of the file that we want to load in
*/
    FILE* file = fopen(file_name, "r");
    int canvas_row;
    int canvas_col;
    
    // Read for how many rows and columns there are
    fscanf(file, "%d %d", &canvas_row, &canvas_col); 
    
    // Change the size of the canvas
    load_realloc_canvas(canvas, canvas_row, canvas_col);

    // Assign the values from the file 
    char canvas_char;
    for (int i = 0; i < canvas_row; i++) {
        for (int j = -1; j < canvas_col; j++) {
            fscanf(file, "%c", &canvas_char);
            if (canvas_char != '\n') { // Skip the newline character
                (canvas->board)[i][j] = canvas_char;
            }
        }
    }

    fclose(file);
}

void load(Canvas* canvas, char* command, int length) {
/*
This function loads a canvas from the file with file_name
@param canvas: is the current canvas
@param command: is the command the user inputted
@param length: is the length of the command string
*/
    // Get the file_name
    int file_name_length = length - 2;
    char* file_name = malloc((file_name_length) * sizeof(char));

    save_find_file(command, file_name);

    // Load the canvas from the file
    load_from_file(canvas, file_name);
}

void read_command(Canvas* canvas) {
/*
This function reads the commands given by the user
@param canvas: is the current canvas
*/
    int size = 100;
    int length = 0;

    // Allocating memory for the command string
    char* command = (char*)malloc(sizeof(char) * size);

    // Keep asking for input until program is quit
    printf("Enter your command: ");
    while (fgets(command + length, size - length, stdin)) {
        length += strlen(command + length);   

        // If the character is a newline, switch it to a null character
        if (command[length - 1] == '\n') {
            command[length - 1] = '\0';
            break;
        } 

        size += 1;
        command = realloc(command, size * sizeof(char));
    }

    // TODO: if it is a specific command, it will call the function of that command
    /*
    1.      q - quit
    2.      h - help
    3.      w - write
    4.      a - add
    5.      d - delete
    6.      r - resize
    7.      s - save
    8.      l - load
    9.      e - erase
    */
    if (command[0] == 'q') { // quit
        quit(command, canvas);
    } else if (command[0] == 'h') { // help
        print_help();
        read_command(canvas);
    } else if (command[0] == 'w') { // write
        write(command, length, canvas);
        print_canvas(canvas);
        read_command(canvas);
    } else if (command[0] == 'a') { //add
        add(command, canvas);
        print_canvas(canvas);
        read_command(canvas);
    } else if (command[0] == 'd') { // delete
        delete(canvas, command);
        print_canvas(canvas);
        read_command(canvas);
    } else if (command[0] == 'r') { // resize
        resize();
        print_canvas(canvas);
        read_command(canvas);
    } else if (command[0] == 's') { // save
        save(command, canvas, length);
        print_canvas(canvas);
        read_command(canvas);
    } else if (command[0] == 'l') { // load
        load(canvas, command, length);
        print_canvas(canvas);
        read_command(canvas);
    } else if (command[0] == 'e') { // erase
        erase(command, length, canvas);
        print_canvas(canvas);
        read_command(canvas);
    } else { // If the command is any other letter
        printf("Unrecognized command. Type h for help.\n");
        read_command(canvas); //Recursively call back to ask for a command to read again
    }
}

void draw_default_canvas(Canvas* canvas) {
/*
This function draws up the default canvas size
We do this when we are given invalid input
@param: Canvas with no given dimensions, so we give default values
*/
    printf("Invalid arguments. Setting default canvas size.\n");
    canvas->num_rows = 10;
    canvas->num_cols = 10;
}

void draw_canvas(Canvas* canvas) {
    int rows = canvas->num_rows;
    int cols = canvas->num_cols;
/*
This function draws the canvas
Allocate memory and assign whitespace
@param: Canvas with the dimensions
*/
    canvas->board = (char **)malloc(rows * sizeof(char *));
    for (int i = 0; i < rows; i++) {
        canvas->board[i] = (char *)malloc(cols * sizeof(char));
        for (int j = 0; j < cols; j++) {
            canvas->board[i][j] = '*'; // * are used to visualize blank spaces
        }
    }
}

int main(int argc, char** argv) {
    struct Canvas canvas;

    // TODO: Check for any invalid input
    if (argc != 3) { // If there are more or less arguments than there should be
        draw_default_canvas(&canvas);
    } else { // If there are a good amount of arguments, read the values
        sscanf(argv[1], " %d", &canvas.num_rows);
        sscanf(argv[2], " %d", &canvas.num_cols);

        if (canvas.num_rows <= 0 || canvas.num_cols <= 0) { // If the arguments have an invalid value
            draw_default_canvas(&canvas);
        }
    }

    // Create the canvas
    draw_canvas(&canvas);

    // Start asking for commands
    read_command(&canvas);

    return 0;
}

//PRINT HELP 
/*
printf("Wrong number of command line arguments entered.\n");
printf("Usage: ./paint.out [num_rows num_cols]\n");

printf("The number of rows is not an integer.\n");
printf("The number of rows is less than 1.\n");

printf("The number of columns is not an integer.\n");
printf("The number of columns is less than 1.\n");

"Improper draw command.\n"
"Improper resize command.\n"
"Improper erase command.\n"
"Improper add command.\n"
"Improper delete command.\n"
"Improper save command or file could not be created.\n"
"Improper load command or file could not be opened.\n"

*/