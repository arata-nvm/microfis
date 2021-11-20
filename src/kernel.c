#include <common.h>
#include <console.h>
#include <segmentation.h>

void kmain() {
  console_clear();
  console_write("[*] initializing segmentation...");
  segmentation_init();
  console_write("done\n");
}
