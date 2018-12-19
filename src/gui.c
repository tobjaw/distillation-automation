#include <stdio.h>
#include "gui.h"
#include "compat.h"

void putchars(const char *s) {
  if (!*s)
    return;
  while (*s) {
    putchar(*s++);
  }
}

void cursor_hide(void) { printf("\x1B[?25l"); }

void cursor_show(void) { printf("\x1B[?25h"); }

void screen_reset(void) {
  // reset cursor to top left corner
  static char chars[] = {KEY_ESCAPE, KEY_BRACKET_SQUARE_OPEN, 'H'};
  putchars(chars);
}

void screen_clear(void) {
  // clear screen
  static char chars[] = {KEY_ESCAPE, KEY_BRACKET_SQUARE_OPEN, '2', 'J'};
  putchars(chars);
}

void gui_draw_menu_item(menu_item item, int selected) {
  char selection_indicator;
  if (selected) {
    selection_indicator = '*';
  } else {
    selection_indicator = ' ';
  }
  printf(COLOR_BG_YELLOW COLOR_FG_BLACK "(%c)" COLOR_NORMAL " %s\n",
         selection_indicator, item.title);
}

void gui_draw(menu_item items[], int menu_length, int selection,
              unsigned int timeout) {
  int i;
  screen_reset();

  for (i = 0; i < menu_length; i++) {
    gui_draw_menu_item(items[i], selection == i);
  }

  printf("\nAutorun in %04d...", timeout);
}

void program_execute(menu_item* menu, int selection) {
  screen_clear();
  screen_reset();
  cursor_show();
  menu[selection].exec();
  screen_clear();
  screen_reset();
  cursor_hide();
}

void GUI(menu_item menu[], int menu_length, int selection) {
  int input = 0;
  int timeout = GUI_TIMEOUT;

  screen_clear();
  gui_draw(menu, menu_length, selection, timeout);

  while (1) {

    if (STDIN_HAS_DATA) {
      input = getchar();
      timeout = GUI_TIMEOUT;
    } else if (timeout > 0) {
      timeout -= 50;
      input = 0;
      _sleep(50);
    } else {
      timeout = GUI_TIMEOUT;
      program_execute(menu,selection);
    }

    switch (input) {
    case KEY_ONE:
    case KEY_TWO:
    case KEY_THREE:
    case KEY_FOUR:
    case KEY_FIVE:
    case KEY_SIX:
    case KEY_SEVEN:
    case KEY_EIGHT:
    case KEY_NINE:
      selection = (input - 48) - 1;

      if (!(selection > menu_length - 1)) {
        program_execute(menu,selection);
      }
      break;

    case KEY_ENTER:
    case KEY_SPACE:
      program_execute(menu,selection);
      break;

    case KEY_TAB:
      if (selection == menu_length - 1) {
        selection = 0;
      } else {
        selection += 1;
      }
      break;


    case KEY_ESCAPE:
      if (getchar() != KEY_BRACKET_SQUARE_OPEN)
        break;

      input = getchar();
      if (input == 65 && selection > 0) {
        selection -= 1;
      }

      if (input == 66 && selection < menu_length - 1) {
        selection += 1;
      }

      break;
    }

    gui_draw(menu, menu_length, selection, timeout);
  }
}
