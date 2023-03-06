#include "BTcom.h"

#include <Arduino.h>
#include <BluetoothSerial.h>
#include <iostream>
#include <string>
#include <sstream>

BluetoothSerial SerialBT;

void BTinit()
{

  SerialBT.begin("ESP32test");
  delay(1000);

}

int getBT()
{
    String inputPWM = "0";

    if (SerialBT.available() > 0) 
    {

        inputPWM = SerialBT.readStringUntil('\n').toInt();


        int PWM = inputPWM.toInt();
        return PWM;
    }
    return 9999;
}