#include "ESP32_NOW.h"
#include "WiFi.h"
#include <esp_mac.h>

// Slave Board Mac: 68:FE:71:80:5D:C0
// Master Mac Address: 94:51:DC:5D:4C:6C

#define ESPNOW_WIFI_CHANNEL 6
struct JoystickPositions {
  uint16_t xVal;
  uint16_t yVal;
};

JoystickPositions received;

void onDataReceive(const esp_now_recv_info* info, const uint8_t* data, int len) {
  memcpy(&received, data, sizeof(received));

  Serial.print("x: ");
  Serial.print(received.xVal);
  Serial.print(" | y: ");
  Serial.println(received.yVal);
}

void setup() {
  Serial.begin(115200);

  // Initialize the Wi-Fi module
  WiFi.mode(WIFI_STA);
  WiFi.setChannel(ESPNOW_WIFI_CHANNEL);
  while (!WiFi.STA.started()) {
    delay(100);
  }
  // Initialize the ESP-NOW protocol
  if (!ESP_NOW.begin()) {
    Serial.println("Failed to initialize ESP-NOW");
    Serial.println("Reeboting in 5 seconds...");
    delay(5000);
    ESP.restart();
  }

  // Call onDataReceive function when data arrives
  esp_now_register_recv_cb(onDataReceive);
}

void loop() {
}
