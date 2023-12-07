#include <SoftwareSerial.h>
#include <WiFiEsp.h>
#include <stdio.h>

#ifndef HAVE_HWSERIAL1
SoftwareSerial Serial1(13,12);
#endif

//wifi 정보
const char ssid[] = "IoT";
const char pass[] = "qwer1234";
int status = WL_IDLE_STATUS;
int reqCount = 0;

WiFiEspServer server(80);

void setup(){
  Serial.begin(9600); // GM65를 위한 하드웨어 시리얼
  Serial1.begin(9600); // WiFi를 위한 소프트웨어 시리얼
  Serial1.listen();
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
          client.print("barcode ");
          String barcode = scanBarcode();
          client.print(barcode);
          client.print("<br>\r\n");
          client.print("</html>\r\n");
          break;
        }

        if (c == '\n') {
          currentLineIsBlank = true;
        }
        else if (c != '\n') {
          currentLineIsBlank = false;
        }
      }
    }
    delay(10);
    client.stop();
    Serial.println("Client disconnected");
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

String scanBarcode() {
  String barcode = "";  
  if(Serial.available()){
    barcode = Serial.readStringUntil('\n');
    Serial.println(barcode);
  }
  return barcode; 
}
