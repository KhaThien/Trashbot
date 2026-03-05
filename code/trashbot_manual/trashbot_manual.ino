#include "ESP32_NOW.h"
#include "WiFi.h"
#include <esp_mac.h>

// Master Mac Address: 94:51:DC:5D:4C:6C
#define ESPNOW_WIFI_CHANNEL 6
#define BRAKE_ON HIGH
#define BRAKE_OFF LOW
#define STOP_ON LOW
#define STOP_OFF HIGH
#define REVERSE_ON LOW
#define REVERSE_OFF HIGH
// LEFT motor
#define directionLeft 19
#define brakeLeft 18
#define speedLeft 17
// RIGHT motor
#define directionRight 26
#define brakeRight 27
#define speedRight 12

enum MotionState { IDLE,
                   FORWARD,
                   REVERSE,
                   LEFT,
                   RIGHT };
MotionState currentState;

struct JoystickPositions {
  uint16_t xVal;
  uint16_t yVal;
};
JoystickPositions received;

// Initialize idle limits
int deadzone_lower_limit;
int deadzone_upper_limit;

// Prototypes
void onDataReceive(const esp_now_recv_info* info, const uint8_t* data, int len);
void setUpPins();
void setUpEspNow();
void idle();
void forward(int directionPin, int speedPin, int speed);
void reverse(int directionPin, int speedPin, int speed);
void brakeOn(int brakePin);
void brakeOff(int brakePin);
void setDirectionForward(int directionPin);
void setDirectionReverse(int directionPin);
void setMotorSpeed(int speedPin, int y);

void setup() {
  Serial.begin(115200);

  setUpPins();
  idle();

  // Call onDataReceive function when data arrives
  setUpEspNow();
  esp_now_register_recv_cb(onDataReceive);

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
    brakeOff(brakeLeft);
    brakeOff(brakeRight);
    if (x < deadzone_lower_limit) {
      analogWrite(speedLeft, 0);
      reverse(directionRight, speedRight, y);
    } else if (x > deadzone_upper_limit) {
      analogWrite(speedRight, 0);
      reverse(directionLeft, speedLeft, y);
    } else {
      reverse(directionLeft, speedLeft, y);
      reverse(directionRight, speedRight, y);
    }
  } else if (y > deadzone_upper_limit) {
    brakeOff(brakeLeft);
    brakeOff(brakeRight);
    if (x < deadzone_lower_limit) {
      analogWrite(speedLeft, 0);
      forward(directionRight, speedRight, y);
    } else if (x > deadzone_upper_limit) {
      analogWrite(speedRight, 0);
      forward(directionLeft, speedLeft, y);
    } else {
      forward(directionLeft, speedLeft, y);
      forward(directionRight, speedRight, y);
    }
  } else {
    idle();
  }
}

void setUpEspNow() {
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
}

void onDataReceive(const esp_now_recv_info* info, const uint8_t* data, int len) {
  memcpy(&received, data, sizeof(received));
}

void setUpPins() {
  pinMode(directionLeft, OUTPUT);
  pinMode(brakeLeft, OUTPUT);
  pinMode(speedLeft, OUTPUT);

  pinMode(directionRight, OUTPUT);
  pinMode(brakeRight, OUTPUT);
  pinMode(speedRight, OUTPUT);

  digitalWrite(directionLeft, REVERSE_OFF);
  digitalWrite(brakeLeft, BRAKE_OFF);
  analogWrite(speedLeft, 0);

  digitalWrite(directionRight, REVERSE_OFF);
  digitalWrite(brakeRight, BRAKE_OFF);
  analogWrite(speedRight, 0);
}

/* State Handlers */
void idle() {
  brakeOn(brakeLeft);
  brakeOn(brakeRight);
  analogWrite(speedLeft, 0);
  analogWrite(speedRight, 0);
}

void setDirectionForward(int directionPin) {
  digitalWrite(directionPin, REVERSE_OFF);
}

void setDirectionReverse(int directionPin) {
  digitalWrite(directionPin, REVERSE_ON);
}
void brakeOn(int brakePin) {
  digitalWrite(brakePin, BRAKE_ON);
}

void brakeOff(int brakePin) {
  digitalWrite(brakePin, BRAKE_OFF);
}

void forward(int directionPin, int speedPin, int speed) {
  setMotorSpeed(speedPin, speed);
  setDirectionForward(directionPin);
}

void reverse(int directionPin, int speedPin, int speed) {
  setMotorSpeed(speedPin, speed);
  setDirectionReverse(directionPin);
}

void setMotorSpeed(int speedPin, int y) {
  int speed;
  if (y > deadzone_upper_limit) {
    speed = map(y, 0, 100, 0, 255);
  } else if (y < deadzone_lower_limit) {
    speed = map(y, 100, 0, 0, 255);
  }
  analogWrite(speedPin, 127);  //Burst
  analogWrite(speedPin, speed);
}
