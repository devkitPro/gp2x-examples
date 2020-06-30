#include <orcus.h>
#include <string.h>
#include <stdlib.h>

#define LEN 100

void printBuf(uint32_t* buf) {
  for(int i = LEN ; i-- ; ) {
    uartPrintf("0x%x ", buf[i]);
  }
  uartPrintf("\n");
}

int main(int argc, char* argv[]) {
  gp2xInit();

  int size = sizeof(uint32_t) * LEN;
  
  uint32_t* source = malloc(size);
  uint32_t* dest = malloc(size);

  for(int i = LEN ; i-- ; ) {
    source[i] = i;
  }
  memset(dest, 0, size);

  uartPrintf("Source before:\n");
  printBuf(source);
  uartPrintf("Dest before:\n");
  printBuf(dest);

  uartPrintf("Copying data and reversing...\n");
  dmaConfigureChannelMem(0, WORDS_8, -1, 1);
  dmaStart(0, size, (uint32_t) (source+LEN-1), (uint32_t) dest);
  bool isRunning = true;
  bool hasFinished = false;
  while((isRunning = dmaIsTransferring(0)) != false && (hasFinished = dmaHasFinished(0)) != true) {
    uartPrintf("Waiting for DMA to complete: %d %d\n", isRunning, hasFinished);
  }
  uartPrintf("DMA complete\n");
  
  uartPrintf("Dest after:\n");
  printBuf(dest);
  
  while(1);
}
