#ifndef BEWEBSERVER_HANDLE_H
#define BEWEBSERVER_HANDLE_H

#include "BeWebServer_Files.h"
#include "BeParseButtonAction.h"

/* Function Handle Action */
void handleMcuName(AsyncWebServerRequest *request);
void handlePressButton(AsyncWebServerRequest *request);

void handleMcuName(AsyncWebServerRequest *request){
  String jsonResult = "{\"code\":0,\"result\":\""+ String(mcuName) + "\"}" ;
  serverSend(200, "text/plain", jsonResult, request);
}

void handlePressButton(AsyncWebServerRequest *request) {
  ParseActionResult result = ParseActionResultIsUnknow;
  for (uint8_t i = 0; i < request->args(); i++) {
    //Serial.printf("%s = %s\n",request->argName(i).c_str(),request->arg(i).c_str());
    if (request->argName(i) == "action") {     
      result = BeParseButtonAction(request->arg(i).c_str());
      break;
    }
  }

  String resultString = "";
  int resultCode = -1;
  switch(result){
    case ParseActionResultIsFailure:
      resultCode = -1;
      resultString = "Parse Json Failure.";
      break;
    case ParseActionResultIsPressBusy:
      resultCode = -1;
      resultString = "Button Press is busying.";
      break;
    case ParseActionResultIsUnknow:
      resultCode = -1;
      resultString = "Action Unknow.";
      break;
    case ParseActionResultIsButtonUP:
      resultCode = 0;
      resultString = "UP Pressed.";
      break;
    case ParseActionResultIsButtonSTOP:
      resultCode = 0;
      resultString = "STOP Pressed.";
      break;
    case ParseActionResultIsButtonDOWN:
      resultCode = 0;
      resultString = "DOWN Pressed.";
      break;
    default:
    break;    
  }

  BeMQTT_PublicMessage((char *)pubtopic, (char *)resultString.c_str());

  String jsonResult = String("{\"code\":" + String(resultCode) + ",\"result\":\"" + resultString + "\"}") ;
  //char * test;
  //sprintf(test,"{\"code\":%d,\"result\":\"%s\"}",resultCode,resultString.c_str());
  
  serverSend(200, "text/plain", jsonResult, request);
}




#endif
