#define KEY_TAB 9
#define KEY_ENTER 13
#define KEY_ESCAPE 27
#define KEY_SPACE 32
#define KEY_BRACKET_SQUARE_OPEN 91

/**
 * A single menu item as used in the GUI.
 */
typedef struct {
  char title[60];
  void (*exec)();
} menu_item;

void screen_clear(void);
void screen_reset(void);
void GUI(menu_item menu[], int menu_length, int selection);
