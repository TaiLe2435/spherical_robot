#include <ArduinoBLE.h>

#define SERVICE_UUID "229739d7-b3a2-48c0-aa48-16f181fc7dc4"
#define CHARACTERISTIC_UUID "4401ba75-2b0f-4b18-9028-696b467dafe9"

BLEService service(SERVICE_UUID);
BLECharacteristic characteristic(CHARACTERISTIC_UUID, BLENotify | BLERead | BLEWrite, 3);

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!BLE.begin()) {
    Serial.println("Failed to initialize Bluetooth");
    while (1);
  }

  BLE.setLocalName("ArduinoBLE");

  BLE.setAdvertisedService(service);
  service.addCharacteristic(characteristic);
  BLE.addService(service);

  BLE.advertise();
  Serial.println("Bluetooth device active, waiting for connections...");
}

void loop() {
  BLEDevice central = BLE.central();  // Wait for a central device to connect
  std::string heading = "000";
  if (central) {
    Serial.print("Connected to central device: ");
    Serial.println(central.address());

    while (central.connected()) {
      // Read the data from the characteristic
      uint8_t* heading;
      int len = characteristic.valueLength();
      heading = new uint8_t[len];
      characteristic.readValue(heading, len);
//      Serial.print("Heading: ");
//      for (int i = 0; i < len; i++) {
//        Serial.print(heading[i]);
//        Serial.print(", ");
//      }
//      Serial.println();
//      delete[] heading;

      characteristic.setValue(heading, 3);
      
      // Print the received heading value
      Serial.print("Heading: ");
      Serial.print(heading[0]);
      Serial.print(", ");
      Serial.print(heading[1]);
      Serial.print(", ");
      Serial.println(heading[2]);

      // Wait for a short period of time before sending the next data packet
      delay(100);
    }

    Serial.println("Disconnected from central device");
  }
}
