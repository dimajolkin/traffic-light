#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <iostream>
#include <ESP8266HTTPClient.h>

#include <WiFiClient.h>

#include "lib.h"
#include "teamcity.h"
#include "trafficLight.h"

#define DELAY_REQUEST 10000

String configs[] = {
  "wifi.name=*****",
  "wifi.password=*****",
  //  "teamCity.host=http://guest:guest@172.30.21.47:8111",
  //  "teamCity.project=E6_Engine",
  //  "teamCity.branch=epic/E-06013/story/B-64996-fca-win-sticker-button"
};

//#define NETWORK_NAME "Kracozabra"
//#define NETWORK_PASSWORD "09655455"
//#define TS_URL "http://admin:admin@192.168.0.2:8111/httpAuth/app/rest/builds/?locator=buildType:TrafficLight_Build,state:any,count:1"

#define NETWORK_NAME "****"
#define NETWORK_PASSWORD "****"
#define TS_URL "http://guest:guest@172.30.21.47:8111/httpAuth/app/rest/builds/?locator=buildType:E6_Engine,state:any,branch:develop,count:1"

#define out Serial

ESP8266WiFiMulti WiFiMulti;

TeamCitySettings ts(String(TS_URL));
TrafficLight traffic;

//{"http": "connection refused"}

void setup() {
  Serial.begin(9600);
  for (uint8_t t = 4; t > 0; t--) {
    out.println("{\"setup\": \"wait\"}");
    delay(500);
    Serial.flush();
  }

  WiFi.mode(WIFI_STA);
  out.printf("{\"setup\":\"confirm\"}\n");
  WiFiMulti.addAP(NETWORK_NAME, NETWORK_PASSWORD);
  while (WiFiMulti.run() != WL_CONNECTED) {
    out.println("{\"setup\": \"wait connect\"}");
    delay(500);
  }

  IPAddress myIP = WiFi.localIP();
  String ipStr = String(myIP[0]) + "." + String(myIP[1]) + "." + String(myIP[2]) + "." + String(myIP[3]);
  out.printf("{\"setup\": \"connect\", \"ip\":\"%s\"}\n", ipStr.c_str());
}

void loop() {
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {
    WiFiClient client;
    HTTPClient http;
    if (http.begin(client, ts.getUrl().c_str())) {
      http.addHeader("Accept", "application/json");
      int httpCode = http.GET();
      if (httpCode > 0) {
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          out.println(traffic.answer(http.getString()));
        }
      } else {
        out.printf("{\"http\": \"%s\"}\n", http.errorToString(httpCode).c_str());
      }
      http.end();
    } else {
      out.println("{\"http\": \"failed\"}");
    }
  }

  delay(DELAY_REQUEST);
}
