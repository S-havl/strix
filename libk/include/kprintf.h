#pragma once

#include <stdint.h>
#include <stddef.h>

/* Standard VGA Text Mode Color Definitions (0-15) */

/* Base Colors (Bits 0-2) */
#define COLOR_BLACK          0  /* Black */
#define COLOR_BLUE           1  /* Blue */
#define COLOR_GREEN          2  /* Green */
#define COLOR_CYAN           3  /* Cyan */
#define COLOR_RED            4  /* Red */
#define COLOR_MAGENTA        5  /* Magenta */
#define COLOR_BROWN          6  /* Brown */
#define COLOR_LIGHT_GREY     7  /* Light Grey (VGA hardware default) */

/* High-Intensity / Bright Colors (Bit 3) - Recommended for TEXT ONLY */
#define COLOR_DARK_GREY      8  /* Dark Grey */
#define COLOR_LIGHT_BLUE     9  /* Light Blue */
#define COLOR_LIGHT_GREEN   10  /* Light Green */
#define COLOR_LIGHT_CYAN    11  /* Light Cyan */
#define COLOR_LIGHT_RED     12  /* Light Red */
#define COLOR_LIGHT_MAGENTA 13  /* Light Magenta */
#define COLOR_YELLOW        14  /* Yellow */
#define COLOR_WHITE         15  /* White */


void kprintf(uint8_t text_color, uint8_t background_color, const char* str);

void clear_screen();
