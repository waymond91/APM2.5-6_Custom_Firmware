#include "MS5611.h"

//Class Constructor
MS5611::MS5611(){
}

void MS5611::initialize(){
  pinMode(pressureSelect, OUTPUT);
  pinMode(imuSelect, OUTPUT);

  digitalWrite(pressureSelect, HIGH);
  digitalWrite(imuSelect, HIGH);
}

void spiWrite(uint8_t reg){
  uint8_t dump;
  digitalWrite(pressureSelect, LOW);
  dump = SPI.transfer(reg);
  digitalWrite(pressureSelect, HIGH);
}

uint16_t spiRead16Bits(uint8_t reg){
  uint8_t dump, byte_H, byte_L;
  uint16_t return_value;

  digitalWrite(pressureSelect, LOW);
  dump = SPI.transfer(reg);
  byte_H = SPI.transfer(0x00);
  byte_L = SPI.transfer(0x00);
  digitalWrite(pressureSelect, HIGH);

  return_value = ((uint16_t) byte_H << 8) | (byte_L);
  return(return_value);
}

uint32_t spiReadADC(){
  uint8_t dump, byte_H, byte_M, byte_L;
  uint32_t return_value;
  uint8_t reg = 0x00;

  digitalWrite(pressureSelect, LOW);
  dump = SPI.transfer(reg);
  byte_H = SPI.transfer(0x00);
  byte_M = SPI.transfer(0x00);
  byte_L = SPI.transfer(0x00);
  digitalWrite(pressureSelect, HIGH);

  return_value = (((uint32_t) byte_H) << 16 ) | (((uint32_t) byte_M) << 8) | (byte_L);
  return(return_value);
}
