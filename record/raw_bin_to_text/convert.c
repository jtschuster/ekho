#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define COUNT 500
long double curvedata[2][COUNT];
int main(int argc, char **argv) {
	double timestamp;
	FILE *rawdata,*rawdata_text;
	if (argc < 1) {
		fprintf(stderr, "Usage:   convert <raw_data_file> \n");
		exit(0);
	}


	// Load file
	rawdata=fopen(argv[1], "r");
	rawdata_text = fopen("rawData.txt", "w");
	// fprintf(rawdata_text, "voltage,current\n"); // voltage, current

	int n;
	double last_timestamp = 0.0;
	while(1) {
		n = fread(&timestamp, sizeof(double), 1, rawdata);
		// read from file all x points (voltages)
		n = fread(curvedata[0], sizeof(long double), COUNT, rawdata);
		// read from file all y points (currents)
		n = fread(curvedata[1], sizeof(long double), COUNT, rawdata);
		printf("%lf\n", timestamp);

		if(n < 1) {
			break;
		}
		else {
			for (int i=0; i<COUNT; i++)
		{

		fprintf(rawdata_text, "%.17lf\t%.17Lf\t%.17Lf\n", timestamp, curvedata[0][i], curvedata[1][i]); // voltage, current
			//printf( "%Lf, %Lf\n",curvedata[0][i],curvedata[1][i]);
		}//end for


		}//end else


	}//end while

fclose(rawdata);
fclose(rawdata_text);

return 0;
}//end main
