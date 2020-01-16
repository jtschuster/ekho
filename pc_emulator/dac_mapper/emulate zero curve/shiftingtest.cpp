#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#define NUMPOINTS 65    // x axis (voltage) points
#define RESISTANCE 470 // 470 ohms on Ekho emulator board

long double VOLTAGES[NUMPOINTS]  = { 0, 0.16615384615, 0.33230769230, 0.49846153845, 0.66461538460, 0.83076923075, 0.99692307690, 1.16307692305, 1.32923076920, 1.49538461535, 1.66153846150, 1.82769230765, 1.99384615380, 2.15999999995, 2.32615384610, 2.49230769225, 2.65846153840, 2.82461538455, 2.99076923070, 3.15692307685, 3.32307692300, 3.48923076915, 3.65538461530, 3.82153846145, 3.98769230760, 4.15384615375, 4.31999999990, 4.48615384605, 4.65230769220, 4.81846153835, 4.98461538450, 5.15076923065, 5.31692307680, 5.48307692295, 5.64923076910, 5.81538461525, 5.98153846140, 6.14769230755, 6.31384615370, 6.47999999985, 6.64615384600, 6.81230769215, 6.97846153830, 7.14461538445, 7.31076923060, 7.47692307675, 7.64307692290, 7.80923076905, 7.97538461520, 8.14153846135, 8.30769230750, 8.47384615365, 8.63999999980, 8.80615384595, 8.97230769210, 9.13846153825, 9.30461538440, 9.47076923055, 9.63692307670, 9.80307692285, 9.96923076900, 10.13538461515, 10.30153846130, 10.46769230745, 10.63384615360};



uint16_t dacvalues[NUMPOINTS];
double dac_map[4097];
uint16_t interpolate_f(uint16_t _adc_14_value, uint16_t _shift_bits) ;
double dac_lookup(double dacvalue);
void loadmap(char mapfname[], double maparray[]);
int main(){
  printf("loading the DAC map\n");

  // Load DAC maps
  char dm_literal[] = "/Users/arwaalsubhi/ekho-main/code/emulator/Ekho_Emulato_revE/test/emulate zero curve/febprofile/dacmap.dat";
  loadmap(dm_literal, dac_map);

// double actual_voltage_digital = (412*3.3/4096)  ; // / 0.0019178 // 412 =  0.33201465201 volt , i=20=>3.32307692300 = 4123
// uint16_t VS = (uint16_t) actual_voltage_digital;
int16_t VS = 128;
for (int i =0; i<NUMPOINTS;i++)
{
// Convert to dac voltage value
long double temp = VOLTAGES[i];
// Use dacmap to convert we should play out with Teensy
uint16_t	dacValueV = dac_lookup(temp);
dacvalues[i] = dacValueV;
printf("[%d] = %d  %Lf \n,",i,dacvalues[i],VOLTAGES[i]);
} // end for
//printf("\n");
uint16_t temp = interpolate_f(VS, 6);
printf("interpolate value = %d \n",temp);
return 0;
}

uint16_t interpolate_f(uint16_t _adc_14_value, uint16_t _shift_bits)

{ //shifted by 6 why?
  // after getting the voltage value (Vs) shift it by 6 to get the integer value from the [0-64] range.
  //2^6 = 64 which is the number of points on x axis (voltage) on IV curve.
	uint16_t left = _adc_14_value >> _shift_bits;
  printf("i=%d Dac[i]=%d Dac[i-1]=%d Dac[i+1]=%d\n",left,dacvalues[left], dacvalues[left-1],dacvalues[left+1]);
	return	dacvalues[left] -
				((
					(uint32_t)(dacvalues[left] - dacvalues[left+1]) *
					(uint32_t)(_adc_14_value - (left << _shift_bits))
				) >> _shift_bits);
}


void loadmap(char mapfname[], double maparray[]) {
	// Load DAC map
	FILE * dacFile=fopen(mapfname, "r");
	char line[128];
	int ndx=0;
	while(fgets(line, 128, dacFile) != NULL) {
		maparray[ndx++] = atof(line);
	}
	// Edge case
	maparray[ndx]=maparray[ndx-1];
	fclose(dacFile);
}


double dac_lookup(double dacvalue) {
	int low = 0;
	int high = 4095;
	int mid = 0;

	// edge cases
	if(dacvalue < dac_map[0]) return 0;
	if(dacvalue > dac_map[4095]) return 4095;

	// binary search
	while (low <= high) {
		mid = (low + high) / 2;
		if(dac_map[mid] >= dacvalue){
			high = mid-1;
		} else {
			low = mid + 1;
		}
	}
	return low;
}
