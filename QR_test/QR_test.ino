#include <SoftwareSerial.h>

SoftwareSerial GM65_BarcodeReader(2, 3);

void setup() {

  Serial.begin(9600);

  GM65_BarcodeReader.begin(9600);

}

void loop() {



  while (GM65_BarcodeReader.available()) {
    String barcode_Data = GM65_BarcodeReader.readStringUntil('\n');
    Serial.print("read : ");
    Serial.println(barcode_Data);
    barcode_Data.trim();  // trim 함수를 수정하였습니다.
 
  }

}
