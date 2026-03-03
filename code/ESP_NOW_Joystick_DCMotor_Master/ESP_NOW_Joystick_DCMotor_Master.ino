#include "ESP32_NOW.h"
#include "WiFi.h"
#include <esp_mac.h>

// Joystick
#define xPin 34
#define yPin 35
int xVal;
int yVal;
// Slave Board Mac: 68:FE:71:80:5D:C0
// Master Mac Address: 94:51:DC:5D:4C:6C

#define ESPNOW_WIFI_CHANNEL 6

// Add Slave Mac to peerInfo stuct
uint8_t slaveMAC[] = { 0x68, 0xFE, 0x71, 0x80, 0x5D, 0xC0 };

// Package to be sent to the slave board
struct JoystickPositions {
  uint16_t xVal;
  uint16_t yVal;
};

JoystickPositions data;

void setup() {
  Serial.begin(115200);


  // Initialize the Wi-Fi module
  WiFi.mode(WIFI_STA);
  WiFi.setChannel(ESPNOW_WIFI_CHANNEL);
  while (!WiFi.STA.started()) {
    delay(100);
  }
  esp_now_send(slaveMAC, (uint8_t *)&data, sizeof(data));

  // Initialize the ESP-NOW protocol
  if (!ESP_NOW.begin()) {
    Serial.println("Failed to initialize ESP-NOW");
    Serial.println("Reeboting in 5 seconds...");
    delay(5000);
    ESP.restart();
  }


  esp_now_peer_info_t peerInfo;
  // Clear out garbage in peerInfo struct before adding Mac @
  memset(&peerInfo, 0, sizeof(peerInfo));
  // Add Mac @ which is exactly 6 bytes
  memcpy(peerInfo.peer_addr, slaveMAC, 6);
  peerInfo.channel = ESPNOW_WIFI_CHANNEL;
  // No encryption so slave board does not need to register master Mac@
  peerInfo.encrypt = false;

  esp_now_add_peer(&peerInfo);
}

void loop() {
  data.xVal = analogRead(xPin);
  data.yVal = analogRead(yPin);
  delay(20);
  esp_now_send(slaveMAC, (uint8_t *)&data, sizeof(data));
}
