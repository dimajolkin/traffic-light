//#include <ArduinoJson.h>
#include <SoftwareSerial.h>
#include "traffic.h"
#include <TimerOne.h>

#define PIN_RED 8
#define PIN_GREEN 10

#define ESP_PIN_RX 6
#define ESP_PIN_TX 7

SoftwareSerial esp(ESP_PIN_TX, ESP_PIN_RX);
Lumen red(PIN_RED);
Lumen green(PIN_GREEN);
Traffic traffic(&red, &green);

void setup()
{
  Serial.begin(9600);
  esp.begin(9600);
  Serial.println("Init Traffic-light v0.1");
  Timer1.initialize((500000 * 2));         // initialize timer1, and set a 1/2 second period             // setup pwm on pin 9, 50% duty cycle
  Timer1.attachInterrupt(callback);  // attaches callback() as a timer overflow interrupt

  traffic.setup();
  traffic.check();
  esp.flush();
}


String json = "";
String command = traffic.getCommand();

void callback() {
  traffic.handler();
}

String cmd;
void loop() {
  if (esp.available() > 0) {
    json = esp.readString();
    json.trim();
    cmd = traffic.parse("command", json);
    traffic.setCommand(cmd);
    Serial.println(cmd);
  }
  delay(1000);
}
