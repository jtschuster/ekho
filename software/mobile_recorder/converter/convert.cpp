/*
This code written by Joiah Hester and Modified by Arwa Alsubhi.
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
/////////////////////////////////////////////////////////////////

#define COUNT 512
#define ALL_CURVES_LEN 100
#define NUMPOINTS 65
#define DEGREE 5

int tempint;
int numcurves = 0;
int cpsavg = 0;
double record_time_ms = 0.0;
long double coeff[DEGREE];
long double allcurves[ALL_CURVES_LEN][NUMPOINTS];
long double curvedata[2][COUNT];
int indexInAllCurves = 0;
unsigned char buf[10];
unsigned char buft[4];
char output_file_name[255];
char rawpair_file_name[255];

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

void create_curves_with_regression(void);
struct current
{
	int raw_value;
	int gain;
};

struct current get_current(int current1, int current2, int current3);

#define READSIZE 12 // read 12 bytes from serial ports
#define BUFSIZE 1024
#define SENSE_Resistor 10				// 10 Ohms sense resistor on Ekho board
#define ADC_to_Voltage 0.0008056640625L // = 3.3/2^12
#define Current1_gain 11				//the none-inverting opamplifier gain for current 1 on Ehko board
#define Current2_gain 121				// the none-inverting opamplifier gain for current 2 on Ehko board
#define Current3_gain 1331				// the none-inverting opamplifier gain for current 3 on Ehko board
#define Voltage_gain 2					// the none-inverting opamplifier gain for voltage on Ehko board

#define SATURATION_THRESHOLD 4030
#define NUMPOINTS 65

int port;
int count = 0;

/////////////////////////////

FILE *surface_outf, *rawpair_file, *sdRawData_file;

void idle_impl()
{
	int n = 0;
	int points = 0;
	double start_timestamp;
	double End_timestamp;
	double recordtime = 0.0;

	//  printf("inside idle_impl\n");

	int totalN = 0;
	while (1)
	{

		if (points == 0)
		{
			n = fread(&buft, sizeof(unsigned char), 4, sdRawData_file); // get the start time of the curve
			start_timestamp = ((uint32_t)(buft[3]) << 24) + ((uint32_t)(buft[2]) << 16) + ((uint32_t)(buft[1]) << 8) + ((uint32_t)(buft[0]));

			// printf ("start_timestamp : %f \n", start_timestamp);
		}

		if (points == COUNT - 1)
		{
			n = fread(&buft, sizeof(unsigned char), 4, sdRawData_file); // get the start time of the curve
			End_timestamp = ((uint32_t)(buft[3]) << 24) + ((uint32_t)(buft[2]) << 16) + ((uint32_t)(buft[1]) << 8) + ((uint32_t)(buft[0]));

			//	printf ("End_timestamp : %f \n", End_timestamp);
		}

		n = fread(&buf, sizeof(unsigned char), 10, sdRawData_file);
		totalN += n;
		if (n < 10)
		{
			// printf("End wrting %d curves\n",numcurves );
			printf("=======\n %d IV-curves are recorded for %lf seconds.\n=======\n", numcurves, recordtime);
			// printf("total bytes %d \n",totalN );
			break;
		}

		//////get raw data////////////
		int raw_current_1 = (((buf[1]) << 8) + buf[0]);
		int raw_current_2 = (((buf[3]) << 8) + buf[2]);
		int raw_current_3 = (((buf[5]) << 8) + buf[4]);
		int raw_voltage = (((buf[7]) << 8) + buf[6]);
		int raw_vref = (((buf[9]) << 8) + buf[8]);
		//printf("%d , %d, %d, %d, %d \n", raw_current_1, raw_current_2, raw_current_3,raw_voltage, raw_vref);

		//get the higher amplified unsaturated current and its gain
		struct current Current = get_current(raw_current_1, raw_current_2, raw_current_3);

		// 1- convert ADC readings to voltages by multiplying by 3.3/2^12
		long double current = Current.raw_value * ADC_to_Voltage;
		long double voltage = raw_voltage * ADC_to_Voltage;
		long double vref = raw_vref * ADC_to_Voltage;

		///////////////////////////////////////////////
		// 2- subtract vref from  current value and  voltage
		current = current - vref;
		voltage = vref - (voltage / Voltage_gain);
		// 3- get current values (I)by applying Ohm's law I= V/R , where R = 10 Ohms on Ekho board and V is the voltage values stored in current variable
		current = current / SENSE_Resistor;

		voltage = voltage * 51.0L; // 51 voltage division factor on Ekho board, 51 = (R1+R2)/R1 = (200K+10M)/200K = (200000.0+10000000.0))/200000.0

		// 4- divide current by the current gain value
		current = current / Current.gain;

		// check for edge cases
		if (voltage < 0)
		{
			voltage = 0;
		}
		if (current < 0)
		{
			current = 0;
		}
		// store the values in curvedata array
		curvedata[0][points] = voltage;
		curvedata[1][points] = current;
		// printf("point %d, v %lf, c %lf \n", points, voltage, current);

		// printf("points %d\n", points);
		// printf("(%Lf,%Lf)\n",current,voltage);
		points++;

		if (points == COUNT)
		{
			recordtime += (End_timestamp - start_timestamp) * 0.001;
			//	printf ("start_timestamp : %f \n", start_timestamp);
			//	printf ("End_timestamp : %f \n", End_timestamp);
			start_timestamp = start_timestamp * 0.001;
			fwrite(&start_timestamp, sizeof(double), 1, rawpair_file);
			//printf ("after writing time stampe\n");
			// Write to file all x points (voltages)
			fwrite(&curvedata[0], sizeof(long double), COUNT, rawpair_file);
			// Write to file all y points (currents)
			fwrite(&curvedata[1], sizeof(long double), COUNT, rawpair_file);
			create_curves_with_regression();
			// Pipe curve output to file
			start_timestamp = start_timestamp * 1000;

			//printf("End wrting %d curves\n",numcurves );
			// Prepare for next curve
			points = 0;
			numcurves++;
			//	printf("\n");
			//	printf ("scale factor after wrting to file : %u / %u\n", sTimebaseInfo.numer, sTimebaseInfo.denom);
		}

		//printf("%.9Le\t%d\n",current, Current.gain); //

		//fprintf(outfile, "%.9lf	%.9lf	%.9lf %.9lf \n", current_1, current_2, current_3,voltage); //, timestamp
	} //end while
} // end idle_impl

////////////////////////////////////////////////////////
struct current get_current(int current1, int current2, int current3)
{ // this function start with the reading from 1331 gain opamlifier and check if it is not saturated return it and its coressponding gain
	//otherwise check the reading from 121 gain opamplifier if saturated go to ckeck the reading from 11 gain opamlifier.
	struct current c;
	if (current3 < SATURATION_THRESHOLD) //&& current3>= VREF_ADC
	{

		c.raw_value = current3;
		c.gain = 1331;
		return c;
	}

	if (current2 < SATURATION_THRESHOLD)
	{

		c.raw_value = current2;
		c.gain = 121;
		return c;
	}

	if (current1 < SATURATION_THRESHOLD)
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
void sort(long double array[2][COUNT])
{
	for (int i = 1; i < COUNT; i++)
	{
		int j = i;
		while (j > 0 && array[0][j - 1] > array[0][j])
		{
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
bool polynomialfit(int obs, int degree, long double *dx, long double *dy, long double *store) /* n, p */
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

	for (i = 0; i < obs; i++)
	{
		gsl_matrix_set(X, i, 0, 1.0);
		for (j = 0; j < degree; j++)
		{
			gsl_matrix_set(X, i, j, pow(dx[i], j));
		}
		gsl_vector_set(y, i, dy[i]);
	}

	ws = gsl_multifit_linear_alloc(obs, degree);
	gsl_multifit_linear(X, y, c, cov, &chisq, ws);

	/* store result ... */
	for (i = 0; i < degree; i++)
	{
		store[i] = gsl_vector_get(c, i);
	}

	gsl_multifit_linear_free(ws);
	gsl_vector_free(c);
	return true; /* we do not "analyse" the result (cov matrix mainly)
		  to know if the fit is "good" */
}
///////////////////////////////////////////////////////////
long double polycurve(long double voltage, long double *coeffs, int degree)
{
	long double ret = coeffs[1] * voltage + coeffs[0];
	int i;
	for (i = 2; i < degree; i++)
	{
		ret += coeffs[i] * gsl_pow_int(voltage, i);
	}
	return ret;
}
//////////////////////////////////////////////
long double clamp_current(long double current)
{
	return fmin(fmax(current, 0), 0.1f);
}
//////////////////////////////////////////////
void create_curves_with_regression(void)
{
	long double tox[3];
	long double toy[3];

	// Clear curve first to all zeros
	for (int j = 0; j < NUMPOINTS - 1; j++)
	{
		allcurves[indexInAllCurves][j] = 0;
	}

	// Sort curve data cloud on Voltage
	sort(curvedata);
	// for (int i =0 ; i<COUNT; i++)
	// {
	//
	// 	printf("(%Lf,%Lf)\n",curvedata[0][i], curvedata[1][i]);
	// }
	// printf("\n");
	// Get bounds for polyline
	long double leftmostpoint = curvedata[0][0];
	//printf("first point in curve %Lf\n", leftmostpoint);
	long double rightmostpoint = curvedata[0][COUNT - 1];
	//  printf("last point in curve %Lf\n", rightmostpoint);
	//		printf("====================================\n");
	// Get poly regression of all points
	polynomialfit(COUNT, DEGREE, curvedata[0], curvedata[1], coeff);
	int lastIndex = 63;
	int firstIndex = 0;
	for (int j = 0; j < NUMPOINTS - 1; j++)
	{
		if (VOLTAGES[j] < leftmostpoint)
		{
			// Get the index where the regression starts in allcurves
			firstIndex = j + 1;
		}
		if (VOLTAGES[j] > leftmostpoint && VOLTAGES[j] < rightmostpoint)
		{
			allcurves[indexInAllCurves][j] = clamp_current(polycurve(VOLTAGES[j], coeff, DEGREE));
		}
		if (VOLTAGES[j] > rightmostpoint)
		{
			lastIndex = j;
			break;
		}
	}

	// Draw a line through left part past point cloud, using points already
	// in curve
	memcpy(tox, &VOLTAGES[firstIndex + 1], sizeof(long double) * 3);
	memcpy(toy, &allcurves[indexInAllCurves][firstIndex + 1], sizeof(long double) * 3);
	polynomialfit(3, 2, tox, toy, coeff);
	for (int j = 0; j < firstIndex; j++)
	{
		allcurves[indexInAllCurves][j] = clamp_current(polycurve(VOLTAGES[j], coeff, 2));
	}

	// Draw a line through right part past point cloud, using points already
	// in curve
	// 2-degree since it will most likely curve into zero
	memcpy(tox, &VOLTAGES[lastIndex - 3], sizeof(long double) * 3);
	memcpy(toy, &allcurves[indexInAllCurves][lastIndex - 3], sizeof(long double) * 3);
	polynomialfit(3, 2, tox, toy, coeff);

	// We only want a negative slope between two points, if not negative
	// then render it at zero height
	for (int j = lastIndex; j < NUMPOINTS - 1; j++)
	{
		if (coeff[1] < 0)
		{
			allcurves[indexInAllCurves][j] = clamp_current(polycurve(VOLTAGES[j], coeff, 2));
		}
		else
		{
			//allcurves[indexInAllCurves][j] = 0;
		}
	}
} //end create_curves_with_regression()
//////////////////////////////////////////////

int main(int argc, char **argv) // Create Main Function For Bringing It All Together
{

	if (argc < 3)
	{
		fprintf(stderr, "This program converts the .sdraw file from the mobile recorder teensy into a .raw file.\n");
		fprintf(stderr, "	Usage:   ./record <output_file_name> <sdRawData_file_name>\n");
		exit(0);
	}
	if (argc > 2)
	{
		strcpy(rawpair_file_name, argv[1]);
		strcat(rawpair_file_name, ".raw");
	}
	printf("Saving to and %s..\n", rawpair_file_name);
	rawpair_file = fopen(rawpair_file_name, "w");
	sdRawData_file = fopen(argv[2], "r");

	idle_impl();

	fclose(rawpair_file);
	fclose(sdRawData_file);
	return 0;
}
