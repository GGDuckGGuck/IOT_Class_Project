#include "WiFiEsp.h"
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(2, 3); // RX, TX
#endif

char data;

const char ssid[] = "KT_GiGA_2G_Wave2_BDB2";
const char pass[] = "bf4edc0603";
int status = WL_IDLE_STATUS;
int reqCount = 0;

WiFiEspServer server(80);

void setup(){

  // initialize serial for debugging
  Serial.begin(9600);
  // initialize serial for ESP module
  Serial1.begin(9600);
  // initialize ESP module
  WiFi.init(&Serial1);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {

    Serial.println("WiFi shield not present");

    // don't continue
    while (true);
  }


  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {

    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);

    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);

  }


  Serial.println("You're connected to the network");
  printWifiStatus();
  // start the web server on port 80

  server.begin();

}


void loop() {

  // listen for incoming clients
  WiFiEspClient client = server.available();

  if (client) {
    Serial.println("New client");
    boolean currentLineIsBlank = true;

    // 하드웨어 시리얼에서 데이터를 읽어 `char data`에 저장합니다.
    if(Serial.available()) {
      data = Serial.read();

      Serial.print("Data_Print: ");
      Serial.println(data);
    }
    
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);

        if (c == '\n' && currentLineIsBlank) {
          Serial.println("Sending response");
          client.print(
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Connection: close\r\n"
            "Refresh: 20\r\n"
            "\r\n");
          client.print("<!DOCTYPE HTML>\r\n");
          client.print("<html>\r\n");
          client.print("<h1>Hello World!</h1>\r\n");
          client.print("Requests received: ");
          client.print(++reqCount);
          client.print("<br>\r\n");
          client.print("Received data: ");
          client.print(data);  // 시리얼에서 읽은 데이터를 웹 페이지에 출력합니다.
          client.print("<br>\r\n");
          client.print("</html>\r\n");
          break;
        }
        if (c == '\n') {
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          currentLineIsBlank = false;
        }
      }
    }

    // give the web browser time to receive the data
    delay(10);

    // close the connection:
    client.stop();
    Serial.println("Client disconnected");
  }
}


void printWifiStatus() {
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print where to go in the browser
  Serial.println();
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
  Serial.println();
}
