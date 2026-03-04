#include "ESP32_NOW.h"
#include "WiFi.h"
#include <esp_mac.h>

// H-bridge & DC Motor
#define speedPin_right 32
#define forwardPin_right 33
#define reversePin_right 25

// TO DO : Need to update when connecting to left motor
#define speedPin_left 26
#define forwardPin_left 27
#define reversePin_left 14

// PWM settings
#define pwmFreq 1000
#define pwmResolution 16  // 0–65535

// Initialize idle limits
int deadzone_lower_limit;
int deadzone_upper_limit;

// Prototypes
void move_forward(int pin_forward, int pin_reverse);
void move_backward(int pin_forward, int pin_reverse);
void no_direction(int pin_forward, int pin_reverse);
void set_speed(int y, int speedPin);

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

  // Serial.print("x: ");
  // Serial.print(received.xVal);
  // Serial.print(" | y: ");
  // Serial.println(received.yVal);
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

  // Direction pins
  pinMode(forwardPin_right, OUTPUT);
  pinMode(reversePin_right, OUTPUT);

  pinMode(forwardPin_left, OUTPUT);
  pinMode(reversePin_left, OUTPUT);

  // Attach PWM directly to pin (newer ESP32 core)
  ledcAttach(speedPin_right, pwmFreq, pwmResolution);
  ledcAttach(speedPin_left, pwmFreq, pwmResolution);
  
  // Initialize both motor speeds to 0
  ledcWrite(speedPin_right, 0);
  no_direction(forwardPin_right, reversePin_right);
  ledcWrite(speedPin_left, 0);
  no_direction(forwardPin_left, reversePin_left);

  // Calculate the idle limits
  deadzone_lower_limit = 40;
  deadzone_upper_limit = 60;
}

void loop() {

  // Map joystick values to x and y for easy conversion
  int x = map(received.xVal, 0, 4095, 0, 100);
  int y = map(received.yVal, 4095, 0, 0, 100);

  Serial.print("x: ");
  Serial.print(x);
  Serial.print(" | y: ");
  Serial.println(y);
  
  if (y < deadzone_lower_limit) {
    move_backward(forwardPin_right, reversePin_right);
    move_backward(forwardPin_left, reversePin_left);
    if (x < deadzone_lower_limit) {
      ledcWrite(speedPin_left, 0);
      set_speed(y, speedPin_right);
    } else if (x > deadzone_upper_limit) {
      ledcWrite(speedPin_right, 0);
      set_speed(y, speedPin_left);
    } else {
      set_speed(y, speedPin_right);
      set_speed(y, speedPin_left);
    }
  } else if (y > deadzone_upper_limit) {
    move_forward(forwardPin_right, reversePin_right);
    move_forward(forwardPin_left, reversePin_left);
    if (x < deadzone_lower_limit) {
      ledcWrite(speedPin_left, 0);
      set_speed(y, speedPin_right);
    } else if (x > deadzone_upper_limit) {
      ledcWrite(speedPin_right, 0);
      set_speed(y, speedPin_left);
    } else {
      set_speed(y, speedPin_right);
      set_speed(y, speedPin_left);
    }
  } else {
    ledcWrite(speedPin_right, 0);
    ledcWrite(speedPin_left, 0);
    no_direction(forwardPin_right, reversePin_right);
    no_direction(forwardPin_left, reversePin_left);
  }
}

void move_forward(int pin_forward, int pin_reverse) {
  digitalWrite(pin_forward, HIGH);
  digitalWrite(pin_reverse, LOW);
}
void move_backward(int pin_forward, int pin_reverse){
  digitalWrite(pin_forward, LOW);
  digitalWrite(pin_reverse, HIGH);
}
void no_direction(int pin_forward, int pin_reverse){
  digitalWrite(pin_forward, LOW);
  digitalWrite(pin_reverse, LOW);
}
void set_speed(int y, int speedPin){
  int speed;
  if (y > deadzone_upper_limit){ 
    speed = map(y, 0, 100, 0, 65535);
  } else if (y < deadzone_lower_limit){
    speed = map(y, 100, 0, 0, 65535);
  }
  ledcWrite(speedPin, speed);
} 
