#include <SoftwareSerial.h>
#ifndef HAVE_HWSERIAL1
#include <WiFiEsp.h>
#include <stdio.h>
#endif

#include <WiFiEsp.h>

SoftwareSerial GM65(2,3); // GM65는 소프트웨어 시리얼을 계속 사용

//wifi 정보
// const char ssid[] = "IoT";
// const char pass[] = "qwer1234";
const char ssid[] = "KT_GIGA_2G_Wave2_BDB2";
const char pass[] = "bf4edc0603";
int status = WL_IDLE_STATUS;     // the Wifi radio's status
int reqCount = 0;                // number of requests received

WiFiEspServer server(80);

void setup(){
  Serial.begin(9600); // ESP 모듈은 하드웨어 시리얼을 사용
  GM65.begin(9600);
  WiFi.init(&Serial); // ESP 모듈이 연결된 시리얼 포트로 초기화

  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
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
            "Refresh: 20\r\n"        // refresh the page automatically every 20 sec
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
  if(GM65.available()){
    String barcode = GM65.readStringUntil('\n');
    Serial.println(barcode);
    return barcode;
  }
}






