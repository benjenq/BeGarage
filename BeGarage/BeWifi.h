#if defined(ARDUINO_ESP8266_NODEMCU) || defined(ARDUINO_ESP8266_WEMOS_D1MINI) || defined(ESP32)
 
#else
   #error "This project is only for ESP8266: NodeMCU, Wemos D1 R2&mini And ESP32 Chipset"
#endif

#ifndef BEWIFI_H
#define BEWIFI_H

#if defined(ARDUINO_ESP8266_NODEMCU) || defined(ARDUINO_ESP8266_WEMOS_D1MINI)
 #include <ESP8266WiFi.h>
 extern "C" {
   #include "user_interface.h"
 }
 #define ESP_reset ESP.reset()
#elif defined(ESP32)
 #include <WiFi.h>
 #define ESP_reset ESP.restart()
#endif


#include <WiFiManager.h>

void configWifiCallback (WiFiManager *myWiFiManager){
  wifiConnected = false;
  Serial.println("");
  Serial.print(F("Entered config mode:"));
  Serial.println(WiFi.softAPIP());
}


boolean BeWifi_Setup(){
  #if defined(ARDUINO_ESP8266_NODEMCU) || defined(ARDUINO_ESP8266_WEMOS_D1MINI)
  //wifi_set_phy_mode(PHY_MODE_11G);
  #endif

  /** 預設連線方式
  WiFi.mode(WIFI_STA);
  WiFi.begin(wifiSSID, wifiPassword);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(F("Wifi Connected."));

  wifiConnected = true;
  return wifiConnected;
  */

/** WiFiManager 一旦啟動 Portal 設定且以 STA 模式連線成功後，從此以後會一併啟用無加密的 AP 模式。
 *  連上此 AP 一樣可透過 Web 控制，有洩漏風險。加上 WiFi.mode(WIFI_STA); 避免產生 AP 模式。 */
  WiFi.mode(WIFI_STA);
  WiFiManager wifiManager;
  wifiManager.setDebugOutput(false);
  wifiManager.setAPCallback(configWifiCallback);
  
  if(!wifiManager.autoConnect(projName, wifiPassword)) {
    ESP_reset;
    delay(1000);
  }
  else{
    wifiConnected = true;
    Serial.println(F(""));
    Serial.print(F("Wifi Connected: "));
    Serial.println(WiFi.localIP());
  }
  return wifiConnected;

}



#endif
