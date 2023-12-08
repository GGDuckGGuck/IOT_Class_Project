char data;

void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);

}

void loop() {
  
  if(Serial.available()) {
    data = Serial.read();
  }

  if(data == 'a') {
    digitalWrite(13,1);
    delay(1000);
    digitalWrite(13,0);
    delay(1000);
  }
}
