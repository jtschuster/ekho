/*
	Make sure you have GSL installed (GNU Scientific Library)
	$ brew install Gsl
*/

// This makes PRIu64 work in printf
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <gsl/gsl_multifit.h>
#include <mach/mach.h>
#include <mach/mach_time.h>

using namespace std;

#define COUNT 500
#define NUMPOINTS 65
#define RESISTANCE 470



long double allcurves[NUMPOINTS];
int indexInAllCurves = 0;
int indexInBufCurves = 0;
uint64_t time_per_frame;
double dac_map[4097];
FILE *surface_inf, *DAC_volts_out;
char  DAC_volts_out_name[255];
long double VOLTAGES[NUMPOINTS]  = { 0, 0.12085189123076923, 0.24170378246153845,0.3625556736923077, 0.4834075649230769, 0.6042594561538461, 0.7251113473846154, 0.8459632386153846, 0.9668151298461538, 1.087667021076923, 1.2085189123076923, 1.3293708035384615, 1.4502226947692307, 1.571074586, 1.6919264772307692, 1.8127783684615384, 1.9336302596923076, 2.054482150923077, 2.175334042153846, 2.2961859333846153, 2.4170378246153845, 2.5378897158461537, 2.658741607076923, 2.779593498307692, 2.9004453895384614, 3.0212972807692307, 3.142149172, 3.263001063230769, 3.3838529544615383, 3.5047048456923076, 3.625556736923077, 3.746408628153846, 3.8672605193846152, 3.9881124106153845, 4.108964301846154, 4.229816193076923, 4.350668084307692, 4.471519975538461, 4.592371866769231, 4.713223758, 4.834075649230769, 4.954927540461538, 5.0757794316923075, 5.196631322923077, 5.317483214153846, 5.438335105384615, 5.559186996615384, 5.680038887846154, 5.800890779076923, 5.921742670307692, 6.042594561538461, 6.1634464527692305, 6.284298344, 6.405150235230769, 6.526002126461538, 6.646854017692307, 6.767705908923077, 6.888557800153846, 7.009409691384615, 7.130261582615384, 7.251113473846154, 7.371965365076923, 7.492817256307692, 7.613669147538461, 7.7345210387692305 };

uint16_t dacvalues[65];

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

int main(int argc, char **argv) {
	if (argc < 2) {
		fprintf(stderr, "Usage: ./emulate <surface file>\n");
		exit(0);
	}


	// Load file

	surface_inf=fopen(argv[1], "r");
	// strcpy(DAC_volts_out_name, argv[2]);
	// strcat (DAC_volts_out_name,".txt");
//  DAC_volts_out = fopen(DAC_volts_out_name, "w");
	// Load DAC maps
	char dm_literal[] = "febprofile/dacmap.dat";
	loadmap(dm_literal, dac_map);


	double timestamp= 0;


	int n;
	while(1)
	{
		n = fread(&timestamp, sizeof(double), 1, surface_inf);
    //	printf("\t%.9lf\t",timestamp);
		n = fread(&allcurves, sizeof(long double), NUMPOINTS, surface_inf);

		if(n < 1) {
			break;
		}


		// Get DAC values for newest curve to play out
		for(int i =0;i<NUMPOINTS;i++) {
	 	// Convert to dac voltage value
		long double dacValueV = ((RESISTANCE * allcurves[i] + VOLTAGES[i]));
		long double dacValue_Zero_curve = ((RESISTANCE * 0 + VOLTAGES[i]));
		// Use dacmap to convert to actual pre-amplified voltage we should play out with Teensy

    int temp = dac_lookup(dacValueV);
		int temp1 = dac_lookup(dacValue_Zero_curve);
		// if (VOLTAGES[i]-dacValueV != 0)
		//  {
     	 printf("%Lf, %Lf, %Lf, %d, %d, %Lf\n", dacValue_Zero_curve , dacValueV,VOLTAGES[i], temp,temp1, allcurves[i] );
    // 	}


		}


   printf("\n");

	}




	fclose(surface_inf);
	return 0;
}
