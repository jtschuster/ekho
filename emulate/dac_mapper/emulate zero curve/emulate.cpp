

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>


//////////////// header file to get elapsed time for diffrents OS
// #if defined(_WIN32)
// #include <Windows.h>
// #elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
// #include <unistd.h>	/* POSIX flags */
// #include <time.h>	/* clock_gettime(), time() */
// #include <sys/time.h>	/* gethrtime(), gettimeofday() */
// #if defined(__MACH__) && defined(__APPLE__)
// #include <mach/mach.h>
// #include <mach/mach_time.h>
// #endif
// #else
// #error "Unable to define getRealTime( ) for an unknown OS."
// #endif
/////////////////////////////////////////////////////////////////



#define COUNT 500      // number of points per curve
#define NUMPOINTS 65    // x axis (voltage) points
#define RESISTANCE 470 // 470 ohms on Ekho emulator board

unsigned char buf[8];
int port;
long double allcurves[NUMPOINTS];
double dac_map[4097];
FILE *surface_inf;
double getRealTime(void);
int n;
long double VOLTAGES[NUMPOINTS]  = { 0, 0.16615384615, 0.33230769230, 0.49846153845, 0.66461538460, 0.83076923075, 0.99692307690, 1.16307692305, 1.32923076920, 1.49538461535, 1.66153846150, 1.82769230765, 1.99384615380, 2.15999999995, 2.32615384610, 2.49230769225, 2.65846153840, 2.82461538455, 2.99076923070, 3.15692307685, 3.32307692300, 3.48923076915, 3.65538461530, 3.82153846145, 3.98769230760, 4.15384615375, 4.31999999990, 4.48615384605, 4.65230769220, 4.81846153835, 4.98461538450, 5.15076923065, 5.31692307680, 5.48307692295, 5.64923076910, 5.81538461525, 5.98153846140, 6.14769230755, 6.31384615370, 6.47999999985, 6.64615384600, 6.81230769215, 6.97846153830, 7.14461538445, 7.31076923060, 7.47692307675, 7.64307692290, 7.80923076905, 7.97538461520, 8.14153846135, 8.30769230750, 8.47384615365, 8.63999999980, 8.80615384595, 8.97230769210, 9.13846153825, 9.30461538440, 9.47076923055, 9.63692307670, 9.80307692285, 9.96923076900, 10.13538461515, 10.30153846130, 10.46769230745, 10.63384615360};
/*  Where are these values come from ?
   10.8 volts is max voltage range can be produced by the emulator board and we have 65 points on x axis (voltage).
	 so 10.8/65 = 0.16615384615L. to get all 65 points value on x axis, 0.16615384615 is added to get next point on the axis.
    long double v=0;
   for (int j=1;j<65;j++)
   {
       v+=0.16615384615L; // 10.8(max voltage range)/65points
   }


*/

uint16_t dacvalues[NUMPOINTS];
uint8_t DACvalues[NUMPOINTS*2];

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


// Serial init and configuration
void init_serial(int argc, char **argv) {
	struct termios settings;

	// Open the serial port
	port = open(argv[1], O_RDWR);
	if (port < 0) {
		fprintf(stderr, "Unable to open %s\n", argv[1]);
		exit(0);
	}

	// Configure the port
	tcgetattr(port, &settings);
	cfmakeraw(&settings);
	tcsetattr(port, TCSANOW, &settings);
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
	if (argc < 1) {
		fprintf(stderr, "Usage:   ./emulate <emulate port> \n");
		exit(0);
	}
	//Sleep for a second
	usleep(1000000);

	// Load file

	//surface_inf=fopen(argv[2], "r");

	// Load DAC maps
	char dm_literal[] = "febprofile/dacmap.dat";
	loadmap(dm_literal, dac_map);

	 //Now init serial to emulation teensy
	init_serial(argc, argv);
	double timestamp = 0;
	double last_timestamp=0.0;
	double start;
	double end;
	double elapsed;
	double elapsedNano;
	double timestampNano;


	// start = getRealTime( );
	// int n;
	// while(1) {
	// 	n = fread(&timestamp, sizeof(double), 1, surface_inf);
  //   	//printf("timestamp\t%.9lf\t\n",timestamp);
	// 	n = fread(&allcurves, sizeof(long double), NUMPOINTS, surface_inf);
	//
	// 	if(n < 1) {
	// 		break;
	// 	}
	// 	timestampNano = (timestamp - last_timestamp) * 1000000; //timpestamp is stored in millisec
	// 	last_timestamp = timestamp;

		// Get DAC values for newest curve to play out

			for (int i =0; i<NUMPOINTS;i++)
	{
	 	// Convert to dac voltage value
		long double temp = ((RESISTANCE * 0 + VOLTAGES[i]));
		// Use dacmap to convert we should play out with Teensy
	  uint16_t	dacValueV = dac_lookup(temp);
		dacvalues[i] = dacValueV;
     printf("%d,%Lf\n",dacvalues[i],VOLTAGES[i]);
	} // end for



	// Send curve to teensy
		n = write(port, dacvalues, NUMPOINTS * 2);
		printf("sent=%d\n", n);
		if (n < NUMPOINTS * 2) {
			fprintf(stderr, "error writing dac curve to port\n");
			exit(0);
		}



		// Busy wait here for specified timestamp amount then start next curve
		// TODO: Speed improvement: preload curve while sleeping
	// 	start = getRealTime( );
	// 	while(1) {
	// 		usleep(1);
	// 		end = getRealTime( );
	// 		elapsed = end - start;
	// 		elapsedNano = elapsed * 1000000000;
	// 		if(elapsedNano > timestampNano)
	// 		{  	printf("elapsedNano: %f\t timestampNano :%f   ,start : %f   end: %f \n",elapsedNano ,timestampNano, start, end);
	// 			break;
	// 		}
	// 	}
	// }

  // uint16_t Zero_curve[NUMPOINTS]= {0,59,106,153,200,246,293,340,387,433,481,527,574,621,667,714,761,807,854,900,947,993,1039,1086,1134,1180,1226,1273,1321,1367,1414,1462,1508,1555,1602,1649,1695,1743,1789,1836,1883,1930,1977,2024,2070,2117,2164,2211,2258,2305,2352,2399,2446,2493,2539,2586,2634,2680,2727,2775,2821,2868,2915,2962,3009};
	// n = write(port, Zero_curve, NUMPOINTS * 2);
	// if (n < NUMPOINTS * 2) {
	// 	fprintf(stderr, "error writing dac curve to port\n");
	// 	exit(0);
	// }

	//Right after we are done send a completely zeroed out surface
		// for(int i =0;i<NUMPOINTS;i++) {
		// 	dacvalues[i] = 0;
		// }
		// // Send curve to teensy
		// n = write(port, dacvalues, NUMPOINTS * 2);
		// if (n < NUMPOINTS * 2) {
		// 	fprintf(stderr, "error writing dac curve to port\n");
		// 	exit(0);
		// }

	//printf("Ended. Send zero curve\n");

	//fclose(surface_inf);
close(port);
	return 0;
}

