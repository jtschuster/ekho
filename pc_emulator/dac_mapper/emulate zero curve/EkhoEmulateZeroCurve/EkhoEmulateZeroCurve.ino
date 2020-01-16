#include <ADC.h>

ADC *adc = new ADC();
//uint16_t current_curve[65]={0,65,129,195,258,323,387,453,516,581,645,710,774,839,904,969,1033,1097,1162,1227,1291,1356,1421,1485,1549,1614,1679,1744,1807,1871,1936,2001,2063,2128,2192,2257,2321,2386,2452,2514,2579,2643,2709,2772,2838,2903,2967,3030,3094,3159,3225,3289,3353,3417,3483,3547,3612,3675,3741,3805,3870,3934,3998,4062,4095};
uint16_t current_curve1[65]={0,72,136,200,265,329,393,458,522,586,650,715,779,843,907,971,1036,1100,1164,1228,1292,1355,1419,1483,1547,1611,1675,1739,1804,1868,1932,1997,2061,2126,2190,2255,2319,2383,2448,2512,2577,2641,2706,2770,2834,2899,2963,3028,3092,3157,3221,3286,3350,3414,3479,3543,3608,3672,3737,3801,3865,3930,3994,4059,4123};
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
  Serial.print (" i =");
  Serial.print (left);
  Serial.print (" dac[i] = ");
  Serial.print (current_curve1[left]);
   Serial.print (" dac[i+1] = ");
  Serial.print (current_curve1[left+1]);
  Serial.println();
	return	current_curve1[left] -
				((
					(uint32_t)(current_curve1[left] - current_curve1[left+1]) * 
					(uint32_t)(_adc_14_value - (left << _shift_bits))
				) >> _shift_bits);
};

void setup() {
  Serial.begin(9600);
	pinMode(A2, INPUT); //
  pinMode(A21, OUTPUT);
	pinMode(statPin, OUTPUT);

	////// config ADC1 /////
//	adc->setAveraging(1, ADC_1); // set number of averages
//	adc->setResolution(12, ADC_1); // set bits of resolution
//	adc->setConversionSpeed(ADC_CONVERSION_SPEED::HIGH_SPEED, ADC_1); // change the conversion speed
//	adc->setSamplingSpeed(ADC_SAMPLING_SPEED::HIGH_SPEED, ADC_1); // change the sampling speed


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
	//voltage = 412;
	voltage = adc->analogRead(A2,ADC_0);
  double actual_voltage = voltage *0.0008058608; // voltage *(3.3/4095)
 Serial.print(actual_voltage);
 Serial.print(",");
//double actual_voltage_digital = actual_voltage / 0.002575398;
//Serial.print(actual_voltage_digital);
// Serial.print(",");
//uint16_t vs = (uint16_t)actual_voltage_digital;
	
	// Write to DAC
 //Serial.println(voltage);
 //delay(1);
// Serial.print(",");
Serial.print(interpolate_f(voltage,6));
//Serial.print("\n");

// uint16_t vs;
//
// if (voltage >15) 
// {
//  vs = voltage-15;
//  }
// else {
//  //vs = voltage/2;
//   vs = 0;
// }
 //analogWrite(A21, interpolate_f(voltage,6));
 //analogWrite(A21,voltage);
 //double vs_voltage = vs *0.0008058608; 

//delay(1);

}
