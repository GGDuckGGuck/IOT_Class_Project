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
        String barcode_Data = GM65.readStringUntil('\n');
        Serial.print("read : ");
        Serial.println(barcode_Data);
        barcode_Data.trim();  // trim 함수를 수정하였습니다.
        barcode = barcode_Data;      
    }
  }    
}

void loop() { 
  scanBarcode();
  char barcode_char = barcode.charAt(0); // String을 char로 변환합니다.
  Serial.write(barcode_char);
  Serial.flush();
  delay(2000);
}
