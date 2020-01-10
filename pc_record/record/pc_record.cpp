////////////////////////////////////
//// FROM REVF
/*
This code written by Joiah Hester and Modified by Arwa Alsubhi and Jackson Schuster.
To run this code make sure you have GSL (GNU Scientific Library) installed in your machine
$ brew install Gsl
Two files will be generated by executing this code: *.ivs (has all IV curves) and *.raw (has raw data (voltage,current) pair)
*/
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
#include <gsl/gsl_multifit.h>

//////////////// header file to get elapsed time for diffrents OS
#if defined(_WIN32)
#include <Windows.h>
#elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
#include <unistd.h>	/* POSIX flags */
#include <time.h>	/* clock_gettime(), time() */
#include <sys/time.h>	/* gethrtime(), gettimeofday() */
#if defined(__MACH__) && defined(__APPLE__)
#include <mach/mach.h>
#include <mach/mach_time.h>
#endif
#else
#error "Unable to define getRealTime( ) for an unknown OS."
#endif
#define COUNT 500
#define NUMPOINTS 65
#define DEGREE 5

int tempint;
int numcurves = 0;
double record_time_ms = 0.0;
long double coeff[DEGREE];
long double allcurves[NUMPOINTS];
long double curvedata[2][COUNT];
char output_file_name[255];
char rawpair_file_name[255];

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

void create_curves_with_regression(void);
struct current {
int raw_value;
int gain;
};

struct current get_current(int current1,int current2,int current3);

#define READSIZE 12             // read 12 bytes from serial ports
#define BUFSIZE 1024
#define SENSE_Resistor 10      // 10 Ohms sense resistor on Ekho board
#define ADC_to_Voltage 0.0008056640625L  // = 3.3/2^12
#define Current1_gain 11     //the none-inverting opamplifier gain for current 1 on Ehko board
#define Current2_gain 121   // the none-inverting opamplifier gain for current 2 on Ehko board
#define Current3_gain 1331  // the none-inverting opamplifier gain for current 3 on Ehko board
#define Voltage_gain 2      // the none-inverting opamplifier gain for voltage on Ehko board
#define VREF_ADC 1118

#define SATURATION_THRESHOLD 4030
#define NUMPOINTS 65

int port;
int count =0;
double getRealTime(void);

///////////////////////////////////
//// FROM EKHO-NEW
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
double getRealTime( )
{
#if defined(_WIN32)
	FILETIME tm;
	ULONGLONG t;
#if defined(NTDDI_WIN8) && NTDDI_VERSION >= NTDDI_WIN8
	/* Windows 8, Windows Server 2012 and later. ---------------- */
	GetSystemTimePreciseAsFileTime( &tm );
#else
	/* Windows 2000 and later. ---------------------------------- */
	GetSystemTimeAsFileTime( &tm );
#endif
	t = ((ULONGLONG)tm.dwHighDateTime << 32) | (ULONGLONG)tm.dwLowDateTime;
	return (double)t / 10000000.0;

#elif (defined(__hpux) || defined(hpux)) || ((defined(__sun__) || defined(__sun) || defined(sun)) && (defined(__SVR4) || defined(__svr4__)))
	/* HP-UX, Solaris. ------------------------------------------ */
	return (double)gethrtime( ) / 1000000000.0;

#elif defined(__MACH__) && defined(__APPLE__)
	/* OSX. ----------------------------------------------------- */
	static double timeConvert = 0.0;
	if ( timeConvert == 0.0 )
	{
		mach_timebase_info_data_t timeBase;
		(void)mach_timebase_info( &timeBase );
		timeConvert = (double)timeBase.numer /
			(double)timeBase.denom /
			1000000000.0;
	}
	return (double)mach_absolute_time( ) * timeConvert;

#elif defined(_POSIX_VERSION)
	/* POSIX. --------------------------------------------------- */
#if defined(_POSIX_TIMERS) && (_POSIX_TIMERS > 0)
	{
		struct timespec ts;
#if defined(CLOCK_MONOTONIC_PRECISE)
		/* BSD. --------------------------------------------- */
		const clockid_t id = CLOCK_MONOTONIC_PRECISE;
#elif defined(CLOCK_MONOTONIC_RAW)
		/* Linux. ------------------------------------------- */
		const clockid_t id = CLOCK_MONOTONIC_RAW;
#elif defined(CLOCK_HIGHRES)
		/* Solaris. ----------------------------------------- */
		const clockid_t id = CLOCK_HIGHRES;
#elif defined(CLOCK_MONOTONIC)
		/* AIX, BSD, Linux, POSIX, Solaris. ----------------- */
		const clockid_t id = CLOCK_MONOTONIC;
#elif defined(CLOCK_REALTIME)
		/* AIX, BSD, HP-UX, Linux, POSIX. ------------------- */
		const clockid_t id = CLOCK_REALTIME;
#else
		const clockid_t id = (clockid_t)-1;	/* Unknown. */
#endif /* CLOCK_* */
		if ( id != (clockid_t)-1 && clock_gettime( id, &ts ) != -1 )
			return (double)ts.tv_sec +
				(double)ts.tv_nsec / 1000000000.0;
		/* Fall thru. */
	}
#endif /* _POSIX_TIMERS */

	/* AIX, BSD, Cygwin, HP-UX, Linux, OSX, POSIX, Solaris. ----- */
	struct timeval tm;
	gettimeofday( &tm, NULL );
	return (double)tm.tv_sec + (double)tm.tv_usec / 1000000.0;
#else
	return -1.0;		/* Failed. */
#endif
}
/////////////////////////////////////////////
//// END EKHO-NEW

