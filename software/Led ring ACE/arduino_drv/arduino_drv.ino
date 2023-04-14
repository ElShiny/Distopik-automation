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
   if (Serial.read() == '\n') {
     Serial.println(a, DEC);
     //writeByte(a);
     writeSpiBuffer(a);
    }
  }
//  for(uint8_t i = 0; i<256; i+=2){
//    delay(2);
//    writeByte(15);
//    delay(1);
//    writeByte(i);
//  
//  }
//  delay(1000);
}

void writeByte(uint8_t data){
  digitalWrite(ssPin, LOW);
  SPI.transfer(data);
  digitalWrite(ssPin, HIGH);
}

void readSpi(void){
  cli();

  digitalWrite(ssPin, LOW);
  a = SPI.transfer(0x00);
  digitalWrite(ssPin, HIGH);

  Serial.print("INSTR: ");
  Serial.println(a, DEC);

  if(a == 10)readSpiByte();
  if(a == 20)readSpiBuffer();

  sei();
}

void readSpiByte(void){
  cli();

  _delay_us(10);
  digitalWrite(ssPin, LOW);
  int b = SPI.transfer(0x00);
  digitalWrite(ssPin, HIGH);

  Serial.print("DATA: ");
  Serial.println(b, DEC);

  sei();
}

void readSpiBuffer(){
  cli();

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
  sei();
}

void writeSpiBuffer(uint8_t length){
  while(!digitalRead(2));
  cli();

  digitalWrite(ssPin, LOW);
  SPI.transfer(15);
  digitalWrite(ssPin, HIGH);

  _delay_us(10);
  digitalWrite(ssPin, LOW);
  SPI.transfer(length);
  digitalWrite(ssPin, HIGH);

  _delay_us(10);
  digitalWrite(ssPin, LOW);
  SPI.transfer(0);
  digitalWrite(ssPin, HIGH);


  uint8_t data[90] = {};
  for(int i = 0; i<length; i++){
    data[i] = i;
  }


  for(int i = 0; i<length; i++){
    _delay_us(10);
    digitalWrite(ssPin, LOW);
    SPI.transfer(data[i]);
    //SPI.transfer(0x00);
    digitalWrite(ssPin, HIGH);    
  }
  delay(1);
  sei();
}