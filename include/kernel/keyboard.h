#pragma once
#include <stdint.h>

#define KEY_CHAR_MASK 0x00ff
#define KEY_SHIFT_MASK 0x0100
#define KEY_CTRL_MASK 0x0200
#define KEY_ALT_MASK 0x0400

void keyboard_init();

uint16_t keyboard_getkey();
