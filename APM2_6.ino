#include <SPI.h>

//LED output pins
const int ledBlue = 25;
const int ledYellow = 26;
const int ledRed = 27;

//SPI Bus SlaveSelect pins
const int imuSelect = 53;
const int pressureSelect = 40;  

/*
_______________DataFlash(AT45DB161-MU) Pinout_______________
PJ1 = MOSI_DF = 14
PJ2 = SCK_DF = NA
PA6 = CS_DF = 28
PG0 = RST_DEF = 41
I assume these pins go to some SPI hardware...
*/

/*
_______________Magnetometer(HMC5883L) Pinout________________
SCL = PD0 = 21
SDA = PD1 = 20
This seems like the defualt bus
*/

/*
_______________Servo PPM Encoder Pinout______________________
Definitely a lot more going on in the circuit than I orignally thought.
ATMEGA32U4  ATMEGA2560
PPM_PD1      PA2
PPM_PD0      PA1

PPM_PC2 => Throws switch for whole system
This pin is on the ATMEGA32U4

This will throw the connections for pins

RX0-1                     ____PPM_TX (Normally Open)        
ATMEGA2560_PE0 (RX?)_____|____3DR_RX (Normally Closed)

TX0-O                     ____PPM_RX (Normally Open)
ATMEGA2560_PE1 (TX?)_____|____3DR_TX (Normally Closed)
*/


/*
_______________Servo Outputs_________________________________
OUT1 = PB6 = 12
OUT2 = PB5 = 11
OUT3 = PH5 = 8
OUT4 = PH4 = 7
OUT5 = PH3 = 6
OUT6 = PE5 = 3
OUT7 = PE4 = 2
OUT8 = PE3 = 5
*/
const int servo_count = 8;
const int servo_pins[8] = {12,11,8,7,6,3,2,5};

/*
_______________Extra IO_______________________________________
IO1 = PF0 = ADC0
IO2 = PF1 = ADC1
IO3 = PF2 = ADC2
IO4 = PF3 = ADC3
IO5 = PF4 = ADC4
IO6 = PF5 = ADC5
IO7 = PF6 = ADC6
IO8 = PF8 = ADC7
IO9 = PK0 = ADC8
*/
const int io_count = 9;
const int io_pins[9] = {A0, A1, A2, A3, A4, A5, A6, A7, A8};

void setup () {
  
  Serial.begin(9600);
  
  pinMode(ledBlue, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledRed, OUTPUT);
  
  digitalWrite(ledBlue, HIGH);
  digitalWrite(ledYellow, HIGH);
  digitalWrite(ledRed, HIGH);
  
  pinMode(imuSelect, OUTPUT);
  pinMode(pressureSelect, OUTPUT);
  
  digitalWrite(imuSelect, HIGH);
  digitalWrite(pressureSelect, HIGH);
  
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV16);
  SPI.setDataMode(SPI_MODE3);
  delay(100);
}

void loop () {
  
  byte resultat = spi_read(0x3B | 0b10000000);
  Serial.println(resultat, BIN);
  delay(1000);

}

byte spi_read(byte read_command) {
  digitalWrite(imuSelect, LOW);
  SPI.transfer(read_command);
  byte miso_data = SPI.transfer(0);
  digitalWrite(imuSelect, HIGH);
  return miso_data;
}
