#include <orcus.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
  gp2xInit();
  uartPrintf("******************\n");
  uartPrintf("Second CPU example\n");
  uartPrintf("******************\n\n");

  uartPrintf("Init ARM940\n");
  arm940Init(ARM940_MEMORY_BANK);

  uartPrintf("Is ARM940 running? %s\n", arm940Running() ? "Yes" : "No");

  //  arm940Halt();
  //  usleep(1000000);
  arm940Run();

  uartPrintf("Is ARM940 running? %s\n", arm940Running() ? "Yes" : "No");

  while(1) {
    uartPrintf("Still running 0x%x\n", *((uint32_t*)0x3000000));
    usleep(1000000);
  }
}
