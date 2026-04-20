#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <math.h>   // needed for pow()

// Change this for each ESP32 node
#define NODE_ID "NODE_4"

// Pointer to BLE scan object
BLEScan *pBLEScan;

// -------- Distance Calculation Function --------
// Converts RSSI into estimated distance (meters)
float calculateDistance(int rssi) {
  int A = -59;      // RSSI at 1 meter (you can calibrate later)
  float n = 2.0;    // environmental factor (2 = open space)

  // Apply distance formula
  return pow(10.0, ((float)(A - rssi)) / (10.0 * n));
}

// -------- Callback when a BLE device is found --------
class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {

  void onResult(BLEAdvertisedDevice advertisedDevice) {

    // Make sure the device has a name
    if (advertisedDevice.haveName()) {

      // Get the device name
      String deviceName = advertisedDevice.getName().c_str();

      // Only react to OUR beacon
      if (deviceName.startsWith("AEGIS_TAG")) {

        // Get RSSI value
        int rssi = advertisedDevice.getRSSI();

        // Convert RSSI to distance
        float distance = calculateDistance(rssi);

        // Print results
        Serial.print(NODE_ID);
        Serial.print(" sees ");
        Serial.print(deviceName);

        Serial.print(" | RSSI: ");
        Serial.print(rssi);

        Serial.print(" | Distance: ");
        Serial.print(distance);
        Serial.println(" m");
      }
    }
  }
};

// -------- Setup --------
void setup() {
  // Start serial monitor
  Serial.begin(115200);

  // Small delay for stability
  delay(1000);

  // Startup message
  Serial.print(NODE_ID);
  Serial.println(" scanner starting...");

  // Initialize BLE (scanner doesn't need a name)
  BLEDevice::init("");

  // Get scanner object
  pBLEScan = BLEDevice::getScan();

  // Attach callback
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());

  // Enable active scan (more accurate)
  pBLEScan->setActiveScan(true);

  // Scan timing settings
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);
}

// -------- Loop --------
void loop() {

  // Print scanning status
  Serial.print(NODE_ID);
  Serial.println(" scanning...");

  // Scan for 3 seconds
  pBLEScan->start(3, false);

  // Clear old results to free memory
  pBLEScan->clearResults();

  // Small delay before next scan
  delay(1000);
}