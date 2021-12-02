#pragma once

#define SYSCALL_WRITE 1

int write(int fd, char *buf, int size);
