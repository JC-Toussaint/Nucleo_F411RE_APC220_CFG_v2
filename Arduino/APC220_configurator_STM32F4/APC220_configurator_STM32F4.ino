#include <SoftwareSerial.h>
 
const int pinRX =  2;  /* <-- TXD */
const int pinTX =  8;  /* --> RXD */
const int pinSET= 12;
 
SoftwareSerial apc220(pinRX, pinTX); // Crt softserial port and bind tx/rx to appropriate PINS
 
void setupSoftAPC(void){
pinMode(pinSET, HIGH);
 
apc220.begin(9600);
}
 
void setSettings(void){
digitalWrite(pinSET, LOW); // pulling SET to low will put apc220 in config mode
delay(50); // stabilize please
apc220.println("WR 433900 4 9 3 0"); // ask for data
delay(50);
 
while (apc220.available()) {
    Serial.write(apc220.read());
    }
digitalWrite(pinSET, HIGH); // put apc220 back in operation
delay(200);
}

void getSettings(void) {
digitalWrite(pinSET, LOW); // pulling SET to low will put apc220 in config mode
delay(50); // stabilize please
apc220.println("RD"); // ask for data
delay(50);
 
while (apc220.available()) {
    Serial.write(apc220.read());
    }
digitalWrite(pinSET, HIGH); // put apc220 back in operation
delay(200);
}
 
void setup(){
Serial.begin(9600);
setupSoftAPC();
setSettings();
}
 
void loop(){
apc220.println("Hello World!");
delay(5000);
}
