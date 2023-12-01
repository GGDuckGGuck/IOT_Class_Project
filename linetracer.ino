#define enA 10//Enable1 L298 Pin enA 
#define in1 9 // 전진
#define in2 8 // 후진 
#define in3 7 // 후진
#define in4 6 // 전진
#define enB 5 //Enable2 L298 Pin enB 

#define R_S A0 //ir sensor Right
#define L_S A1 //ir sensor Left

#define Side_S A3

float Command = 0;
char location = 'AAA';

void setup(){ // put your setup code here, to run once

  pinMode(R_S, INPUT); // declare if sensor as input  
  pinMode(L_S, INPUT); // declare ir sensor as input
  pinMode(Side_S, INPUT); // declare if sensor as input  


  pinMode(enA, OUTPUT); // declare as output for L298 Pin enA 
  pinMode(in1, OUTPUT); // declare as output for L298 Pin in1 
  pinMode(in2, OUTPUT); // declare as output for L298 Pin in2 
  pinMode(in3, OUTPUT); // declare as output for L298 Pin in3   
  pinMode(in4, OUTPUT); // declare as output for L298 Pin in4 
  pinMode(enB, OUTPUT); // declare as output for L298 Pin enB 

  analogWrite(enA, 100); // 모터 세기 조절
  analogWrite(enB, 100); // 모터 세기 조절
  delay(1000);
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

void turnLeft(){ //turnLeft
digitalWrite(in1, HIGH); //Right Motor forword Pin 
digitalWrite(in2, LOW);  //Right Motor backword Pin 
digitalWrite(in3, HIGH); //Left Motor backword Pin 
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
  if((digitalRead(R_S) == 0)&&(digitalRead(L_S) == 0)&&(digitalRead(Side_S)) == 0) {
    Command = 1;  // 전진
  }

  else if((digitalRead(R_S) == 1)&&(digitalRead(L_S) == 0)&&(digitalRead(Side_S) == 0)) {
    Command = 2;  // 오른쪽으로 라인 맞추기
  }

  else if((digitalRead(R_S) == 0)&&(digitalRead(L_S) == 1)&&(digitalRead(Side_S) == 0)) {
    Command = 3;  // 왼쪽으로 라인 맞추기
  }

  else if(digitalRead(Side_S == 1)) {
    Command = 4; //STOP
  }
}


void Get_QR_Code_location(String location) {
  if(location == 'AAA') {
    Command = 100;
  }

  if(location == 'BBB') {
    Command = 200;
  }
}

void landing() {

}


void loop(){  
  read_sensor_values();

  if(Command == 0){forword();}   //if Right Sensor and Left Sensor are at White color then it will call forword function

  if(Command == 1){turnRight();} //if Right Sensor is Black and Left Sensor is White then it will call turn Right function  

  if(Command == 2){turnLeft();}  //if Right Sensor is White and Left Sensor is Black then it will call turn Left function

  if(Command == 3){
    while() {
      Stop();
      if (Get_QR_Code_location())
      {
        break;
      }

      else if (landing())
      {
        break;
      }
    }
  } 

  if(Command == 100){ //좌회전 하는 코드
    turnLeft();
    delay(2000);
    forward();    
  }

  if(Command == 200){ //우회전 하는 코드
    turnRight();
    delay(2000);
    forward();
  }

 
}