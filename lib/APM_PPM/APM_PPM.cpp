#include "Arduino.h"
#include "APM_PPM.h"

volatile unsigned int count;

//Class Constructor
APM_PPM::APM_PPM(){

}

void APM_PPM::initialize(){

  pinMode(debugOut, OUTPUT);
  pinMode(ppmIn, INPUT);

  cli();  //Disable interrupts
  TCCR5A = 0x00; // Clear confiugration registers
  TCCR5B = 0x00;

  TCCR5B = ( 1 << ICES5 ) | ( 1 << CS51); // use rising edge as trigger, prescale_clk/8
  TIMSK5 = ( 1 << ICIE5 ); // allow input capture interrupts

  // Clear TIMER5
  TCNT5H = 0x00;
  TCNT5L = 0x00;
  sei();  //Enable interrupts

  count = 0;
}

ISR(TIMER5_CAPT_vect){

  //Check how many clock cycles have elapsed since last time
  uint16_t high_cnt;
  high_cnt = (unsigned int)ICR5L;
  high_cnt |= (unsigned int)ICR5H << 8;

  //If it has been more than 10000 ticks = 5mS
  //then reset the count
  if( high_cnt < 10000){
    count ++;
  }
  else{
    count = 0;
  }



 if(count < 8){
  //*********************************
  //Put count dependent code below:
  if(count == 7){ // <================== These need to match
    digitalWrite(debugOut, HIGH);
  }
  //********************************
 }
  if(count != 7){ // <================== These need to match
    digitalWrite(debugOut, LOW);
  }


  //Reset the clock count
  TCNT5H = 0x00;
  TCNT5L = 0x00;
  // clear input capture flag
  TIFR5 = ( 1 << ICF5 );

}
