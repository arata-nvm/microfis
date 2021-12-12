#pragma once
#include <stdbool.h>
#include <stdint.h>

typedef struct {
  uint32_t inode;
} file_t;

typedef struct {
  uint32_t inode;
  char name[32];
} dirent;

typedef struct {
  file_t *(*open)(char *filename);
  bool (*close)(file_t *file);
  uint32_t (*read)(file_t *file, uint32_t size, char *buf);
  dirent *(*readdir)(uint32_t index);
} fs_ops_t;

typedef struct {
  fs_ops_t ops;
} fs_node_t;

file_t *fs_open(char *filename);
bool fs_close(file_t *file);
uint32_t fs_read(file_t *file, uint32_t size, char *buf);
dirent *fs_readdir(uint32_t index);

void fs_init();
