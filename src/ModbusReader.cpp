#include "ModbusReader.h"

void ModbusReader::initModbus()
{
    Serial2.begin(115200);
    node.begin(1, Serial2);
}

void ModbusReader::readModbusDataFromDevice()
{
    AddressRegistry_3100();
    AddressRegistry_3106();
    AddressRegistry_310D();
    AddressRegistry_311A();
    AddressRegistry_331B();
    AddressRegistry_3200();
    AddressRegistry_331A();
    AddressDeviceTemp();
    AddressGeneratedTotals();
    AddressConsumedTotals();
}

void ModbusReader::AddressRegistry_3100()
{
    if (node.readInputRegisters(0x3100, 6) == node.ku8MBSuccess)
    {
        mbData.pvVoltage = node.getResponseBuffer(0x00) / 100.0f;
        mbData.pvCurrent = node.getResponseBuffer(0x01) / 100.0f;
        mbData.pvPower = (node.getResponseBuffer(0x02) | node.getResponseBuffer(0x03) << 16) / 100.0f;
        mbData.batVoltage = node.getResponseBuffer(0x04) / 100.0f;
        mbData.batChargingCurrent = node.getResponseBuffer(0x05) / 100.0f;

        #ifdef DEBUG
            Serial.print("PV Voltage: ");
            Serial.println(mbData.pvVoltage);

            Serial.print("PV Current: ");
            Serial.println(mbData.pvCurrent);

            Serial.print("PV Power: ");
            Serial.println(mbData.pvPower);

            Serial.print("Battery Voltage: ");
            Serial.println(mbData.batVoltage);

            Serial.print("Battery Charge Current: ");
            Serial.println(mbData.batChargingCurrent);
        #endif
    }
}

void ModbusReader::AddressRegistry_3106()
{
    if (node.readInputRegisters(0x3106, 2) == node.ku8MBSuccess)
    {
        mbData.batChargingPower = (node.getResponseBuffer(0x00) | node.getResponseBuffer(0x01) << 16) / 100.0f;

        #ifdef DEBUG
            Serial.print("Battery Charge Power: ");
            Serial.println(mbData.batChargingPower);
        #endif
    }
}

void ModbusReader::AddressRegistry_310D()
{
    if (node.readInputRegisters(0x310D, 3) == node.ku8MBSuccess)
    {
        mbData.loadCurrent = node.getResponseBuffer(0x00) / 100.0f;
        mbData.loadPower = (node.getResponseBuffer(0x01) | node.getResponseBuffer(0x02) << 16) / 100.0f;

        #ifdef DEBUG
            Serial.print("Load Current: ");
            Serial.println(mbData.loadCurrent);

            Serial.print("Load Power: ");
            Serial.println(mbData.loadPower);
        #endif
    }
}

void ModbusReader::AddressRegistry_311A()
{
    if (node.readInputRegisters(0x311A, 2) == node.ku8MBSuccess)
    {
        mbData.batRemainingPercent = node.getResponseBuffer(0x00) / 1.0f;
        mbData.batRemainingPercent = node.getResponseBuffer(0x01) / 100.0f;

        #ifdef DEBUG
            Serial.print("Battery Remaining %: ");
            Serial.println(mbData.batRemainingPercent);

            Serial.print("Battery Temperature: ");
            Serial.println(mbData.batRemainingPercent);
        #endif
    }
}

void ModbusReader::AddressRegistry_331B()
{
    if (node.readInputRegisters(0x331B, 2) == node.ku8MBSuccess)
    {
        mbData.batOverallCurrent = (node.getResponseBuffer(0x00) | node.getResponseBuffer(0x01) << 16) / 100.0f;

        #ifdef DEBUG
            Serial.print("Battery Discharge Current: ");
            Serial.println(mbData.batOverallCurrent);
        #endif
    }
}

// To nie tak...
void ModbusReader::AddressRegistry_3200()
{
    if (node.readInputRegisters(0x3200, 3) == node.ku8MBSuccess)
    {
        mbData.batStatus = node.getResponseBuffer(0x00);
        mbData.pvStatus = node.getResponseBuffer(0x01);
        mbData.loadStatus = node.getResponseBuffer(0x02);

        #ifdef DEBUG
            Serial.print("Battery Status: ");
            Serial.println(mbData.batStatus);
            Serial.print("PV Status: ");
            Serial.println(mbData.pvStatus);
            Serial.print("Load Status: ");
            Serial.println(mbData.loadStatus);
        #endif
    }
}

void ModbusReader::AddressDeviceTemp(){
    if (node.readInputRegisters(0x3111, 1) == node.ku8MBSuccess)
    {
        mbData.mpptTemperature = node.getResponseBuffer(0x00) / 100.0f;

        #ifdef DEBUG
            Serial.print("MPPT device temp: ");
            Serial.println(mbData.mpptTemperature);
        #endif
    }
}

void ModbusReader::AddressGeneratedTotals(){
    if (node.readInputRegisters(0x330C, 8) == node.ku8MBSuccess)
    {
        mbData.genTotalToday = ( node.getResponseBuffer(0x00) | node.getResponseBuffer(0x01) << 16 ) / 100.0f;
        mbData.genTotalMonth = ( node.getResponseBuffer(0x02) | node.getResponseBuffer(0x03) << 16 ) / 100.0f;
        mbData.genTotalYear = ( node.getResponseBuffer(0x04) | node.getResponseBuffer(0x05) << 16 ) / 100.0f;
        mbData.genTotalAll = ( node.getResponseBuffer(0x06) | node.getResponseBuffer(0x07) << 16 ) / 100.0f;

        #ifdef DEBUG
            Serial.print("Total Generated Today: ");
            Serial.println(mbData.genTotalToday);
            Serial.print("Total Generated Month: ");
            Serial.println(mbData.genTotalMonth);
            Serial.print("Total Generated Year: ");
            Serial.println(mbData.genTotalYear);
            Serial.print("Total Generated Alltime: ");
            Serial.println(mbData.genTotalAll);
        #endif
    }
}

void ModbusReader::AddressConsumedTotals(){
    if (node.readInputRegisters(0x3304, 8) == node.ku8MBSuccess)
    {
        mbData.consTotalToday = ( node.getResponseBuffer(0x00) | node.getResponseBuffer(0x01) << 16 ) / 100.0f;
        mbData.consTotalMonth = ( node.getResponseBuffer(0x02) | node.getResponseBuffer(0x03) << 16 ) / 100.0f;
        mbData.consTotalYear = ( node.getResponseBuffer(0x04) | node.getResponseBuffer(0x05) << 16 ) / 100.0f;
        mbData.consTotalAll = ( node.getResponseBuffer(0x06) | node.getResponseBuffer(0x07) << 16 ) / 100.0f;

        #ifdef DEBUG
            Serial.print("Total Consumed Today: ");
            Serial.println(mbData.consTotalToday);
            Serial.print("Total Consumed Month: ");
            Serial.println(mbData.consTotalMonth);
            Serial.print("Total Consumed Year: ");
            Serial.println(mbData.consTotalYear);
            Serial.print("Total Consumed Alltime: ");
            Serial.println(mbData.consTotalAll);
        #endif
    }
}

void ModbusReader::AddressRegistry_331A(){
    if (node.readInputRegisters(0x331A, 1) == node.ku8MBSuccess)
    {
        mbData.test = node.getResponseBuffer(0x00) / 100.0f;

        #ifdef DEBUG
            Serial.print("331A Batery voltage?: ");
            Serial.println(mbData.test);
        #endif
    }
}