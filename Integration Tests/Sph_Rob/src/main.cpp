#include <Arduino.h>
#include "IMU.h"
#include "robotControl.h"

void setup() {
  Serial.begin(115200);
  initIMU();
  BTinit();
}

void loop() {
  float desired = 0; // is going to be coming from python program
  float kP = 0.2;
  bool flag = true;
  float heading = poseEstimation();
  // Serial.println(heading);

  // P control
  if(flag) // py will set flag 1 until robot gets to target pos
  {
    float error = desired - heading;
    float leftWheel = 100 + kP*error;
    float rightWheel = 100 - kP*error;
    move(leftWheel, rightWheel);
  }

}