#include <Servo.h> 
#include <SoftwareSerial.h>


SoftwareSerial mySerial(12, 11); // RX, TX
int direcPin = 10;        // Porta do SERVO e a porta 8 => PWM
int farolPin = 6;       // Porta do Farol e a porta 11 => PWM
//int farolPinR = 9;       // Porta do Farol e a porta 11 => PWM
int ln1 = 7;    // Porta da Reversao e a porta 10 => PWM
int ln2 = 8;    // Porta da Reversao e a porta 10 => PWM
int motorPin = 3;        // Porta do Motor e a porta 6 => PWM
int turboPin = 13                                                                                                                                   ;        // Porta do Turbo e a porta 9 => Digital
int backlightPin = 9;   // Porta da Sinaleira e a porta 13 => Digital
//int backlightPinR = 4;   // Porta da Sinaleira e a porta 13 => Digital
int anglebound = 24;
Servo myservo;           // Declaracao do servo
boolean op = false;      // Variavel utilizada para controlar o estado de envio de um comando
int zero = 88;           // Posicao que centraliza o volante
int speed = 0;
int aux;
void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);
  myservo.attach(direcPin                                                                                                                                                      );
  myservo.write(zero + anglebound);
  delay(1000);
  myservo.write(zero - anglebound);
  delay(1000);
  myservo.write(zero);
  
  pinMode(direcPin, OUTPUT);   // sets the pin as output
  pinMode(farolPin, OUTPUT); 
  pinMode(motorPin, OUTPUT);
  pinMode(turboPin, OUTPUT); 
  pinMode(ln1, OUTPUT); 
  pinMode(ln2, OUTPUT); 
  pinMode(backlightPin, OUTPUT); 
  stopMotor(); 
  
  for(int i = 0; i < 3; i++){
    frontLightOn();
    delay(500);
    frontLightOff();
    delay(500);
  } 
}

//ENGINE CONTROL

void turboOn(){
    analogWrite(turboPin, 255);
}

void turboOff(){
      analogWrite(turboPin, 0);
      analogWrite(motorPin, 0);
}

void goForward(){
    digitalWrite(ln1, LOW);   
    digitalWrite(ln2, HIGH);
    analogWrite(motorPin, speed); 
}

void goBackward(){
    digitalWrite(ln2, LOW);
    digitalWrite(ln1, HIGH);
    analogWrite(motorPin, speed); 
}

void stopMotor(){
    analogWrite(motorPin, 0);
    analogWrite(turboPin, 0);
}

//STEERING CONTROL

void turn(int i){
  myservo.write(i);
}

void turnLeft(){
  turn(zero+anglebound);
}

void turnRight(){
  turn(zero-anglebound);
}

void turnCenter(){
   turn(zero);
}

//LIGHTS CONTROL

void frontLightOn(){
    analogWrite(farolPin, 170);
    //analogWrite(farolPinR, 170);
}

void frontLightOff(){
    analogWrite(farolPin, 0);
}

void frontLightHigh(){
    analogWrite(farolPin, 255);
}

void backLightOn(){
    digitalWrite(backlightPin, HIGH);

}

void backLightOff(){
    digitalWrite(backlightPin, LOW);
}

//Arduino LOOP

void loop(){
 if(mySerial.available()){
  byte c = mySerial.read();
  
  //Serial.println(c);
  
  if(c == '!') //int 33
    op = true;
  else
  if(op){//Aplicado a todo switch
  //Serial.println(c);
    if(c > 50){
      c = abs(c);
      if(c < 100 ){
          //Dobrar esq de 100 a 149, 100..149 = 49 possibilidades o zero é separado
          if(c <= anglebound)
            //turn(zero +(c-50));
            Serial.println("Agora vai");
            myservo.write((int)(zero +(c-50)));
            Serial.println("Dobrou esquerda");
            Serial.println(zero +(c-50));
        }else if(c == 100){//exatamente 0
          turn(zero);
          Serial.println("Centralizou volante");
          Serial.println(zero);
          // Exatamente 150
        }else if(c < 150){ 
          //Dobrar direita de 151 ate 199 
          int au = (c - 100);
          if(au <= anglebound)
          turn(zero - au);
          Serial.println("Dobrou direita");
          Serial.println(zero-au);
          //####################################################################
        } else if(c < 160){
          //Andar em frente 200 ate 209
          speed = (c - 150) * 28;
          Serial.println("Andou para frente");
          Serial.println((c - 150) * 28);
          turboOff();
          goForward();
        } else if (c < 170 ){
         //Andar de re 110 ate 119
         speed = (c - 160) * 28;
         Serial.println("Andou para tras");
         Serial.println((c - 160) * 28);
         turboOff();
         goBackward(); 
        }
    }else{
  switch (c) {
  case 0:
    speed = 255;
    goForward(); 
    Serial.println("Andar para frente max speed");
    break;
  case 1:
    speed = 255;
    goBackward();
    Serial.println("Andar para trás max speed");
    break;
  case 2:
    turnLeft();
    Serial.println("Dobrar esquerda");
    break;
  case 3:
    turnRight();
    Serial.println("Dobrar direita");
    break;
  case 4:
    turnCenter();
    Serial.println("Centralizar volante");
    break;
  case 5:
    turboOn();
    Serial.println("Turbo Ligado");
    break;
  case 6:
    turboOff();
    Serial.println("Turbo Desligado");
    break;
  case 7:
    stopMotor();
    Serial.println("Desligado motor");
    break;
  case 8: 
    frontLightOff();
    backLightOff();
    Serial.println("Desligou todas luzes");
    break;
  case 9:
    frontLightOn();
    backLightOn();
    Serial.println("Acendeu todas luzes");
    break;
  case 10:
    frontLightOn();
    Serial.println("Ligou Farois dianteiros");
    break;
  case 11:
    frontLightOff();
    Serial.println("Desligou Farois Dianteiros");
    break;
  case 12:
    frontLightHigh();
    Serial.println("Ligou farol luz alta");
    break; 
  case 13:
    backLightOn();
    Serial.println("Ligou sinaleira traseira");
    break;
  case 14:
    backLightOff();
    Serial.println("Desligou sinaleira traseira");
    break;
  case 15:
    delay(100);
    Serial.println("Timer 100");
    break;
  case 16:
    delay(250);
    Serial.println("Timer 250");
    break;
  case 17:
    delay(500); 
    Serial.println("Timer 500");
    break;
  case 18:
    delay(1000);
    Serial.println("Timer 1 segundo");
    break;
    }
  }
  op = false;
 }
}
}
  //#######################
  //Tabela de comandos
  //#######################
  //Receber comando => !
  //Andar para frente => 0
  //Andar para tras => 1
  //Dobrar esquerda => 2
  //Dobrar direita => 3
  //Dobrar centro => 4
  //Ativar Turbo => 5
  //Desativar Turbo => 6
  //Parar motor => 7
  //Desligar todas luzes => 8
  //Ligar todas luzes => 9
  //Ligar Luz Baixa => 10
  //Desligar farois => 11
  //Ligar Luz Alta => 12
  //Ligar luz traseira => 13
  //Desligar luz traseira => 14
  //Temporizador de 100 milisegundos => 15
  //Temporizador de 250 milisegundos => 16
  //Temporizador de 500 milisegundos => 17
  //Temporizador de 1000 milisegundos => 18
  //Dobrar dir de -1 a -49
  //Centralizar Exatamente -50
  //Dobrar esq de -51 a -99
  //Andar em frente -100 ate 109
  //Andar de re -110 ate 119

