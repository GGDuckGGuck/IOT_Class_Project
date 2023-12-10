#include "WiFiEsp.h"
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial ESP(2, 3); // RX, TX
#endif
#include <stdio.h>
char data;

const char ssid[] = "U+Net5DD8";
const char pass[] = "2DD#438P17";
int status = WL_IDLE_STATUS;
int reqCount = 0;

WiFiEspServer server(80);

void setup(){
  Serial.begin(9600);
  ESP.begin(9600);
  WiFi.init(&ESP);
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while (true);
  }
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
  }
  Serial.println("You're connected to the network");
  printWifiStatus();
  server.begin();
}


void loop() {

  // listen for incoming clients
  WiFiEspClient client = server.available();

  if (client) {
    Serial.println("New client");
    boolean currentLineIsBlank = true;

    // 하드웨어 시리얼에서 데이터를 읽어 `char data`에 저장
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
            "\r\n");
          client.print("<!DOCTYPE HTML>\r\n");
          client.print("<html>\r\n");
          client.print("<h1>Hello World!</h1>\r\n");
          client.print("<br>\r\n");
          client.print("Received data: ");
          client.print(data);  // 시리얼에서 읽은 데이터를 웹 페이지에 출력합니다.
          client.print("-------------------------------");
          client.print("컨테이너: a");
          client.print("내용물: a");
          client.print("위치: 오른쪽");
          client.print("컨테이너: b");
          client.print("내용물: b");
          client.print("위치: 왼쪽");
          client.print("-------------------------------");
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