//main.c
//HW15
#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include<stdio.h>  //for sprintf
#include "myPIC32.h"     //a buncha #pragmas
#include "ili9341.h"


int main() {

  __builtin_disable_interrupts();
     __builtin_mtc0(_CP0_CONFIG, _CP0_CONFIG_SELECT, 0xa4210583);

      // 0 data RAM access wait states
      BMXCONbits.BMXWSDRM = 0x0;

      // enable multi vector interrupts
      INTCONbits.MVEC = 0x1;

      // disable JTAG to get pins back
      DDPCONbits.JTAGEN = 0;

      TRISAbits.TRISA4 = 0; //User LED
      TRISBbits.TRISB4 = 1; //USER button
      LATAbits.LATA4 = 1; //start LED high

      SPI1_init();
      LCD_init();
      LCD_clearScreen(ILI9341_BLACK);
     
      
      
        T2CONbits.TCKPS = 0; // Timer2 prescaler N=1 (1:1)
        PR2 = 2399; // PR = PBCLK / N / desiredF - 1
        TMR2 = 0; // initial TMR2 count is 0
        OC1CONbits.OCM = 0b110; // PWM mode without fault pin; other OC1CON bits are defaults
        OC1RS = 0; // duty cycle
        OC1R = 0; // initialize before turning OC1 on; afterward it is read-only
        T2CONbits.ON = 1; // turn on Timer2
        OC1CONbits.ON = 1; // turn on OC1
      
      
      _CP0_SET_COUNT(0);
      __builtin_enable_interrupts();


      char m[100];
  while(1){
        while (_CP0_GET_COUNT()<24000000/20){} //20 Hz
        
        LATAINV = 0b10000; //Heartbeat
        _CP0_SET_COUNT(0);
     //draw stuff here
        


    }
}


void __ISR(_TIMER3_VECTOR, IPL5SOFT) Timer3ISR(void) {

IFS0bits.T3IF = 0; //flag

// how many times has the interrupt occurred?

// set the duty cycle and direction pin

}

void setPWM(int percent){
    OC1RS = percent;
}