////
double GLOBAL_START;
////

FILE *surface_outf, *rawpair_file;

void idle_impl()
{
	int n=0;
	int points = 0;
	double timestamp = 0.0;
	double start;
	double end;
	double elapsed;
	unsigned char buf[BUFSIZE];
	int buffered_bytes = 0;
	double idle_start;
	start = getRealTime( );
	idle_start = start;
	memset(buf, 0, sizeof(unsigned char) * BUFSIZE);

	while (1) {
			n = read(port, buf+buffered_bytes, READSIZE); // read from port
			buffered_bytes += n;

			if (buffered_bytes >= BUFSIZE-READSIZE) {
				printf("Error! Buffer filled up. Dumping %d bytes.\n",buffered_bytes);
				buffered_bytes = 0;
				memset(buf, 0, sizeof(unsigned char) * BUFSIZE);
				continue;
			}
			if (buffered_bytes >= READSIZE) {
				//print out buffer status
				// printf("[%d]\t",buffered_bytes);
				// for (int i=0; i < buffered_bytes; i++) {
				// 	printf(" %x",buf[i]);
				// }
				// printf("\n");

				//verify that the buffer has a complete chunk of data
				//unsigned int check = ((buf[11] << 8) + buf[10]);
				if (buf[10] != 0xFF || buf[11] != 0xFF) {
					//look for an ffff and reset.
					int found = 0;
					for (int i=0; i < buffered_bytes-1; i++) {
						if (buf[i] == 0xff && buf[i+1] == 0xff) {
							//reset the buffer
							printf("error, lost some bytes shifting by %d\n",i+2);
							memmove(buf, buf+i+2,BUFSIZE-buffered_bytes);
							buffered_bytes -= (i+2);
							found = 1;
							break;
						} //if
					} //for
					if (!found) continue;
					if (buffered_bytes < READSIZE) continue;
				}
			} else {
        continue;
        }


		//////get raw data////////////
		int raw_current_1 = (((buf[1]) << 8) + buf[0]) ;
		int raw_current_2 = (((buf[3]) << 8) + buf[2]) ;
		int raw_current_3 = (((buf[5]) << 8) + buf[4]) ;
		int raw_voltage = (((buf[7]) << 8) + buf[6]);
		int raw_vref     = (((buf[9]) << 8) + buf[8]);

		memmove(buf, buf+READSIZE, BUFSIZE-READSIZE);
		if(buffered_bytes < READSIZE) {
			//printf("AAAH! (%d)\n",buffered_bytes);
			exit(1);
		}
		buffered_bytes -= READSIZE;

		//get the higher amplified unsaturated current and its gain
		struct current Current= get_current(raw_current_1,raw_current_2,raw_current_3);

      	// 1- convert ADC readings to voltages by multiplying by 3.3/2^12
		long double current = Current.raw_value * ADC_to_Voltage;
		long double voltage   = raw_voltage *  ADC_to_Voltage;
		long double vref      = raw_vref * ADC_to_Voltage;

		///////////////////////////////////////////////
		// 2- subtract vref from  current value and  voltage
		current = current - vref;
		voltage = vref - (voltage/Voltage_gain);
    	// 3- get current values (I)by applying Ohm's law I= V/R , where R = 10 Ohms on Ekho board and V is the voltage values stored in current variable
		current = current /SENSE_Resistor;

		voltage = voltage * 51.0L  ; // 51 voltage division factor on Ekho board, 51 = (R1+R2)/R1 = (200K+10M)/200K = (200000.0+10000000.0))/200000.0

    	// 4- divide current by the current gain value
        current = current / Current.gain;

			// check for edge cases
		if (voltage<0){
			voltage=0;
		}
        if (current<0){
			current=0;
		}
        // store the values in curvedata array
		curvedata[0][points] = voltage;
		curvedata[1][points] = current;

		//	 printf("%Lf,%Lf\n",current,voltage);


		if (points++ == COUNT - 1) {
			// Figure out how many seconds elapsed for this curve
			///////////////////////////////////////////
			end = getRealTime( );
			elapsed = end - start; // Time elapsed in second
			//	printf ("start : %f  end: %f \n",start,end);
			// Pipe timestamp to both files
			timestamp +=  end - GLOBAL_START;
		printf ("timestamp : %f \n", timestamp);
		fwrite(&timestamp, sizeof(double), 1, surface_outf);
			fwrite(&timestamp, sizeof(double), 1, rawpair_file);
			// Write to file all x points (voltages)
		fwrite(&curvedata[0], sizeof(long double), COUNT, rawpair_file);
			// Write to file all y points (currents)
		fwrite(&curvedata[1], sizeof(long double), COUNT, rawpair_file);
			// Write Voltages and currents at once:
			// fwrite(&curvedata, sizeof(long double), COUNT*2, rawpair_file);

create_curves_with_regression();
			// Pipe curve output to file
fwrite(&allcurves, sizeof(long double), NUMPOINTS, surface_outf);
			if(timestamp > record_time_ms) {
				printf("=======\nIV-surface %s created!\n Recorded %d curves for %lf seconds to output file %s \n=======\n", output_file_name, numcurves+1, timestamp, rawpair_file_name);
				fclose(rawpair_file);
				fclose(surface_outf);
				close(port);
				exit(1);
				break;
			}
			// Prepare for next curve
			points = 0;
			numcurves++;
			// start = getRealTime( );
			printf("Should break out of while \n");
			break;
		}



		//fprintf(outfile1,"%d\t%d\t%d\t%d\t%d\n",raw_current_1, raw_current_2, raw_current_3,raw_voltage,raw_vref); //
		//fprintf(outfile2, "%.9Le\t%.9Le\t%.9Le\t%.9Le\t%.9Le\n",current_1, current_2, current_3,voltage,vref);
		//fprintf(outfile2, "%.9Le\t%.9Le\t%.9lf\t",current,voltage, timestamp);
   	//fprintf(outfile2, "%Lf,%Lf\n",current,voltage); //timestamp
		//printf("%.9Le\t%d\n",current, Current.gain); //

		//fprintf(outfile, "%.9lf	%.9lf	%.9lf %.9lf \n", current_1, current_2, current_3,voltage); //, timestamp
	}//end while
	double idle_end = getRealTime();
	double diff = idle_end - idle_start;
	printf("Idle_impl took %lf \n", diff);
}// end idle_impl

