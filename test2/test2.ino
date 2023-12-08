#include <SoftwareSerial.h>
SoftwareSerial GM65(2,3);
String barcode = "";
void setup() {
 
  Serial.begin(9600);
  GM65.begin(9600);
  
}

void scanBarcode() {
  Serial.println("scanBarcode");

  while (barcode == "") {
    Serial.println("Tlqkf");
    while (GM65.available()) {
      String barcode_Data = GM65.readStringUntil('\n');
      Serial.print("read : ");
      Serial.println(barcode_Data);
      barcode = barcode_Data;
    }
  }  
}

void loop() { 
  scanBarcode();
  Serial.println("OUT");
  delay(2000);

  barcode = "";
}