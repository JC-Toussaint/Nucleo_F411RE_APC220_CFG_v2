#include <SoftwareSerial.h>
#include <string.h>

 /*
  Parameters      Bytes Instruction
  Frequency       6     Unit is KHz,for example 434MHz is 434000.
  Rf data rate    1     1,2,3 and 4 refer to 2400, 4800, 9600, 19200bps separetely.
  Output power    1     from 0 to 9, 9 means 13dBm(20mW).
  UART rate       1     0,1,2,3,4,5 and 6 refers to 1200, 2400, 4800, 9600, 19200, 38400, 57600bps separately.
  Series checkout 1     Series checkout：0 means no check,1 means even parity,2 means odd parity.
 */
 
const int pinRX = 12;  /* <-- TXD */
const int pinTX = 11;  /* --> RXD */
const int pinSET= 13;
int iter=0; 
SoftwareSerial apc220(pinRX, pinTX); // Crt softserial port and bind tx/rx to appropriate PINS
 
void setupSoftAPC(void){
pinMode(pinSET, HIGH);
 
apc220.begin(9600);
}
 
void setSettings(void){
digitalWrite(pinSET, LOW); // pulling SET to low will put apc220 in config mode
delay(50); // stabilize please
//apc220.println("WR 433900 4 9 3 0"); // ask for data
// RF 9600bps 20mW UART rate 38400bps
apc220.println("WR 433900 3 9 3 0"); // ask for data
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
apc220.println("iter : " + String(iter++));
delay(50);
#ifdef RECEIPT
  while (apc220.available() == 0) {}     //wait for data available
  String teststr = Serial.readString();  //read until timeout
#endif  
}
