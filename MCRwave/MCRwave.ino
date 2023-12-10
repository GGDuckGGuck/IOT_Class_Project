
const int motorPin = 5; // 모터의 핀

long duration;
int distance;
int safeDistance = 10; // 사람을 감지하는 안전 거리

void setup() {

  pinMode(motorPin, OUTPUT);
  
  Serial.begin(9600);
}

void loop() {
  // 초음파 센서로 거리 측정
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  

  
  Serial.print("Distance: ");
  Serial.println(distance);

  // 안전 거리 이내에 물건을 감지하면 모터를 멈춤
  if (distance < safeDistance) {
    digitalWrite(motorPin, LOW); // 모터 멈춤
  } else {
    digitalWrite(motorPin, HIGH); // 모터 동작
  }
  
  delay(1000); // 1초마다 거리 재측정
}