#include <Arduino.h>
#include "WiFiManager.h"
#include "HTTPClient.h"
#include <ArduinoJson.h>
#include <Wire.h>
#include <ModbusMaster.h>
#include <TimerEvent.h>

// MOJE INCLUDES
#include "ModbusReader.h"
#include "SetupData.h"


WiFiManager wifiManager;
ModbusReader mbReader;
SetupData mbSetup;

TimerEvent modbusUpdate;
TimerEvent postTimer;
TimerEvent getTimer;

time_t postPrevTime, setupPrevTime, livePrevTime;
time_t bmePrevTime;

bool wifiConnected = false;

int i = 1;

void sendPost(bool liveData, ModbusData data)
{
  HTTPClient http;
  if(liveData){
    http.begin("https://modbus-back.herokuapp.com/live");  
  }
  else{
    http.begin("https://modbus-back.herokuapp.com/data");
  }
  
  http.addHeader("Content-Type", "application/json");

  StaticJsonDocument<2048> doc;
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

  if(httpResponseCode == 200 && liveData){
    Serial.println("[] HTTP POST LIVE DATA sent.");
  } else if (httpResponseCode == 200 && !liveData){
    Serial.println("[] HTTP POST SAVE DATA sent.");
  } else {
    Serial.println("[] HTTP POST failed.");
    wifiConnected = false;
  }
  http.end();
}

void getSetup(){
  HTTPClient http;
  http.begin("https://modbus-back.herokuapp.com/setup/modbus1");
  http.addHeader("Content-Type", "application/json");

  int httpResponseCode = http.GET();

  if(httpResponseCode == 200){
    DynamicJsonDocument doc(512);
    deserializeJson(doc, http.getString());

    mbSetup.modbusID              = (const char*)doc["modbusID"];
    mbSetup.readingUpdateInterval = (const char*)doc["readingUpdateInterval"];
    mbSetup.postUpdateInterval    = (const char*)doc["postUpdateInterval"];
    mbSetup.setupUpdateInterval   = (const char*)doc["setupUpdateInterval"];

    // na wypadek blednej (np.0) wartosci trzeba ustawic default, inaczej zapetla sie na post lub get
    if( mbSetup.postUpdateInterval.toInt() < 1000 ) mbSetup.postUpdateInterval = 1000;
    if( mbSetup.readingUpdateInterval.toInt() < 1000 ) mbSetup.readingUpdateInterval = 1000;
    if( mbSetup.setupUpdateInterval.toInt() < 1000 ) mbSetup.setupUpdateInterval = 1000;

    Serial.print("[] HTTP GET done, response: ");
    Serial.println(httpResponseCode);
  } else {
    Serial.print("[] HTTP GET fail, response: ");
    Serial.println(httpResponseCode);
    wifiConnected = false;
  }
  http.end();
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
    sendPost(true, mbReader.mbData);
    Serial.println("Manual POST sent..");
  }
}

void checkWIFIstatus(){
  if(wifiConnected == false){
    wifiConnected = wifiManager.autoConnect("ESP32-MODBUS");
    getSetup();
  }
}

void setup()
{
  Serial.begin(115200);

  wifiManager.setTimeout(60);
  wifiManager.setConnectRetries(2);
  wifiConnected = wifiManager.autoConnect("ESP32-MODBUS");

  pinMode(RESET_WIFI_PIN, INPUT_PULLUP);
  pinMode(MANUAL_POST_PIN, INPUT_PULLUP);

  mbReader.initModbus();
  getSetup();

  mbReader.mbData.modbusID = "modbus1";

  // Set the interval timing as well as pass the function name
  // modbusUpdate.set(500, timerOneFunc);
  // postTimer.set(500, timerTwoFunc);s
  // getTimer.set(500, getSetup);
}

// LOOP
void loop()
{
  if (millis() - postPrevTime >= mbSetup.postUpdateInterval.toInt() )
  {
    sendPost(false, mbReader.mbData);
    postPrevTime = millis();
  }
  // else if (millis() - livePrevTime >= 1000)
  // {
  //   sendPost(true, mbReader.mbData);
  //   livePrevTime = millis();
  // }
  else if (millis() - bmePrevTime >= mbSetup.readingUpdateInterval.toInt() )
  {
    mbReader.readModbusDataFromDevice();
    sendPost(true, mbReader.mbData);
    bmePrevTime = millis();
  }
  else if(millis() - setupPrevTime >= mbSetup.setupUpdateInterval.toInt() )
  {
    getSetup();
    setupPrevTime = millis();
  }
  else
  {
    checkManualPostPin();
    checkResetWifiButton();
    checkWIFIstatus();
  }
}