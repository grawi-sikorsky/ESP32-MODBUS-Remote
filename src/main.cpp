#include <Arduino.h>
#include "WiFiManager.h"
#include "HTTPClient.h"
#include <ArduinoJson.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <Wire.h>
#include <ModbusMaster.h>

// MOJE INCLUDES
#include "ModbusData.h"
#include "SetupData.h"

ModbusMaster node;
Adafruit_BMP280 bme;
WiFiManager wifiManager;
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

  StaticJsonDocument<700> doc;
  doc["modbusID"]               = data.modbusID;
  doc["pvVoltage"]              = data.pvVoltage;
  doc["pvCurrent"]              = data.pvCurrent;
  doc["pvPower"]                = data.pvPower;
  doc["pvTotalChargingToday"]   = data.pvTotalChargingToday;
  doc["pvTotalCharging"]        = data.pvTotalCharging;
  doc["batVoltage"]             = data.batVoltage;
  doc["batCurrent"]             = data.batCurrent;
  doc["mpptTemperature"]        = data.mpptTemperature;
  doc["batStatus"]              = data.batStatus;
  doc["batChargingStatus"]      = data.batChargingStatus;
  doc["batDischargingStatus"]   = data.batDischargingStatus;
  doc["loadVoltage"]            = data.loadVoltage;
  doc["loadCurrent"]            = data.loadCurrent;
  doc["loadPower"]              = data.loadPower;
  doc["energyConsumedToday"]    = data.energyConsumedToday;
  doc["energyConsumedTotal"]    = data.energyConsumedTotal;
  doc["espTemperature"]         = data.espTemperature;
  doc["espPressure"]            = data.espPressure;

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
//  Serial.begin(115200);
  Serial.begin(115200);
  Serial2.begin(9600);

  wifiManager.autoConnect("ESP32-MODBUS");
  pinMode(RESET_WIFI_PIN, INPUT_PULLUP);
  pinMode(MANUAL_POST_PIN, INPUT_PULLUP);

  Wire.begin();

  if (!bme.begin(0x76)) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                      "try a different address!"));
  }

  // communicate with Modbus slave ID 2 over Serial (port 0)
  node.begin(1, Serial2);

}

void loop()
{
  if (millis() - postPrevTime >= mbSetup.postUpdateInterval.toInt())
  {
    sendPost(mbData);
    postPrevTime = millis();
  }
  else if (millis() - bmePrevTime >= mbSetup.readingUpdateInterval.toInt())
  {
    mbData.modbusID               = "modbus1";
    mbData.pvVoltage              = "32";
    mbData.pvCurrent              = "8";
    mbData.pvPower                = "202";
    mbData.pvTotalChargingToday   = "150";
    mbData.pvTotalCharging        = "12000";
    mbData.batVoltage             = "25";
    mbData.batCurrent             = "0.1";
    mbData.mpptTemperature        = "25";
    mbData.batStatus              = "Normal";
    mbData.batChargingStatus      = "No charging";
    mbData.batDischargingStatus   = "Load on";
    mbData.loadVoltage            = "12.9";
    mbData.loadCurrent            = "0";
    mbData.loadPower              = "0";
    mbData.energyConsumedToday    = "0";
    mbData.energyConsumedTotal    = "0";
    mbData.espTemperature         = bme.readTemperature();
    mbData.espPressure            = bme.readPressure();

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

  //MODBUS....
  // static uint32_t k;
  // uint8_t j, result;
  // uint16_t data[6];
  
  // k++;
  
  // // set word 0 of TX buffer to least-significant word of counter (bits 15..0)
  // node.setTransmitBuffer(0, lowWord(k));
  
  // // set word 1 of TX buffer to most-significant word of counter (bits 31..16)
  // node.setTransmitBuffer(1, highWord(k));
  
  // // slave: write TX buffer to (2) 16-bit registers starting at register 0
  // result = node.writeMultipleRegisters(0, 2);
  
  // // slave: read (6) 16-bit registers starting at register 2 to RX buffer
  // result = node.readHoldingRegisters(2, 6);
  
  // // do something with data if read is successful
  // if (result == node.ku8MBSuccess)
  // {
  //   for (j = 0; j < 6; j++)
  //   {
  //     data[j] = node.getResponseBuffer(j);
  //     Serial.println(data[j]);
  //   }
    
  // }
}