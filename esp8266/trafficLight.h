#include <ArduinoJson.h>
DynamicJsonDocument doc(2048);

class TrafficLight {

  public:
    String execute(String json) {
      DeserializationError error = deserializeJson(doc, json);
      if (error) {
        return "none";
      }

      JsonArray builds = doc["build"];
      for (JsonObject build : builds) {
        const char* number = build["number"];
        const char* status = build["status"];
        //running
        //finished
        const char* state = build["state"];
        if (status && state) {
            return build["status"].as<String>() + "-" + build["state"].as<String>();  
        }
      }

      return "none";
    }

    String answer(String json) {
      String color = execute(json);
      return "{\"status\": \"tm-"+ color +"\"}";
    }
};
