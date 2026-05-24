#include "ESP32_NOW.h"
#include "WiFi.h"
#include <esp_mac.h>
#include "driver/rtc_io.h"  // For sleep mode

// Joystick
#define ESPNOW_WIFI_CHANNEL 6
#define xPin 34
#define yPin 32
#define sleep_pin GPIO_NUM_25  // Left joystick button
#define wake_pin GPIO_NUM_4    // Right joystick button
#define LED_BUILTIN 2
RTC_DATA_ATTR int bootCount = 0;


// Add Slave Mac to peerInfo stuct
uint8_t slaveMAC[] = { 0x68, 0xFE, 0x71, 0x80, 0x5D, 0xC0 };  // Master Mac Address: 94:51:DC:5D:4C:6C
int xVal;
int yVal;

// Package to be sent to the slave board
struct JoystickPositions {
  uint16_t xVal;
  uint16_t yVal;
  bool isButtonPressed;
};
JoystickPositions data;

void print_wakeup_reason() {
  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch (wakeup_reason) {
    case ESP_SLEEP_WAKEUP_EXT0: Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_ULP: Serial.println("Wakeup caused by ULP program"); break;
    default: Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason); break;
  }
}

void setup() {
  Serial.begin(115200);
  delay(500);

  // Joystick unpressed sends high voltage (3.3V)
  // Set both pins high so they are awake and not-sleep at reboot
  pinMode((int)wake_pin, INPUT_PULLUP);
  pinMode((int)sleep_pin, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  //Increment boot number and print it every reboot
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));
  print_wakeup_reason();  //Print the wakeup reason for ESP32

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
  memset(&peerInfo, 0, sizeof(peerInfo));   // Clear out garbage in peerInfo struct before adding Mac @
  memcpy(peerInfo.peer_addr, slaveMAC, 6);  // Add Mac @ which is exactly 6 bytes
  peerInfo.channel = ESPNOW_WIFI_CHANNEL;   // No encryption so slave board does not need to register master Mac@
  peerInfo.encrypt = false;
  esp_now_add_peer(&peerInfo);
}

void loop() {
  /* Send package to slave board */
  data.isButtonPressed = digitalRead((int)sleep_pin) == 0;
  data.xVal = analogRead(xPin);
  data.yVal = analogRead(yPin);
  delay(20);
  esp_now_send(slaveMAC, (uint8_t *)&data, sizeof(data));

  // Printing to console to check x & y values; asleep & awake states
  Serial.print("x: ");
  Serial.print(data.xVal);
  Serial.print(" | y: ");
  Serial.println(data.yVal);

  Serial.print("Timestamp: ");
  Serial.println(millis());
  delay(1000);

  if (digitalRead((int)sleep_pin) == 0) {
    //Tell RTC to watches wake_pin during deep sleep. When wake_pin goes low, wake up
    esp_sleep_enable_ext0_wakeup(wake_pin, 0);  //1 = High, 0 = Low
    rtc_gpio_pulldown_dis(wake_pin);            //disable internal pull down to pin is not forced LOW as LOW means button is pressed
    rtc_gpio_pullup_en(wake_pin);               // enable internal pull up so pin sits HIGH when at rest meaning button is not pressed or activated


    //Go to sleep now
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("Going to sleep now");
    esp_deep_sleep_start();
  }
}
