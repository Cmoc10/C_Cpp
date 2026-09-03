#include "mbstrings.h"

/* mbslen - multi-byte string length
 * - Description: returns the number of UTF-8 code points ("characters")
 * in a multibyte string. If the argument is NULL or an invalid UTF-8
 * string is passed, returns -1.
 *
 * - Arguments: A pointer to a character array (`bytes`), consisting of UTF-8
 * variable-length encoded multibyte code points.
 *
 * - Return: returns the actual number of UTF-8 code points in `src`. If an
 * invalid sequence of bytes is encountered, return -1.
 *
 * - Hints:
 * UTF-8 characters are encoded in 1 to 4 bytes. The number of leading 1s in the
 * highest order byte indicates the length (in bytes) of the character. For
 * example, a character with the encoding 1111.... is 4 bytes long, a character
 * with the encoding 1110.... is 3 bytes long, and a character with the encoding
 * 1100.... is 2 bytes long. Single-byte UTF-8 characters were designed to be
 * compatible with ASCII. As such, the first bit of a 1-byte UTF-8 character is
 * 0.......
 *
 * You will need bitwise operations for this part of the assignment!
 */
size_t mbslen(const char* bytes) {
    // Check for NULL input
    if (bytes == NULL) {
        return (size_t)-1;
    }

    size_t count = 0;
    
    while (*bytes != '\0') {
        // Check the first byte to determine character length
        unsigned char first_byte = (unsigned char)*bytes;
        
        // Single-byte character (ASCII-compatible)
        if ((first_byte & 0x80) == 0) {
            bytes++;
            count++;
            continue;
        }
        
        // Determine character length based on leading bits
        int char_length = 0;
        if ((first_byte & 0xE0) == 0xC0) {  // 2-byte character
            char_length = 2;
        } else if ((first_byte & 0xF0) == 0xE0) {  // 3-byte character
            char_length = 3;
        } else if ((first_byte & 0xF8) == 0xF0) {  // 4-byte character
            char_length = 4;
        } else {
            // Invalid UTF-8 sequence
            return (size_t)-1;
        }
        
        // Validate continuation bytes
        for (int i = 1; i < char_length; i++) {
            // Move to next byte
            bytes++;
            
            // Check if we've hit the null terminator prematurely
            if (*bytes == '\0') {
                return (size_t)-1;
            }
            
            // Check that continuation byte starts with 10
            if (((unsigned char)*bytes & 0xC0) != 0x80) {
                return (size_t)-1;
            }
        }
        
        // Move to next character
        bytes++;
        count++;
    }
    
    return count;
}
