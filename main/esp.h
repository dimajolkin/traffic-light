

class ESPClient {
  protected:
    SoftwareSerial *ESPserial;
    
  public:

    ESPClient(int p1, int p2) {
      ESPserial = new SoftwareSerial(p1, p2);
    }
    void setup() {
      ESPserial->begin(115200);
      //waitReady();
    }

    void waitReady() {
      while (true) {
        if (ESPserial->available() > 0) {
          
          String msg = ESPserial->readString();
          Serial.println(msg);
          if (msg == "[SETUP] Ready") {
            break;
          }
        }
      }
    }
    void setConfig(String config) {
            
      while (true) {
        ESPserial->println(config);
        //Serial.println(config);
        if (ESPserial->available() > 0) {
          String msg = ESPserial->readString();
          Serial.println(msg);
          if (contains(msg, "OK")) {
            break;
          }
        }
        delay(500);
      }
    }

    void proxy() {
      if ( ESPserial->available() )   {
        Serial.write( ESPserial->read() );
      }

      // listen for user input and send it to the ESP8266
      if ( Serial.available() )       {
        ESPserial->write( Serial.read() );
      }
    }
};
