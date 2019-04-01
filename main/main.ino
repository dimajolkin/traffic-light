#include <SoftwareSerial.h>
#include <Arduino.h>
#include "lib.h"

#define PIN_RED 10
#define PIN_GREEN 8

#define STATUS_RED 2
#define STATUS_GREEN 3
#define STATUS_NONE 4

int status = STATUS_NONE; //green
boolean blinking = false;

SoftwareSerial ESPserial(3, 2); // TX | RX

String configs[] = {
  "wifi.name=Dealersocket_Airport",
  "wifi.password=*****",
  //  "teamCity.host=http://guest:guest@172.30.21.47:8111",
  //  "teamCity.project=E6_Engine",
  //  "teamCity.branch=epic/E-06013/story/B-64996-fca-win-sticker-button"
};


class Lumen {
  protected:
    int pin;
    int status = 0;
  public:
    Lumen(int pin): pin(pin) {}
    void attach() {
      pinMode(pin, OUTPUT);
      off();
    }

    void on() {
      if (status == 1) {
        return;
      }
      status = 1;
      digitalWrite(pin, HIGH);   // зажигаем светодиод
    }

    void off() {
      if (status == 0) {
        return;
      }

      status = 0;
      digitalWrite(pin, LOW);    // выключаем светодиод
    }

};

class Traffic {
  protected:
    Lumen _red;
    Lumen _green;
    int status = 0;
  public:
    Traffic(Lumen red, Lumen green): _red(red), _green(green) {}

    void red() {
      status = 0;
      _red.on();
      _green.off();
    }

    void green() {
      status = 1;
      _red.off();
      _green.on();
    }

    void blink() {
      if (status == 0) {
        green();
      } else {
        red();
      }
    }

};


Lumen red(PIN_RED);
Lumen green(PIN_GREEN);
Traffic traffic(red, green);

void check() {

  delay(5000);
  Serial.println("Check RED.");
  red.on();
  delay(2000);
  red.off();
  delay(2000);

  Serial.println("Check Green.");
  green.on();
  delay(2000);
  green.off();
  delay(2000);

  Serial.println("Completion.");
  red.on();
  green.on();
  delay(2000);
  red.off();
  green.off();
  delay(2000);
}

void setup()
{
  red.attach();
  green.attach();
  Serial.begin(115200);     // communication with the host computer
  ESPserial.begin(115200);

  Serial.println("Remember to to set Both NL & CR in the serial monitor.");
  Serial.println("Ready");

  check();
}

void loop()
{
  // listen for communication from the ESP8266 and then write it to the serial monitor
  if ( ESPserial.available() > 0)   {
    String msg = ESPserial.readString();
    if (contains(msg, "[ANSWER]")) {
      if (contains(msg, "red")) {
        status = STATUS_RED;
      } else if (contains(msg, "green")) {
        status = STATUS_GREEN;
      }
    } else if (contains(msg, "[HTTP]")) {
      Serial.println( msg );
    } else {
      Serial.println("Undefined msg: " + msg);
    }
  }

  if (status == STATUS_RED) {
    traffic.red();
  } else if (status == STATUS_GREEN) {
    traffic.green();
  } else {
    traffic.blink();
    delay(1000);
  }
}
