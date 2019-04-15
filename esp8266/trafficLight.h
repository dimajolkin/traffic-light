#include <ArduinoJson.h>
DynamicJsonDocument doc(2048);

#define COMMAND_RED "red"
#define COMMAND_RED_BLINK "red-blink"
#define COMMAND_GREEN "green"
#define COMMAND_GREEN_BLINK "green-blink"
#define COMMAND_WAIT "wait"

class TrafficLight {

  public:
    String execute(String json) {
      DeserializationError error = deserializeJson(doc, json);
      if (error) {
        Serial.println(F("Failed to read file, using default configuration"));
        return "wait";
      }

      JsonArray builds = doc["build"];
      for (JsonObject build : builds) {
        const char* number = build["number"];
        const char* status = build["status"];
        //running
        //finished
        const char* state = build["state"];
        if (build["status"] == "SUCCESS") {
          if (build["state"] == "running") {
            return "green-blink";
          }
          return "green";
        }

        if (build["status"] == "FAILURE") {
          if (build["state"] == "running") {
            return "red-blink";
          }

          return "red";
        }
      }

      return "wait";
    }


    String answer(String json) {
      String color = execute(json);
      return "{\"command\": \""+ color +"\"}";
    }
};
