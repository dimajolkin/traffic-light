
struct  Url {
    String auth;
    String host;
    unsigned int port;
    String path;
};

class HttpServer {
  protected:
    WiFiEspClient *client;

    inline boolean send(Url *url) {
      client->stop();
      while (client->connected()) {
        Serial.println("Wait client disconnect");
      }

      // if you get a connection, report back via serial
      if (client->connect(url->host.c_str(), url->port)) {
        client->println("GET " + url->path + " HTTP/1.1");
        client->println("Accept: application/json");
        client->println("Host: " + url->host + ":" + String(url->port));
        client->println("Authorization: " + url->auth);
        client->println("Connection: close");
        client->println();
        return true;
      }

      return false;
    }

  public:
    HttpServer() {
      client = new WiFiEspClient();
    }

    String get(Url *url) {
      send(url);
      // read headers
      while (client->connected()) {
        if (client->readStringUntil('\n') == "\r") {
          break;
        }
      }

      String rawBody = "";
      boolean isStart = false;
      while (client->available()) {
        char c = client->read();
        if (c == '\n' || c == '\r') continue;

        if (c == '{') {
          isStart = true;
        }

        if (isStart) {
          rawBody += c;
        }
      }

      client->stop();
      return rawBody;
    }
};
