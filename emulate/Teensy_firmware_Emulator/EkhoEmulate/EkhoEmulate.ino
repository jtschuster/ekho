#include <ADC.h>

ADC *adc = new ADC();
uint16_t current_curve[65];
const int statPin=2;
uint16_t voltage=0;
unsigned char buf[512];
uint16_t bcount = 0;
uint16_t i = 0;
byte response[2];
uint16_t interpolate_f(uint16_t _adc_14_value, uint16_t _shift_bits) 

{ //shifted by 6 why?
  // after getting the voltage value (Vs) shift it by 6 to get the integer value from the [0-64] range. 
  //2^6 = 64 which is the number of points on x axis (voltage) on IV curve. 
	uint16_t left = _adc_14_value >> _shift_bits;
	return	current_curve[left] -
				((
					(uint32_t)(current_curve[left] - current_curve[left+1]) * 
					(uint32_t)(_adc_14_value - (left << _shift_bits))
				) >> _shift_bits);
};

void setup() {
	pinMode(A2, INPUT); // VS_AMP
	pinMode(statPin, OUTPUT);//for debugging
  pinMode(A21, OUTPUT);


////// config ADC0 /////
  adc->setAveraging(1, ADC_0); // set number of averages
  adc->setResolution(12, ADC_0); // set bits of resolution
  adc->setConversionSpeed(ADC_CONVERSION_SPEED::HIGH_SPEED, ADC_0); // change the conversion speed
  adc->setSamplingSpeed(ADC_SAMPLING_SPEED::HIGH_SPEED, ADC_0); // change the sampling speed

	// 12-bit DAC
	analogWriteResolution(12); 

	delay(2000);
}

void loop() {
	// Calculate output value (TODO: dont use float math, use a table)
	voltage = adc->analogRead(A2,ADC_0);
	// Start reading in a new curve if necessary
 
	while(Serial.available()) {
		if(bcount == 0) {
			digitalWrite(statPin, LOW);
		}
		buf[bcount++] = Serial.read();
		if(bcount > 129) {
			digitalWrite(statPin, HIGH);
                        
			for(i=0;i<130;i+=2) {
				current_curve[i / 2] = (buf[i+1] << 8) + buf[i]; // Teensy3.6 is a big endian
			}
			bcount = 0; 
			break;
		}
	}

    
	// Write to DAC
 analogWrite(A21, interpolate_f(voltage,6));
 
}
