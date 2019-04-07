
class Request {

  protected:
    boolean sender = false;
    String host;
    int port;
    String url;
  public:
    Request(String host, int port, String url): host(host), port(port), url(url) {}
};

class Response {
  protected:
    String rawBody;
  public:
    Response(String rawBody): rawBody(rawBody) {}

    String getBody() {
      return rawBody;
    }
};

class HttpServer {
  protected:
    WiFiEspClient *client;

    boolean send(String host, uint32_t port, String url) {
      client->stop();
      while (client->connected()) {
        Serial.println("Wait client disconnect");
      }

      // if you get a connection, report back via serial
      if (client->connect(host.c_str(), port)) {
        Serial.println("Connected to server");
        client->println("GET " + url + " HTTP/1.1");
        client->println("Host: " + host);
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

    Response get(String host, String url) {
        return get(host, 80, url);
    }
    
    Response get(String host, uint32_t port, String url) {
      send(host, port, url);
      // read headers
      while (client->connected()) {
        String line = client->readStringUntil('\n');
        if (line == "\r") {
          break;
        }
      }

      String rawBody = "";
      while (client->available()) {
        String line = client->readStringUntil('\n');
        if (line == "") continue;
        rawBody += line;
      }

      client->stop();
      return Response(rawBody);
    }
};
