/**
 * 變數區
 */
#ifndef BEPARAMETER_H
#define BEPARAMETER_H

#if defined(ARDUINO_ESP8266_NODEMCU) || defined(ARDUINO_ESP8266_WEMOS_D1MINI)
  //D3|D4 開機起始時通電，會觸發按鈕動作，所以避免使用 D3|D4
  #define PIN_UP   D0
  #define PIN_STOP D1
  #define PIN_DOWN D2
  #define OUTPUT_LOW  HIGH
  #define OUTPUT_HIGH LOW
  char mcuName[] = "ESP8266";
#elif defined(ESP32)
  #warning "ESP32 尚未定義 GPIO"
  #ifndef LED_BUILTIN
    #define LED_BUILTIN 2
  #endif
  #define PIN_UP   12
  #define PIN_STOP 14
  #define PIN_DOWN 27
  #define OUTPUT_LOW  HIGH
  #define OUTPUT_HIGH LOW
  char mcuName[] = "ESP32";
#endif

//專案 WIFI SSID 與密碼
char projName[] = "BeGarage";
//const char *wifiSSID = "your-wifi-ssid";
char wifiPassword[] = "your-wifi-password";
bool wifiConnected = false;

//按鈕動作 Flag
bool isBtnPressing = false; //正在按鈕動作中
int pressDurationMs = 250; //按鈕持續時間(毫秒 1/1000)
//測試自動按壓按鈕
//unsigned long buttonPressMillis;

//斷網重開機
bool readyForReboot = false;
unsigned long rebootMs = 1000 * 110; //預計多久後重開機(毫秒 1/1000)
#endif
