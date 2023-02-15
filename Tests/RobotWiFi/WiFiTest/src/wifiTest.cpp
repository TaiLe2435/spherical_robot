#include <Arduino.h>
#include <WiFi.h>
#include <iostream>
#include <string>
#include <sstream>
   
// Motor Driver Pins
const int BENBL = 26;
const int BPHASE = 15;
const int AENBL = 27;
const int APHASE = 14;
const int Modepin = 5;

// Global Variables
const float b = 0.0085;
const float l = 0.0365;
const float k = 0.342;
int* ptr;

// Function Definition
void move(int uR, int uL);
void stop();
int* circle(float radius);

const char* ssid = "GoldenPenguinVI";
const char* password =  "LoiLocTai6";

WiFiServer wifiServer(80);

void setup() {

  Serial.begin(115200);

  delay(1000);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");
  Serial.println(WiFi.localIP());

  wifiServer.begin();
}

void loop() {

  WiFiClient client = wifiServer.available();

  if (client) {

    while (client.connected()) {
      String inputPWM = "0";

      if (client.available()>0) {
        inputPWM = client.readStringUntil('\n').toInt();
        // int PWM = c.toInt();
        // client.write("hi");
        int PWM = inputPWM.toInt();
        move(PWM, PWM);
      }

      delay(10);
    }

    client.stop();
    Serial.println("Client disconnected");

  }
}

void move(int uR, int uL) {
  // Setting up motor driver
  digitalWrite(Modepin, HIGH); // turn on
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

void stop() {   // could make a slowly stop function == take in wheel PWMS and slowly decrement them until zero
  // Setting up motor driver
  digitalWrite(Modepin, HIGH); // turn on
  digitalWrite(APHASE, LOW);   // direction: LOW = forward | HIGH = back
  digitalWrite(BPHASE, LOW);
  analogWrite(AENBL, 0);
  analogWrite(BENBL, 0);

  // Serial.println("Stopped");

}

int* circle(float radius) { // 220 for 10 cm 250 for 5 cm
  static int PWM[2] = {0, 255}; // [right PWM, left PWM] 150 lowest 255 highest

  if(radius > 0.06) {
    PWM[1] = 230;
  }

  // float vR = 2*M_PI*(radius + l/2);
  // float vL = 2*M_PI*(radius - l/2);  
  // float v = (vR + vL) / 2;

  // // float w = (vR - vL)/l;

  float v = 0.5;
  float w = v/radius;
  PWM[0] = PWM[1] - (w * l) /( b * k);

  if(PWM[0] < 0) {
    PWM[0] = 0;
  }

  return PWM;
}