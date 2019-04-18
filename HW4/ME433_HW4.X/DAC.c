//DAC.c
#include "spi.h"
#include<xc.h>           // processor SFR definitions

void initDAC(){
  initSPI1();
  //divide by 2*(BRG+1) -> 48MHz/4=12MHz baud
}

void writeDAC(unsigned int input, char channel){
  unsigned int buffer;
  
  if (channel=='a'){
    buffer = (0b0111 << 12);
  }
  
  if (channel=='b'){
     buffer = (0b1111 << 12);
  }
  buffer = buffer | ((input&0b1111111111)<<2);
  LATBbits.LATB15 = 0; // CS low. Begin write
  ioSPI(buffer>>8);
  ioSPI(buffer & 0b11111111);
  LATBbits.LATB15 = 1; //CS high. End write.
  
}
