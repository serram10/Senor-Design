#include <bluefruit.h>

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Starting AEGIS_TAG_1 beacon...");

  // Start BLE
  Bluefruit.begin();
  Bluefruit.setTxPower(4);
  Bluefruit.setName("AEGIS_TAG_1");

  // Clear any old advertising data
  Bluefruit.Advertising.stop();
  Bluefruit.Advertising.clearData();

  // Add standard BLE advertising fields
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();
  Bluefruit.Advertising.addName();

  // Keep advertising continuously
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244);
  Bluefruit.Advertising.setFastTimeout(30);
  Bluefruit.Advertising.start(0);

  Serial.println("AEGIS_TAG_1 is now advertising");
}

void loop() {
  delay(1000);
}