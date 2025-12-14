#include <SoftwareSerial.h>
#include <DHT.h>

// --- PIN DEFINITIONS ---
#define GREEN_LED 8
#define RED_LED 3
#define BUZZER 6

#define GAS_SENSOR_PIN A0
#define DHT_PIN 2
#define IR_SENSOR_PIN 13
#define DHTTYPE DHT11

#define BT_RX_PIN 10
#define BT_TX_PIN 11

// --- THRESHOLDS ---
const int SMOKE_THRESHOLD = 150;
const float TEMP_THRESHOLD = 30.0;
const int IR_DETECTED_SIGNAL = LOW;

// --- OBJECTS ---
DHT dht(DHT_PIN, DHTTYPE);
SoftwareSerial BTSerial(BT_RX_PIN, BT_TX_PIN);

void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);

  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(IR_SENSOR_PIN, INPUT);

  dht.begin();

  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(RED_LED, LOW);
  digitalWrite(BUZZER, LOW);
}

void loop() {
  float t = dht.readTemperature();
  int smokeValue = analogRead(GAS_SENSOR_PIN);
  int irValue = digitalRead(IR_SENSOR_PIN);

  if (isnan(t)) {
    t = 25.0;
  }

  if (smokeValue > SMOKE_THRESHOLD || t > TEMP_THRESHOLD || irValue == IR_DETECTED_SIGNAL) {
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);
    tone(BUZZER, 1000, 250);

    String alertMessage = "!!! FIRE ALERT !!! ";
    alertMessage += "Smoke: " + String(smokeValue);
    alertMessage += " | Temp: " + String(t) + "C";

    if (irValue == IR_DETECTED_SIGNAL) {
      alertMessage += " | FLAME DETECTED";
    }

    Serial.println(alertMessage);
    BTSerial.println(alertMessage);

    delay(300);
    noTone(BUZZER);
    delay(200);
  } else {
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW);
    noTone(BUZZER);

    String statusMessage = "Status: SAFE | Smoke: " + String(smokeValue);
    statusMessage += " | Temp: " + String(t) + "C";
    statusMessage += String(" | IR: ") + (irValue == HIGH ? "Clear" : "Object");

    Serial.println(statusMessage);
    delay(1500);
  }
}
