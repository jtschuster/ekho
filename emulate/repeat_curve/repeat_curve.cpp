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
#define COUNT 500
#define NUMPOINTS 65




long double VOLTAGES[NUMPOINTS] = {0, 0.16615384615, 0.33230769230, 0.49846153845, 0.66461538460, 0.83076923075, 0.99692307690, 1.16307692305, 1.32923076920, 1.49538461535, 1.66153846150, 1.82769230765, 1.99384615380, 2.15999999995, 2.32615384610, 2.49230769225, 2.65846153840, 2.82461538455, 2.99076923070, 3.15692307685, 3.32307692300, 3.48923076915, 3.65538461530, 3.82153846145, 3.98769230760, 4.15384615375, 4.31999999990, 4.48615384605, 4.65230769220, 4.81846153835, 4.98461538450, 5.15076923065, 5.31692307680, 5.48307692295, 5.64923076910, 5.81538461525, 5.98153846140, 6.14769230755, 6.31384615370, 6.47999999985, 6.64615384600, 6.81230769215, 6.97846153830, 7.14461538445, 7.31076923060, 7.47692307675, 7.64307692290, 7.80923076905, 7.97538461520, 8.14153846135, 8.30769230750, 8.47384615365, 8.63999999980, 8.80615384595, 8.97230769210, 9.13846153825, 9.30461538440, 9.47076923055, 9.63692307670, 9.80307692285, 9.96923076900, 10.13538461515, 10.30153846130, 10.46769230745, 10.63384615360};
/*  Where are these values come from ?
   10.8 volts is max voltage range can be produced by the emulator board and we have 65 points on x axis (voltage).
	 so 10.8/65 = 0.16615384615L. to get all 65 points value on x axis, 0.16615384615 is added to get next point on the axis.
    long double v=0;
   for (int j=1;j<65;j++)
   {
       v+=0.16615384615L; // 10.8(max voltage range)/65points
   }
*/
double curve_time;
double timestamp;
char outfile_name[255];
FILE *surface_inf, *outfile;
long double curvepoints[NUMPOINTS];

int main(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "repeat_curve: creates a ten second IVS file from one curve of a recording.\n");
        fprintf(stderr, "\tFormat: ./repeat_curve <.ivs file> <time of curve> <output file>\n");
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
