#include "spi.h"

void initSPI1(){
  TRISB15bits.TRISB15 = 0; //output
  TRISB13bits.TRISB13 = 0; //output
  TRISA1bits.TRISA1 = 1; //input

  RPB15Rbits.RPB15R = 0b0011; //pin RB15 is SS1
  RPB13Rbits.RPB13R = 0b0011; //pin RB13 is SDO1
  SDI1Rbits.SDI1R = 0b0000; //pin RPA1 is SDI1
  //SCK1 is fixed to RPB14

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
}

int ioSPI(int buffer){
  LATB15bits.LATB15 = 0; // CS low. Begin write
  SPI1BUF = buffer;
  int rx = SPI1BUF;
  LATB15bits.LATB15 = 1; //CS high. End write.
  return(rx);

}
