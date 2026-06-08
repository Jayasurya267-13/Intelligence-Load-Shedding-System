#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Sensor
const int sensorPin = A0;

// Relays
const int relayHigh = 8;
const int relayMedium = 9;
const int relayLow = 10;

// Buzzer
const int buzzer = 11;

// Thresholds
float threshold1 = 0.2;
float threshold2 = 0.5;

void setup() {
  pinMode(relayHigh, OUTPUT);
  pinMode(relayMedium, OUTPUT);
  pinMode(relayLow, OUTPUT);
  pinMode(buzzer, OUTPUT);

  Serial.begin(9600);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("System Starting");
  delay(2000);
  lcd.clear();

  // All loads ON initially
  digitalWrite(relayHigh, LOW);
  digitalWrite(relayMedium, LOW);
  digitalWrite(relayLow, LOW);
}

void loop() {
  int sensorValue = analogRead(sensorPin);

  float voltage = sensorValue * (5.0 / 1023.0);
  float current = (voltage - 2.5) / 0.185;

  Serial.print("Current: ");
  Serial.println(current);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("I:");
  lcd.print(current);

  // NORMAL
  if (current < threshold1) {
    digitalWrite(relayHigh, LOW);
    digitalWrite(relayMedium, LOW);
    digitalWrite(relayLow, LOW);
    digitalWrite(buzzer, LOW);

    lcd.setCursor(0, 1);
    lcd.print("ALL Loads ON");
  }

  // MEDIUM OVERLOAD
  else if (current >= threshold1 && current < threshold2) {
    digitalWrite(relayLow, HIGH);     // OFF low
    digitalWrite(relayMedium, LOW);
    digitalWrite(relayHigh, LOW);
    digitalWrite(buzzer, HIGH);

    lcd.setCursor(0, 1);
    lcd.print("Low Load OFF ");
  }

  // SEVERE OVERLOAD
  else {
    digitalWrite(relayLow, HIGH);
    digitalWrite(relayMedium, HIGH);  // OFF medium
    digitalWrite(relayHigh, LOW);     // High stays ON
    digitalWrite(buzzer, HIGH);

    lcd.setCursor(0, 1);
    lcd.print("Med Load OFF ");
  }

  delay(1000);
}