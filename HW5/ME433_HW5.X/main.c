//main.c
//HW5
#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include "myPIC32.h"     //a buncha #pragmas
#include "DAC.h"

int main() {

  __builtin_disable_interrupts();
      initPIC();

      TRISAbits.TRISA4 = 0; //User LED
      TRISBbits.TRISB4 = 1; //USER button
      LATAbits.LATA4 = 1; //start LED high

      initDAC(); //initialize DAC
     
      _CP0_SET_COUNT(0);    
      __builtin_enable_interrupts();


  while(1){
      
      while (_CP0_GET_COUNT()<24000000/1000){;} //change this to 1kHz
      LATAINV = 10000; //invert bit 4, A4
      setVoltage(512,'a'); //0-1023 = 0-3.3V
      setVoltage(256,'b');
      _CP0_SET_COUNT(0);
  
    }
}
     