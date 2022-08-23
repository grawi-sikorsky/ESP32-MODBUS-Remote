#include <ModbusMaster.h>
#include "ModbusData.h"
#include "SetupData.h"

class ModbusReader{

    private:
        ModbusMaster node;
        uint8_t currentRegistryNumber = 0;
    
    public:
        ModbusData mbData;
        SetupData setupData;

        void initModbus();
        void readModbusDataFromDevice();
        void AddressRegistry_3100();
        void AddressRegistry_3106();
        void AddressRegistry_310C();
        void AddressRegistry_311A();
        void AddressRegistry_331B();
        void AddressRegistry_3200();
        void AddressRegistry_331A();
        void AddressDeviceTemp();
        void AddressGeneratedTotals();
        void AddressConsumedTotals();
};