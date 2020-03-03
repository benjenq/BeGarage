/**
 * 定義按鈕動作：上，停止，下
 */
#ifndef BEBUTTON_H
#define BEBUTTON_H

#include <Ticker.h>
#include "BeParameter.h"

Ticker buttonTicker;
void BeButtonPress_UP();
void BeButtonPress_STOP();
void BeButtonPress_DOWN();
void BeButton_Release();


void BeButton_Setup(){
  pinMode(PIN_UP, OUTPUT);
  pinMode(PIN_STOP, OUTPUT);
  pinMode(PIN_DOWN, OUTPUT);
  
  digitalWrite(PIN_UP,OUTPUT_LOW);
  digitalWrite(PIN_STOP,OUTPUT_LOW);
  digitalWrite(PIN_DOWN,OUTPUT_LOW);

  isBtnPressing = false;
  //buttonPressMillis = millis();
}

void BeButton_Loop(){
  /* 測試
  if(millis() - buttonPressMillis > 5000){
    buttonPressMillis = millis();
    BeButtonPress_UP();
  }
   End 測試 */

}

/**
 * 按鈕動作定義區
 */

void BeButtonPress_UP(){
  if(isBtnPressing){
    return;
  }
  isBtnPressing = true;
  digitalWrite(LED_BUILTIN, OUTPUT_HIGH);
  digitalWrite(PIN_UP,OUTPUT_HIGH);
  digitalWrite(PIN_STOP,OUTPUT_LOW);
  digitalWrite(PIN_DOWN,OUTPUT_LOW);
  buttonTicker.attach_ms(pressDurationMs,BeButton_Release);
  //Serial.println(F("Button UP pressed."));
}
void BeButtonPress_STOP(){
  if(isBtnPressing){
    return;
  }
  isBtnPressing = true;
  digitalWrite(LED_BUILTIN, OUTPUT_HIGH);
  digitalWrite(PIN_UP,OUTPUT_LOW);
  digitalWrite(PIN_STOP,OUTPUT_HIGH);
  digitalWrite(PIN_DOWN,OUTPUT_LOW);  
  buttonTicker.attach_ms(pressDurationMs,BeButton_Release);
  //Serial.println(F("Button STOP pressed."));
}
void BeButtonPress_DOWN(){
  if(isBtnPressing){
    return;
  }
  isBtnPressing = true;
  digitalWrite(LED_BUILTIN, OUTPUT_HIGH);
  digitalWrite(PIN_UP,OUTPUT_LOW);
  digitalWrite(PIN_STOP,OUTPUT_LOW);
  digitalWrite(PIN_DOWN,OUTPUT_HIGH);  
  buttonTicker.attach_ms(pressDurationMs,BeButton_Release);
  //Serial.println(F("Button DOWN pressed."));
}

void BeButton_Release(){
  buttonTicker.detach();
  digitalWrite(LED_BUILTIN, OUTPUT_LOW);
  digitalWrite(PIN_UP,OUTPUT_LOW);
  digitalWrite(PIN_STOP,OUTPUT_LOW);
  digitalWrite(PIN_DOWN,OUTPUT_LOW);  
  isBtnPressing = false;
  //Serial.println(F("Button released."));
}

#endif
