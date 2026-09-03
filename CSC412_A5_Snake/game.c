#include "game.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "linked_list.h"
#include "mbstrings.h"

/** Updates the game by a single step, and modifies the game information
 * accordingly. Arguments:
 *  - cells: a pointer to the first integer in an array of integers representing
 *    each board cell.
 *  - width: width of the board.
 *  - height: height of the board.
 *  - snake_p: pointer to your snake struct (not used until part 2!)
 *  - input: the next input.
 *  - growing: 0 if the snake does not grow on eating, 1 if it does.
 */
void update(int* cells, size_t width, size_t height, snake_t* snake_p,
            enum input_key input, int growing) {
    // If game is already over, don't update
    if (g_game_over) {
        return;
    }
        
    // Get new position based on input or current direction
    size_t new_row = snake_p->body[0].row;  // Head of the snake
    size_t new_col = snake_p->body[0].col;
    
    // Update direction based on input if valid
    if (input != INPUT_NONE) {
        // Don't allow 180-degree turns
        if (snake_p->length != 1) {
            if ((input == INPUT_LEFT && snake_p->direction != INPUT_RIGHT) ||
                (input == INPUT_RIGHT && snake_p->direction != INPUT_LEFT) ||
                (input == INPUT_UP && snake_p->direction != INPUT_DOWN) ||
                (input == INPUT_DOWN && snake_p->direction != INPUT_UP)) {
                snake_p->direction = input;
            }
        }
        else {
            snake_p->direction = input;
        }
    }
    
    // Calculate new position based on current direction
    switch (snake_p->direction) {
        case INPUT_LEFT:
            new_col--;
            break;
        case INPUT_RIGHT:
            new_col++;
            break;
        case INPUT_UP:
            new_row--;
            break;
        case INPUT_DOWN:
            new_row++;
            break;
        default:
            break;
    }
    
    // Get the cell value at the new position
    int new_cell = cells[new_row * width + new_col];
    
    // Check for wall collision
    if (new_cell & FLAG_WALL) {
        g_game_over = 1;
        return;
    }
    
    // Check for snake self-collision (excluding previous tail position)
    for (size_t i = 1; i < snake_p->length - 1; i++) {
        if (new_row == snake_p->body[i].row && 
            new_col == snake_p->body[i].col) {
            g_game_over = 1;
            return;
        }
    }
    
    // Check for food
    int food_eaten = 0;
    if (new_cell & FLAG_FOOD) {
        g_score++;
        food_eaten = 1;
        // Place new food somewhere on the board
        place_food(cells, width, height);
    }
    
    // Adjust snake length and body
    if (food_eaten && growing) {
        // Increase snake length
        snake_p->length++;
        snake_p->body = realloc(snake_p->body, snake_p->length * sizeof(snake_pos_t));
        if (!snake_p->body) {
            // Handle memory allocation failure
            g_game_over = 1;
            return;
        }
    }
    
    // Shift snake body - move existing segments
    for (size_t i = snake_p->length - 1; i > 0; i--) {
        snake_p->body[i] = snake_p->body[i-1];
    }
    
    // Update head position
    snake_p->body[0].row = new_row;
    snake_p->body[0].col = new_col;
    
    // Update board cells
    // Clear previous snake position
    for (size_t r = 0; r < height; r++) {
        for (size_t c = 0; c < width; c++) {
            if (cells[r * width + c] & FLAG_SNAKE) {
                cells[r * width + c] = FLAG_PLAIN_CELL;
            }
        }
    }
    
    // Mark new snake positions
    for (size_t i = 0; i < snake_p->length; i++) {
        cells[snake_p->body[i].row * width + snake_p->body[i].col] = FLAG_SNAKE;
    }
}


/** Sets a random space on the given board to food.
 * Arguments:
 *  - cells: a pointer to the first integer in an array of integers representing
 *    each board cell.
 *  - width: the width of the board
 *  - height: the height of the board
 */
void place_food(int* cells, size_t width, size_t height) {
    /* DO NOT MODIFY THIS FUNCTION */
    unsigned food_index = generate_index(width * height);
    if (*(cells + food_index) == FLAG_PLAIN_CELL) {
        *(cells + food_index) = FLAG_FOOD;
    } else {
        place_food(cells, width, height);
    }
    /* DO NOT MODIFY THIS FUNCTION */
}

/** Prompts the user for their name and saves it in the given buffer.
 * Arguments:
 *  - `write_into`: a pointer to the buffer to be written into.
 */
void read_name(char* write_into) {
    // Variable to store the number of bytes read
    ssize_t bytes_read;
    
    while (1) {
        // Print prompt
        printf("Name > ");
        
        // Flush stdout to ensure prompt is displayed before reading
        fflush(stdout);
        
        // Read input from standard input (file descriptor 0)
        bytes_read = read(STDIN_FILENO, write_into, 1000);
        
        // Check for read errors
        if (bytes_read < 0) {
            perror("Error reading name");
            continue;
        }
        
        // Null-terminate the input
        write_into[bytes_read] = '\0';
        
        // Remove trailing newline if present
        if (bytes_read > 0 && write_into[bytes_read - 1] == '\n') {
            write_into[bytes_read - 1] = '\0';
            bytes_read--;
        }
        
        // Check if name is empty
        if (bytes_read == 0) {
            printf("Name Invalid: must be longer than 0 characters.\n");
            continue;
        }
        
        // Name is valid, exit the loop
        break;
    }
}

/** Cleans up on game over — should free any allocated memory so that the
 * LeakSanitizer doesn't complain.
 * Arguments:
 *  - cells: a pointer to the first integer in an array of integers representing
 *    each board cell.
 *  - snake_p: a pointer to your snake struct. (not needed until part 2)
 */
void teardown(int* cells, snake_t* snake_p) {
    // Free the dynamically allocated cells array
    free(cells);
    
}