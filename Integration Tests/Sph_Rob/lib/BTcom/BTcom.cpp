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
    String input = "0";
    SerialBT.flush();
    if (SerialBT.available() > 0) 
    {

        input = SerialBT.readStringUntil('\n').toInt();

        int val= input.toInt();
        return val;
    }
    return 9999;
}