#include "game_setup.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Some handy dandy macros for decompression
#define E_CAP_HEX 0x45
#define E_LOW_HEX 0x65
#define S_CAP_HEX 0x53
#define S_LOW_HEX 0x73
#define W_CAP_HEX 0x57
#define W_LOW_HEX 0x77
#define DIGIT_START 0x30
#define DIGIT_END 0x39

/** Initializes the board with walls around the edge of the board.
 *
 * Modifies values pointed to by cells_p, width_p, and height_p and initializes
 * cells array to reflect this default board.
 *
 * Returns INIT_SUCCESS to indicate that it was successful.
 *
 * Arguments:
 *  - cells_p: a pointer to a memory location where a pointer to the first
 *             element in a newly initialized array of cells should be stored.
 *  - width_p: a pointer to a memory location where the newly initialized
 *             width should be stored.
 *  - height_p: a pointer to a memory location where the newly initialized
 *              height should be stored.
 */
enum board_init_status initialize_default_board(int** cells_p, size_t* width_p,
                                                size_t* height_p) {
    *width_p = 20;
    *height_p = 10;
    int* cells = malloc(20 * 10 * sizeof(int));
    *cells_p = cells;
    for (int i = 0; i < 20 * 10; i++) {
        cells[i] = FLAG_PLAIN_CELL;
    }

    // Set edge cells!
    // Top and bottom edges:
    for (int i = 0; i < 20; ++i) {
        cells[i] = FLAG_WALL;
        cells[i + (20 * (10 - 1))] = FLAG_WALL;
    }
    // Left and right edges:
    for (int i = 0; i < 10; ++i) {
        cells[i * 20] = FLAG_WALL;
        cells[i * 20 + 20 - 1] = FLAG_WALL;
    }

    // Add snake
    cells[20 * 2 + 2] = FLAG_SNAKE;

    return INIT_SUCCESS;
}

/** Initialize variables relevant to the game board.
 * Arguments:
 *  - cells_p: a pointer to a memory location where a pointer to the first
 *             element in a newly initialized array of cells should be stored.
 *  - width_p: a pointer to a memory location where the newly initialized
 *             width should be stored.
 *  - height_p: a pointer to a memory location where the newly initialized
 *              height should be stored.
 *  - snake_p: a pointer to your snake struct (not used until part 2!)
 *  - board_rep: a string representing the initial board. May be NULL for
 * default board.
 */
enum board_init_status initialize_game(int** cells_p, size_t* width_p,
                                       size_t* height_p, snake_t* snake_p,
                                       char* board_rep) {
    enum board_init_status status;
    
    if (board_rep == NULL) {
        status = initialize_default_board(cells_p, width_p, height_p);
    } else {
        status = decompress_board_str(cells_p, width_p, height_p, snake_p, board_rep);
    }

    if (status == INIT_SUCCESS) {
        // Initialize global variables here
        g_game_over = 0;
        g_score = 0;
        
        // Initialize snake using the snake_t struct
        snake_p->length = 1;
        snake_p->direction = INPUT_RIGHT;
        snake_p->body = malloc(sizeof(snake_pos_t));  // Allocate memory for the snake body
        if (snake_p->body == NULL) {
            // Handle memory allocation failure
            g_game_over = 1;
            return INIT_ERR_WRONG_SNAKE_NUM;
        }
        // Set initial position for the snake's first segment
        snake_p->body[0].row = 2;
        snake_p->body[0].col = 2;
        place_food(*cells_p, *width_p, *height_p);
    }
    
    return status;
}


/** Takes in a string `compressed` and initializes values pointed to by
 * cells_p, width_p, and height_p accordingly. Arguments:
 *      - cells_p: a pointer to the pointer representing the cells array
 *                 that we would like to initialize.
 *      - width_p: a pointer to the width variable we'd like to initialize.
 *      - height_p: a pointer to the height variable we'd like to initialize.
 *      - snake_p: a pointer to your snake struct (not used until part 2!)
 *      - compressed: a string that contains the representation of the board.
 * Note: We assume that the string will be of the following form:
 * B24x80|E5W2E73|E5W2S1E72... To read it, we scan the string row-by-row
 * (delineated by the `|` character), and read out a letter (E, S or W) a number
 * of times dictated by the number that follows the letter.
 */
