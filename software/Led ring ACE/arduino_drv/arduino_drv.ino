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
uint8_t devs;

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

  devs = howManyDevs();
  Serial.print("DEVS: ");
  Serial.println(devs, DEC);

}

void loop() {

  while (Serial.available() > 0) {
    int a = Serial.parseInt();
    int b = Serial.parseInt();
   if (Serial.read() == '\n') {
     Serial.println(a, DEC);
     if(a == 256){
       writeSpiBuffer(21, 90, b);
     }
     else writeByte(a);
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
  uint8_t data[5];

  digitalWrite(ssPin, LOW);
  for(int i = 0; i<devs; i++){
  data[i] = SPI.transfer(0x00);
  }
  digitalWrite(ssPin, HIGH);
  
  for(int i = 0; i < devs; i++){
  if(data[i] != 0){
  Serial.print("INSTR: ");
  Serial.println(data[i], DEC);


  if(data[i] < 10)readSpiByte();
  if(data[i] >= 20 && data[i]<100)readSpiBuffer();
  if(data[i] == 253)readSpiByte();
  if(data[i] == 254){readSpiBuffer();}
  if(data[i] == 255)readSpiByte();
  if(data[i] == 101){readSpiByte(); writeSpiBuffer(21, 90, 0);}
  }}

}

void readSpiByte(void){
int b;
  //_delay_us(10);
  digitalWrite(ssPin, LOW);
  
  for(int i = 0; i < devs; i++){
   b = SPI.transfer(0x00);
  Serial.print("DATA: ");
  Serial.println(b, DEC);
  }

  digitalWrite(ssPin, HIGH);
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

uint8_t howManyDevs(void){

  uint8_t word = 0xab;
  int ret = 0;
  int i = 0;

  while(!digitalRead(2));

  digitalWrite(ssPin, LOW);
  ret = SPI.transfer(word);
  if(ret != 0){digitalWrite(ssPin, HIGH); return 0;}

  while(ret != word){
    ret = SPI.transfer(0x00);

    if(i > 30)break;
    i++;
  }
  digitalWrite(ssPin, HIGH);    

  return i;
}
