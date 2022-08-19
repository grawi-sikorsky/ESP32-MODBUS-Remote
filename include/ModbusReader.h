#include <ModbusMaster.h>
#include "ModbusData.h"

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

enum RegisterEnum {
        EAddressRegistry_3100 		= 1,
        EAddressRegistry_3106 		= 2,
        EAddressRegistry_310D 		= 3,
        EPageMaintain 			    = 4,
        EPageSetup 					= 5,
        EPageMove 					= 6,
        EPageSpeed 					= 7,
        EPageFilament 			    = 11,
        EPageBedlevel 			    = 12,
        EPageBabyStep               = 13,
        EPageSelect 				= 14,
        EPageYesno 					= 15,
        EPageFlow 					= 21,
        EPageKill 					= 30,
        EPageScreenSaver 		= 34,
        EPageBedlevelAuto 	= 35,
};

class ModbusReader{

    private:
        ModbusMaster node;
        ModbusData mbData;
        uint8_t currentRegistryNumber = 0;

    
    public:
        ModbusData getData(){ return mbData; }

        void initModbus();
        void readModbusDataFromDevice();
        void AddressRegistry_3100();
        void AddressRegistry_3106();
        void AddressRegistry_310D();
        void AddressRegistry_311A();
        void AddressRegistry_331B();
        void AddressRegistry_3200();
        void AddressRegistry_331A();
        void AddressDeviceTemp();
        void AddressGeneratedTotals();
        void AddressConsumedTotals();
};