
#include <Arduino.h>
#include <map>
#include <iostream>
#include <cassert>

boolean contains(String str, String substring);


class Input {
  protected:
    String cmd = "";
  public:
    Input(String cmd): cmd(cmd) {}

    boolean is(String name) {
      return contains(cmd, name + "=");
    }

    String get(String name) {
      return cmd.substring(cmd.indexOf("=") + 1);
    }
};

class AbstractSetting {
  protected:
    const int inputDealy = 1000;

  public:
    virtual String getName() = 0;
    std::map<String, String> config = {};
    std::map<String, String> getConfig() {
      return config;
    }

    boolean isValid() {
      for (auto it = config.begin(); it != config.end(); ++it) {
        //Serial.println(it->second);
        if (it->second == "") {
          return false;
        }
      }

      return true;
    }

    void serialInput() {
      while (!isValid()) {
        if (Serial.available() > 0) {
          Input input(Serial.readString());
          for (auto it = config.begin(); it != config.end(); ++it) {
            String alias = getName() + "." + it->first;
            if (input.is(alias)) {
               it->second = input.get(alias);
               it->second.replace("\n", "");
               it->second.replace("\r", "");
               Serial.println("OK: Set " + it->first + " = " + get(it->first));
            }
          }
        }

        delay(inputDealy);
      }
    }


    String get(String name) {
      return config[name];
    }
};


boolean contains(String str, String substring) {
  return str.indexOf(substring) != -1;
}
