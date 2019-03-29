#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>


class WifiSettings {
  protected:
    String name = "Dealersocket_Airport";
    String password = "Spring1.618";
  public:

    boolean isValid() {
      return name != "" && password != "";
    }

    void setName(String _name) {
      name = _name;
    }

    String getName() {
      return name;
    }

    void setPassword(String _password) {
      password = _password;
    }

    String getPassword() {
      return password;
    }

    void serialInput() {
      while (!isValid()) {
        if (Serial.available() > 0) {
          String command = Serial.readString();
          if (contains(command, "wifi.name=")) {
            setName(command.substring(command.indexOf("=") + 1));
            Serial.println("OK: Set name wifi: " + getName());
          }

          if (contains(command, "wifi.password=")) {
            setPassword(command.substring(command.indexOf("=") + 1));
            Serial.println("OK: Set password wifi: " + getPassword());
          }

        }
        //Serial.println("Failed: Wait wifi settings wifi.name=<name> wifi.password=<pasword>");
        delay(1000);
      }
    }
};

class ESPWifi {
  protected:
     ESP8266WiFiMulti wifi;
  public:

    ESPWifi() {}

    void configuration(WifiSettings wifisettings) {
        WiFi.mode(WIFI_STA);
        
      //WiFiMulti.addAP("Dealersocket_Airport", "Spring1.618");
      wifi.addAP(wifisettings.getName().c_str(), wifisettings.getPassword().c_str());
    }

    boolean isConnect() {
      return wifi.run() == WL_CONNECTED;
    }

};
