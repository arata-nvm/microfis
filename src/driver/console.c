#include <common.h>
#include <console.h>
#include <io.h>

#define PORT_COMMAND 0x3D4
#define PORT_DATA 0x3D5
#define COMMAND_HIGH 0xE
#define COMMAND_LOW 0xF

#define BUFFER_MAX_X 80
#define BUFFER_MAX_Y 25

static char *FRAME_BUFFER = (char *)0xB8000;

static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;

static uint32_t buffer_index(uint8_t x, uint8_t y) {
  return (x + BUFFER_MAX_X * y) * 2;
}

static void scroll() {
  for (uint8_t y = 0; y < BUFFER_MAX_Y - 1; y++) {
    for (uint8_t x = 0; x < BUFFER_MAX_X; x++) {
      FRAME_BUFFER[buffer_index(x, y)] = FRAME_BUFFER[buffer_index(x, y + 1)];
    }
  }

  for (uint8_t x = 0; x < BUFFER_MAX_X; x++) {
    FRAME_BUFFER[buffer_index(x, BUFFER_MAX_Y - 1)] = 0;
  }
}

static void move_cursor(uint8_t x, uint8_t y) {
  int pos = x + BUFFER_MAX_X * y;

  outb(PORT_COMMAND, COMMAND_HIGH);
  outb(PORT_DATA, (pos >> 8) & 0xFF);
  outb(PORT_COMMAND, COMMAND_LOW);
  outb(PORT_DATA, pos & 0xFF);
}

void console_clear() {
  for (uint8_t y = 0; y < BUFFER_MAX_Y; y++) {
    for (uint8_t x = 0; x < BUFFER_MAX_X; x++) {
      FRAME_BUFFER[buffer_index(x, y)] = 0;
    }
  }
}

void console_putchar(char c) {
  if (c == '\r') {
    cursor_x = 0;
  } else if (c == '\n') {
    cursor_x = 0;
    cursor_y++;
  } else if (c == '\b') {
    if (cursor_x != 0) {
      cursor_x--;
      FRAME_BUFFER[buffer_index(cursor_x, cursor_y)] = ' ';
    }
  } else {
    FRAME_BUFFER[buffer_index(cursor_x, cursor_y)] = c;
    cursor_x++;
  }

  if (cursor_x >= BUFFER_MAX_X) {
    cursor_x = 0;
    cursor_y++;
  }

  if (cursor_y >= BUFFER_MAX_Y) {
    scroll();
    cursor_y = BUFFER_MAX_Y - 1;
  }
  move_cursor(cursor_x, cursor_y);
}

void console_write(char *s) {
  while (*s) {
    console_putchar(*s);
    s++;
  }
}

void console_write_dec(uint32_t i) {
  char buf[16];
  buf[15] = 0;

  char *buf_ptr = &buf[14];
  do {
    *buf_ptr = '0' + (i % 10);
    buf_ptr--;
    i /= 10;
  } while (i != 0);

  console_write(buf_ptr + 1);
}
