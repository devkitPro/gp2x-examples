#include <gp2xregs.h>
#include <orcus.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void fillBuffer(int16_t* buffer, int period) {
  bool highOrLow = true;
  for(int i = 0 ; i < (0xFFFF/2) ; i += 2) {
    if((i % period) == 0) {
      highOrLow = !highOrLow;
    }
    buffer[i] = (highOrLow ? 32767 : -32768);
    buffer[i+1] = buffer[i];
  }
}

void handleHeadphones() {
  static bool wereConnected = false;
  bool areConnected = audioHeadphonesConnected();
  if(areConnected && !wereConnected) {
    audioMuteSpeakers();
    audioUnmuteHeadphones();
    wereConnected = !wereConnected;
  } else if(!areConnected && wereConnected) {
    audioMuteHeadphones();
    audioUnmuteSpeakers();
    wereConnected = !wereConnected;
  }
}

int main(int argc, char* argv[]) {
  gp2xInit();
  uartPrintf("Audio example\n");
  
  int16_t* buffer = (int16_t*) malloc(0xFFFF);
  audioInit(0);
  audioUnmuteSpeakers();
  // In a real example you may wish to do this check in your main loop to capture
  // headphones being unplugged/plugged back in. It will always return false on an F200 because
  // the audio signals are physically switched. You can't run it while a sample is playing
  // though as the AC97 link is busy while PCM data is being transmitted.
  handleHeadphones();
  audioSetVolume(31, 31);
  
  for(int i = 0 ; i < 500 ; i += 50) {
    fillBuffer(buffer, i);
    audioPlaySample(0xFFFF, buffer);
    while(audioSamplePlaying());
    usleep(1000000);
  }
  
  while(1);
}
