#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>


//////////////// header file to get elapsed time for diffrents OS
#if defined(_WIN32)
#include <Windows.h>
#elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
#include <unistd.h>   /* POSIX flags */
#include <time.h>     /* clock_gettime(), time() */
#include <sys/time.h> /* gethrtime(), gettimeofday() */
#if defined(__MACH__) && defined(__APPLE__)
#include <mach/mach.h>
#include <mach/mach_time.h>
#endif
#else
#error "Unable to define getRealTime( ) for an unknown OS."
#endif
/////////////////////////////////////////////////////////////////
#define COUNT 512
#define NUMPOINTS 65


double curve_time;
double timestamp;
char outfile_name[255];
FILE *surface_inf, *outfile;
long double curvepoints[NUMPOINTS];

int main(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "repeat_curve: creates a ten second IVS file from one curve of a recording.\n");
        fprintf(stderr, "\tFormat: ./repeat_curve <path_to_ivs_file> <time_of_curve> <output_file_name>\n");
        fprintf(stderr, "\tNote: time must be to at least 1 decimal place.");
        exit(0);
    }
    
    curve_time = strtod(argv[2], NULL);
    fprintf(stderr, "curve_time selected: %lf\n", curve_time);
    strcpy(outfile_name, argv[3]);
    strcat(outfile_name, ".ivs");
    surface_inf = fopen(argv[1], "r");
    int n;
    while (1)
	{
		n = fread(&timestamp, sizeof(double), 1, surface_inf);
		//printf("timestamp\t%.9lf\t\n",timestamp);
		n = fread(&curvepoints, sizeof(long double), NUMPOINTS, surface_inf);
		fprintf(stderr, "file read from, n= %d\n", n);

		if (n < 1)
		{
			fprintf(stderr, "No matching timestamp found!\n");
            fclose(surface_inf);
			break;
		}
        // Found the right curve. Make new ivs file
        double diff;
        diff = timestamp - curve_time;
        fprintf(stderr,"time diff: %lf\n", diff);
        if (diff < 0) {
            diff = -diff;
        }

        if (diff < 0.1) {
            fprintf(stderr, "curve found at time %lf. Writing to file %s.", timestamp, outfile_name );
            outfile = fopen(outfile_name, "w");
            double time = 0.0;
            for (; time < 10; time+=0.25) {
                fwrite(&time, sizeof(double), 1, outfile);
                fwrite(&curvepoints, sizeof(long double), NUMPOINTS, outfile);
            }
            fclose(outfile);
            fclose(surface_inf);
            exit(0);
        }
    }

}
