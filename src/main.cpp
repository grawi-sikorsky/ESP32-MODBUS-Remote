#include <Arduino.h>
#include "WiFiManager.h"
#include "HTTPClient.h"
#include <ArduinoJson.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <Wire.h>

#include <ModbusMaster.h>
// instantiate ModbusMaster object
ModbusMaster node;


Adafruit_BMP280 bme;

// DEFINES
#define RESET_WIFI_PIN 19
#define MANUAL_POST_PIN 13
#define MANUAL_PIN 12

#define POST_INTERVAL 120000
#define BME_INTERVAL 5000
#define I2C_SDA 21
#define I2C_SCL 22

WiFiManager wifiManager;

time_t postPrevTime;
time_t bmePrevTime;

int i = 1;

class modbusData{
  public: String modbusID;
  public: String pvVoltage;
  public: String pvCurrent;
  public: String pvPower;
  public: String pvTotalChargingToday;
  public: String pvTotalCharging;
  public: String batVoltage;
  public: String batCurrent;
  public: String mpptTemperature;
  public: String batStatus;
  public: String batChargingStatus;
  public: String batDischargingStatus;
  public: String loadVoltage;
  public: String loadCurrent;
  public: String loadPower;
  public: String energyConsumedToday;
  public: String energyConsumedTotal;
  public: float espTemperature;
  public: float espPressure;
};

modbusData mbData;

void sendPost(modbusData data)
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
}

void checkResetWifiButton(){
  if(digitalRead(RESET_WIFI_PIN) == LOW){
    wifiManager.resetSettings();
  }
}

void checkManualPostPin(){
  if(digitalRead(MANUAL_POST_PIN) == LOW){
    sendPost(mbData);
    Serial.println("manual POST sent..");
  }
}

void setup()
{
  Serial.begin(115200);
  Serial2.begin(115200);

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
  if (millis() - postPrevTime >= POST_INTERVAL)
  {
    i++;
    Serial.println(i);

    sendPost(mbData);

    postPrevTime = millis();
  }
  else if (millis() - bmePrevTime >= BME_INTERVAL)
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
  else
  {
    checkManualPostPin();
    checkResetWifiButton();
  }

  //MODBUS....
  static uint32_t k;
  uint8_t j, result;
  uint16_t data[6];
  
  k++;
  
  // set word 0 of TX buffer to least-significant word of counter (bits 15..0)
  node.setTransmitBuffer(0, lowWord(k));
  
  // set word 1 of TX buffer to most-significant word of counter (bits 31..16)
  node.setTransmitBuffer(1, highWord(k));
  
  // slave: write TX buffer to (2) 16-bit registers starting at register 0
  result = node.writeMultipleRegisters(0, 2);
  
  // slave: read (6) 16-bit registers starting at register 2 to RX buffer
  result = node.readHoldingRegisters(2, 6);
  
  // do something with data if read is successful
  if (result == node.ku8MBSuccess)
  {
    for (j = 0; j < 6; j++)
    {
      data[j] = node.getResponseBuffer(j);
      Serial.println(data[j]);
    }
    
  }
  else
  {
    Serial.println("lipa");
    delay(1000);
  }
}