////////////////////////////////////////////////////////
struct current get_current(int current1,int current2,int current3)
{   // this function start with the reading from 1331 gain opamlifier and check if it is not saturated return it and its coressponding gain
	//otherwise check the reading from 121 gain opamplifier if saturated go to ckeck the reading from 11 gain opamlifier.
	struct current c ;
	if (current3<SATURATION_THRESHOLD ) //&& current3>= VREF_ADC
	{
c.raw_value = current3;
		c.gain = 1331 ;
		return c;
	}
	if (current2<SATURATION_THRESHOLD )
	{
		c.raw_value = current2;
		c.gain = 121;
		return c;
	}
	if (current1<SATURATION_THRESHOLD )
	{
		c.raw_value = current1;
		c.gain = 11;
		return c;
	}
	else
	{
		c.raw_value = 0;
		c.gain = 1;
		return c;
	}
}
////////////////////////////////////////////////////////
void sort(long double array[2][COUNT]) {
	for (int i = 1; i < COUNT; i++) {
		int j = i;
		while (j > 0 && array[0][j - 1] > array[0][j]) {
			long double tempx = array[0][j - 1];
			long double tempy = array[1][j - 1];
			// x
			array[0][j - 1] = array[0][j];
			array[0][j] = tempx;
			// y
			array[1][j - 1] = array[1][j];
			array[1][j] = tempy;
			j = j - 1;
		}
	}
}
///////////////////////////////////////////////
bool polynomialfit(int obs, int degree, long double *dx,  long double *dy, long double *store) /* n, p */
{
  gsl_multifit_linear_workspace *ws;
  gsl_matrix *cov, *X;
  gsl_vector *y, *c;
   double chisq;

  int i, j;

  X = gsl_matrix_alloc(obs, degree);
  y = gsl_vector_alloc(obs);
  c = gsl_vector_alloc(degree);
  cov = gsl_matrix_alloc(degree, degree);

  for(i=0; i < obs; i++) {
    gsl_matrix_set(X, i, 0, 1.0);
    for(j=0; j < degree; j++) {
      gsl_matrix_set(X, i, j, pow(dx[i], j));
    }
    gsl_vector_set(y, i, dy[i]);
  }

  ws = gsl_multifit_linear_alloc(obs, degree);
  gsl_multifit_linear(X, y, c, cov, &chisq, ws);

  /* store result ... */
  for(i=0; i < degree; i++)
  {
    store[i] = gsl_vector_get(c, i);
  }

  gsl_multifit_linear_free(ws);
  gsl_vector_free(c);
  return true; /* we do not "analyse" the result (cov matrix mainly)
		  to know if the fit is "good" */
}
///////////////////////////////////////////////////////////
long double polycurve(long double voltage, long double *coeffs, int degree) {
	long double ret = coeffs[1] * voltage + coeffs[0];
	int i;
	for(i=2;i<degree;i++) {
		ret+= coeffs[i] * gsl_pow_int(voltage,i);
	}
	return ret;
}
//////////////////////////////////////////////
long double clamp_current(long double current) {
	return fmin(fmax(current, 0), 0.1f);
}
//////////////////////////////////////////////
void create_curves_with_regression(void)
{
				long double tox[3];
				long double toy[3];

				// Clear curve first to all zeros
				for (int j = 0; j < NUMPOINTS - 1; j++) {
					allcurves[j] = 0;
				}

				// Sort curve data cloud on Voltage
				sort(curvedata);

				// Get bounds for polyline
				long double leftmostpoint = curvedata[0][0];
				long double rightmostpoint = curvedata[0][COUNT - 1];

				// Get poly regression of all points
				polynomialfit(COUNT, DEGREE, curvedata[0], curvedata[1], coeff);
				int lastIndex = 63;
				int firstIndex = 0;
				for (int j = 0; j < NUMPOINTS - 1; j++) {
					if (VOLTAGES[j] < leftmostpoint) {
						// Get the index where the regression starts in allcurves
						firstIndex = j + 1;
					}
					if (VOLTAGES[j] > leftmostpoint && VOLTAGES[j] < rightmostpoint) {
						allcurves[j] = clamp_current(polycurve(VOLTAGES[j], coeff, DEGREE));
					}
					if (VOLTAGES[j] > rightmostpoint) {
						lastIndex = j;
						break;
					}
				}

				// Draw a line through left part past point cloud, using points already
				// in curve
				memcpy(tox, &VOLTAGES[firstIndex+1], sizeof(long double) * 3);
				memcpy(toy, &allcurves[firstIndex+1], sizeof(long double) * 3);
				polynomialfit(3, 2, tox, toy, coeff);
				for (int j = 0; j < firstIndex; j++) {
					allcurves[j] = clamp_current(polycurve(VOLTAGES[j], coeff, 2));
				}

				// Draw a line through right part past point cloud, using points already
				// in curve
				// 2-degree since it will most likely curve into zero
				memcpy(tox, &VOLTAGES[lastIndex - 3], sizeof(long double) * 3);
				memcpy(toy, &allcurves[lastIndex - 3],sizeof(long double) * 3);
				polynomialfit(3, 2, tox, toy, coeff);

				// We only want a negative slope between two points, if not negative
				// then render it at zero height
				for (int j = lastIndex; j < NUMPOINTS - 1; j++) {
					if (coeff[1] < 0) {
						allcurves[j] = clamp_current(polycurve(VOLTAGES[j], coeff, 2));
					} else {
						allcurves[j] = 0;
					}
				}
			}//end create_curves_with_regression()
