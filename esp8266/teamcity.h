

class TeamCitySettings {

  protected:
   // String url = "http://admin:admin@192.168.0.2:8111/httpAuth/app/rest/builds/?locator=buildType:TrafficLight_Build,state:any,count:1";
    String url = "";
  public:

    TeamCitySettings(String url): url(url) {}

    boolean isValid() {
      return url != "";
    }

    void serialInput() {
      delay(500);
      while (!isValid()) {
        if (Serial.available() > 0) {
          String command = Serial.readString();
          if (contains(command, "teamCity.url=")) {
            url = command.substring(command.indexOf("=") + 1);
            url.replace("\n", "");
            Serial.printf("{\"setup\": \"input_ok\"}\n");
          }
        }
        delay(500);
      }
    }

    String getUrl() {
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
