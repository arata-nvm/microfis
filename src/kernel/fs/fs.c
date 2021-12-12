#include <fs.h>
#include <heap.h>
#include <stdbool.h>
#include <string.h>

typedef struct file_item {
  char name[32];
  char content[256];
} file_item_t;

fs_node_t *fs_root;

uint32_t file_count;
file_item_t *file_items;

file_t *fs_open(char *filename) {
  if (fs_root->ops.open != NULL) {
    return fs_root->ops.open(filename);
  } else {
    return NULL;
  }
}

bool fs_close(file_t *file) {
  if (fs_root->ops.close != NULL) {
    fs_root->ops.close(file);
    return true;
  } else {
    return false;
  }
}

uint32_t fs_read(file_t *file, uint32_t size, char *buf) {
  if (fs_root->ops.read != NULL) {
    return fs_root->ops.read(file, size, buf);
  } else {
    return 0;
  }
}

dirent *fs_readdir(uint32_t index) {
  if (fs_root->ops.readdir != NULL) {
    return fs_root->ops.readdir(index);
  } else {
    return NULL;
  }
}

static file_t *fs_open_impl(char *filename) {
  for (uint32_t i = 0; i < file_count; i++) {
    if (!strcmp(file_items[i].name, filename)) {
      file_t *file = kmalloc(sizeof(file_t));
      memset(file, 0, sizeof(file_t));
      file->inode = i;
      return file;
    }
  }

  return NULL;
}

static bool fs_close_impl(file_t *file) {
  kfree(file);
  return true;
}

static uint32_t fs_read_impl(file_t *file, uint32_t size, char *buf) {
  if (file->inode >= file_count) {
    return 0;
  }

  file_item_t item = file_items[file->inode];
  strncpy(buf, item.content, size);
  uint32_t file_size = strlen(item.content);
  return file_size > size ? size : file_size;
}

static dirent *fs_readdir_impl(uint32_t index) {
  if (index >= file_count) {
    return NULL;
  }

  dirent *entry = kmalloc(sizeof(dirent));
  entry->inode = index;
  strcpy(entry->name, file_items[index].name);
  return entry;
}

void fs_init() {
  fs_root = kmalloc(sizeof(fs_node_t));
  fs_root->ops.open = fs_open_impl;
  fs_root->ops.close = fs_close_impl;
  fs_root->ops.read = fs_read_impl;
  fs_root->ops.readdir = fs_readdir_impl;

  file_count = 2;
  file_items = kmalloc(sizeof(file_item_t) * file_count);
  memset(file_items, 0, sizeof(file_item_t) * file_count);

  strcpy(file_items[0].name, "/hello.txt");
  strcpy(file_items[0].content, "Hello world!");

  strcpy(file_items[1].name, "/note.txt");
  strcpy(file_items[1].content, "This is note.txt");
}
