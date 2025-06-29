#define BLYNK_TEMPLATE_ID "TMPL6TLhbjqXy"
#define BLYNK_TEMPLATE_NAME "Gas Leakage"
#define BLYNK_AUTH_TOKEN "6uD8oU5kYVyvq4IoZDgCm75tR-NpEd3-"

#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "DESKTOP-8BL3MIG 5639";       // Your WiFi name
char pass[] = "786786786";          // Your WiFi password

int smokeA0 = A0;                  // Analog pin for MQ-135
int sensorThres = 300;            // Adjust this threshold based on real readings

unsigned long warmupTime = 30000; // 30 seconds warm-up for MQ-135
unsigned long startTime;

BlynkTimer timer;

void sendSensor() {
  // Wait for sensor warm-up
  if (millis() < startTime + warmupTime) {
    Serial.println("Warming up sensor...");
    return;
  }

  int raw = analogRead(smokeA0);           // 0–1023 from MQ-135
  int scaled = map(raw, 0, 1023, 0, 200);  // Scale for gauge 0–200

  Serial.print("Gas Level (raw): ");
  Serial.print(raw);
  Serial.print(" | Scaled: ");
  Serial.println(scaled);

  Blynk.virtualWrite(V0, scaled);  // Update Blynk Gauge

  if (raw > sensorThres) {
    Blynk.logEvent("gas_alert", "Gas Leakage Detected!");
  }
}

void setup() {
  pinMode(smokeA0, INPUT);
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  startTime = millis();  // Save start time for warm-up
  timer.setInterval(2500L, sendSensor); // Every 2.5 seconds
}

void loop() {
  Blynk.run();
  timer.run();
}
