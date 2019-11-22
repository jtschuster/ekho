
#include <ADC.h> // inculde ADC library
#include <SD.h>
#include <SPI.h>
const int chipSelect = BUILTIN_SDCARD;
int timerFlage = 0, NumCurve =0;
uint8_t smartLoad_pins[8]={5,6,7,8,9,10,11,12}; // define an array of 8 pins to randomally generate a smart load
ADC *adc = new ADC(); // adc object
uint16_t voltage, current_1,current_2,current_3, Vref;
uint32_t timeStamp;
byte timeStampData[4];
int pointCount = 0 ;
int led = 13;
byte rawData[10];
unsigned long EndTimetMillis;
const long RecordTime = 10000; //10ms, 1800000= 30min, 60000 = 1 min , 600000= 10 mins// change to any valaue that you want in milisecond
 unsigned long StartTimetMillis;
 int StartFlag = 1;
 File dataFile;
 
void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);   
  if (!SD.begin(chipSelect)) 
  {
   // Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }

  else{
    
      digitalWrite(led, HIGH);
      dataFile = SD.open("rawData.raw", FILE_WRITE);
  }
  
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(12,OUTPUT);
 
  //Teensy has two ADC modules ADC0 and ADC1, So we need to congigure both modules since ADC pins use them.
  ////// ADC0 module/////
  adc->setAveraging(1, ADC_0); // set number of averages
  adc->setResolution(12, ADC_0); // set 12 bits of resolution for ADC0 module
  adc->setConversionSpeed(ADC_CONVERSION_SPEED::HIGH_SPEED,ADC_0); // change the conversion speed 
  adc->setSamplingSpeed(ADC_SAMPLING_SPEED::HIGH_SPEED,ADC_0); // change the sampling speed
  
//  //////ADC1 module////////////////
  adc->setAveraging(1, ADC_1);
  adc->setResolution(12, ADC_1); // set 12 bits of resolution for ADC1 module
  adc->setConversionSpeed(ADC_CONVERSION_SPEED::HIGH_SPEED,ADC_1 ); // change the conversion speed 
  adc->setSamplingSpeed(ADC_SAMPLING_SPEED::HIGH_SPEED,ADC_1 ); // change the sampling speed

 
}

void loop() {

for (int i=0; i<=255;i++)
{ 
set_smart_Load(i); // set the load by sending a random number from the range [0-255] to a "set_smart_Load" function, where 255 = maximum value can be represent by 8 bits

delayMicroseconds(550);        // pauses for 550 microseconds (swichs for smart load need max 500us to be on or off)
//////  // Get IV point

  current_3 = adc->analogRead(A20); // read a new value, will return ADC_ERROR_VALUE if the comparison is false.
  current_2 = adc->analogRead(A18); // read a new value, will return ADC_ERROR_VALUE if the comparison is false.
  current_1 = adc->analogRead(A16); 
  voltage = adc->analogRead(A6);
  Vref = adc->analogRead(A9);
  
  rawData[0] = (byte) current_1 & 0xff;
  rawData[1] = (byte) (current_1 >> 8) & 0xff;
  rawData[2] = (byte) current_2 & 0xff;
  rawData[3] = (byte) (current_2 >> 8) & 0xff;
  rawData[4] = (byte) current_3 & 0xff;
  rawData[5] = (byte) (current_3 >> 8) & 0xff;
  rawData[6] = (byte) voltage & 0xff;
  rawData[7] = (byte) (voltage >> 8) & 0xff;   
  rawData[8] = (byte) Vref & 0xff;
  rawData[9] = (byte) (Vref >> 8) & 0xff;  
  
  if (dataFile) 
  {
    if (StartFlag == 1)
    {
      StartTimetMillis = millis();
      StartFlag =0;
    }
  if (pointCount == 0)
     {
      timeStamp =  millis(); //record the start time of each curve
      timeStampData[3] = (byte) (( timeStamp & 0xFF000000) >> 24 );
      timeStampData[2] = (byte) (( timeStamp & 0x00FF0000) >> 16 );
      timeStampData[1] = (byte) (( timeStamp & 0x0000FF00) >> 8  );
      timeStampData[0] = (byte) (( timeStamp & 0X000000FF)       );
      dataFile.write(timeStampData, 4);
    }
    if (pointCount == 4990)
  {
       EndTimetMillis =  millis();
      timeStampData[3] = (byte) (( EndTimetMillis & 0xFF000000) >> 24 );
      timeStampData[2] = (byte) (( EndTimetMillis & 0x00FF0000) >> 16 );
      timeStampData[1] = (byte) (( EndTimetMillis & 0x0000FF00) >> 8  );
      timeStampData[0] = (byte) (( EndTimetMillis & 0X000000FF)       );
      dataFile.write(timeStampData, 4);

  }

    
    pointCount += dataFile.write(rawData, 10);
    
  } 
  
  if (pointCount == 5000)
  {
    EndTimetMillis =  millis();
    pointCount = 0;
    NumCurve++;
     if (EndTimetMillis-StartTimetMillis >= RecordTime)
     {
          
        dataFile.close();
        digitalWrite(led, LOW);
//        Serial.print("Number of recorded curve = ");
//         Serial.print(NumCurve);
//          Serial.print("\n");
//          Serial.print("StartTimetMillis = ");
//          Serial.print(StartTimetMillis);
//           Serial.print("\n");
//           Serial.print("EndTimetMillis = ");
//           Serial.print(EndTimetMillis);
//         Serial.print("\n");
//          Serial.print(EndTimetMillis-StartTimetMillis);
        exit(0); // stop the program
      }
  }

 



}
}

void set_smart_Load(uint8_t value) 
{ 
  
  for (byte i=0; i<8; i++) { // loop through 8 bits of recived value. check if it is one , drives the corresponding pin high. if it is 0 drives the corresponding pin low.
    byte state = bitRead(value, i);
    digitalWrite(smartLoad_pins[i], state);
  }
  
} 
