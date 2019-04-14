//main.c
#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include "DAC.h"
#include<math.h>



// DEVCFG0
#pragma config DEBUG = 11 // no debugging
#pragma config JTAGEN = 0 // no jtag
#pragma config ICESEL = 11 // use PGED1 and PGEC1
#pragma config PWP = 111111111 // no write protect
#pragma config BWP = 1 // no boot write protect
#pragma config CP = 1 // no code protect

// DEVCFG1
#pragma config FNOSC = 011 // use primary oscillator with pll
#pragma config FSOSCEN = 0 // turn off secondary oscillator
#pragma config IESO = 0 // no switching clocks
#pragma config POSCMOD = 10 // high speed crystal mode
#pragma config OSCIOFNC = 1 // disable secondary osc
#pragma config FPBDIV = 00 // divide sysclk freq by 1 for peripheral bus clock
#pragma config FCKSM = 11 // do not enable clock switch
#pragma config WDTPS = 00000 // use slowest wdt
#pragma config WINDIS = 1 // wdt no window mode
#pragma config FWDTEN = 0 // wdt disabled
#pragma config FWDTWINSZ = 11 // wdt window at 25%

// DEVCFG2 - get the sysclk clock to 48MHz from the 8MHz crystal
#pragma config FPLLIDIV = 001 // divide input clock to be in range 4-5MHz
#pragma config FPLLMUL = 111 // multiply clock after FPLLIDIV
#pragma config FPLLODIV = 001 // divide clock after FPLLMUL to get 48MHz
#pragma config UPLLIDIV = 001 // divider for the 8MHz input clock, then multiplied by 12 to get 48MHz for USB
#pragma config UPLLEN = 1 // USB clock on

// DEVCFG3
#pragma config USERID = 0 // "josh" without upper/lowercase nibbles
#pragma config PMDL1WAY = 0 // allow multiple reconfigurations
#pragma config IOL1WAY = 0 // allow multiple reconfigurations
#pragma config FUSBIDIO = 1 // USB pins controlled by USB module
#pragma config FVBUSONIO = 1 // USB BUSON controlled by USB module



int main() {

  __builtin_disable_interrupts();

      // set the CP0 CONFIG register to indicate that kseg0 is cacheable (0x3)
      __builtin_mtc0(_CP0_CONFIG, _CP0_CONFIG_SELECT, 0xa4210583);

      // 0 data RAM access wait states
      BMXCONbits.BMXWSDRM = 0x0;

      // enable multi vector interrupts
      INTCONbits.MVEC = 0x1;

      // disable JTAG to get pins back
      DDPCONbits.JTAGEN = 0;

      // do your TRIS and LAT commands here
      //A4 to be output
      //B4 to be input

      TRISAbits.TRISA4 = 0;
      TRISBbits.TRISB4 = 1;
      LATAbits.LATA4 = 1; //start high

      initDAC(); //initialize DAC

      float triangle[] = {};
      for (int i=0;i<200;i++){ //this will be updated at 1000Hz so 200 pts is one period at 5Hz
        triangle[i] = -10.24*abs(i-100)+1024; //initialize triangle wave at 5Hz
      }
      float sine[] = {};
      for (int i=0;i<200;i++){ //100 pts is one period at 10Hz
        sine[i] = 512*sin(( (i+25)/50) * 3.14159265359)+512; //initialize sine wave at 10Hz
      }

      _CP0_SET_COUNT(0);

      __builtin_enable_interrupts();



  while(1){
    for (int i=0;i<200;i++){
    while (_CP0_GET_COUNT()<24000){} //core timer is 24MHz, we update at 1kHz
    writeDAC(triangle[i],'a');
    writeDAC(sine[i],'b');
    _CP0_SET_COUNT(0);
  }


  return 0;
}
