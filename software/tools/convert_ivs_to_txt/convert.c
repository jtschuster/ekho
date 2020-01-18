#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define NUMPOINTS 65

long double VOLTAGES[NUMPOINTS]  = { 0, 0.16615384615, 0.33230769230, 0.49846153845, 0.66461538460, 0.83076923075, 0.99692307690, 1.16307692305, 1.32923076920, 1.49538461535, 1.66153846150, 1.82769230765, 1.99384615380, 2.15999999995, 2.32615384610, 2.49230769225, 2.65846153840, 2.82461538455, 2.99076923070, 3.15692307685, 3.32307692300, 3.48923076915, 3.65538461530, 3.82153846145, 3.98769230760, 4.15384615375, 4.31999999990, 4.48615384605, 4.65230769220, 4.81846153835, 4.98461538450, 5.15076923065, 5.31692307680, 5.48307692295, 5.64923076910, 5.81538461525, 5.98153846140, 6.14769230755, 6.31384615370, 6.47999999985, 6.64615384600, 6.81230769215, 6.97846153830, 7.14461538445, 7.31076923060, 7.47692307675, 7.64307692290, 7.80923076905, 7.97538461520, 8.14153846135, 8.30769230750, 8.47384615365, 8.63999999980, 8.80615384595, 8.97230769210, 9.13846153825, 9.30461538440, 9.47076923055, 9.63692307670, 9.80307692285, 9.96923076900, 10.13538461515, 10.30153846130, 10.46769230745, 10.63384615360};

long double current[NUMPOINTS];
int main(int argc, char **argv) {
	double timestamp;
	FILE *IVSData,*IVSData_text;
	if (argc < 1) {
		fprintf(stderr, "Usage:   convert <IV_data_file> \n");
		exit(0);
	}


	// Load file
	IVSData=fopen(argv[1], "r");
	IVSData_text = fopen("IVSData.txt", "w");
	fprintf(IVSData_text, "voltage,current\n"); // voltage, current

	int n;
	while(1) {
	n = fread(&timestamp, sizeof(double), 1, IVSData);
	// read from file all currents
	n = fread(&current, sizeof(long double), NUMPOINTS, IVSData);


	if(n < 1) {
		break;
	}
else {
for (int i=0; i<NUMPOINTS; i++)
{

 fprintf(IVSData_text, "%lf\t%.20Lf\t%.20Lf\n",timestamp,VOLTAGES[i],current[i]); // voltage, current
}//end if


}//end else


}//end while

fclose(IVSData);
fclose(IVSData_text);

return 0;
}//end main
