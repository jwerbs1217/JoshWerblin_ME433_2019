//DAC.c
#include "spi.h"

void initDAC(){
  initSPI1();
  SPI1BRG = 1; //divide by 2*(BRG+1) -> 48MHz/4=12MHz baud
  SPI1CONbits.MODE16 = 1; //16 bit SPI1
}

void writeDAC(int input, char channel){
  if (channel=='a'){
    buffer = (0b0011 << 12) || (input<<2);
  }
  if (channel=='b'){
    buffer = (0b1011 << 12) || (input<<2);
  }
  int rx = ioSPI(buffer);
}
