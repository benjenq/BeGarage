/**
 * Hosted on https://github.com/benjenq/BeGarage
*/
#include "BeParameter.h"
#include "BeWifi.h"
#include "BeMQTT.h"
#include "BeWebServer.h"
#include "BeParseButtonAction.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  BeButton_Setup();
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, OUTPUT_HIGH);
  if(!BeWifi_Setup()){
    return;
  }
  if(!BeWebServer_Setup()){
    digitalWrite(LED_BUILTIN, OUTPUT_HIGH);
    return;
  }
  digitalWrite(LED_BUILTIN, OUTPUT_LOW);
  BeMQTT_Setup();
}

void loop() {
  if(readyForReboot){
    if(millis() > rebootMs)
    ESP_reset;
    return;
  }
  if(!wifiConnected){
    return;
  }
  // put your main code here, to run repeatedly:
  BeButton_Loop();
  BeMQTT_Loop();

  if(WiFi.status() != WL_CONNECTED){
    Serial.println(F("Wifi disconnected. Ready for reboot..."));
    readyForReboot = true;
    rebootMs = millis() + rebootMs;
  }  
}
