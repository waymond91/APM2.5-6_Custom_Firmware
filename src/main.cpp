#include <SPI.h>
#include <MPU6000.h>
#include <Wire.h>
#include <I2Cdev.h>
#include <HMC5883L.h>
//#include <MS5611.h>
#include <APM_PPM.h>

#define PC_BAUD 115200

//LED output pins
const int ledBlue = 25;
const int ledYellow = 26;
const int ledRed = 27;

//SPI Bus SlaveSelect pins
MPU6000 imu;
float accelVec[3];
float gyroVec[3];

#define X 0
#define Y 1
#define Z 2

/*
_______________GPS (uBlox NEO-6M-0-001) Pinout_______________
TX1 = PD3
RX1 = PD2
I believe this is Serial1 on the ATMEGA2560
*/
#define GPS_BAUD 9600

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
HMC5883L mag;
int16_t magVec[3];

/*
_______________Servo PPM Encoder Pinout______________________
ATMEGA32U4    ATMEGA2560
PPM_OUT       PPM_IN = PL_1(IPC5)
*/
APM_PPM rxin;
const int ppmChannelCount = 8;
// Variable definition for Input Capture interrupt


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

  Serial.begin(PC_BAUD);
  Serial1.begin(GPS_BAUD);

  pinMode(ledBlue, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledRed, OUTPUT);



  digitalWrite(ledBlue, HIGH);
  digitalWrite(ledYellow, HIGH);
  digitalWrite(ledRed, HIGH);

  mag.initialize();
  imu.initialize();

  rxin.initialize();



}

void loop () {
  /*
  //Update GPS
  while(Serial1.available()){
    Serial.write(Serial1.read());
  }
  */
  /*
  mag.readScaled();
  imu.readScaled();
  Serial.println(mag.data[0],10);
  delay(1);
  */
  //Serial.println(TCNT5);
  delay(1);
}
