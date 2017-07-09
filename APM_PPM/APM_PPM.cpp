#include "Arduino.h"
#include "APM_PPM.h"


volatile unsigned int count, lastCount;
volatile uint16_t _ppmIn_[numChannel];
volatile uint8_t _ppmFlag_;
volatile uint8_t _ppmPass_;

//Class Constructor
APM_PPM::APM_PPM(){
  _ppmPass_ = 0xFF;
}

void APM_PPM::initialize(){

  for(int j = 0; j<servo_count; j++){
    pinMode(servoPin[j], OUTPUT);
  }

  pinMode(debugOut, OUTPUT);
  pinMode(ppmIn, INPUT);

  /*
   * Configure TIMER5 (16bit Timer) to run at 2Mhz
   * and enable the interrupt capture interrupt service routine
   */
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

/*
 * Returns  the most recent PWM input values from the PPM input line.
 *
 * The returned number represents the number of clock ticks between the PWM
 * rising edges. Because TIMER5 is clocking off the main clock at 1/8 the speed:
 * 1 Tick = 1/(2x10^6) seconds
 *
 * Note that this function works even if some or all of the PPM throughput
 * channels have been turned off.
 */
void APM_PPM::read(uint16_t val[numChannel]){
  for(int j = 0; j<numChannel; j++){
    val[j] = _ppmIn_[j];
    _ppmFlag_ &= ~(1<<j);
  }
}

/*
 * Checks to see if ANY new data has been recieved via PPM
 */
bool APM_PPM::newData(){
  if(_ppmFlag_ == 0x00){
    return (false);
  }
  else{
    return (true);
  }
}

 /*
  * Halts PPM throughput to a single servo output.
  *
  * Note that these servo output numbers are zero-indexed.
  */
void APM_PPM::halt(uint8_t servoNum){
  _ppmPass_ &= ~(1 << servoNum);
}

/*
 * Resumes PPM throughput to a single servo output.
 *
 * Note that these servo output numbers are zero-indexed.
 */
void APM_PPM::start(uint8_t servoNum){
  _ppmPass_ |= (1 << servoNum);
}

/*
 * Halts all PPM throughput to servo output.
 */
void APM_PPM::haltAll(){
  _ppmPass_ = (uint8_t)0x00;
}

/*
 * Resumes all PPM throughput to servo output.
 */
void APM_PPM::startAll(){
  _ppmPass_ = (uint8_t) 0xFF;
}

/*
 * Check to see which servos output channels are being driven from the PPM.
 *
 * This returns a bit flag:
 * | BIT 7 | BIT 6 | BIT 5 | BIT 4 | BIT 3 | BIT 2 | BIT 1 | BIT 0 |
 * | SERVO8| SERVO7| SERVO6| SERVO5| SERVO4| SERVO3| SERVO2| SERVO1|
 *
 * If the bit is HIGH than the PPM is controlling that output.
 */

uint8_t APM_PPM::checkRunning(){
  return _ppmPass_;
}

/*
 * This is the primary interrupt handling the input capture for the PPM line.
 */

ISR(TIMER5_CAPT_vect){

  //Check how many clock cycles have elapsed since last time
  uint16_t ticks;
  ticks = (unsigned int)ICR5L;
  ticks |= (unsigned int)ICR5H << 8;

  //If it has been more than 10000 ticks than the signal is valid PPM input
  if( ticks < 10000){
    //Don't write to memory outside of the array
    if(count < numChannel){
     //Update pulsewidth values
     cli(); //Halt interrupts
     _ppmIn_[count] = ticks;
     _ppmFlag_ |= (1 << count);
     sei(); //Restart interrupts

     lastCount = count; //Required to drive the servo outputs low next cycle
   }
    count ++;
  }
  //If it has been more than 10000 ticks = 5mS
  //then reset the count
  else{
    count = 0;
  }

  //Update servo outputs
  if( (_ppmPass_ & ((uint8_t)1 << count)) == (1 << count)  ){
    digitalWrite(servoPin[count],HIGH);
  }
  digitalWrite(servoPin[lastCount],LOW);


  //Reset the clock count
  TCNT5H = 0x00;
  TCNT5L = 0x00;
  // clear input capture flag
  TIFR5 = ( 1 << ICF5 );
}
