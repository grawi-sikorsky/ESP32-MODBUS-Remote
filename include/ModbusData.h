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
    String pvStatus;
    String batVoltage;
    String batDischargeCurrent;
    String batChargingCurrent;
    String batChargingPower;
    String batRemainingPercent;
    String batTemperature;
    String batOverallCurrent;
    String batStatus;
    String batChargingStatus;
    String batDischargingStatus;
    String mpptTemperature;
    String loadVoltage;
    String loadCurrent;
    String loadPower;
    String loadStatus;
    String energyConsumedToday; //
    String energyConsumedTotal; //
    String genTotalToday;
    String genTotalMonth;
    String genTotalYear;
    String genTotalAll;
    String consTotalToday;
    String consTotalMonth;
    String consTotalYear;
    String consTotalAll;

    String test;
    float espTemperature;
    float espPressure;
};