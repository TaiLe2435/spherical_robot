#include <Arduino.h>
#include <BluetoothSerial.h>
#include <iostream>
#include <string>
#include <sstream>
#include <Wire.h> // I2C lib
#include <LSM6.h> // Accel and Gyro lib
#include <LIS3MDL.h> // Magnetometer lib

// Motor Driver Pins
const int BENBL = 2; //bin1
const int BPHASE = 3; // bin2
const int AENBL = 1; // ain2
const int APHASE = 0; // ain1

LSM6 gyroAcc; // creating objects
LIS3MDL mag;

float mx, my, mz;

// Function Definition
void move(int uR, int uL);

void setup() 
{
  Serial.begin(115200);

  // MAG Init
  Wire.begin();
  
  if (!mag.init())
   {
     Serial.println("Failed to detect and initialize mag");
     while(1); // preserves error message on serial monitor
   }
  mag.enableDefault();
  mag.read();

}

void loop() 
{
  // get data from mag
  mag.read();
  mx = mag.m.x;
  my = mag.m.y;
  mz = mag.m.z;

  Serial.print(mx);
  Serial.print(' ');
  Serial.print(my);
  Serial.print(' ');
  Serial.println(mz);

  // move motors
  // move(80, 80);

}

void move(int uR, int uL) {
  /*
  Am a little confused on control of motor driver. I think aph and bph low then pwm to
  aenbl and benbl makes it move forward then aenbl and benbl low with pwm to phase 
  makes it move backwards. But Aayush just has it hard coded to move forward so idk what.
  Going to test with actual DRV8833 board later.
  */

  // Setting up motor driver
  // digitalWrite(Modepin, HIGH); // turn on
  digitalWrite(APHASE, LOW);   // direction: LOW = forward | HIGH = back
  digitalWrite(BPHASE, LOW);

  // non-blocking
  static unsigned long pulse = 0;

  if(millis() - pulse > 20) {

    analogWrite(AENBL, uR);
    analogWrite(BENBL, uL);
    // Serial.print(uL);
    // Serial.print(' ');
    // Serial.println(uR);
    pulse = millis();

  } 

}