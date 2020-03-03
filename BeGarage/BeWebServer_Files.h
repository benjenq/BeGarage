#ifndef BEWEBSERVER_FILES_H
#define BEWEBSERVER_FILES_H

#ifdef ESP32
  #include <SPIFFS.h>
#endif
#include <FS.h>

#include "ESPAsyncWebServer.h"

/* Function Define */
bool beWebServer_Mounted();
void serverSend(int num, const char * datatype, String message, AsyncWebServerRequest *request);
//讀檔案
void handleFileNotFound(AsyncWebServerRequest *request);
bool handleFileRead(String path, AsyncWebServerRequest *request);
String getContentType(String filename, AsyncWebServerRequest *request);

bool beWebServer_Mounted(){
  if (!SPIFFS.begin()) {
    Serial.println("SPIFFS mount Failed, abort...");
    return false;
  }
  Serial.println("SPIFFS mount Successfully.");
  return true;
}
void serverSend(int num, const char * datatype, String message, AsyncWebServerRequest *request) {
  AsyncWebServerResponse *response = request->beginResponse(num,datatype,message);
  response->addHeader("Access-Control-Allow-Origin", "*");
  request->send(response);
}

void handleFileNotFound(AsyncWebServerRequest *request) { /* Replace webserver.xxxx with request->xxxx */
  if (!handleFileRead(request->url().c_str(), request)) {
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += request->url().c_str();
    message += "\nMethod: ";
    message += (request->method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += request->args();
    message += "\n";
    for (uint8_t i = 0; i < request->args(); i++) {
      message += " " + String(request->argName(i).c_str()) + ": " + request->arg(i).c_str() + "\n";
    }
    serverSend(404, "text/plain", message, request);
  }
}

bool handleFileRead(String path, AsyncWebServerRequest *request) {
  if (path.endsWith("/")) {
    path += "index.html";
  }
  String contentType = getContentType(path, request);
  // Handle .gz file
  String pathWithGz = path + ".gz";
  if (SPIFFS.exists(path) || SPIFFS.exists(pathWithGz)) {
    if (SPIFFS.exists(pathWithGz)) { //Check if .gz file exist
      path += ".gz";
    }
    //File file = SPIFFS.open(path, "r");
    Serial.println("handleFileRead: " + path);
    request->send(SPIFFS, path, contentType); /* Replace webserver.streamFile(file, contentType) with request->send(SPIFFS, path, contentType) */
    //file.close();
    return true;
  }

  return false;
}

String getContentType(String filename, AsyncWebServerRequest *request) {
  if (request->hasArg("download")) return "application/octet-stream"; /* Replace webserver.hasArg("download") with request->hasArg("download") */ 
  else if (filename.endsWith(".htm")) return "text/html";
  else if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".png")) return "image/png";
  else if (filename.endsWith(".gif")) return "image/gif";
  else if (filename.endsWith(".jpg")) return "image/jpeg";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".xml")) return "text/xml";
  else if (filename.endsWith(".pdf")) return "application/x-pdf";
  else if (filename.endsWith(".zip")) return "application/x-zip";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

#endif
