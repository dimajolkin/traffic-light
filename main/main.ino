#include <Arduino.h>
#include <SoftwareSerial.h>

#include <Arduino.h>
#include "lib.h"
#include "wifi.h"
#include "traffic.h"

#define PIN_RED 10
#define PIN_GREEN 8

#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial ESPserial(7, 8); // RX, TX
#endif

String ssid = "Kracozabra";            // your network SSID (name)
String pass = "09655455";        // your network password

int reqCount = 0;

Lumen red(PIN_RED);
Lumen green(PIN_GREEN);
Traffic traffic(red, green);

//WiFiEspServer server(80);
Wifi wifi(&ESPserial);
WiFiEspClient client;

class Request {

  protected:
    boolean sender = false;

  public:
    void send() {

      sender = false;
      client.stop();
      while (client.connected()) {
        Serial.println("Wait client disconnect");
      }

      // if you get a connection, report back via serial
      if (client.connect("example.com", 80)) {
        Serial.println("Connected to server");
        // Make a HTTP request
        client.println("GET / HTTP/1.1");
        client.println("Host: example.com");
        client.println("Connection: close");
        client.println();
        
        sender = true;
      }
    }

    

    String response() {
      // read headers
      while(client.connected()) {
        String line = client.readStringUntil('\n');
        if (line == "\r") {
          break;
          }
       }

       String answer = "";
       while(client.available()) {
        //String line = 
        char c = client.read();
        //answer += client.read();
        Serial.print(c);
       }

        
        client.stop();

        return answer;
//      if (!sender) {
//        return "";
//      }
//      String answer = "";
//      while (true) {
//        // if there are incoming bytes available
//        // from the server, read them and print them
//
//        while (client.available()) {
//          char c = client.read();
//          Serial.print(c);
//          answer.concat(c);
//        }
//
//        // if the server's disconnected, stop the client
//        if (!client.connected()) {
//          client.stop();
//          break;
//        }
//      }
//
//      return answer;
    }
};

Request request;
void setup()
{
  Serial.begin(9600);
  Serial.println("Init");

  traffic.setup();

  wifi.setup();
  while (!wifi.isConnect()) {
    wifi.connect(ssid, pass);
  }
  wifi.printStatus();

  // start the web server on port 80
  //server.begin();

  Serial.println();
  Serial.println("Starting connection to server...");

  
}

void loop() {
  Serial.println("Send request in example.com");
  
  request.send();
  String response = request.response();
  Serial.println("begin");
  Serial.println(response);
  Serial.println("end");

  delay(5000);

}

//void loop2()
//{
//  // listen for incoming clients
//  WiFiEspClient client = server.available();
//  if (client) {
//    Serial.println("New client");
//    // an http request ends with a blank line
//    boolean currentLineIsBlank = true;
//    while (client.connected()) {
//      if (client.available()) {
//        char c = client.read();
//        Serial.write(c);
//        // if you've gotten to the end of the line (received a newline
//        // character) and the line is blank, the http request has ended,
//        // so you can send a reply
//        if (c == '\n' && currentLineIsBlank) {
//          Serial.println("Sending response");
//
//          // send a standard http response header
//          // use \r\n instead of many println statements to speedup data send
//          client.print(
//            "HTTP/1.1 200 OK\r\n"
//            "Content-Type: text/html\r\n"
//            "Connection: close\r\n"  // the connection will be closed after completion of the response
//            "Refresh: 20\r\n"        // refresh the page automatically every 20 sec
//            "\r\n");
//          client.print("<!DOCTYPE HTML>\r\n");
//          client.print("<html>\r\n");
//          client.print("<h1>Hello World!</h1>\r\n");
//          client.print("Requests received: ");
//          client.print(++reqCount);
//          client.print("<br>\r\n");
//          client.print("Analog input A0: ");
//          client.print(analogRead(0));
//          client.print("<br>\r\n");
//          client.print("</html>\r\n");
//          break;
//        }
//        if (c == '\n') {
//          // you're starting a new line
//          currentLineIsBlank = true;
//        }
//        else if (c != '\r') {
//          // you've gotten a character on the current line
//          currentLineIsBlank = false;
//        }
//      }
//    }
//    // give the web browser time to receive the data
//    delay(10);
//
//    // close the connection:
//    client.stop();
//    Serial.println("Client disconnected");
//  }
//}
