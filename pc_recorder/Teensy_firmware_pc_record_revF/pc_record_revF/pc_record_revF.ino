
#include <ADC.h> // inculde ADC library
uint8_t smartLoad_pins[8]={5,6,7,8,9,10,11,12}; // define an array of 8 pins to randomally generate a smart load
ADC *adc = new ADC(); // adc object
uint16_t voltage, current_1,current_2,current_3, Vref;
byte response[12];

void setup() {
  //Serial.begin(115200); 
 
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
  
  //////ADC1 module////////////////
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
  

//  
 
  response[0] = (byte) current_1 & 0xff;
  response[1] = (byte) (current_1 >> 8) & 0xff;
  response[2] = (byte) current_2 & 0xff;
  response[3] = (byte) (current_2 >> 8) & 0xff;
  response[4] = (byte) current_3 & 0xff;
  response[5] = (byte) (current_3 >> 8) & 0xff;
  response[6] = (byte) voltage & 0xff;
  response[7] = (byte) (voltage >> 8) & 0xff;   
  response[8] = (byte) Vref & 0xff;
  response[9] = (byte) (Vref >> 8) & 0xff;   
  response[10] = 0xFF;
  response[11] = 0xFF;

Serial.write(response, 12);   // send the reading through serial port to the pc


delayMicroseconds(10); 
}
}

void set_smart_Load(uint8_t value) 
{ 
  
  for (byte i=0; i<8; i++) { // loop through 8 bits of recived value. check if it is one , drives the corresponding pin high. if it is 0 drives the corresponding pin low.
    byte state = bitRead(value, i);
    digitalWrite(smartLoad_pins[i], state);
  }
  
} 
