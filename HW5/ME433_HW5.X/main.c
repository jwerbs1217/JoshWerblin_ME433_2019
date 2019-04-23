//main.c
//HW5
#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include "myPIC32.h"     //a buncha #pragmas
#include "IO_expander.h"

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

      initExpander();
      setExpander(0x00,0xF0); //pin 7 is input, pin 0 is output (don't care about others))
      
      setExpander(0x0A,1); //set pin 0 to high 
  
      
      _CP0_SET_COUNT(0);    
      __builtin_enable_interrupts();


  while(1){
      
      if (_CP0_GET_COUNT()>24000000/2){
      LATAINV = 10000; //Heartbeat
      _CP0_SET_COUNT(0);
      }
      
      if(getExpander()>>7 == 0){
          setExpander(0x0A,1); //set pin high
      } else {
          setExpander(0x0A,0);
      }
      
  
    }
}
     