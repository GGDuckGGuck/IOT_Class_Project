#include "WiFiEsp.h" // ESP8266 WiFi 모듈을 위한 라이브러리
#ifndef HAVE_HWSERIAL1 // 하드웨어 시리얼이 없는 경우에 대비한 조건문

#include "SoftwareSerial.h" // 소프트웨어 시리얼 라이브러리
SoftwareSerial ESP(2, 3); // RX, TX
#endif
#include <stdio.h>

char data; // 데이터를 저장할 char 타입 변수 선언
char databox;

const char ssid[] = "KT_GiGA_2G_Wave2_BDB2"; // 연결할 WiFi SSID
const char pass[] = "bf4edc0603"; // 연결할 WiFi Password
int status = WL_IDLE_STATUS; // WiFi 상태 초기값 설정
int reqCount = 0; // 요청 카운터

WiFiEspServer server(80); // 80 포트로 서버 시작

void setup(){

  Serial.begin(9600); // 시리얼 통신 시작
  ESP.begin(9600); // ESP8266 시리얼 통신 시작
  WiFi.init(&ESP); // WiFi 초기화

  if (WiFi.status() == WL_NO_SHIELD) { // WiFi 쉴드가 없는 경우 에러 메시지 출력
    Serial.println("WiFi shield not present");
    while (true);
  }

  while ( status != WL_CONNECTED) { // WiFi에 연결될 때까지 반복
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass); // WiFi 연결 시도
  }

  Serial.println("You're connected to the network"); // 연결 성공 메시지 출력
  printWifiStatus(); // WiFi 상태 출력
  server.begin(); // 서버 시작

}

void loop() {

  // 들어오는 클라이언트 체크
  WiFiEspClient client = server.available();

  if (client) { // 클라이언트가 연결된 경우
    Serial.println("New client");
    boolean currentLineIsBlank = true;

    // 시리언에서 데이터 읽고 'data'에 저장
    if(Serial.available()) {
      data = Serial.read();

      if(data == "a") {
        databox = data;
      }

      else if(data == "b") {
        databox = data;
      }

      Serial.print("Data_Print: ");
      Serial.println(databox); // 읽은 데이터 출력
    }
    
    while (client.connected()) { // 클라이언트가 연결된 동안

      if (client.available()) { // 클라이언트로부터 데이터가 도착한 경우
        char c = client.read(); // 데이터 읽기
        Serial.write(c); // 읽은 데이터 쓰기

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

          client.print("Requests received: ");
          client.print(++reqCount);
          client.print("<br>\r\n");

          client.print("-------------------------------");
          client.print("<br>\r\n");
          client.print("Received data: ");
          client.print("<br>\r\n");
          client.println(databox);  // 시리얼에서 읽은 데이터를 웹 페이지에 출력합니다.

          client.print("-------------------------------");
          client.print("<br>\r\n");
          client.print("Container: a");
          client.print("<br>\r\n");
          client.print("Contents: Galaxy");
          client.print("<br>\r\n");
          client.print("Location: Right");
          client.print("<br>\r\n");          
          client.print("-------------------------------");
          client.print("<br>\r\n");

          client.print("-------------------------------");
          client.print("<br>\r\n"); 
          client.print("Container: b");
          client.print("<br>\r\n");
          client.print("Contents: Iphone");
          client.print("<br>\r\n");
          client.print("Location: Left");
          client.print("<br>\r\n");
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

    // 웹 브라우저가 데이터를 받는 시간을 주기 위해 10ms 대기
    delay(10);

    // 연결 종료
    client.stop();
    Serial.println("Client disconnected"); // 클라이언트 연결 종료 메시지 출력
  }
}

void printWifiStatus() {

  // 연결된 네트워크의 SSID 출력
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // WiFi 쉴드의 IP 주소 출력
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // 브라우저에서 이 페이지를 보려면 어디로 가야 하는지 출력
  Serial.println();
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
  Serial.println();
  
}
