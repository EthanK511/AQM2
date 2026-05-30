#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <SensirionI2cScd4x.h>
#include "time.h"

// User config
const char* WIFI_SSID     = "SSID";
const char* WIFI_PASSWORD = "PASS";

// Database info
const char* FIREBASE_HOST   = "HOST ADRESS";
const char* FIREBASE_SECRET = "DATABASE SECRET";
const char* DEVICE_ID       = "DEVICE NAME";

const uint64_t SLEEP_SECONDS = 30ULL;

const char* NTP_POOL = "pool.ntp.org";
const long  GMT_OFFSET_SEC = 0;
const int   DAYLIGHT_OFFSET_SEC = 0;

// timout setings
const unsigned long WIFI_CONNECT_TIMEOUT_MS   = 10000UL;
const unsigned long FIREBASE_HTTP_TIMEOUT_MS  = 8000UL;
const unsigned long NTP_SYNC_TIMEOUT_MS       = 4000UL;
const uint8_t       SCD4X_I2C_ADDRESS         = 0x62;
const uint8_t       SENSOR_INIT_RETRIES       = 3;
const uint8_t       SENSOR_DATA_ATTEMPTS      = 12;
const unsigned long SENSOR_POWERUP_DELAY_MS   = 1500;
const unsigned long SENSOR_READY_POLL_MS      = 1000;
const unsigned long SENSOR_FIRST_MEASUREMENT_DELAY_MS = 7000;

// SCD41 io pin config
const int I2C_SDA_PIN = 6;
const int I2C_SCL_PIN = 7;

// devices
SensirionI2cScd4x scd4x;
WiFiClientSecure wifiClientSecure;

unsigned long getUnixTime() {
  time_t now = 0;
  time(&now);
  if (now < 1600000000UL) return 0;
  return (unsigned long) now;
}

bool connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < WIFI_CONNECT_TIMEOUT_MS) {
    delay(200);
  }
  return (WiFi.status() == WL_CONNECTED);
}

bool sendToFirebase(uint16_t co2, float temperature, float humidity, unsigned long timestamp) {
  String url = String("https://") + FIREBASE_HOST + "/readings/" + DEVICE_ID + ".json?auth=" + FIREBASE_SECRET;

  StaticJsonDocument<256> doc;
  doc["co2"] = co2;
  doc["temperature"] = temperature;
  doc["humidity"] = humidity;
  doc["timestamp"] = timestamp;
  doc["status"] = "OK";
  String payload;
  serializeJson(doc, payload);

  wifiClientSecure.setInsecure();
  HTTPClient http;
  http.setTimeout(FIREBASE_HTTP_TIMEOUT_MS);
  if (!http.begin(wifiClientSecure, url)) {
    http.end();
    return false;
  }
  http.addHeader("Content-Type", "application/json");
  int httpCode = http.POST(payload);
  http.end();
  return (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_CREATED || httpCode == HTTP_CODE_NO_CONTENT);
}

void logErrorToFirebase(const char* message) {
  if (WiFi.status() != WL_CONNECTED) return;
  String url = String("https://") + FIREBASE_HOST + "/errors/" + DEVICE_ID + ".json?auth=" + FIREBASE_SECRET;
  StaticJsonDocument<192> doc;
  doc["error"] = message;
  doc["timestamp"] = getUnixTime();
  String payload;
  serializeJson(doc, payload);

  wifiClientSecure.setInsecure();
  HTTPClient http;
  http.setTimeout(4000);
  if (http.begin(wifiClientSecure, url)) {
    http.addHeader("Content-Type", "application/json");
    http.POST(payload);
    http.end();
  }
}

