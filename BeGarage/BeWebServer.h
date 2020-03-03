/** BeWebServer.h
功能說明：
-  使用 SPIFF 作為網頁檔案空間
-  檔案處理：BeWebServer_Files.h，固定寫法，可直接複製移植
-  動作處理：BeWebServer_Handle.h, 與 BeWebServer_Files.h 相依，依目的客製化


需修改 ESPAsyncWebServer.h 如下，加入 ESP32/ESP8266 的判定，否則與 WifiManager 合併使用時，會出現重複定義的錯誤
Fixing code needed in ESPAsyncWebServer.h if "redeclaration of 'HTTP_GET' " occurred:

-- Replace :

#ifndef WEBSERVER_H
typedef enum {
 ...
} WebRequestMethod;
#endif

-- With:

#if defined(ESP32)
  #ifndef WEBSERVER_H
    #define HTTP_METHOD_IMPLEMENT
  #endif
#elif defined(ESP8266)
  #ifndef ESP8266WEBSERVER_H
    #define HTTP_METHOD_IMPLEMENT
  #endif
#endif

#ifdef HTTP_METHOD_IMPLEMENT
  typedef enum {
    HTTP_GET     = 0b00000001,
    HTTP_POST    = 0b00000010,
    HTTP_DELETE  = 0b00000100,
    HTTP_PUT     = 0b00001000,
    HTTP_PATCH   = 0b00010000,
    HTTP_HEAD    = 0b00100000,
    HTTP_OPTIONS = 0b01000000,
    HTTP_ANY     = 0b01111111,
  } WebRequestMethod;
#endif
*/

#ifndef BEWEBSERVER_H
#define BEWEBSERVER_H

#if defined(ARDUINO_ESP8266_NODEMCU) || defined(ARDUINO_ESP8266_WEMOS_D1MINI)
  #include <ESP8266mDNS.h>
#elif defined(ESP32)
  #include <ESPmDNS.h>
#endif

#include "ESPAsyncWebServer.h"
AsyncWebServer webserver(80);
#include "BeWebServer_Handle.h"

boolean BeWebServer_Setup(){
  if (!MDNS.begin(projName)) {
    Serial.println(F("Error setting up MDNS responder!"));
    return false;
  }
  else{
    Serial.println(F("mDNS responder started"));
  }
  if(!beWebServer_Mounted()){
    return false;
  }

  webserver.on("/button",HTTP_GET, handlePressButton);
  webserver.on("/mcuname",HTTP_GET, handleMcuName);
  webserver.onNotFound(handleFileNotFound);
  webserver.begin();
  Serial.print(F("HTTP server started.\nYou can connect to http://"));
  Serial.print(String(projName));
  Serial.println(F(".local within web browser from any device"));
  return true;
    
}

#endif
