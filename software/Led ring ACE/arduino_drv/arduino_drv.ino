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
  Serial.begin(9600);

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

//  while (Serial.available() > 0) {
//    int a = Serial.parseInt();
//   if (Serial.read() == '\n') {
//     Serial.println(a, DEC);
//     writeByte(a);
//    }
//  }
  for(uint8_t i = 0; i<256; i+=2){
    delay(2);
    writeByte(15);
    delay(1);
    writeByte(i);
  
  }
  delay(1000);
}

void writeByte(uint8_t data){
  digitalWrite(ssPin, LOW);
  SPI.transfer(data);
  digitalWrite(ssPin, HIGH);
}

void readSpi(void){
  cli();
  digitalWrite(ssPin, LOW);
  _delay_us(50);
  a = SPI.transfer(0x00);
  digitalWrite(ssPin, HIGH);

  if(a == 0xcc){
    _delay_us(50);
    digitalWrite(ssPin, LOW);
    _delay_us(50);
    a = SPI.transfer(0x00);
    digitalWrite(ssPin, HIGH);
    //Serial.print("VAL: ");
    Serial.println(a, DEC);
  }
  sei();
}