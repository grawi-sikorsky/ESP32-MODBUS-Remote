#include <Arduino.h>
// #include "WiFi.h"
#include "WiFiManager.h"
#include "BluetoothSerial.h"
#include "HTTPClient.h"
#include <ArduinoJson.h>

// DEFINES
// #define WIFI_NAME "Naplet"
// #define WIFI_PASS "Napletszatana89"
#define WIFI_NAME "PrzekleteObliczeBelzebuba"
#define WIFI_PASS "xx"
#define WIFI_DEVICE_NAME "ESP32 Divajs"
#define WIFI_TIMEOUT_MS 20000

#define POST_INTERVAL 60000

WiFiManager wifiManager;

time_t prevTime;

int i = 1;

void sendPost(int someint)
{
  HTTPClient http;
  http.begin("https://modbus-back.herokuapp.com/data");
  http.addHeader("Content-Type", "application/json");

  StaticJsonDocument<200> doc;
  doc["modbusID"] = "modbus1";
  doc["batteryVoltage"] = someint;
  doc["solarVoltage"] = 22;

  String requestBody;
  serializeJson(doc, requestBody);

  int httpResponseCode = http.POST(requestBody);

  http.end();
}

void setup()
{
  Serial.begin(115200);
  wifiManager.autoConnect("ESP32-MODBUS", "password");
}

void loop()
{
  if (millis() - prevTime >= POST_INTERVAL)
  {
    i++;
    Serial.println(i);

    sendPost(i);

    prevTime = millis();
  }
  else
  {
    // TODO: Getting data from modbus sensors?
  }
}