/*
This function returns the real time, in seconds, or -1.0 if an error occurred.
 *
 * Time is measured since an arbitrary and OS-dependent start time.
 * The returned real time is only useful for computing an elapsed time
 * between two calls to this function.
  * Author:  David Robert Nadeau
  * Site:    http://NadeauSoftware.com/
  * License: Creative Commons Attribution 3.0 Unported License
  *     http://creativecommons.org/licenses/by/3.0/deed.en_US
 */
// double getRealTime( )
// {
// #if defined(_WIN32)
// 	FILETIME tm;
// 	ULONGLONG t;
// #if defined(NTDDI_WIN8) && NTDDI_VERSION >= NTDDI_WIN8
// 	/* Windows 8, Windows Server 2012 and later. ---------------- */
// 	GetSystemTimePreciseAsFileTime( &tm );
// #else
// 	/* Windows 2000 and later. ---------------------------------- */
// 	GetSystemTimeAsFileTime( &tm );
// #endif
// 	t = ((ULONGLONG)tm.dwHighDateTime << 32) | (ULONGLONG)tm.dwLowDateTime;
// 	return (double)t / 10000000.0;
//
// #elif (defined(__hpux) || defined(hpux)) || ((defined(__sun__) || defined(__sun) || defined(sun)) && (defined(__SVR4) || defined(__svr4__)))
// 	/* HP-UX, Solaris. ------------------------------------------ */
// 	return (double)gethrtime( ) / 1000000000.0;
//
// #elif defined(__MACH__) && defined(__APPLE__)
// 	/* OSX. ----------------------------------------------------- */
// 	static double timeConvert = 0.0;
// 	if ( timeConvert == 0.0 )
// 	{
// 		mach_timebase_info_data_t timeBase;
// 		(void)mach_timebase_info( &timeBase );
// 		timeConvert = (double)timeBase.numer /
// 			(double)timeBase.denom /
// 			1000000000.0;
// 	}
// 	//printf("timeconvert %f\n", timeConvert);
// 	return (double)mach_absolute_time( ) * timeConvert;
//
// #elif defined(_POSIX_VERSION)
// 	/* POSIX. --------------------------------------------------- */
// #if defined(_POSIX_TIMERS) && (_POSIX_TIMERS > 0)
// 	{
// 		struct timespec ts;
// #if defined(CLOCK_MONOTONIC_PRECISE)
// 		/* BSD. --------------------------------------------- */
// 		const clockid_t id = CLOCK_MONOTONIC_PRECISE;
// #elif defined(CLOCK_MONOTONIC_RAW)
// 		/* Linux. ------------------------------------------- */
// 		const clockid_t id = CLOCK_MONOTONIC_RAW;
// #elif defined(CLOCK_HIGHRES)
// 		/* Solaris. ----------------------------------------- */
// 		const clockid_t id = CLOCK_HIGHRES;
// #elif defined(CLOCK_MONOTONIC)
// 		/* AIX, BSD, Linux, POSIX, Solaris. ----------------- */
// 		const clockid_t id = CLOCK_MONOTONIC;
// #elif defined(CLOCK_REALTIME)
// 		/* AIX, BSD, HP-UX, Linux, POSIX. ------------------- */
// 		const clockid_t id = CLOCK_REALTIME;
// #else
// 		const clockid_t id = (clockid_t)-1;	/* Unknown. */
// #endif /* CLOCK_* */
// 		if ( id != (clockid_t)-1 && clock_gettime( id, &ts ) != -1 )
// 			return (double)ts.tv_sec +
// 				(double)ts.tv_nsec / 1000000000.0;
// 		/* Fall thru. */
// 	}
// #endif /* _POSIX_TIMERS */
//
// 	/* AIX, BSD, Cygwin, HP-UX, Linux, OSX, POSIX, Solaris. ----- */
// 	struct timeval tm;
// 	gettimeofday( &tm, NULL );
// 	return (double)tm.tv_sec + (double)tm.tv_usec / 1000000.0;
// #else
// 	return -1.0;		/* Failed. */
// #endif
// }
