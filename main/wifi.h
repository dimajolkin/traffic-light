
#include "WiFiEsp.h"

class Wifi {

  protected:
    SoftwareSerial *serial;
    int status = WL_IDLE_STATUS;
    bool _isConnect = false;
    String ip;

  public:
    Wifi(SoftwareSerial *serial): serial(serial) {}

    void setup() {
      serial->begin(9600);
      WiFi.init(serial);
      // check for the presence of the shield
      if (WiFi.status() == WL_NO_SHIELD) {
        Serial.println("WiFi shield not present");
        // don't continue
        while (true);
      }
    }

    void connect(String n, String p) {
      // attempt to connect to WiFi network
      while ( status != WL_CONNECTED) {
        Serial.print("Attempting to connect to WPA SSID: ");
        Serial.println(n);
        // Connect to WPA/WPA2 network
        status = WiFi.begin(n.c_str(), p.c_str());
      }

      _isConnect = true;
    }

    void printStatus() {
      // print the SSID of the network you're attached to
      Serial.print("SSID: ");
      Serial.println(WiFi.SSID());

      // print your WiFi shield's IP address
      IPAddress ip = WiFi.localIP();
      Serial.print("IP Address: ");
      Serial.println(ip);

    }

    boolean isConnect() {
      return _isConnect;
    }

    IPAddress getIp() {
      return WiFi.localIP();
    }
};
