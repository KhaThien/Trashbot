#include "driver/rtc_io.h"

#define wakeup_pin GPIO_NUM_33  // Only RTC IO are allowed
#define sleep_pin GPIO_NUM_25
RTC_DATA_ATTR int bootCount = 0;
RTC_DATA_ATTR int sleep_intiation_count = 0;

/*
  Method to print the reason by which ESP32
  has been awaken from sleep
*/
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
  delay(1000);  //Take some time to open up the Serial Monitor

  pinMode((int)wakeup_pin, INPUT_PULLUP);
  pinMode((int)sleep_pin, INPUT_PULLUP);
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);

  //Increment boot number and print it every reboot
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));

  //Print the wakeup reason for ESP32
  print_wakeup_reason();
}

void loop() {
  Serial.print("Timestamp: ");
  Serial.println(millis());
  delay(1000);

  if (digitalRead((int)sleep_pin) == 0) {
    esp_sleep_enable_ext0_wakeup(wakeup_pin, 0);  //1 = High, 0 = Low
    rtc_gpio_pulldown_dis(wakeup_pin);
    rtc_gpio_pullup_en(wakeup_pin);

    //Go to sleep now
    digitalWrite(2, LOW);
    Serial.println("Going to sleep now");
    esp_deep_sleep_start();
    Serial.println("This will never be printed");
  }
}
