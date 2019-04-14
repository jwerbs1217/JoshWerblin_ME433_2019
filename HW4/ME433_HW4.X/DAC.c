//DAC.c
#include "spi.h"

void initDAC(){
  initSPI1();
  SPI1BRG = 1; //divide by 2*(BRG+1) -> 48MHz/4=12MHz baud
  SPI1CONbits.MODE16 = 1; //16 bit SPI1
}

void writeDAC(int input, char channel){
  if (channel=='a'){
    buffer = (0b0111 << 12) || (input<<2);
  }
  if (channel=='b'){
    buffer = (0b1111 << 12) || (input<<2);
  }
  while(!SPI1STATbits.SPIRBF) { // wait to receive the byte
  ;
}
  int rx = ioSPI(buffer);
}
