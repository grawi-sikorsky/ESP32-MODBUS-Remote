#include <Arduino.h>
#include "WiFiManager.h"
#include "HTTPClient.h"
#include <ArduinoJson.h>
#include <Adafruit_BMP280.h>
#include <Wire.h>

Adafruit_BMP280 bme;

// DEFINES
#define RESET_WIFI_PIN 19

#define POST_INTERVAL 60000
#define BME_INTERVAL 5000

WiFiManager wifiManager;

time_t postPrevTime;
time_t bmePrevTime;

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

void checkResetWifiButton(){
  if(digitalRead(RESET_WIFI_PIN) == LOW){
    wifiManager.resetSettings();
  }
}

void setup()
{
  Serial.begin(115200);
  wifiManager.autoConnect("ESP32-MODBUS");
  pinMode(RESET_WIFI_PIN, INPUT_PULLUP);

  // Wire.begin();
  // Wire.setClock(400000);

  if (!bme.begin()) {
  Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                    "try a different address!"));
  }

  /* Default settings from datasheet. */
  bme.setSampling(Adafruit_BMP280::MODE_FORCED,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

}

void loop()
{
  if (millis() - postPrevTime >= POST_INTERVAL)
  {
    i++;
    Serial.println(i);

    sendPost(i);

    postPrevTime = millis();
  }
  else if (millis() - bmePrevTime >= BME_INTERVAL)
  {
    if (bme.takeForcedMeasurement()) {
      // can now print out the new measurements
      Serial.print(F("Temperature = "));
      Serial.print(bme.readTemperature());
      Serial.println(" *C");

      Serial.print(F("Pressure = "));
      Serial.print(bme.readPressure());
      Serial.println(" Pa");

      Serial.print(F("Approx altitude = "));
      Serial.print(bme.readAltitude(1013.25)); /* Adjusted to local forecast! */
      Serial.println(" m");

      Serial.println();
    } else {
      Serial.println("Forced measurement failed!");
    }

    bmePrevTime = millis();
  }
  else
  {
    checkResetWifiButton();
    // TODO: Getting data from modbus sensors?
  }
}