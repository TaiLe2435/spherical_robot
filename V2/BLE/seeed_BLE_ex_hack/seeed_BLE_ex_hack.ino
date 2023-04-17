#include <ArduinoBLE.h>

//BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214"); // Bluetooth® Low Energy LED Service
BLEService headingService("229739d7-b3a2-48c0-aa48-16f181fc7dc4");
BLEService distanceService("db31bf8b-76eb-422e-877c-984f4a202758");
//BLEService PongService("0945a7e7-7778-4ea4-a47d-cd1bfa40f37a");
// Bluetooth® Low Energy LED Switch Characteristic - custom 128-bit UUID, read and writable by central
//BLEByteCharacteristic switchCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLEByteCharacteristic headingCharacteristic("4401ba75-2b0f-4b18-9028-696b467dafe9", BLERead | BLEWriteWithoutResponse| BLENotify);
BLEByteCharacteristic distanceCharacteristic("59077dfa-9f09-454b-8ad3-52870c7cbe19", BLERead | BLEWriteWithoutResponse | BLENotify);
//BLEByteCharacteristic PongCharacteristic("afc713c5-7066-4824-8bcd-fddee1c99371", BLERead);

// const int ledPin = LED_BUILTIN; // pin to use for the LED
uint8_t heading = 0;
uint8_t distance = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // set LED pin to output mode
//  pinMode(ledPin, OUTPUT);

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy module failed!");

    while (1);
  }

  // set advertised local name and service UUID:
  BLE.setLocalName("Pong");
  BLE.setAdvertisedService(headingService);
  BLE.setAdvertisedService(distanceService);

  // add the characteristic to the service
  headingService.addCharacteristic(headingCharacteristic);
  distanceService.addCharacteristic(distanceCharacteristic);

  // add service
  BLE.addService(headingService);
  BLE.addService(distanceService);

  // set the initial value for the characeristic:
  headingCharacteristic.writeValue(0);
  distanceCharacteristic.writeValue(0);

  // start advertising
//  BLE.setAdvertisingInterval(32);
//  BLE.setConnectionInterval(8, 8);
  BLE.advertise();
  delay(100);

  Serial.println("Pong Peripheral");
  Serial.print("MAC: ");
  Serial.println(BLE.address());
}

void loop() {
  // listen for Bluetooth® Low Energy peripherals to connect:
  BLEDevice central = BLE.central();
  BLE.poll();
  // if a central is connected to peripheral:
//  if (central) {
//    Serial.print("Connected to central: ");
//    // print the central's MAC address:
//    Serial.println(central.address());

    // while the central is still connected to peripheral:
  if (central) {
        while (central.connected()){
        if (headingCharacteristic.written() || distanceCharacteristic.written()) {
//          heading = headingCharacteristic.value();
//            headingCharacteristic.writeValue(10);
//          distance = distanceCharacteristic.value();  
            headingCharacteristic.readValue(heading);
            distanceCharacteristic.readValue(distance); 
          Serial.print("Heading: ");
          Serial.println(heading);
          Serial.print("Distance: ");
          Serial.println(distance);    
          }
        }
      }

    // when the central disconnects, print it out:
//    Serial.print(F("Disconnected from central: "));
//    Serial.println(central.address());
}
