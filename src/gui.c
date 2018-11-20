#include <stdio.h>
#include "gui.h"

void putchars(const char *s) {
  if (!*s)
    return;
  while (*s) {
    putchar(*s++);
  }
}

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
  printf("(%c) %s\n", selection_indicator, item.title);
}

void gui_draw(menu_item items[], int menu_length, int selection) {
  int i;
  screen_clear();
  screen_reset();

  for (i = 0; i < menu_length; i++) {
    gui_draw_menu_item(items[i], selection == i);
  }
}

void GUI(menu_item menu[], int menu_length, int selection) {
  int input;

  gui_draw(menu, menu_length, selection);

  while (1) {
    input = getchar();

    switch (input) {
    case KEY_ENTER:
    case KEY_SPACE:
      screen_clear();
      screen_reset();
      menu[selection].exec();
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

    gui_draw(menu, menu_length, selection);
  }
}
