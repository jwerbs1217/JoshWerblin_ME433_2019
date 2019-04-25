//main.c
//HW5
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
      
      _CP0_SET_COUNT(0);    
      __builtin_enable_interrupts();

      int k=0;
      char m[100];

  while(1){

      
        if (_CP0_GET_COUNT()>24000000/10){ //10 Hz
        LATAINV = 0b10000; //Heartbeat
        sprintf(m,"Hello World %d!  ",k);
        _CP0_SET_COUNT(0);
        LCD_print(m,28,32,ILI9341_WHITE,ILI9341_BLACK);
        LCD_drawbar(k,100,5,28,40,ILI9341_CYAN,ILI9341_BLACK);
        k++; if(k>100){k=0;}
        sprintf(m,"%d Hz",24000000/(_CP0_GET_COUNT())); //frame rate
        LCD_print(m,0,312,ILI9341_MAGENTA,ILI9341_BLACK);       
      }
      
  
    }
}
     