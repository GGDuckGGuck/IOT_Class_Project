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

int Command = 0;
int i=0;
int tempCommand = 0;
String barcode = "";

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

  analogWrite(enA, 200); // 모터 세기 조절
  analogWrite(enB, 200); // 모터 세기 조절
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
  if((digitalRead(R_S) == 0)&&(digitalRead(L_S) == 0)) {
    Command = 1;  // 전진
  }

  else if((digitalRead(R_S) == 1)&&(digitalRead(L_S) == 0)&&(digitalRead(Side_S) == 0)) {
    Command = 2;  // 오른쪽으로 라인 맞추기
  }

  else if((digitalRead(R_S) == 0)&&(digitalRead(L_S) == 1)&&(digitalRead(Side_S) == 0)) {
    Command = 3;  // 왼쪽으로 라인 맞추기
  }

  else if((digitalRead(R_S) == 1)&&(digitalRead(L_S) == 1)) {
    Command = 4;  // 전진
  }

  else if((digitalRead(R_S) == 1)&&(digitalRead(L_S) == 1)&&(digitalRead(Side_S) == 1)) {
    Command = 5; //STOP
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
        Serial.print("barcode value is now: ");
        Serial.println(barcode);
        Serial.print("barcode length is: ");
        Serial.println(barcode.length());
    }


  }  
}


void Get_QR_Code_Landing_Point() {

  scanBarcode();  
  Serial.println(barcode);

  // Check the barcode value and set tempCommand accordingly
  if(barcode == "9999") {
      Serial.println(barcode);
      Serial.println("Success");
      tempCommand = 100;
      barcode = "";
  }
  else if(barcode == "8888") {
      Serial.println(barcode);
      tempCommand = 200;
      barcode = "";
  }
  else {
    Serial.println("fail");
    barcode = "";
  }
}

void landing() {
  //부저음 울리기
}

void turn_ninty() {
  if(i==0) {
      Get_QR_Code_Landing_Point();

      Serial.println(tempCommand);
      

      if(tempCommand == 100){ //우회전 하는 코드

        Serial.println("tempCommand");
        right_angle();
        delay(2000);
        forword();
        delay(100); 
      }

      else if(tempCommand == 200){ //좌회전 하는 코드
        left_angle();
        delay(2000);
        forword();
        delay(100);
      }  
      i++;
      read_sensor_values();
      return;
    }

  else if(i==1) {
      landing();

      if(tempCommand == 100){ //우회전 하는 코드
        right_angle();
        delay(2000);
        forword(); 
        delay(100);
      }

      else if(tempCommand == 200){ //좌회전 하는 코드
        left_angle();
        delay(2000);
        forword();
        delay(100);
      }
      i=0;
      read_sensor_values();
      return;
  }
}


void loop(){  
  read_sensor_values();
  
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
      Serial.println("command4");
      Stop();
      turn_ninty();
    }
}