
class TeamCitySettings {

  public:
    String host;
    String project;
    String branch;

    TeamCitySettings() {
      host = "http://guest:guest@172.30.21.47:8111";
      project = "E6_Engine";
      branch = "develop";
    }
};

class TeamCity {
  protected:
    Url *url;
  public:
    TeamCity(Url *url): url(url) {}

    Url *getUrl() {
      return url;
    }

    void prepare(DynamicJsonDocument doc) {
      //JsonObject obj = doc.as<JsonObject>();
      Serial.println(" ===== ");

//      Serial.println(doc[String("0")].as<String>());
//
//      String output;
//      serializeJson(doc, output);
//
//       Serial.println(output);
    }

};
