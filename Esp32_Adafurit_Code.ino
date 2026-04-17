#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEAdvertising.h>
#include <BLEScan.h>

BLEScan* pBLEScan;
BLEAdvertising* pAdvertising;

int rssi1 = 0, rssi2 = 0, rssi3 = 0;
int lastRssi1 = 0, lastRssi2 = 0, lastRssi3 = 0;
bool hasAny = false;

void updateAdvertisement() {
  pAdvertising->stop();

  BLEAdvertisementData advData;
  advData.setName("ESP32_Node4");

  if (hasAny) {
    String mfData = "";
    mfData += (char)0xFF;
    mfData += (char)0xFF;
    mfData += (char)(rssi1 & 0xFF);
    mfData += (char)((rssi1 >> 8) & 0xFF);
    mfData += (char)(rssi2 & 0xFF);
    mfData += (char)((rssi2 >> 8) & 0xFF);
    mfData += (char)(rssi3 & 0xFF);
    mfData += (char)((rssi3 >> 8) & 0xFF);
    advData.setManufacturerData(mfData);
  }

  pAdvertising->setAdvertisementData(advData);
  pAdvertising->start();
}

class MyCallbacks : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) override {
    if (!advertisedDevice.haveName()) return;
    String name = advertisedDevice.getName().c_str();

    if (name == "ESP32_Node1") { rssi1 = advertisedDevice.getRSSI(); hasAny = true; }
    if (name == "ESP32_Node2") { rssi2 = advertisedDevice.getRSSI(); hasAny = true; }
    if (name == "ESP32_Node3") { rssi3 = advertisedDevice.getRSSI(); hasAny = true; }
  }
};

void setup() {
  Serial.begin(19200);
  delay(1000);

  BLEDevice::init("ESP32_Node4");
  BLEServer* pServer = BLEDevice::createServer();
  pAdvertising = BLEDevice::getAdvertising();
  updateAdvertisement();

  pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyCallbacks());
  pBLEScan->setActiveScan(true);
}

void loop() {
  pBLEScan->start(2, false);
  pBLEScan->clearResults();

  if (hasAny && (rssi1 != lastRssi1 || rssi2 != lastRssi2 || rssi3 != lastRssi3)) {
    updateAdvertisement();
    lastRssi1 = rssi1; lastRssi2 = rssi2; lastRssi3 = rssi3;
  }

  delay(500);
}


// hello worjknfdshnjds 