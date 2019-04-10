#include <SoftwareSerial.h>
#include <ArduinoJson.h>

#include "wifi.h"
#include "traffic.h"
#include "server.h"
#include "teamcity.h"

#define PIN_RED 10
#define PIN_GREEN 8

#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial ESPserial(7, 8); // RX, TX
#endif

String ssid = "Kracozabra";            // your network SSID (name)
String pass = "09655455";        // your network password

Lumen red(PIN_RED);
Lumen green(PIN_GREEN);
Traffic traffic(&red, &green);

Wifi wifi(&ESPserial);
HttpServer server;

Url *teamcityUrl = new Url();

//StaticJsonDocument<200> doc;
DynamicJsonDocument doc(200);

void setup()
{
  Serial.begin(9600);
//  Serial.println("Init");
  
  teamcityUrl->auth = "Basic YWRtaW46YWRtaW4=";
  teamcityUrl->host = "192.168.0.2";
  teamcityUrl->port = 8111;
  teamcityUrl->path = "/httpAuth/app/rest/builds/?locator=buildType:TrafficLight_Build,state:any,count:1";

  traffic.setup();
  wifi.setup();
  while (!wifi.isConnect()) {
    wifi.connect(ssid, pass);
  }
  wifi.printStatus();
}

void loop() {
  String response = server.get(teamcityUrl);
  Serial.println(response);

  TeamCity tc(teamcityUrl);
  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, response.c_str());
  // Test if parsing succeeds.
  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
  } else {
    tc.prepare(doc);
  }

  delay(5000);
}
