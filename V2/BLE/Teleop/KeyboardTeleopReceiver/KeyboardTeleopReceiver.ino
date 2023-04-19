#include <ArduinoBLE.h>

const int BIN_1 = 10; // Need to assign more meaningful names
const int BIN_2 = 9;
const int AIN_2 = 8;
const int AIN_1 = 7;

BLEService service("19B10000-E8F2-537E-4F6C-D104768A1214");
BLECharCharacteristic characteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite | BLENotify);

void setup() {
  Serial.begin(9600);
  while (!Serial);
  
  // Initialize BLE
  if (!BLE.begin()) {
    Serial.println("Failed to initialize BLE");
    while (1);
  }
  
  // Add the service and characteristic
  BLE.setLocalName("ArduinoBLETeleop");

  BLE.setAdvertisedService(service);
  BLE.addService(service);
  service.addCharacteristic(characteristic);

  // Start advertising the BLE service
  BLE.advertise();
  
  Serial.println("Waiting for connection...");
}

void loop() {
  // Check for a BLE connection
  BLEDevice central = BLE.central();

  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());

    while (central.connected()) {
      // Check for a message from the Python program
      if (characteristic.written()) {
        // Read the message and print it to the serial monitor
        char message = characteristic.value();
        Serial.println(message);
        keyboardHandler(message);
      }
    }

    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
  }
}

void initMotors()
{
  Serial.println("Initializing motor states");
//  pinMode(BIN_1, OUTPUT);
//  pinMode(BIN_2, OUTPUT);
//  pinMode(AIN_1, OUTPUT);
//  pinMode(AIN_2, OUTPUT);
//  
//  digitalWrite(BIN_1, LOW);
//  digitalWrite(BIN_2, LOW);
//  digitalWrite(AIN_1, LOW);
//  digitalWrite(AIN_2, LOW);
//  
  }

void stop()
{
  
  Serial.println("Stopped");  
//  analogWrite(BIN_1, 0);
//  digitalWrite(BIN_2, LOW);
//  analogWrite(AIN_2, 0);
//  digitalWrite(AIN_1, LOW);
//  
  }

void move(int uL, int uR) // need to add ability to take negative values
{
  // non-blocking
  static unsigned long pulse = 0;

  if(millis() - pulse > 20) {

//     if(uL >= 0)    
//      {
//        digitalWrite(BIN_1, LOW); // send normal PWM
//        digitalWrite(BIN2, HIGH);
//        analogWrite(BIN2, uL);
//      }
//      else
//      {
//        digitalWrite(BIN_1, HIGH); // change direction
//        digitalWrite(BIN_2, LOW);
//        analogWrite(BIN_1, -uL);
//      }
//    
//      if(uR >= 0)
//      {
//        digitalWrite(AIN_1, LOW); // send normal PWM
//        digitalWrite(AIN_2, HIGH);
//        analogWrite(AIN_2, uR);
//      }
//      else
//      {
//        digitalWrite(AIN_1, HIGH); // change direction
//        digitalWrite(AIN_2, LOW);
//        analogWrite(AIN_1, -uR);
//      }
//    analogWrite(BIN_1, uR);
//    analogWrite(AIN_2, uL);
    Serial.print(uL);
    Serial.print(' ');
    Serial.println(uR);
    pulse = millis();

  } 
}

void keyboardHandler(char command)
{
  switch(command) {
    case 'w':
      Serial.println("Move Forward");
      // move(80 ,80);
      break;
    case 'a':
      Serial.println("Turn Left");
      // move(-80 ,80);
      break;
    case 's':
      Serial.println("Move Backward");
      // move(-80 ,-80);
      break;
    case 'd':
      Serial.println("Turn Right");
      // move(80 ,-80);
      break;
    default:
      Serial.println("Invalid Entry");
      // stop();
      break;
    }
}
