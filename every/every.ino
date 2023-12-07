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
  Serial.begin(115200); // GM65를 위한 하드웨어 시리얼
  Serial1.begin(115200); // WiFi를 위한 소프트웨어 시리얼
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

        if (c == '
' && currentLineIsBlank) {
          Serial.println("Sending response");
          client.print(
            "HTTP/1.1 200 OK\r
"
            "Content-Type: text/html\r
"
            "Connection: close\r
"  
            "Refresh: 20\r
"
            "\r
");
          client.print("<!DOCTYPE HTML>\r
");
          client.print("<html>\r
");
          client.print("<h1>Hello World!</h1>\r
");
          client.print("Requests received: ");
          client.print(++reqCount);
          client.print("<br>\r
");
          client.print("barcode ");
          String barcode = scanBarcode();
          client.print(barcode);
          client.print("<br>\r
");
          client.print("</html>\r
");
          break;
        }

        if (c == '
') {
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
  String barcode = "";  
  if(Serial.available()){
    barcode = Serial.readStringUntil('
');
    Serial.println(barcode);
  }
  return barcode; 
}
