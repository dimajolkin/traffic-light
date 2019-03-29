

class TeamCitySettings: public AbstractSetting {

  public:
    String getName() {
      return "teamCity";
    }

    TeamCitySettings() {
      config["host"] = "http://guest:guest@172.30.21.47:8111";
      config["project"] = "E6_Engine";
      config["branch"] = "develop";
    }
};

class TeamCity {
  protected:
    String host;
    String branch;
    String project;

  public:
    TeamCity(TeamCitySettings settings) {
      host = settings.get("host");
      branch = settings.get("branch");
      project = settings.get("project");
    }

    String getUrl() {
      return host + "/httpAuth/app/rest/builds/?locator=buildType:" + project + ",state:any,branch:" + branch + ",count:1";
    }

    String parseXml(String xml) {
      if (contains(xml, "status=\"SUCCESS\"")) {
        return "green";
      }

      if (contains(xml, "status=\"FAILURE\"")) {
        return "red";
      }

      return "none";
    }

};
