#include <Arduino.h>
#include "WiFiManager.h"
#include "HTTPClient.h"
#include <ArduinoJson.h>
// #include <Adafruit_Sensor.h>
// #include <Adafruit_BMP280.h>
#include <Wire.h>
#include <ModbusMaster.h>

// MOJE INCLUDES
// #include "ModbusData.h"
#include "ModbusReader.h"
#include "SetupData.h"


ModbusMaster node;
//Adafruit_BMP280 bme;
WiFiManager wifiManager;
ModbusReader mbReader;
ModbusData mbData;
SetupData mbSetup;

time_t postPrevTime, setupPrevTime;
time_t bmePrevTime;

int i = 1;


void sendPost(ModbusData data)
{
  HTTPClient http;
  http.begin("https://modbus-back.herokuapp.com/data");
  http.addHeader("Content-Type", "application/json");

  StaticJsonDocument<4000> doc;
  doc["modbusID"]               = data.modbusID;
  doc["pvVoltage"]              = data.pvVoltage;
  doc["pvCurrent"]              = data.pvCurrent;
  doc["pvPower"]                = data.pvPower;
  doc["pvStatus"]               = data.pvStatus;

  doc["batVoltage"]             = data.batVoltage;
  doc["batDischargeCurrent"]    = data.batDischargeCurrent;
  doc["batChargingCurrent"]     = data.batChargingCurrent;
  doc["batChargingPower"]       = data.batChargingPower;
  doc["batRemainingPercent"]    = data.batRemainingPercent;
  doc["batTemperature"]         = data.batTemperature;
  doc["batOverallCurrent"]      = data.batOverallCurrent;
  doc["batStatus"]              = data.batStatus;
  doc["batChargingStatus"]      = data.batChargingStatus;
  doc["batDischargingStatus"]   = data.batDischargingStatus;

  doc["loadVoltage"]            = data.loadVoltage;
  doc["loadCurrent"]            = data.loadCurrent;
  doc["loadPower"]              = data.loadPower;
  doc["loadStatus"]             = data.loadStatus;

  doc["consTotalToday"]         = data.consTotalToday;
  doc["consTotalMonth"]         = data.consTotalMonth;
  doc["consTotalYear"]          = data.consTotalYear;
  doc["consTotalAll"]           = data.consTotalAll;
  doc["genTotalToday"]          = data.genTotalToday;
  doc["genTotalMonth"]          = data.genTotalMonth;
  doc["genTotalYear"]           = data.genTotalYear;
  doc["genTotalAll"]            = data.genTotalAll;
  doc["mpptTemperature"]        = data.mpptTemperature;

  String requestBody;
  serializeJson(doc, requestBody);

  int httpResponseCode = http.POST(requestBody);

  http.end();

  Serial.println("POST sent.");
}

void getSetup(){
  HTTPClient http;
  http.begin("https://modbus-back.herokuapp.com/setup/modbus1");
  http.addHeader("Content-Type", "application/json");

  int httpResponseCode = http.GET();

  DynamicJsonDocument doc(1024);
  deserializeJson(doc, http.getString());

  mbSetup.modbusID              = (const char*)doc["modbusID"];
  mbSetup.readingUpdateInterval = (const char*)doc["readingUpdateInterval"];
  mbSetup.postUpdateInterval    = (const char*)doc["postUpdateInterval"];
  mbSetup.setupUpdateInterval   = (const char*)doc["setupUpdateInterval"];

  http.end();

  Serial.print("GET done with HTTP response: ");
  Serial.println(httpResponseCode);
}

void initRemoteSetup(){

}

void checkResetWifiButton(){
  if(digitalRead(RESET_WIFI_PIN) == LOW){
    wifiManager.resetSettings();
  }
}

void checkManualPostPin(){
  if(digitalRead(MANUAL_POST_PIN) == LOW){
    sendPost(mbData);
    Serial.println("Manual POST sent..");
  }
}

void setup()
{
  Serial.begin(115200);

  wifiManager.autoConnect("ESP32-MODBUS");
  pinMode(RESET_WIFI_PIN, INPUT_PULLUP);
  pinMode(MANUAL_POST_PIN, INPUT_PULLUP);

  // Wire.begin();

  // if (!bme.begin(0x76)) {
  //   Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
  //                     "try a different address!"));
  // }

  mbReader.initModbus();
}

void loop()
{
  if (millis() - postPrevTime >= mbSetup.postUpdateInterval.toInt())
  {
    sendPost(mbReader.mbData);
    postPrevTime = millis();
  }
  else if (millis() - bmePrevTime >= mbSetup.readingUpdateInterval.toInt())
  {
    // mbData.modbusID               = "modbus1";
    // mbData.pvVoltage              = "32";
    // mbData.pvCurrent              = "8";
    // mbData.pvPower                = "202";
    // mbData.pvTotalChargingToday   = "150";
    // mbData.pvTotalCharging        = "12000";
    // mbData.batVoltage             = "25";
    // mbData.batOverallCurrent      = "0.1";
    // mbData.mpptTemperature        = "25";
    // mbData.batStatus              = "Normal";
    // mbData.batChargingStatus      = "No charging";
    // mbData.batDischargingStatus   = "Load on";
    // mbData.loadVoltage            = "12.9";
    // mbData.loadCurrent            = "0";
    // mbData.loadPower              = "0";
    // mbData.energyConsumedToday    = "0";
    // mbData.energyConsumedTotal    = "0";
    // mbData.espTemperature         = bme.readTemperature();
    // mbData.espPressure            = bme.readPressure();

    mbReader.readModbusDataFromDevice();

    bmePrevTime = millis();

  }
  else if(millis() - setupPrevTime >= mbSetup.setupUpdateInterval.toInt())
  {
    getSetup();
    setupPrevTime = millis();
  }
  else
  {
    checkManualPostPin();
    checkResetWifiButton();
  }
}