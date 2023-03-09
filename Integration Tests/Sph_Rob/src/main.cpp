#include <Arduino.h>
#include "IMU.h"
#include "robotControl.h"
#include "BTcom.h"

float gyroCalib[3];

void setup() 
{
  Serial.begin(115200);
  float* calib = initIMU();
  gyroCalib[0] = calib[0];
  gyroCalib[1] = calib[1];
  gyroCalib[2] = calib[2];
  BTinit();
}

void loop() 
{
  // Test variables
  float kP = 0.6;
  bool flag = true;
  // float desired = 0; // is going to be coming from python program

  int desired = getBT();
  //Serial.println(desired);

  float heading = poseEstimation(gyroCalib[0], gyroCalib[1], gyroCalib[2]);
  // Serial.println(heading);

  // P control                                                          // works
  if(desired != 9999 && flag == true) // py will set flag 1 until robot gets to target pos
  {
    float error = desired - (int)heading;
    float leftWheel =  100 + kP*error;
    float rightWheel = 100 - kP*error;
    move(leftWheel, rightWheel);

    // Serial.print("left: ");
    // Serial.print(leftWheel);
    // Serial.print(" right: ");
    // Serial.print(rightWheel);
    // Serial.print(" desired: ");
    // Serial.println(desired);

  }
  else if (desired == 9999)
  {
    move(0,0);
    // Serial.println("9999");
  }
}