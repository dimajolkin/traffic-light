
#include "lib.h"
#include "teamcity.h"
#include "wifi.h"

/**
   wifi.name=<>
   wifi.password=<>
*/

//ESP8266WiFiMulti wifi;
ESPWifi wifi;
WifiSettings wifisettings;
TeamCitySettings tmsettings;

TeamCity *tm;

void setup() {
  Serial.begin(115200);
  // Serial.setDebugOutput(true);
  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }
  
  Serial.println("[SETUP] Ready");
  
  wifisettings.serialInput();
  tmsettings.serialInput();
  
  tm = new TeamCity(tmsettings);

  wifi.configuration(wifisettings);

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());   //You can g
}

void loop() {
  // wait for WiFi connection
  if (wifi.isConnect()) {

    WiFiClient client;
    HTTPClient http;
    
    //Serial.print("[HTTP] begin...\n");
    http.begin(client, tm->getUrl());
    
    //Serial.println(tm->getUrl());
    //Serial.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = http.GET();
    // Serial.println(httpCode);

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      //Serial.printf("[HTTP] GET... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        Serial.println("[ANSWER] " + tm->parseXml(http.getString()));
      }
    } else {
      Serial.printf("[HTTP] GET... failed, error: %s\r\n", http.errorToString(httpCode).c_str());
      //Serial.printf("[HTTP] GET... failed \r\n");
    }

    http.end();
  }

  delay(10000);
}
