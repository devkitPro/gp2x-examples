#include <stdlib.h>
#include <gp2xregs.h>
#include <orcus.h>
#include <unistd.h>
#include <stdio.h>

#define RED 0xF800
#define GREEN 0x07E0

int main(int argc, char* argv[]) {
  gp2xInit();
  setbuf(stdout, NULL);
  printf("About to try enabling MMU\n");
  volatile unsigned int* aValue = (unsigned int*)(SZ_1M*5);
  volatile unsigned int* aValueMoved = (unsigned int*)(SZ_1M*4);
  *aValue = 123456789;
  *aValueMoved = 987654321;
  printf("aValue: %u, aValueMoved: %u\n", (*aValue), (*aValueMoved));

  // normally you wouldn't bother with all this, you'd just call mmuCachesInitOn();
  // I want to prove the MMU is translating addresses correctly though
  cacheEnableI();
  mmuSetDomainAccess(0, MANAGER);
  uint32_t* l1Table = mmuNewL1Table();

  uint32_t tmp = l1Table[4];
  l1Table[4] = l1Table[5];
  l1Table[5] = tmp;
  
  mmuEnable(l1Table);
  cacheInvalidateDI();
  cacheEnableD();

  printf("aValue: %u, aValueMoved: %u\n", (*aValue), (*aValueMoved));

  while(1) {    
    if(btnStateDebounced() & R) {
      return 0;
    }
  }
}
