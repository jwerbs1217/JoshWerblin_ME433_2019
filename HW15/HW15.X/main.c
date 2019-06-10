//main.c
//HW15
#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include<stdio.h>  //for sprintf
#include "myPIC32.h"     //a buncha #pragmas
#include "ili9341.h"

int j = 0;

int main() {
    char pixR[240] = {};
    int i =0;
    for (i=0;i<240;i++){
        pixR[i] = i;
    }
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

  TRISBbits.TRISB9 = 0; //direction pin
  LATBbits.LATB4 = 1; //start direction high
  
  RPA0Rbits.RPA0R = 0b0101; //OC pin init

  //Timer 2 stuff
  T2CONbits.TCKPS = 0; // Timer2 prescaler N=1 (1:1)
  PR2 = 2399; // PR = PBCLK / N / desiredF - 1
  TMR2 = 0; // initial TMR2 count is 0
  OC1CONbits.OCM = 0b110; // PWM mode without fault pin; other OC1CON bits are defaults
  OC1RS = 0; // duty cycle
  OC1R = 0; // initialize before turning OC1 on; afterward it is read-only
  T2CONbits.ON = 1; // turn on Timer2
  OC1CONbits.ON = 1; // turn on OC1
  
  T3CONbits.TCKPS = 0b011; //1:8 prescaler
  TMR3 = 0;
  PR3 = 59999;



  _CP0_SET_COUNT(0);
  __builtin_enable_interrupts();
  T3CONbits.ON = 1;
  
  LCD_plot(pixR,  240, 50, ILI9341_RED);

  char m[100];
  
  while (1) {
    while (_CP0_GET_COUNT() < 24000000 / 20) {} //20 Hz
    _CP0_SET_COUNT(0);
    LATAINV = 0b10000; //Heartbeat
  }
}


void __ISR(_TIMER_3_VECTOR, IPL5SOFT) Timer3ISR(void) {
    j++;
  IFS0bits.T3IF = 0; //flag
  if (OC1RS == 100){
      OC1RS = 0;
  }
  else{
      OC1RS++;
  }

  // how many times has the interrupt occurred?
  // set the duty cycle and direction pin
  if (j>100){
    LATBINV = 1<<9;
    j=0;
  }
}

