#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX
void setup(){
  Serial.begin(9600);
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
}

void loop(){
  if (Serial.available() > 0){
    int s = Serial.read();
    Serial.println(s);
    mySerial.write(Serial.read());
  }
 Serial.println("Hello,how are you?");
  delay(1000);
}
