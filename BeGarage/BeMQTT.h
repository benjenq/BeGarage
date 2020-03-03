#ifndef BEMQTT_H
#define BEMQTT_H

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#endif

#include <PubSubClient.h>
#include "BeParseButtonAction.h"

const char* mqtt_server = "mqtt-server-domain-name";
const char* mqtt_clientid = "garage_r";
const int mqtt_server_port = 2883;
//const char* mqtt_username = "your-mqtt-user-name";
//const char* mqtt_passwd = "your-mqtt-password";
const char* subtopic = "actiontopic";
const char* pubtopic = "statustopic";

unsigned long lastMqttReconnectAttempt = 0;
int mqttReconnectinterval_ms = 10000;

WiFiClient espClient;
PubSubClient client(espClient);

void mqttCallback(char* topic, byte* payload, unsigned int length);
void checkMqttConnection();
bool doMqttConnect();
bool BeMQTT_PublicMessage(const char * _topic, const char * _message);

void BeMQTT_Setup() {
  if (WiFi.status() == WL_CONNECTED) {
    client.setServer(mqtt_server, mqtt_server_port);
    client.setCallback(mqttCallback);
    doMqttConnect();
  }
  lastMqttReconnectAttempt = 0;
}

void BeMQTT_Loop() {
  if (WiFi.status() != WL_CONNECTED) {
    return;
  }
  checkMqttConnection();
  if (!client.connected()) {
    return;
  }
  client.loop();
}

bool BeMQTT_PublicMessage(const char * _topic, const char * _message) {
  if (!client.connected()) {
    return false;
  }
  client.publish(_topic, _message);
  return true;
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    //Serial.print((char)payload[i]);
  }
  //Serial.println();
  String aMsg = String((char *)payload).substring(0, length);
  Serial.println(aMsg);
  ParseActionResult result = BeParseButtonAction(aMsg.c_str());
  switch(result){
    case ParseActionResultIsFailure:
      BeMQTT_PublicMessage((const char *)pubtopic, "Parse Json Failure.");
      break;
    case ParseActionResultIsPressBusy:
      BeMQTT_PublicMessage((const char *)pubtopic, "Button Press is busying.");
      break;
    case ParseActionResultIsUnknow:
      BeMQTT_PublicMessage((const char *)pubtopic, "Action Unknow.");
      break;
    case ParseActionResultIsButtonUP:
      BeMQTT_PublicMessage((const char *)pubtopic, "UP Pressed.");
      break;
    case ParseActionResultIsButtonSTOP:
      BeMQTT_PublicMessage((const char *)pubtopic, "STOP Pressed.");
      break;
    case ParseActionResultIsButtonDOWN:
      BeMQTT_PublicMessage((const char *)pubtopic, "DOWN Pressed.");
      break;
    default:
    break;    
  }
  
  
}

bool doMqttConnect() {
  //bool result = client.connect(mqtt_clientid, mqtt_username, mqtt_passwd);
  bool result = client.connect(mqtt_clientid);
  if (result) {
    Serial.printf("MQTT %s:%i ", mqtt_server, mqtt_server_port);
    Serial.println(F("connected."));
    client.publish(pubtopic, "Garage controller online.");
    client.subscribe(subtopic);
  }
  return result;
}

void checkMqttConnection() {
  // Loop until we're reconnected
  if (!client.connected()) {
    if (millis() - lastMqttReconnectAttempt > mqttReconnectinterval_ms) {
      lastMqttReconnectAttempt = millis();
      if (doMqttConnect()) {
        lastMqttReconnectAttempt = 0;
        // Once connected, publish an announcement...
        Serial.print(F("Garage controller is back to online."));
        //client.publish(pubtopic, "Garage controller is back to online.");
        // ... and resubscribe
        //client.subscribe(subtopic);
      }
      else {
        Serial.print(F("MQTT connect failed, rc="));
        Serial.print(client.state());
        Serial.println(F(", try again in 10 seconds"));
      }
    }

  }
}

#endif
