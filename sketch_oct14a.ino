#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define LED_PIN 5       // D1 → GPIO5
#define SWITCH_PIN 4    // D2 → GPIO4


LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27

bool ledState = false;      // Current state
int brightness = 0;         // LED brightness

// Variables for debouncing
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50; 
int lastSwitchState = HIGH; 
int switchState;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(SWITCH_PIN, INPUT_PULLUP); // Switch to GND
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Buck Converter");
}

void loop() {
  int reading = digitalRead(SWITCH_PIN);

  // Debounce switch
  if (reading != lastSwitchState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != switchState) {
      switchState = reading;

      // Toggle LED state when pressed
      if (switchState == LOW) {
        ledState = !ledState;
      }
    }
  }

  lastSwitchState = reading;

  // Set LED brightness
  if (ledState) {
    brightness = 128;  // Bucked voltage (simulated)
  } else {
    brightness = 255;  // Full voltage (Vin)
  }

  analogWrite(LED_PIN, brightness);

  // Update LCD
  lcd.setCursor(0, 1);
  lcd.print("Vout: ");
  lcd.print(map(brightness, 0, 255, 0, 12)); // Map PWM to 0-12V approx
  lcd.print("V ");

  delay(50);
}
