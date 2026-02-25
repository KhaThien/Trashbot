
// Joystick
#define xPin 34
#define yPin 35
int xVal;
int yVal;

// H-bridge & DC Motor
#define speedPin 32
#define forwardPin 33
#define reversePin 25

// PWM settings
#define pwmFreq 1000
#define pwmResolution 16   // 0–65535

void setup() {
  Serial.begin(115200);

  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
  pinMode(forwardPin, OUTPUT);
  pinMode(reversePin, OUTPUT);

  // Attach PWM directly to pin (newer ESP32 core)
  ledcAttach(speedPin, pwmFreq, pwmResolution);
}

void loop() {

  xVal = analogRead(xPin);
  yVal = analogRead(yPin);

  Serial.print("x: ");
  Serial.print(xVal);
  Serial.print(" | y: ");
  Serial.println(yVal);

  // NOTE: Tested motor is the right wheel

  // JOYSTICK - UP
  if (yVal <= 2800)
  {
    int motorSpeed = map(yVal, 2800, 0, 10000, 65535);
    digitalWrite(forwardPin, HIGH);
    digitalWrite(reversePin, LOW);
    ledcWrite(speedPin, motorSpeed);   // 0–65535
  }

  // JOYSTICK - DOWN
  if (yVal >= 3400)
  {
    int motorSpeed = map(yVal, 3400, 4095, 10000, 65535);
    digitalWrite(forwardPin, LOW);
    digitalWrite(reversePin, HIGH);
    ledcWrite(speedPin, motorSpeed);   // 0–65535
  }

  // JOYSTICK - LEFT
  if (xVal <= 2800)
  {
    int motorSpeed = map(xVal, 2800, 0, 10000, 65535);
    digitalWrite(forwardPin, HIGH);
    digitalWrite(reversePin, LOW);
    ledcWrite(speedPin, motorSpeed);   // 0–65535
  }
  // JOYSTICK - RIGHT
  if (xVal >= 3400)
  {
    int motorSpeed = map(xVal, 3400, 4095, 10000, 65535);
    digitalWrite(forwardPin, LOW);
    digitalWrite(reversePin, LOW);
    ledcWrite(speedPin, 0);   // 0–65535
  }

  // JOYSTICK - IDLE
  if (xVal > 2800 && xVal < 3400 && yVal > 2800 && yVal < 3400)
  {
    digitalWrite(forwardPin, LOW);
    digitalWrite(reversePin, LOW);
  }

}
