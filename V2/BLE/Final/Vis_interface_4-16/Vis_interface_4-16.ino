#include <ArduinoBLE.h>

#define SERVICE_UUID "229739d7-b3a2-48c0-aa48-16f181fc7dc4"
#define CHARACTERISTIC_UUID "4401ba75-2b0f-4b18-9028-696b467dafe9"

BLEService service(SERVICE_UUID);
BLECharacteristic characteristic(CHARACTERISTIC_UUID, BLENotify | BLERead | BLEWrite, 2);

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
  std::string heading = "00";
  if (central) {
    Serial.print("Connected to central device: ");
    Serial.println(central.address());

    while (central.connected()) {
      // Read the data from the characteristic
      byte heading[2];
      int len = characteristic.valueLength();
      characteristic.readValue(heading, len);
      
      // Set the value of the characteristic to the data received from Python
      characteristic.setValue(heading, 2);
      
      // Print the received heading value
      Serial.print("Heading: ");
      Serial.print(heading[0]);
      Serial.print(", ");
      Serial.println(heading[1]);


      // Wait for a short period of time before sending the next data packet
      delay(100);
    }

    Serial.println("Disconnected from central device");
  }
}