void printWakeReason() {
  esp_sleep_wakeup_cause_t reason = esp_sleep_get_wakeup_cause();
  switch (reason) {
    case ESP_SLEEP_WAKEUP_TIMER: Serial.println("Wakeup reason: TIMER"); break;
    case ESP_SLEEP_WAKEUP_EXT0:  Serial.println("Wakeup reason: EXT0");  break;
    case ESP_SLEEP_WAKEUP_EXT1:  Serial.println("Wakeup reason: EXT1");  break;
    case ESP_SLEEP_WAKEUP_UNDEFINED: Serial.println("Wakeup reason: UNDEFINED/RESET"); break;
    default: Serial.printf("Wakeup reason: %d\n", (int)reason); break;
  }
}

void gotoDeepSleep() {
  scd4x.stopPeriodicMeasurement();
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);

  uint64_t sleep_us = (SLEEP_SECONDS == 0 ? 60ULL : SLEEP_SECONDS) * 1000000ULL;
  Serial.printf("Entering deep sleep for %llu seconds...\n", (unsigned long long)(sleep_us / 1000000ULL));
  esp_sleep_enable_timer_wakeup(sleep_us);
  delay(50);
  esp_deep_sleep_start();
}

void setup() {
  Serial.begin(115200);
  delay(50);
  Serial.println("\nBoot");
  printWakeReason();

  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  delay(200);

  bool sensorReady = false;
  for (uint8_t i = 0; i < SENSOR_INIT_RETRIES; ++i) {
    scd4x.begin(Wire, SCD4X_I2C_ADDRESS);
    delay(SENSOR_POWERUP_DELAY_MS);

    scd4x.stopPeriodicMeasurement();
    delay(20);

    if (scd4x.startPeriodicMeasurement() == 0) {
      sensorReady = true;
      break;
    }
    delay(500 + i * 200);
  }

  if (!sensorReady) {
    Serial.println("SCD4x init failed, skipping read");
    logErrorToFirebase("Sensor init failed after wake");
    gotoDeepSleep();
    return;
  }

  delay(SENSOR_FIRST_MEASUREMENT_DELAY_MS);

  if (!connectWiFi()) {
    Serial.println("Wi-Fi connect failed");
    gotoDeepSleep();
    return;
  }

  Serial.print("Wi-Fi connected, IP: ");
  Serial.println(WiFi.localIP());

  configTime(GMT_OFFSET_SEC, DAYLIGHT_OFFSET_SEC, NTP_POOL);
  unsigned long ntpStart = millis();
  while (getUnixTime() == 0 && millis() - ntpStart < NTP_SYNC_TIMEOUT_MS) {
    delay(200);
  }

  bool readOk = false;
  for (uint8_t attempt = 0; attempt < SENSOR_DATA_ATTEMPTS; ++attempt) {
    bool ready = false;
    int16_t statusErr = scd4x.getDataReadyStatus(ready);
    if (statusErr == 0 && ready) {
      uint16_t co2;
      float temperature, humidity;
      if (scd4x.readMeasurement(co2, temperature, humidity) == 0) {
        unsigned long ts = getUnixTime();
        if (ts == 0) ts = millis() / 1000;
        Serial.printf("CO2: %u ppm | T: %.2f C | H: %.2f %% | ts: %lu\n", co2, temperature, humidity, ts);

        if (!sendToFirebase(co2, temperature, humidity, ts)) {
          Serial.println("Firebase send failed");
          logErrorToFirebase("Firebase send failed");
        }
        readOk = true;
        break;
      } else {
        Serial.println("Sensor readMeasurement error");
        logErrorToFirebase("Sensor readMeasurement error");
        delay(500);
      }
    } else {
      if (statusErr != 0) {
        Serial.printf("getDataReadyStatus error: %d\n", statusErr);
        logErrorToFirebase("getDataReadyStatus error");
      } else {
        Serial.println("Data not ready yet, waiting...");
      }
    }
    delay(SENSOR_READY_POLL_MS);
  }

  if (!readOk) logErrorToFirebase("Sensor read failed");

  gotoDeepSleep();
}

void loop() {
}