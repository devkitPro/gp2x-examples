#include <stdlib.h>
#include <orcus.h>

int main(int argc, char* argv[]) {
  gp2xInit();

  for(int i = 0 ; i < argc ; i++) {
    uartPrintf("argv[%d]: %s\n", i, argv[i]);
  }
  
  while(1);
}
