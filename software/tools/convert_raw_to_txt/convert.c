#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

//#define COUNT 500
#define COUNT 512
long double curvedata[2][COUNT];
int main(int argc, char **argv)
{
	double timestamp;
	FILE *rawdata, *rawdata_text;
	char outfile_name[255];
	if (argc <= 1)
	{
		fprintf(stderr, "Usage:   convert <raw_data_file> <output text file>\n");
		exit(0);
	}

	// Load file
	rawdata = fopen(argv[1], "r");
	if (argc > 2)
	{
		strcpy(outfile_name, argv[2]);
		strcat(outfile_name, ".txt");
	}
	else
	{
		strcpy(outfile_name, "rawData.txt");
	}
	printf("Saving to %s...\n", outfile_name);
	rawdata_text = fopen(outfile_name, "w");

	fprintf(rawdata_text, "Timestamp (s), Voltage(V), Current (A)\n"); // voltage, current

	int n;
	while (1)
	{
		n = fread(&timestamp, sizeof(double), 1, rawdata);
		// read from file all x points (voltages)
		n = fread(&curvedata[0], sizeof(long double), COUNT, rawdata);
		// read from file all y points (currents)
		n = fread(&curvedata[1], sizeof(long double), COUNT, rawdata);

		if (n < 1)
		{
			break;
		}
		else
		{
			for (int i = 0; i < COUNT; i++)
			{

				fprintf(rawdata_text, "%lf\t%.20Lf\t%.20Lf\n", timestamp, curvedata[0][i], curvedata[1][i]); // voltage, current
																											 //printf( "%Lf, %Lf\n",curvedata[0][i],curvedata[1][i]);
			}																								 //end for
			fprintf(rawdata_text, "\n");

		} //end else

	} //end while

	fclose(rawdata);
	fclose(rawdata_text);

	return 0;
} //end main
