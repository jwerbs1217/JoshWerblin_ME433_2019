#include "spi.h"
#include<xc.h>           // processor SFR definitions

void initSPI1(){
  TRISBbits.TRISB15 = 0; //output
  TRISBbits.TRISB13 = 0; //output
  TRISBbits.TRISB14 = 0;
  TRISAbits.TRISA1 = 1; //input

  RPB13Rbits.RPB13R = 0b0011; //pin RB13 is SDO1
  SDI1Rbits.SDI1R = 0b0000; //pin RPA1 is SDI1
  //SCK1 is fixed to RPB14
  
  LATBbits.LATB15 = 1; //CS start high
  
  SPI1CON = 0;
  SPI1BUF;
  SPI1BRG = 1; //divide by 2*(BRG+1) -> 48MHz/4=12MHz baud
  SPI1STATbits.SPIROV = 0;
  SPI1CONbits.CKE = 1;
  SPI1CONbits.MSTEN = 1;
  SPI1CONbits.ON = 1;
 }

int ioSPI(unsigned char buffer){
 SPI1BUF = buffer;
 while(!SPI1STATbits.SPIRBF) { // wait to receive the byte
  ;   
}
 SPI1BUF;
int rx = SPI1BUF;
return(rx);
}