//////////////////////////////////////////////
//// END FROM REVF



////////////////////////////////////
//// FROM OLD
#include "rtplot.h"
//#define DEBUG

#define SENSE_RESISTANCE 10.2
#define NUMPOINTS 65

#define MOBILE_EKHO_V2
//#define MOBILE_EKHO
//#define DESKTOP_EKHO


#ifdef MOBILE_EKHO_V2
#define CS_GAIN 200
#define VS_GAIN 4
#endif

#ifdef DESKTOP_EKHO
#define CS_GAIN 10
#define VS_GAIN 1
#endif

unsigned char buf[READSIZE];
GLfloat color1[3] = {1.0,1.0,1.0};

/* Serial init and configuration */
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

void draw_data()
{
	int i;
	/*  Draw something here */
	glPointSize(4.0);
	glColor3fv( color1 );
	glBegin(GL_POINTS);
	glVertex2f(0.1, 0.001);
	for(i=0;i<COUNT;i++) {
		glVertex2f(curvedata[0][i], curvedata[1][i]);
	}
	glEnd();
}
FILE *outfile;
double timestamp=0.0;
uint64_t start;
uint64_t end;
uint64_t elapsed;
uint64_t elapsedNano;
static mach_timebase_info_data_t    sTimebaseInfo;