enum board_init_status decompress_board_str(int** cells_p, size_t* width_p,
                                          size_t* height_p, snake_t* snake_p,
                                          char* compressed) {
    // Reset the output pointers to avoid potential garbage values
    *cells_p = NULL;
    *width_p = 0;
    *height_p = 0;

    if (!compressed || strlen(compressed) == 0) {
        return INIT_ERR_BAD_CHAR;
    }
    // Parse dimensions from start of string (format: "BHxW|")
    if (compressed[0] != 'B') {
        return INIT_ERR_BAD_CHAR;
    }
    char* ptr = compressed + 1;
    char* endptr;
    *height_p = strtoul(ptr, &endptr, 10);
    if (*endptr != 'x') {
        return INIT_ERR_BAD_CHAR;
    }
    ptr = endptr + 1;
    *width_p = strtoul(ptr, &endptr, 10);
    if (*endptr != '|') {
        return INIT_ERR_BAD_CHAR;
    }
    
    // Validate dimensions before allocating
    if (*width_p == 0 || *height_p == 0) {
        return INIT_ERR_INCORRECT_DIMENSIONS;
    }

    // Allocate board array
    *cells_p = malloc((*width_p) * (*height_p) * sizeof(int));
    if (!*cells_p) {
        return INIT_ERR_BAD_CHAR;
    }

    ptr = endptr + 1;
    size_t cell_count = 0; 
    size_t row_count = 0;    // Track current row's cell count
    int snake_count = 0;
    enum board_init_status status = INIT_SUCCESS;

    // Process each character and its count
    while (*ptr) {
        char type = *ptr++;
       
        if (type != 'W' && type != 'E' && type != 'S') {
            status = INIT_ERR_BAD_CHAR;
            break;
        }
        char* count_start = ptr;
        unsigned long count = strtoul(ptr, &endptr, 10);
        if (count_start == endptr || count == 0) {
            status = INIT_ERR_BAD_CHAR;
            break;
        }
        ptr = endptr;
        int flag;
        switch (type) {
            case 'W':
                flag = FLAG_WALL;
                break;
            case 'E':
                flag = FLAG_PLAIN_CELL;
                break;
            case 'S':
                flag = FLAG_SNAKE;
                snake_count++;
                if (snake_count > 1 || count > 1) {
                    status = INIT_ERR_WRONG_SNAKE_NUM;
                    break;
                }
                break;
            default:
                status = INIT_ERR_BAD_CHAR;
                break;
        }

        if (status != INIT_SUCCESS) {
            break;
        }
        
        // Check if adding these cells would exceed a row's width
        for (size_t i = 0; i < count; i++) {
            if (row_count >= *width_p || cell_count >= (*width_p) * (*height_p)) {
                status = INIT_ERR_INCORRECT_DIMENSIONS;
                break;
            }
            
            (*cells_p)[cell_count++] = flag;
            row_count++;
        }
        
        if (status != INIT_SUCCESS) {
            break;
        }
        
        // Reset row count when we hit a delimiter
        if (*ptr == '|') {
            // If we haven't filled the entire row, that's also an error
            if (row_count != *width_p) {
                status = INIT_ERR_INCORRECT_DIMENSIONS;
                break;
            }
            row_count = 0;
            ptr++;
        }
    }
    
    // Final checks if we didn't break early
    if (status == INIT_SUCCESS) {
        // Check final state
        if (cell_count != (*width_p) * (*height_p)) {
            status = INIT_ERR_INCORRECT_DIMENSIONS;
        }
        else if (snake_count != 1) {
            status = INIT_ERR_WRONG_SNAKE_NUM;
        }
    }

    // Clean up if there was an error
    if (status != INIT_SUCCESS) {
        free(*cells_p);
        *cells_p = NULL;
        *width_p = 0;
        *height_p = 0;
    }
    
    return status;
}