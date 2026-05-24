#include "ESP32_NOW.h"
#include "WiFi.h"
#include <esp_mac.h>

// ─── Network ───────────────────────────────────────────
// Slave Mac Address: 88:57:21:8E:AD:9C
// Master Mac Address: 94:51:DC:5D:4C:6C
#define ESPNOW_WIFI_CHANNEL 6

// ─── Motor Controller Signals ──────────────────────────
#define BRAKE_OFF LOW
#define BRAKE_ON HIGH
#define REVERSE_OFF HIGH
#define REVERSE_ON LOW
#define STOP_OFF HIGH
#define STOP_ON LOW

// ─── Pin Definitions ───────────────────────────────────
// LEFT motor
#define brakeLeft 18
#define directionLeft 19
#define speedLeft 17
// RIGHT motor
#define directionRight 26
#define brakeRight 27
#define speedRight 12

int deadzone_lower_limit = -15;
int deadzone_upper_limit = 15;
unsigned long lastReceived = 0;

// ─── Data ──────────────────────────────────────────────
struct JoystickPositions {
  uint16_t xVal;
  uint16_t yVal;
  bool isButtonPressed;
};
JoystickPositions received;

// ─── Prototypes ────────────────────────────────────────
void brakeOff(int brakePin);
void brakeOn(int brakePin);
void forward(int directionPin, int speedPin, int speed);
void handleMotion(int x, int y);
void idle();
void onDataReceive(const esp_now_recv_info* info, const uint8_t* data, int len);
void reverse(int directionPin, int speedPin, int speed);
void setDirectionForward(int directionPin);
void setDirectionReverse(int directionPin);
void setMotorSpeed(int speedPin, int y);
void setUpEspNow();
void setUpPins();

// ─── MAIN ────────────────────────────────────────
void setup() {
  Serial.begin(115200);
  setUpPins();
  idle();
  // Call onDataReceive function when data arrives
  setUpEspNow();
  esp_now_register_recv_cb(onDataReceive);
}
void loop() {

  // Map joystick values to x and y for easy conversion
  int x = map(received.xVal, 0, 4095, -100, 100);
  int y = map(received.yVal, 4095, 0, -100, 100);

  Serial.print("x: ");
  Serial.print(x);
  Serial.print(" | y: ");
  Serial.println(y);

  if (millis() - lastReceived > 500) {  // 500ms timeout
    idle();
    return;
  }
  if (received.isButtonPressed) {
    Serial.println("STOP REQUESTED");
    idle();
    delay(1000);
  } else {
    handleMotion(x, y);
  }
}

// ─── Helper Functions ────────────────────────────────────────
void brakeOff(int brakePin) {
  digitalWrite(brakePin, BRAKE_OFF);
}

void brakeOn(int brakePin) {
  digitalWrite(brakePin, BRAKE_ON);
}

void forward(int directionPin, int speedPin, int speed) {
  setMotorSpeed(speedPin, speed);
  setDirectionForward(directionPin);
}

void handleMotion(int x, int y) {
  int slower = abs(y) * (1 - abs(x) / 100.0);
  if (y < deadzone_lower_limit) {
    brakeOff(brakeLeft);
    brakeOff(brakeRight);
    if (x < deadzone_lower_limit) {
      reverse(directionLeft, speedLeft, slower);
      forward(directionRight, speedRight, y);
    } else if (x > deadzone_upper_limit) {
      forward(directionRight, speedRight, slower);
      reverse(directionLeft, speedLeft, y);
    } else {
      reverse(directionLeft, speedLeft, y);
      forward(directionRight, speedRight, y);
    }
  } else if (y > deadzone_upper_limit) {
    brakeOff(brakeLeft);
    brakeOff(brakeRight);
    if (x < deadzone_lower_limit) {
      forward(directionLeft, speedLeft, slower);
      reverse(directionRight, speedRight, y);
    } else if (x > deadzone_upper_limit) {
      reverse(directionRight, speedRight, slower);
      forward(directionLeft, speedLeft, y);
    } else {
      forward(directionLeft, speedLeft, y);
      reverse(directionRight, speedRight, y);
    }
  } else {
    idle();
  }
}

void idle() {
  brakeOn(brakeLeft);
  brakeOn(brakeRight);
  analogWrite(speedLeft, 0);
  analogWrite(speedRight, 0);
}

void onDataReceive(const esp_now_recv_info* info, const uint8_t* data, int len) {
  lastReceived = millis();
  memcpy(&received, data, sizeof(received));
}

void reverse(int directionPin, int speedPin, int speed) {
  setMotorSpeed(speedPin, speed);
  setDirectionReverse(directionPin);
}

void setDirectionForward(int directionPin) {
  digitalWrite(directionPin, REVERSE_OFF);
}

void setDirectionReverse(int directionPin) {
  digitalWrite(directionPin, REVERSE_ON);
}

void setMotorSpeed(int speedPin, int y) {
  int speed = map(abs(y), 0, 100, 0, 64);
  analogWrite(speedPin, 64);  //Burst
  analogWrite(speedPin, speed);
}

void setUpEspNow() {
  WiFi.mode(WIFI_STA);
  WiFi.setChannel(ESPNOW_WIFI_CHANNEL);
  while (!WiFi.STA.started()) {
    delay(100);
  }
  if (!ESP_NOW.begin()) {
    Serial.println("Failed to initialize ESP-NOW");
    Serial.println("Reeboting in 5 seconds...");
    delay(5000);
    ESP.restart();
  }
}

void setUpPins() {
  pinMode(brakeLeft, OUTPUT);
  pinMode(brakeRight, OUTPUT);
  pinMode(directionLeft, OUTPUT);
  pinMode(directionRight, OUTPUT);
  pinMode(speedLeft, OUTPUT);
  pinMode(speedRight, OUTPUT);
  digitalWrite(brakeLeft, BRAKE_OFF);
  digitalWrite(brakeRight, BRAKE_OFF);
  digitalWrite(directionLeft, REVERSE_OFF);
  digitalWrite(directionRight, REVERSE_OFF);
  analogWrite(speedLeft, 0);
  analogWrite(speedRight, 0);
}
