# APM2.5-6_Custom_Firmware

This repo provides the low-level drivers required to use 3D Robotics old APM 2.5 & 2.6 boards.
These can be found on ebay and amazon for about $40 and are packed with useful sensors, and 
include all the hardware required to start programming your own small UAV in the arduino IDE.

## Hardware Includes:
-	Atmega2560 (Main MCU)
-	MPU-6000   (3 Axis Gyroscope + 3 Axis Accelerometer)
-	MS5611     (Barometric Pressure Sensor)
-	AT45DB161  (4 Mbytes of data flash)
-	Atmega32u4 (3DR RC Receiver PPM Encoder)
-	3 User Leds
-	HMC5883L   (3 Axis Magnetometer)*
-	NEO-6M-001 (uBlox GPS Receiver)*

*Note that the magnetometer and GPS receiver are often included on a seperate daughter board!

The goal for this repo is simply to provide the functionality required to connect to all of the APMs peripherals, 
allowing you to just start writing (hopefully) your own application firmware quickly.
As a controls engineer, the idea of writing my own cascading PIDs sounds like a fun way to spend a vacation. 
It is NOT fully featured firmware found the 3DR website and it will NOT work the APM Mission Planner software. 	
