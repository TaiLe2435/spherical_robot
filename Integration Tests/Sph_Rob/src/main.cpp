#include <Arduino.h>
#include "IMU.h"

void setup() {
  Serial.begin(115200);
  initIMU();
}

void loop() {
  float heading = 0;
  heading = poseEstimation();
  Serial.println(heading);
}