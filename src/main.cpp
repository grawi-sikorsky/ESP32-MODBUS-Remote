#include <Arduino.h>
#include "WiFi.h"

// DEFINES
#define WIFI_NAME "Naplet"
#define WIFI_PASS "Napletszatana89"
#define WIFI_DEVICE_NAME "ESP32 Divajs"
#define WIFI_TIMEOUT_MS 20000

int i = 1;

void connectToWifi(){
  Serial.println("WIFI Connecting...");
  WiFi.mode(WIFI_STA);
  WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
  WiFi.setHostname(WIFI_DEVICE_NAME);
  WiFi.begin(WIFI_NAME, WIFI_PASS);

  unsigned long startTime = millis();

  while(WiFi.status() != WL_CONNECTED && millis() - startTime < WIFI_TIMEOUT_MS){
    Serial.print(" .");
    delay(1000);
  }

  if(WiFi.status() != WL_CONNECTED){
    Serial.println("WIFI Connection Failed.. ");
  }
  else{
    Serial.println("\nWIFI Connection is established.");
  }
}


void setup() {
  Serial.begin(115200);
  connectToWifi();
}

void loop() {
  // put your main code here, to run repeatedly:
  
  while(true){
    delay(500);
    i++;
    Serial.println(i);
  }
  
}