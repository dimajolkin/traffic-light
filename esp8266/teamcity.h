

class TeamCitySettings {

  protected:
    String url = "http://example.com";
  public:

    boolean isValid() {
      return url != "";
    }

    void serialInput() {
      while (!isValid()) {
        if (Serial.available() > 0) {
          String command = Serial.readString();
          if (contains(command, "teamCity.url=")) {
            url = command.substring(command.indexOf("=") + 1);
            Serial.println("OK: Set name url: " + url);
          }
        }
        //Serial.println("Failed: Wait wifi settings wifi.name=<name> wifi.password=<pasword>");
        delay(1000);
      }
    }

    String getUrl()
    {
      return url;
    }
};

class TeamCity {
  protected:
    String url;

  public:
    TeamCity(TeamCitySettings settings) {
      url = settings.getUrl();
    }

    String getUrl() {
      return url;
    }
};
