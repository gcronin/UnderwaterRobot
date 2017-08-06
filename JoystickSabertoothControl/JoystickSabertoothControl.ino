#include <Servo.h>

Servo HB25;
Servo sabertooth1;
Servo sabertooth2;
int STOP = 1500;

void setup() {
  Serial.begin(9600);
  sabertooth1.attach(5);
  sabertooth2.attach(6);
  sabertooth1.writeMicroseconds(STOP);
  sabertooth2.writeMicroseconds(STOP);
  
  // HB-25 initialisation time (5ms)
  delay(5);                                           
  pinMode(9, OUTPUT);
  // Set control pin low on power up
  digitalWrite(9, LOW);
  delay(1000); 
  HB25.attach(9, 800, 2200); 
  HB25.writeMicroseconds(STOP);
}

void loop() {
  // put your main code here, to run repeatedly:
  int LeftA = analogRead(A5);
  int LeftB = analogRead(A4);
  int RightUp = analogRead(A2);
  int pulse1 = map(LeftA, 0 , 1023, 1000, 2000);
  int pulse2 = map(LeftB, 0 , 1023, 1000, 2000);
  int pulse3 = map(RightUp, 0 , 1023, 1000, 2000);

  if(pulse1 < 1550 && pulse1 > 1450) pulse1 = 1500;  //deadband of 50
  if(pulse2 < 1550 && pulse2 > 1450) pulse2 = 1500;  //deadband of 50
  if(pulse3 < 1550 && pulse3 > 1450) pulse3 = 1500;  //deadband of 50

  if(pulse1 < 800) pulse1 = 800;
  if(pulse2 < 800) pulse2 = 800;
  if(pulse3 < 800) pulse3 = 800;

  if(pulse1 > 2200) pulse1 = 2200;
  if(pulse2 > 2200) pulse2 = 2200;
  if(pulse3 > 2200) pulse3 = 2200;

  sabertooth1.writeMicroseconds(pulse1);
  sabertooth2.writeMicroseconds(pulse2);
  HB25.writeMicroseconds(pulse3);
  
  Serial.print("Motor1:  ");
  Serial.print(pulse1);
  Serial.print("  Motor2:  ");
  Serial.print(pulse2);
  Serial.print("  Motor3:  ");
  Serial.println(pulse3);
  delay(50);


}
