
#include "WiFiEsp.h"

class Command {
  protected:
    String command;
    int timeout;
    boolean debug;
  public:
    Command(String command, const int timeout, boolean debug): command(command), timeout(timeout), debug(debug) {}
    Command(String command): command(command) {
      timeout = 100;
      debug = false;
    }

    String send(SoftwareSerial *serial) {
      while (serial->available()) serial->read(); // Clear input buffer
      serial->print(command + "\r\n");
      delay(100);
      String str = "";
      while (true) {
        if (serial->available()) {
          str += serial->readString();
          str.replace(command, "");
          str.trim();
          if (str != "") {
            break;
          }
        }
      }

      return str;
    }

};

class Wifi {

  protected:
    SoftwareSerial *serial;
    int status = WL_IDLE_STATUS;
    bool _isConnect = false;
    String ip;

    void printEncryptionType(int thisType) {
      // read the encryption type and print out the name
      switch (thisType) {
        case ENC_TYPE_WEP:
          Serial.print("WEP");
          break;
        case ENC_TYPE_WPA_PSK:
          Serial.print("WPA_PSK");
          break;
        case ENC_TYPE_WPA2_PSK:
          Serial.print("WPA2_PSK");
          break;
        case ENC_TYPE_WPA_WPA2_PSK:
          Serial.print("WPA_WPA2_PSK");
          break;
        case ENC_TYPE_NONE:
          Serial.print("None");
          break;
      }
      Serial.println();
    }

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

      // print where to go in the browser
      //Serial.println();
      //Serial.print("To see this page in action, open a browser to http://");
      //Serial.println(ip);
      //Serial.println();
    }

    void printMacAddress()
    {
      // get your MAC address
      byte mac[6];
      WiFi.macAddress(mac);

      // print MAC address
      char buf[20];
      sprintf(buf, "%02X:%02X:%02X:%02X:%02X:%02X", mac[5], mac[4], mac[3], mac[2], mac[1], mac[0]);
      Serial.print("MAC address: ");
      Serial.println(buf);
    }

    void printNetworks()
    {
      // scan for nearby networks
      int numSsid = WiFi.scanNetworks();
      if (numSsid == -1) {
        Serial.println("Couldn't get a wifi connection");
        while (true);
      }

      // print the list of networks seen
      Serial.print("Number of available networks:");
      Serial.println(numSsid);

      // print the network number and name for each network found
      for (int thisNet = 0; thisNet < numSsid; thisNet++) {
        Serial.print(thisNet);
        Serial.print(") ");
        Serial.print(WiFi.SSID(thisNet));
        Serial.print("\tSignal: ");
        Serial.print(WiFi.RSSI(thisNet));
        Serial.print(" dBm");
        Serial.print("\tEncryption: ");
        printEncryptionType(WiFi.encryptionType(thisNet));
      }
    }



    boolean isConnect() {
      return _isConnect;
    }

    IPAddress getIp() {
      return WiFi.localIP();
    }
};

class WifiServer {
  protected:
    int port;
  public:
    WifiServer(int port): port(port) {


    }

};
