#include <Arduino.h>

class ModbusData{
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