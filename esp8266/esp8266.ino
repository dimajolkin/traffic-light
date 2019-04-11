#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <iostream>
#include <ESP8266HTTPClient.h>

#include <WiFiClient.h>

#include "lib.h"
#include "wifi.h"
#include "teamcity.h"

ESP8266WiFiMulti WiFiMulti;

WifiSettings ws;
TeamCitySettings ts;
DynamicJsonDocument doc(2048);


String send(String json) {
  DeserializationError error = deserializeJson(doc, json);
  if (error) {
    Serial.println(F("Failed to read file, using default configuration"));
    return "[]";
  }

  JsonArray builds = doc["build"];
  String answer = "[";
  for (JsonObject build : builds) {
    const char* number = build["number"];
    const char* status = build["status"];
    const char* state = build["state"];
    
    answer += "{";
    answer += "\"id\":" + String(number) + ",";
    answer += "\"status\":\"" + String(status) + "\",";
    answer += "\"state\":\"" + String(state) + "\"";
    answer += "},";
  }
  answer.remove(answer.length()-1);
  answer += "]"; //drop last ,

  return answer;
}

void setup() {
  Serial.begin(9600);
  Serial.println();
  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  ws.serialInput();
  ts.serialInput();
  WiFi.mode(WIFI_STA);
  Serial.printf("[SETUP] NET NAME %s...\n", ws.getName().c_str());
  Serial.printf("[SETUP] NET PSWD %s...\n", ws.getPassword().c_str());
  Serial.printf("[SETUP] H %s...\n", ts.getUrl().c_str());

  WiFiMulti.addAP(ws.getName().c_str(), ws.getPassword().c_str());
  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.println("[SETUP] WAIT CONNECT");
    delay(1000);
  }

  Serial.println("[SETUP] CONNECT");

  Serial.print("[SETUP] IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    WiFiClient client;

    HTTPClient http;

    Serial.print("[HTTP] begin...\n");
    if (http.begin(client, ts.getUrl().c_str())) {
      //http.setAuthorization("admin", "admin");
      http.addHeader("Accept", "application/json");

      Serial.print("[HTTP] GET...\n");
      int httpCode = http.GET();
      if (httpCode > 0) {
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          Serial.println("[BEGIN]");
          Serial.println(send(http.getString()));
          Serial.println("[END]");
        }
      } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }

      http.end();
    } else {
      Serial.printf("[HTTP] Unable to connect\n");
    }
  }

  delay(10000);
}
