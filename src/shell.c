#include <console.h>
#include <keyboard.h>
#include <shell.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static void exec_command(char *s) {
  if (!strcmp(s, "clear")) {
    console_clear();
  } else {
    kprintf("command `%s` not found\n", s);
  }
}

void shell_prompt() {
  char buf[256];
  int input_len = 0;
  kprintf("microfis> ");

  while (1) {
    uint16_t key = keyboard_getkey();
    if (key == 0) {
      continue;
    }

    char c = key & KEY_CHAR_MASK;
    if (c == '\n') {
      buf[input_len] = 0;
      input_len = 0;

      kprintf("\n");
      exec_command(buf);
      kprintf("microfis> ");
    } else if (c == '\b') {
      if (input_len == 0) {
        continue;
      }
      kprintf("\b");
      input_len--;
    } else {
      kprintf("%c", c);
      buf[input_len++] = c;
    }
  }
}