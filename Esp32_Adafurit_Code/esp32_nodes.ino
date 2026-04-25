#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <math.h>
#include <WiFi.h>
#include <WiFiUdp.h>

// Change this for each ESP32 node (1-4)
#define NODE_ID "NODE_4"

// -------- WiFi settings --------
const char* ssid     = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

// -------- Pi's IP and port --------
const char* piAddress = "192.168.1.156";
const int piPort = 5005;

WiFiUDP udp;
BLEScan *pBLEScan;

float calculateDistance(int rssi) {
  int A = -59;
  float n = 2.0;
  return pow(10.0, ((float)(A - rssi)) / (10.0 * n));
}

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    if (advertisedDevice.haveName()) {
      String deviceName = advertisedDevice.getName().c_str();
      if (deviceName.startsWith("AEGIS_TAG")) {
        int rssi = advertisedDevice.getRSSI();
        float distance = calculateDistance(rssi);

        Serial.print(NODE_ID);
        Serial.print(" sees ");
        Serial.print(deviceName);
        Serial.print(" | RSSI: ");
        Serial.print(rssi);
        Serial.print(" | Distance: ");
        Serial.print(distance);
        Serial.println(" m");

        // send UDP packet to Pi
        String message = String(NODE_ID) + "," +
                         deviceName + "," +
                         String(rssi) + "," +
                         String(distance);
        udp.beginPacket(piAddress, piPort);
        udp.print(message);
        udp.endPacket();
      }
    }
  }
};

void setup() {
  Serial.begin(115200);
  delay(1000);

  // connect to WiFi
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected! IP: ");
  Serial.println(WiFi.localIP());
  udp.begin(piPort);

  Serial.print(NODE_ID);
  Serial.println(" scanner starting...");

  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);
}

void loop() {
  Serial.print(NODE_ID);
  Serial.println(" scanning...");
  pBLEScan->start(3, false);
  pBLEScan->clearResults();
  delay(1000);
}