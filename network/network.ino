#include "WiFiEsp.h"
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
#include <stdio.h>

char data;
SoftwareSerial Serial1(2, 3); // RX, TX
#endif

const char ssid[] = "KT_GiGA_2G_Wave2_BDB2";
const char pass[] = "bf4edc0603";
int status = WL_IDLE_STATUS;
int reqCount = 0;

WiFiEspServer server(80);

void setup(){
  Serial.begin(9600);
  Serial1.begin(9600);
  WiFi.init(&Serial1);
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

void loop(){
  if(Serial.available()){
    data = Serial.read();
    WiFiEspClient client = server.available();
    if (client) {
      Serial.println("New client");
      boolean currentLineIsBlank = true;

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
            client.print("barcode");
            client.print(data);
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
      delay(10);
      client.stop();
      Serial.println("Client disconnected");
    }
  }
}
void printWifiStatus(){
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.println();
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
  Serial.println();
}