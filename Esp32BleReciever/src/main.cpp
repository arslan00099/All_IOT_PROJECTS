#include <BLEDevice.h>

// Replace with the service UUID of the BLE device you want to receive broadcasts from
#define SERVICE_UUID "1234-5678-90ab-cdef-0123456789ab"

void setup() {
  Serial.begin(115200);

  // Initialize BLE
  BLEDevice::init("");
  if (!BLEDevice::begin()) {
    Serial.println("Error initializing BLE");
    while (1);
  }

  // Set the device name, optional
  BLEDevice::setName("BLE Receiver");

  // Create a BLE scan callback
  BLEScanCallback scanCallback = new BLEAdvertisedDeviceCallbacks();
  // Start scanning for advertising devices
  BLEDevice::startScanning(scanCallback);
}

void loop() {
  BLEDevice::poll();  // Check for BLE events
}

class BLEAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
 public:
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    // Check if the advertised service matches the target service
    if (advertisedDevice.getServiceUUID().toString() == SERVICE_UUID) {
      Serial.print("BLE Device Found: ");
      Serial.println(advertisedDevice.getAddress().toString());

      // Get the advertised payload (if available)
      if (advertisedDevice.isAdvertisingData()) {
        std::string payload = advertisedDevice.getAdvertisingData().getString();
        Serial.print("Payload: ");
        Serial.println(payload);
      }
    }
  }
};
