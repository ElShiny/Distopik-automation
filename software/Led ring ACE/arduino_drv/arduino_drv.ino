/*
 Circuit:

 MOSI: pin 11
 MISO: pin 12
 SCK: pin 13
 SS: pin 7
 INT: pin 2

 by ElShiny
 */

#include <SPI.h>


// pins used for the connection with the sensor
// the other you need are controlled by the SPI library):

const int ssPin = 7;
const int intPin = 2;
int a;

#define F_CPU 16000000

void setup() {
  Serial.begin(115200);

  // start the SPI library:
  SPI.begin();

  // initialize the  data ready and chip select pins:
  pinMode(intPin, INPUT);
  pinMode(ssPin, OUTPUT);
  digitalWrite(ssPin, HIGH);
  

 SPI.setClockDivider(SPI_CLOCK_DIV16);


 attachInterrupt(digitalPinToInterrupt(2), readSpi, FALLING);


  // give the sensor time to set up:
  delay(100);
}

void loop() {

  while (Serial.available() > 0) {
    int a = Serial.parseInt();
    int b = Serial.parseInt();
   if (Serial.read() == '\n') {
     Serial.println(a, DEC);
     if(a == 0){
       writeSpiBuffer(21, 90, b);
     }
     else if(a != 0)writeByte(a);
     //writeSpiBuffer(a);
    }
  }
}

void writeByte(uint8_t data){
  digitalWrite(ssPin, LOW);
  SPI.transfer(data);
  digitalWrite(ssPin, HIGH);
}

void readSpi(void){

  digitalWrite(ssPin, LOW);
  a = SPI.transfer(0x00);
  digitalWrite(ssPin, HIGH);
  if(a == 0)return;
  Serial.print("INSTR: ");
  Serial.println(a, DEC);


  if(a < 10)readSpiByte();
  if(a >= 20 && a<100)readSpiBuffer();
  if(a == 253)readSpiByte();
  if(a == 254){readSpiBuffer();}
  if(a == 255)readSpiByte();
  if(a == 101){readSpiByte(); writeSpiBuffer(21, 90, 0);}

}

void readSpiByte(void){

  //_delay_us(10);
  digitalWrite(ssPin, LOW);
  int b = SPI.transfer(0x00);
  digitalWrite(ssPin, HIGH);

  Serial.print("DATA: ");
  Serial.println(b, DEC);
  Serial.println("END");
}

void readSpiBuffer(){

  digitalWrite(ssPin, LOW);
  int length = SPI.transfer(0x00);
  digitalWrite(ssPin, HIGH);

  Serial.print("LENGTH: ");
  Serial.println(length, DEC);


  Serial.print("DATA: ");
  for(int i = 0; i<length; i++){
    _delay_us(10);
    digitalWrite(ssPin, LOW);
    Serial.print(SPI.transfer(0x00), DEC);
    //SPI.transfer(0x00);
    Serial.print(", ");
    digitalWrite(ssPin, HIGH);    
  }
  Serial.println(" ");
  Serial.println("READ END");

}

void writeSpiBuffer(uint8_t instr, uint8_t length, uint8_t start){
  while(!digitalRead(2));

  if(length>90)length = 90;

  int ret = 0;

  digitalWrite(ssPin, LOW);
  ret = SPI.transfer(instr);
  digitalWrite(ssPin, HIGH);

  if(ret){Serial.println("err"); return;}

  _delay_us(10);
  digitalWrite(ssPin, LOW);
  ret = SPI.transfer(length);
  digitalWrite(ssPin, HIGH);

  if(ret){Serial.println("err"); return;}

  _delay_us(10);
  digitalWrite(ssPin, LOW);
  ret = SPI.transfer(start);
  digitalWrite(ssPin, HIGH);

  if(ret){Serial.println("err"); return;}


  uint8_t data[90] = {};

  for(int i = 0; i<90; i++){
    data[i] = i;
  }


  for(int i = 0; i<length; i++){
    _delay_us(1);
    digitalWrite(ssPin, LOW);
    //SPI.transfer(0xff);
    ret = SPI.transfer(data[i]);
    //SPI.transfer(0x00);
    digitalWrite(ssPin, HIGH);    
    if(ret){Serial.println("err"); return;}
  }
  Serial.println("WRITE END");
  //delay(1);
}
