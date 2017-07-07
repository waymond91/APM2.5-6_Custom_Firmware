#include "MPU6000.h"

//Class constructor
MPU6000::MPU6000(){
}

void MPU6000::initialize(){

  //De-select all devices on SPI bus
  pinMode(imuSelect, OUTPUT);
  pinMode(pressureSelect, OUTPUT);
  digitalWrite(imuSelect, HIGH);
  digitalWrite(pressureSelect, HIGH);

  //Initialize SPI Bus
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV16); //Set to 1 Mhz
  SPI.setDataMode(SPI_MODE3);

  //Reset the IMU
  writeReg(MPUREG_PWR_MGMT_1, BIT_H_RESET);
  delay(100);
  writeReg(MPUREG_PWR_MGMT_1, MPU_CLK_SEL_PLLGYROZ);

  //Disable internal I2C Bus
  writeReg(MPUREG_USER_CTRL, BIT_I2C_IF_DIS);

  /*
   This next section really depends on the physical performance of the aircraft.

   It seems reasonable that the aircraft would not be mechanically oscillating
   100Hz, so we set the internal digital low-pass filter to a cutoff frequency
   of 98Hz.
   It is worth noting that we don't really know kind of digital filter this is,
   or how sharp the corner really is.
   */
   writeReg(MPUREG_ACCEL_CONFIG, BITS_DLPF_CFG_98HZ);

   /*
   Configuring the gyroscope scale:
   It is preferable to have the smallest possible scale to ensure
   gyroscopic accuracy.
   If the aircraft was in a freespin, it seems quite possible for it to be
   rotating once per second = 60 Hz.
   However, this does not really consider fast impulses that do not cause the
   aircraft to complete a full rotation.
   Perhaps this value should be measured later.
   To be safe, configure gyroscope to +/- 1000 deg/sec = 167 rpm.
   */
   writeReg(MPUREG_GYRO_CONFIG, BITS_FS_1000DPS);

   /*
   Configuring the accelerometer scale:
   It is hard to imagine/estimate the g-forces on an RC aircraft.
   We know that there is constantly 1 full G on the aircraft.

   At the moment, we plan on using the accelerometer to calculate the DIRECTION
   of gravity. So we are more concerned with the direction of the vector, not
   the magnitude of it.

   This value should be re-examined once the accelerometer + AHRS can be used to
   estimate the linear acceleration of the aircraft.
   */
   writeReg(MPUREG_ACCEL_CONFIG, BITS_FS_4G);

   /*
   Configuring Sample Rate:
   We have the option to set the sample rate inside the MPU6000 and have the
   device trigger an external interrupt on pin PE6 (schematic is garbled, could be PJ3?)
   However, for the moment, lets use the ATMEGA2560 to control the sample rate
   using a timer interrupt, and leave the MPU6000s FIFO and external interrupt
   pin disabled.
   */
   writeReg(MPUREG_FIFO_EN, (uint8_t) 0x00); //Disable FIFO
   writeReg(MPUREG_INT_ENABLE, (uint8_t)0x01); //Generate interrupt whenever data is ready

}

void MPU6000::readImu(){
  uint8_t byte_H, byte_L, dump;

  //Start a stream read of the sensor outputs:
  uint8_t addr = MPUREG_ACCEL_XOUT_H | 0x80;

  digitalWrite(imuSelect, LOW);

  dump = SPI.transfer(addr);
  // Read AccelX
  byte_H = SPI.transfer(0);
  byte_L = SPI.transfer(0);
  accelX = ((int)byte_H << 8) | byte_L;

  // Read AccelY
  byte_H = SPI.transfer(0);
  byte_L = SPI.transfer(0);
  accelY = ((int)byte_H << 8) | byte_L;

  // Read AccelZ
  byte_H = SPI.transfer(0);
  byte_L = SPI.transfer(0);
  accelZ = ((int)byte_H << 8)| byte_L;

  // Read Temp
  byte_H = SPI.transfer(0);
  byte_L = SPI.transfer(0);
  mpu_temp = ((int)byte_H << 8)| byte_L;

  // Read GyroX
  byte_H = SPI.transfer(0);
  byte_L = SPI.transfer(0);
  gyroX = ((int)byte_H << 8)| byte_L;

  // Read GyroY
  byte_H = SPI.transfer(0);
  byte_L = SPI.transfer(0);
  gyroY = ((int)byte_H << 8) | byte_L;

  // Read GyroZ
  byte_H = SPI.transfer(0);
  byte_L = SPI.transfer(0);
  gyroZ = ((int)byte_H <<8 ) | byte_L;

  digitalWrite(imuSelect, HIGH);
}

uint8_t MPU6000::readReg(uint8_t reg){
  uint8_t value;
  digitalWrite(imuSelect, LOW);
  SPI.transfer(reg | 0x80); //Set read bit
  value = SPI.transfer((uint8_t) 0x00);
  digitalWrite(imuSelect, HIGH);
  return value;
}

void MPU6000::writeReg(uint8_t reg, uint8_t data){
  uint8_t dump;

  digitalWrite(imuSelect, LOW);
  dump = SPI.transfer(reg);
  dump = SPI.transfer(data);
  digitalWrite(imuSelect, HIGH);
}
