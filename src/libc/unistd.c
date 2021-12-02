#include <unistd.h>

int write(int fd, char *buf, int size) {
  int ret;
  asm volatile("int 0x80"
               : "=a"(ret)
               : "a"(SYSCALL_WRITE), "b"(fd), "c"(buf), "d"(size));
  return ret;
}
