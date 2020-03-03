#ifndef BEJSONBUTTONACTION_H
#define BEJSONBUTTONACTION_H

#include "BeButton.h"
enum ParseActionResult{
  ParseActionResultIsFailure = -2,
  ParseActionResultIsPressBusy = -1,
  ParseActionResultIsUnknow = 0,
  ParseActionResultIsButtonUP = 1,
  ParseActionResultIsButtonSTOP = 2,
  ParseActionResultIsButtonDOWN = 3
};

ParseActionResult BeParseButtonAction(const char * actionStr);

ParseActionResult BeParseButtonAction(const char * actionStr) {

  if(isBtnPressing){
    Serial.println(F("Button is in pressing duration."));
    return ParseActionResultIsPressBusy;
  }
  String action = String((const char*)actionStr);
  action.toUpperCase();
  if(action == "BUTTONUP"){
    BeButtonPress_UP();
    return ParseActionResultIsButtonUP;
  }
  else if(action == "BUTTONSTOP"){
    BeButtonPress_STOP();
    return ParseActionResultIsButtonSTOP;
  }
  else if(action == "BUTTONDOWN"){
    BeButtonPress_DOWN();
    return ParseActionResultIsButtonDOWN;
  }
  else{
    Serial.print(F("Unknow Button Action: ")); 
    Serial.print(action);
    return ParseActionResultIsUnknow;
  }
  return ParseActionResultIsUnknow;
}


#endif
