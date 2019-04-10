

class WifiSettings {
  protected:
    String name = "Kracozabra";
    String password = "09655455";
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
