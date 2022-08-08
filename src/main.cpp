#include <Arduino.h>
#include "WiFi.h"
#include "WiFiManager.h"
#include "BluetoothSerial.h"
#include "HTTPClient.h"
#include <ArduinoJson.h>

// DEFINES
// #define WIFI_NAME "Naplet"
// #define WIFI_PASS "Napletszatana89"
#define WIFI_NAME "PrzekleteObliczeBelzebuba"
#define WIFI_PASS "Lilicapirowek89"
#define WIFI_DEVICE_NAME "ESP32 Divajs"
#define WIFI_TIMEOUT_MS 20000

#define POST_INTERVAL 60000

WiFiManager wifiManager;

BluetoothSerial ESP_BT;
String bluetoothData;
std::string wifiName;
std::string wifiPass;
std::string espName;

time_t prevTime;

int i = 1;

void connectToWifi()
{
  Serial.println("WIFI Connecting...");
  WiFi.mode(WIFI_STA);
  WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
  WiFi.setHostname(WIFI_DEVICE_NAME);
  WiFi.begin(WIFI_NAME, WIFI_PASS);

  unsigned long startTime = millis();

  while (WiFi.status() != WL_CONNECTED && millis() - startTime < WIFI_TIMEOUT_MS)
  {
    Serial.print(" .");
    delay(1000);
  }

  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("WIFI Connection Failed.. ");
  }
  else
  {
    Serial.println("\nWIFI Connection is established.");
  }
}

void setupBluetooth()
{
  ESP_BT.begin("ESP32-Modbus");
}

void setWifiCredentialsFromBT(){
  std::string delimiter = "\n";
  std::string costam = std::string(bluetoothData.c_str());
  const char* temp;
  Serial.println(costam.c_str());
  temp = costam.c_str();

  wifiName = costam.substr(0, costam.find("\n"));
  wifiPass = costam.substr(wifiName.length(), costam.find(delimiter));
  espName = costam.substr(wifiPass.length(), costam.find(delimiter));


  Serial.println(wifiName.c_str());
  Serial.println(wifiPass.c_str());
  Serial.println(espName.c_str());
}

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
  setupBluetooth();
  connectToWifi();
}

void loop()
{
  if (ESP_BT.available())
  {
    bluetoothData = ESP_BT.readString();
    Serial.println(bluetoothData);
    setWifiCredentialsFromBT();
  }

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