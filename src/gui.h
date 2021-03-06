/**
 * @file gui.h
 *
 * Interactive menu system
 *
 * Allows to choose different programs interactively using a serial console on
 * the host computer.
 */

#ifndef GUI_H
#define GUI_H

#ifdef __AVR__
#include <avr/io.h>
#define STDIN_HAS_DATA bit_is_set(UCSR0A, RXC0)
#endif // __AVR__
#ifdef __x86_64
#define STDIN_HAS_DATA 0
#endif // __x86_64

#define GUI_TIMEOUT 5000

/**
 * @defgroup KEY_CODES Key Codes used for input handling
 *
 * @{
 */
#define KEY_TAB 9
#define KEY_ENTER 13
#define KEY_ESCAPE 27
#define KEY_SPACE 32
#define KEY_BRACKET_SQUARE_OPEN 91

#define KEY_ZERO 48
#define KEY_ONE 49
#define KEY_TWO 50
#define KEY_THREE 51
#define KEY_FOUR 52
#define KEY_FIVE 53
#define KEY_SIX 54
#define KEY_SEVEN 55
#define KEY_EIGHT 56
#define KEY_NINE 57

#define KEY_PLUS 43
#define KEY_MINUS 45
/** @} */

/**
 * @defgroup COLOR_ESCAPES Color codes used for beautiful console output
 *
 * @{
 */
#define COLOR_NORMAL "\x1B[0m"

#define COLOR_FG_BLACK "\x1B[30m"
#define COLOR_FG_RED "\x1B[31m"
#define COLOR_FG_GREEN "\x1B[32m"
#define COLOR_FG_YELLOW "\x1B[33m"
#define COLOR_FG_BLUE "\x1B[34m"
#define COLOR_FG_MAGENTA "\x1B[35m"
#define COLOR_FG_CYAN "\x1B[36m"
#define COLOR_FG_WHITE "\x1B[37m"

#define COLOR_BG_RED "\x1B[41m"
#define COLOR_BG_YELLOW "\x1B[43m"
#define COLOR_BG_WHITE "\x1B[47m"
/** @} */

/**
 * A single menu item as used in the GUI.
 */
typedef struct {
  char title[60]; /**< title shown in main menu. */
  void (*exec)(); /**< program callback, to be excuted on selection. */
} menu_item;

/**
 * Clear the screen.
 */
void screen_clear(void);

/**
 * Reset cursor to top left corner.
 */
void screen_reset(void);

/**
 * Hide cursor.
 */
void cursor_hide(void);

/**
 * Show cursor.
 */
void cursor_show(void);

/**
 * Draw a single menu item.
 *
 * @param item menu item to draw
 * @param selected whether to draw menu item as selected or not
 * @param menu_item_index index of menu item
 */
void gui_draw_menu_item(menu_item item, int selected, int menu_item_index);

/**
 * Draw the GUI.
 *
 * @param items menu items to draw
 * @param menu_length number of menu items
 * @param selection index of current selection
 * @param timeout amount of time in ms after which the current selection should
 * be auto-executed
 */
void gui_draw(menu_item items[], int menu_length, int selection,
              unsigned int timeout);

/**
 * Execute a program.
 *
 * @param menu array of menu items
 * @param selection index of menu item to execute
 */
void program_execute(menu_item *menu, int selection);

/**
 * GUI main loop.
 *
 * @param menu array of menu items
 * @param menu_length number of menu items
 * @param selection index of current selection
 */
void GUI(menu_item menu[], int menu_length, int selection);

#endif /* GUI_H */
