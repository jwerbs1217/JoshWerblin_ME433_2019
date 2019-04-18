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
/*
  IEC0CLR=0x03800000; // disable all interrupts
SPI1CON = 0; // Stops and resets the SPI1.
SPI2CON = 0;
int rData=SPI1BUF; // clears the receive buffer
IFS0CLR=0x03800000; // clear any existing event
IPC5CLR=0x1f000000; // clear the priority
IPC5SET=0x0d000000; // Set IPL=3, Subpriority 1
IEC0SET=0x03800000; // Enable RX, TX and Error interrupts
SPI1BRG=0x0; // use FPB/2 clock frequency
SPI1STATCLR=0x40; // clear the Overflow
SPI1CON=0x8220; // SPI ON, 8 bits transfer, SMP=1, Master mode
// from now on, the device is ready to transmit and receive data
*/
  
  SPI1CON = 0;
  SPI1BUF;
  SPI1BRG = 1;
  SPI1STATbits.SPIROV = 0;
  SPI1CONbits.CKE = 1;
  SPI1CONbits.MSTEN = 1;
  SPI1CONbits.ON = 1;
 }

void ioSPI(unsigned char buffer){
    
  
  SPI1BUF = buffer;
 while(!SPI1STATbits.SPIRBF) { // wait to receive the byte
  ;   
}
  //int rx = SPI1BUF;
SPI1BUF;
  //return(rx);

}