////END FROM OLD

int main ( int argc, char** argv )   // Create Main Function For Bringing It All Together
{
    // FROM REVF
	if (argc < 3) {
		fprintf(stderr, "This program records IV-surfaces with no rendering.\n");
		fprintf(stderr, "	Usage:   ./record <port> <record_time_ms> <surface_file_name> \n");
		exit(0);
	}
	tempint = atoi(argv[2]);
	if(tempint < 1) {
		fprintf(stderr, "record_time_ms needs to be positive and longer than 1 millisecond\n");
		exit(0);
	}
	record_time_ms = (double)tempint / 1000.0;
	printf ("time required %f\n", record_time_ms);
	if (argc > 2) {
		strcpy(output_file_name, argv[3]);
		strcpy(rawpair_file_name, argv[3]);
		strcat (output_file_name,".ivs");
		strcat (rawpair_file_name,".raw");
	}
	printf("Saving to %s...\n", output_file_name);
	surface_outf = fopen(output_file_name, "w");
    rawpair_file = fopen(rawpair_file_name, "w");
	init_serial(argc, argv);
    // END REVF


    // FROM OLD
	if ( sTimebaseInfo.denom == 0 ) {
		(void) mach_timebase_info(&sTimebaseInfo);
	}
	start = mach_absolute_time();
	if ( sTimebaseInfo.denom == 0 ) {
		(void) mach_timebase_info(&sTimebaseInfo);
	}
	init_serial(argc, argv);
	//int n = write(port, buf, 1);
	//if (n < 1) {
	//	fprintf(stderr, "error sending start command to Ekho device\n");
	//	exit(0);
	//}
	GLOBAL_START = getRealTime();
	run(argc, argv, 4.0, 0.001, 1000, 500);

    return 0;
    // END OLD
}
