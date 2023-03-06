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
  float kP = 0.2;
  bool flag = true;
  // float desired = 0; // is going to be coming from python program

  float desired = getBT();
  // Serial.println(desired);

  float heading = poseEstimation(gyroCalib[0], gyroCalib[1], gyroCalib[2]);
  Serial.println(heading);

  // P control                                                          // works
  if(desired != 9999) // py will set flag 1 until robot gets to target pos
  {
    float error = desired - heading;
    float leftWheel =  kP*error;
    float rightWheel = kP*error;
    move(leftWheel, rightWheel);
  }

}