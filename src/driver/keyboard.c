#include <interrupt.h>
#include <io.h>
#include <keyboard.h>

#define KEYBOARD_DATA 0x60

#define BREAK_MASK 0x80
#define KEY_LSHIFT 0x2a
#define KEY_RSHIFT 0x36
#define KEY_CTRL 0x1d
#define KEY_ALT 0x38

#define ASCII_ESC 0x1b
#define ASCII_BS 0x8
#define ASCII_HT 0x9

static uint8_t key_shift_pressed;
static uint8_t key_ctrl_pressed;
static uint8_t key_alt_pressed;

static uint16_t last_keycode;

static const char keymap_lower[128] = {
    0, ASCII_ESC, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '^',
    ASCII_BS, ASCII_HT, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '@', '[', '\n', 0, 'a', 's',
    'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', ':', 0, 0, ']', 'z', 'x', 'c', 'v',
    'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, '7', '8', '9', '-', '4', '5', '6', '+', '1',
    '2', '3', '0', '.', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, '_', 0, 0, 0, 0, 0, 0, 0, 0, 0, '\\', 0, 0};

static const char keymap_upper[128] = {
    0, ASCII_ESC, '!', '"', '#', '$', '%', '&', '\'', '(', ')', '~', '=', '~',
    ASCII_BS, ASCII_HT, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '`', '{', '\n', 0, 'A', 'S',
    'D', 'F', 'G', 'H', 'J', 'K', 'L', '+', '*', 0, 0, '}', 'Z', 'X', 'C', 'V',
    'B', 'N', 'M', '<', '>', '?', '_', '*', 0, ' ', 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, '\\', 0, 0, 0, 0, 0, 0, 0, 0, 0, '|', 0, 0};

void keyboard_handler(isr_frame_t __attribute__((unused)) frame) {
  uint8_t scancode = inb(KEYBOARD_DATA);
  if ((scancode & BREAK_MASK) != 0) {
    // released

    switch (scancode & ~BREAK_MASK) {
    case KEY_LSHIFT:
      key_shift_pressed &= ~0b10;
      break;
    case KEY_RSHIFT:
      key_shift_pressed &= ~0b01;
      break;
    case KEY_CTRL:
      key_ctrl_pressed = 0;
      break;
    case KEY_ALT:
      key_alt_pressed = 0;
      break;
    }
  } else {
    // pressed

    switch (scancode) {
    case KEY_LSHIFT:
      key_shift_pressed |= 0b10;
      break;
    case KEY_RSHIFT:
      key_shift_pressed |= 0b01;
      break;
    case KEY_CTRL:
      key_ctrl_pressed = 1;
      break;
    case KEY_ALT:
      key_alt_pressed = 1;
      break;

    default:
      if (key_shift_pressed != 0) {
        last_keycode = keymap_upper[scancode] | KEY_SHIFT_MASK;
      } else {
        last_keycode = keymap_lower[scancode];
      }
      if (key_ctrl_pressed != 0) {
        last_keycode |= KEY_CTRL_MASK;
      }
      if (key_alt_pressed != 0) {
        last_keycode |= KEY_ALT_MASK;
      }
    }
  }
}

void keyboard_init() {
  interrupt_add_handler(IRQ1, keyboard_handler);
}

uint16_t keyboard_getkey() {
  uint16_t keycode = last_keycode;
  last_keycode = 0;
  return keycode;
}
