#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#include <WiFiClient.h>

#include "lib.h"
#include "wifi.h"
#include "teamcity.h"

ESP8266WiFiMulti WiFiMulti;


WifiSettings ws;
TeamCitySettings ts;

void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.println();
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
//"http://jigsaw.w3.org/HTTP/connection.html"
void loop() {
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    WiFiClient client;

    HTTPClient http;

    Serial.print("[HTTP] begin...\n");
    if (http.begin(ts.getUrl().c_str())) {
      //http.addHeader("Accept", "application/json");
      //http.setAuthorization("guest", "guest");

      Serial.print("[HTTP] GET...\n");
      // start connection and send HTTP header
      int httpCode = http.GET();

      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          Serial.println("[ANSWER]");
          String payload = http.getString();
          Serial.println(payload);
          Serial.println("[END]");
        }
      } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }

      http.end();
    } else {
      Serial.printf("[HTTP} Unable to connect\n");
    }
  }

  delay(10000);
}
