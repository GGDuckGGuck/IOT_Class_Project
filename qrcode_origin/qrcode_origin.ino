#include <SoftwareSerial.h>

SoftwareSerial GM65_BarcodeReader(2, 3);

void setup() {

  Serial.begin(9600);

  GM65_BarcodeReader.begin(9600);

}

void loop() {

  if(GM65_BarcodeReader.available()){

  String barcode_Data = GM65_BarcodeReader.readStringUntil('\n');

  Serial.println(barcode_Data);

  }
}