#ifndef _MS5611_H
#define _MS5611_H

#include "SPI.h"

//IDK why this has to be out here
const int imuSelect = 53;
const int pressureSelect = 40;

class MS5611{
  public:
    MS5611();
    void initialize();
    uint16_t spiRead16Bits(uint8_t reg);
    uint32_t spiReadADC();
    void spiWrite(uint8_t reg);
    //APM Chip Select Pins:


  private:
};

#endif
