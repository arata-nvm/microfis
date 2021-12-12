#include <console.h>
#include <fs.h>
#include <keyboard.h>
#include <kprintf.h>
#include <shell.h>
#include <stdint.h>
#include <string.h>

static void exec_command(char *s) {
  char *cmd = strtok(s, ' ');
  if (!strcmp(cmd, "clear")) {
    console_clear();
  } else if (!strcmp(cmd, "ls")) {
    uint32_t i = 0;
    while (true) {
      dirent *entry = fs_readdir(i);
      if (entry == NULL) {
        break;
      }

      kprintf("%s\n", entry->name);
      i++;
    }
  } else if (!strcmp(cmd, "cat")) {
    char *filename = strtok(NULL, ' ');
    if (filename == NULL) {
      kprintf("usage: cat <file>\n");
      return;
    }

    file_t *file = fs_open(filename);
    if (file == NULL) {
      kprintf("file not found: %s\n", filename);
      return;
    }

    char buf[32];
    fs_read(file, 32, buf);
    kprintf("%s\n", buf);
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