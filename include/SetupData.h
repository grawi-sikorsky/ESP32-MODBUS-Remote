#include <Arduino.h>
#include "Configuration.h"

class SetupData{
    public: 
        String modbusID;
        String readingUpdateInterval;
        String postUpdateInterval;
        String setupUpdateInterval;

    SetupData();
};

SetupData::SetupData(){
    readingUpdateInterval   = READ_INTERVAL;
    postUpdateInterval      = POST_INTERVAL;
    setupUpdateInterval     = SETUP_CHECK_INTERVAL;
}