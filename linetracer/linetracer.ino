#include <SoftwareSerial.h>
#ifndef HAVE_HWSERIAL1
#include <WiFiEsp.h>
#include <stdio.h>
#endif


#define enA 10//Enable1 L298 Pin enA 
#define in1 9 // 전진
#define in2 8 // 후진 
#define in3 7 // 후진
#define in4 6 // 전진
#define enB 5 //Enable2 L298 Pin enB 

#define R_S A0 //ir sensor Right
#define L_S A1 //ir sensor Left

#define Side_S A3

int status = WL_IDLE_STATUS;     // the Wifi radio's status
int reqCount = 0;                // number of requests received

WiFiEspServer server(80);

int Command = 0;
int Landing_Point = 9999;

int i=0;
int tempCommand = 0;


void setup(){ // put your setup code here, to run once
  Serial.begin(9600);
 


  pinMode(R_S, INPUT); // declare if sensor as input  
  pinMode(L_S, INPUT); // declare ir sensor as input
  pinMode(Side_S, INPUT); // declare if sensor as input  


  pinMode(enA, OUTPUT); // declare as output for L298 Pin enA 
  pinMode(in1, OUTPUT); // declare as output for L298 Pin in1 
  pinMode(in2, OUTPUT); // declare as output for L298 Pin in2 
  pinMode(in3, OUTPUT); // declare as output for L298 Pin in3   
  pinMode(in4, OUTPUT); // declare as output for L298 Pin in4 
  pinMode(enB, OUTPUT); // declare as output for L298 Pin enB 

  analogWrite(enA, 140); // 모터 세기 조절
  analogWrite(enB, 140); // 모터 세기 조절
}

void forword(){  //forword
  digitalWrite(in1, HIGH); //Right Motor forword Pin 
  digitalWrite(in2, LOW);  //Right Motor backword Pin 
  digitalWrite(in3, LOW);  //Left Motor backword Pin 
  digitalWrite(in4, HIGH); //Left Motor forword Pin 
}

void turnRight(){ //turnRight
  digitalWrite(in1, LOW);  //Right Motor forword Pin 
  digitalWrite(in2, HIGH); //Right Motor backword Pin  
  digitalWrite(in3, LOW);  //Left Motor backword Pin 
  digitalWrite(in4, HIGH); //Left Motor forword Pin 
}

void right_angle(){ //turnRight
  digitalWrite(in1, LOW);  //Right Motor forword Pin 
  digitalWrite(in2, LOW); //Right Motor backword Pin  
  digitalWrite(in3, LOW);  //Left Motor backword Pin 
  digitalWrite(in4, HIGH); //Left Motor forword Pin 
}

void turnLeft(){ //turnLeft
  digitalWrite(in1, HIGH); //Right Motor forword Pin 
  digitalWrite(in2, LOW);  //Right Motor backword Pin 
  digitalWrite(in3, HIGH); //Left Motor backword Pin 
  digitalWrite(in4, LOW);  //Left Motor forword Pin 
}

void left_angle(){ //turnLeft
  digitalWrite(in1, HIGH); //Right Motor forword Pin 
  digitalWrite(in2, LOW);  //Right Motor backword Pin 
  digitalWrite(in3, LOW); //Left Motor backword Pin 
  digitalWrite(in4, LOW);  //Left Motor forword Pin 
}

void Stop(){ //stop
  digitalWrite(in1, LOW); //Right Motor forword Pin 
  digitalWrite(in2, LOW); //Right Motor backword Pin 
  digitalWrite(in3, LOW); //Left Motor backword Pin 
  digitalWrite(in4, LOW); //Left Motor forword Pin 
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


void Get_QR_Code_Landing_Point() {
  if(Landing_Point == 9999) {
    tempCommand = 100;
  }

  else if(Landing_Point == 8888) {
    tempCommand = 200;
  }
}

void landing() {
  //부저음 울리기
}

void turn_ninty() {
  if(i==0) {
      Get_QR_Code_Landing_Point();
      

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
  
  if(Command == 1){forword();}   //if Right Sensor and Left Sensor are at White color then it will call forword function

  if(Command == 2){turnRight();} //if Right Sensor is Black and Left Sensor is White then it will call turn Right function  

  if(Command == 3){turnLeft();}  //if Right Sensor is White and Left Sensor is Black then it will call turn Left function

  if(Command == 4){Stop();}

  if(Command == 5){
    Stop();
    turn_ninty();
  }
}