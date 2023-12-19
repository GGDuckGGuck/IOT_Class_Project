#include <SoftwareSerial.h>

SoftwareSerial GM65(2,3); // GM65는 소프트웨어 시리얼을 계속 사용

#define enA 10//Enable1 L298 Pin enA 
#define in1 9 // 전진
#define in2 8 // 후진 
#define in3 7 // 후진
#define in4 6 // 전진
#define enB 5 //Enable2 L298 Pin enB 

#define R_S A0 //ir sensor Right
#define L_S A1 //ir sensor Left
#define Side_S A3

#define piezo 4  // 수동부저

#define trigPin 12 // 초음파 센서의 Trig 핀
#define echoPin 11 // 초음파 센서의 Echo 핀

#define safeDistance 10 // 사람을 감지하는 안전 거리

int Command = 0;
int i=0;
int tempCommand = 0;
String barcode = "";

long duration;
int distance;

void setup(){ // put your setup code here, to run once
  Serial.begin(9600);
  GM65.begin(9600);


  pinMode(R_S, INPUT); 
  pinMode(L_S, INPUT); 
  pinMode(Side_S, INPUT);


  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enB, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(piezo, OUTPUT);

  analogWrite(enA, 160); // 모터 세기 조절
  analogWrite(enB, 160); // 모터 세기 조절
}

void forword(){  //forword
  digitalWrite(in1, HIGH); //Right Motor forword Pin 
  digitalWrite(in2, LOW);  //Right Motor backword Pin 
  digitalWrite(in3, LOW);  //Left Motor backword Pin 
  digitalWrite(in4, HIGH); //Left Motor forword Pin 
}

void turnRight(){ //turnRight
  digitalWrite(in1, LOW); 
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void right_angle(){ //turnRight
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void turnLeft(){ //turnLeft
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void left_angle(){ //turnLeft
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW); 
}

void Stop(){ //stop
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void read_sensor_values()
{
  if(digitalRead(Side_S) == 1) {
    Command = 5; //STOP
  }
  
  else if((digitalRead(R_S) == 0)&&(digitalRead(L_S) == 0)) {
    Command = 1;  // 전진
  }

  else if((digitalRead(R_S) == 1)&&(digitalRead(L_S) == 0)&&(digitalRead(Side_S) == 0)) {
    Command = 2;  // 오른쪽으로 라인 맞추기
  }

  else if((digitalRead(R_S) == 0)&&(digitalRead(L_S) == 1)&&(digitalRead(Side_S) == 0)) {
    Command = 3;  // 왼쪽으로 라인 맞추기
  }

  else if((digitalRead(R_S) == 1)&&(digitalRead(L_S) == 1)) {
    Command = 4;  // STOP
  }

}

void scanBarcode() {
  Serial.println("scanBarcode");

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

void send_HW_Serial() {

  char barcode_char = barcode.charAt(0); // String을 char로 변환합니다. 
  Serial.write(barcode_char);
  Serial.flush();

  Serial.print("HW_Serial: ");
  Serial.println(barcode_char);

}


void Get_QR_Code_Landing_Point() {

  scanBarcode();

  // Check the barcode value and set tempCommand accordingly
  if(barcode == "a") {
      tempCommand = 100;
      
  }
  else if(barcode == "b") {
      tempCommand = 200;
      
  }  
}

//부저음 울리기
void landing() {

  Serial.println("Piezo_Ring");

  digitalWrite(piezo, HIGH);
  delay(1000);
  digitalWrite(piezo, LOW);
  delay(1000);

  Serial.println("Piezo_Ring");
   
}

void turn_Corner() {
  if(i==0) {      

      if(tempCommand == 100){ //우회전 하는 코드
        right_angle();
        delay(2200);
        forword();
        delay(100); 
      }

      else if(tempCommand == 200){ //좌회전 하는 코드
        left_angle();
        delay(2200);
        forword();
        delay(100);
      }  

      i++;
      read_sensor_values();
      return;
    }

  else if(i==1) {
      landing();

      // landing지점 벗어나기
      forword();
      delay(100);
      
      i=0;
      barcode = "";
      read_sensor_values();
      return;
  }
}

void Ultrasonic_Waves() {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2; // 음속을 사용해서 거리 계산

  Serial.print("Distance: ");  
  Serial.println(distance);
  
  if (distance < safeDistance) {
    Command = 4;     
  }

}

void loop(){  

  read_sensor_values();
  Ultrasonic_Waves();
  send_HW_Serial();
  
  if(Command == 1){
    forword();
    Serial.println("command1");
    }
  if(Command == 2){
    turnRight();
    Serial.println("command2");
    } 

  if(Command == 3){
    turnLeft();
    Serial.println("command3");
    }

  if(Command == 4){
    Stop();
    
    Serial.println("command4");
    }

  if(Command == 5){
      Serial.println("command5");
      Stop();
      Get_QR_Code_Landing_Point();
      turn_Corner();
    }
}