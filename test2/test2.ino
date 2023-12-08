#include <SoftwareSerial.h>
SoftwareSerial GM65(2,3);
String barcode = "";
void setup() {
 
  Serial.begin(9600);
  GM65.begin(9600);
  
}

void scanBarcode() {
  while (barcode == "") {
      while (GM65.available()) {
      String barcode_Data = GM65.readStringUntil('
');
      Serial.print("read : ");
      Serial.println(barcode_Data);
      barcode = barcode_Data;
    }
  }  
}

void loop() { 
  scanBarcode();
  char barcode_char = barcode.charAt(0); // String을 char로 변환합니다.
  Serial.println(barcode_char); // char 형태의 barcode를 출력합니다.
  delay(2000);

  barcode = "";
}
