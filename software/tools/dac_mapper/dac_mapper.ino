#include <ADC.h>
#include <SD.h>
#include <SPI.h>

const int chipSelect = BUILTIN_SDCARD;
unsigned long StartTimetMillis;
ADC *adc = new ADC();
int16_t VADC[1000];
int16_t VSAMP[1000];
int led = 13;
uint16_t i;
uint16_t DAC = 0;
double avg1=0, avg2=0;
char file[255];
File dataFile; 

void setup() {
	Serial.begin(9600);
  StartTimetMillis=millis();
	pinMode(A2, INPUT); //VS_amp
  pinMode(A21, OUTPUT);
  pinMode(led, OUTPUT);   

	////// config ADC1 /////
	adc->setAveraging(1, ADC_1); // set number of averages
	adc->setResolution(12, ADC_1); // set bits of resolution
	adc->setConversionSpeed(ADC_CONVERSION_SPEED::HIGH_SPEED, ADC_1); // change the conversion speed
	adc->setSamplingSpeed(ADC_SAMPLING_SPEED::HIGH_SPEED, ADC_1); // change the sampling speed


////// config ADC0 /////
	adc->setAveraging(1, ADC_0); // set number of averages
	adc->setResolution(12, ADC_0); // set bits of resolution
	adc->setConversionSpeed(ADC_CONVERSION_SPEED::HIGH_SPEED, ADC_0); // change the conversion speed
	adc->setSamplingSpeed(ADC_SAMPLING_SPEED::HIGH_SPEED, ADC_0); // change the sampling speed

	// 12-bit DAC
	analogWriteResolution(12); 

	 if (!SD.begin(chipSelect)) 
  {
   // Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  else{
    
      digitalWrite(led, HIGH);
      
      dataFile = SD.open("dacmap.txt", FILE_WRITE);
  }

}

void loop() {

analogWrite(A21,DAC); 
//analogWrite(A21,4095); 

for (i =0;i<1000;i++)
{
      VSAMP[i] =  adc->analogRead(A2);
}

for (i =0;i<1000;i++)
{
  avg1 += VSAMP[i];
}

avg1/=1000;

dataFile.print(String(DAC));
dataFile.print(",");    
dataFile.print(String(avg1));  

//}
//
//dataFile.print(String(000));
//dataFile.print(","); 
//for (i =0;i<1000;i++)
//{
 
//dataFile.print(String(VSAMP[i])); 
//if (i!=999)
//{
//dataFile.print(","); 
//} 
//}
dataFile.println(); 


DAC++;  


//
 if (DAC== 4096)
{
   dataFile.close();
   digitalWrite(led, LOW);
   exit(0);
 }
 
}
