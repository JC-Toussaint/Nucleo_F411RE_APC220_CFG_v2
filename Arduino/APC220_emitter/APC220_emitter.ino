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
  
void setup(){
apc220.begin(9600);
digitalWrite(pinSET, HIGH); // put apc220 in operation
delay(200);

Serial.begin(9600);
}
 
void loop(){
apc220.println("iter : " + String(iter++));
//delay(10);
}
