#include <Arduino.h>
#include "Configuration.h"

class ModbusData{
  public: 
    String modbusID;
    String pvVoltage;
    String pvCurrent;
    String pvPower;
    String pvTotalChargingToday;
    String pvTotalCharging;
    String batVoltage;
    String batCurrent;
    String mpptTemperature;
    String batStatus;
    String batChargingStatus;
    String batDischargingStatus;
    String loadVoltage;
    String loadCurrent;
    String loadPower;
    String energyConsumedToday;
    String energyConsumedTotal;
    float espTemperature;
    float espPressure;
};