#include <SoftwareSerial.h>
void setup(){
  Serial.begin(9600);
}
void loop(){
  int thumb = analogRead(A0);
  int index = analogRead(A1);
  int middle = analogRead(A2);
  int ring = analogRead(A3);
  int pinky = analogRead(A4);
  int x = analogRead(A5);
  int y = analogRead(A6);
  int z = analogRead(A7);
  Serial.println(thumb);
  Serial.write(thumb);
  Serial.println(index);
  Serial.println(middle);
  Serial.println(ring);
  Serial.println(pinky);
  Serial.println(x);
  Serial.println(y);
  Serial.println(z);
  Serial.println("-----------------------------");
  delay(1000);
}
