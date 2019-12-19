#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

#define COUNT 500
#define NUMPOINTS 65

double curve_time;
double timestamp;
double emulated_timestamps[255];
char outfile_name[255];
FILE *emulated_file, *recorded_file;
long double curvepoints[NUMPOINTS];
long double emulated_curves[255][NUMPOINTS];
long double EPS = 0.000000001; // If a point is under this value, it is not used for the mean squared error

long double VOLTAGES[NUMPOINTS] = {0, 0.16615384615, 0.33230769230, 0.49846153845, 0.66461538460, 0.83076923075, 0.99692307690, 1.16307692305, 1.32923076920, 1.49538461535, 1.66153846150, 1.82769230765, 1.99384615380, 2.15999999995, 2.32615384610, 2.49230769225, 2.65846153840, 2.82461538455, 2.99076923070, 3.15692307685, 3.32307692300, 3.48923076915, 3.65538461530, 3.82153846145, 3.98769230760, 4.15384615375, 4.31999999990, 4.48615384605, 4.65230769220, 4.81846153835, 4.98461538450, 5.15076923065, 5.31692307680, 5.48307692295, 5.64923076910, 5.81538461525, 5.98153846140, 6.14769230755, 6.31384615370, 6.47999999985, 6.64615384600, 6.81230769215, 6.97846153830, 7.14461538445, 7.31076923060, 7.47692307675, 7.64307692290, 7.80923076905, 7.97538461520, 8.14153846135, 8.30769230750, 8.47384615365, 8.63999999980, 8.80615384595, 8.97230769210, 9.13846153825, 9.30461538440, 9.47076923055, 9.63692307670, 9.80307692285, 9.96923076900, 10.13538461515, 10.30153846130, 10.46769230745, 10.63384615360};

double load_emulated_curve(FILE *input_file)
{
    double last_timestamp;
    int n;
    int i = 0;
    while (1)
    {
        last_timestamp = (i == 0 ? 0.0 : emulated_timestamps[i - 1]);
        n = fread(&emulated_timestamps[i], sizeof(double), 1, input_file);
        // printf("timestamp\t%.9lf\t\n",timestamp);
        n = fread(&emulated_curves[i], sizeof(long double), NUMPOINTS, input_file);
        // fprintf(stderr, "file read from, n= %d\n", n);

        if (n < 1)
        {
            emulated_timestamps[i + 1] = -1.0;
            for (int j = 0; j < NUMPOINTS; j++)
            {
                emulated_curves[i + 1][j] = -1.0;
            }
            return last_timestamp;
        }
        i++;
    }
}

long double interpolate(int Vindex, double timestamp)
{
    for (int i = 0; i < 256; i++)
    {
        long double ret, ratio;
        if (emulated_timestamps[i] > timestamp)
        {
            if (i == 0)
            {
                ratio = (emulated_timestamps[i] - timestamp) / (emulated_timestamps[i]);
                ret = emulated_curves[i][Vindex] * ratio;
            }
            else
            {
                ratio = (emulated_timestamps[i] - timestamp) / (emulated_timestamps[i] - emulated_timestamps[i - 1]);
                ret = emulated_curves[i - 1][Vindex] + (emulated_curves[i][Vindex] - emulated_curves[i - 1][Vindex]) * ratio;
            }
            fprintf(stderr, "Interpolated value at %lf time, %Lf volts: %Lf amps\n", timestamp, VOLTAGES[Vindex], ret);
            return ret;
        }
        if (emulated_timestamps[i] == -1.0)
        {
            fprintf(stderr, "Interpolate Error! Received timestamp past length of emulation!\n");
            return -1.0;
        }
    }
    fprintf(stderr, "There must have been an error with loading the emulated curve\n");
    return -1.0;
}

//Given timestamp and curve, finds mse
long double interpolated_error(double timestamp, long double *curve)
{
    // long double curve_diff[NUMPOINTS];
    long double diff;
    long double mse = 0.0;
    int significant_points = 1;
    long double actual, percent_error;
    long double mean_percent_error = 0.0;
    for (int i = 0; i < NUMPOINTS; i++)
    {
        actual = interpolate(i, timestamp);
        diff = actual - curve[i];
        if (diff < 0)
        {
            diff = -diff;
        }
        if ((diff < EPS && actual < EPS) || actual == 0.0) { // disregard the values that are effectively 0
            printf("too small\n");
            continue;
        }
        percent_error = diff / actual;
        significant_points++;
        printf("Diff is %Lf\n", diff);
        mse += diff; //* diff * (1 / NUMPOINTS);
        mean_percent_error += percent_error;
    }
    if (significant_points == 0)
    {
        significant_points = 1; // avoid inf errors
    }
    mse = mse / significant_points;
    mean_percent_error = mean_percent_error / significant_points;
    printf("\n\nThis timestamp total mse with %d sig points: %Lf\n\n", significant_points, mse);
    // printf("\n\nThis timestamp total mse with %d sig points: %Lf\n\n", significant_points, mean_percent_error);
    // return mean_percent_error;
    return mse;
}

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        fprintf(stderr, "mse: calculates the MSE of the surface recorded from a emulated surface.\n");
        fprintf(stderr, "\tFormat: ./mse <emulated .ivs file> <recorded .ivs file> <time emulation begins>\n");
        fprintf(stderr, "\tNote: time must be accurate to at least 1 decimal place.\nEmulated surface cannot have more than 255 timestamps\n");
        exit(0);
    }

    curve_time = strtod(argv[3], NULL);
    fprintf(stderr, "curve_time selected: %lf\n", curve_time);
    recorded_file = fopen(argv[2], "r");
    int n;
    while (1)
    {
        n = fread(&timestamp, sizeof(double), 1, recorded_file);
        // printf("timestamp\t%.9lf\t\n",timestamp);
        n = fread(&curvepoints, sizeof(long double), NUMPOINTS, recorded_file);
        // fprintf(stderr, "file read from, n= %d\n", n);

        if (n < 1)
        {
            fprintf(stderr, "No matching timestamp found!\n");
            fclose(recorded_file);
            break;
        }
        // Found the right curve. Make new ivs file
        double diff;
        diff = timestamp - curve_time;
        fprintf(stderr, "time diff: %lf\n", diff);
        if (diff < 0)
        {
            diff = -diff;
        }
        double first_timestamp = timestamp;
        if (diff < 0.1)
        {
            double emulation_length;
            emulated_file = fopen(argv[1], "r");
            emulation_length = load_emulated_curve(emulated_file);
            fclose(emulated_file);
            double record_elapsed = 0.0;
            long double total_error = 0.0;
            int k = 0;
            printf("recorded_elapsed : %lf\nemulation_length: %lf\n", record_elapsed, emulation_length);
            // fclose(recorded_file);
            // exit(0);
            timestamp = 0.0;
            while (timestamp - first_timestamp < emulation_length)
            {
                n = fread(&timestamp, sizeof(double), 1, recorded_file);
                n = fread(&curvepoints, sizeof(long double), NUMPOINTS, recorded_file);
                fprintf(stderr, "n = %d\n", n);
                printf("timestamp: %lf\ntimestamp - first: %lf\n", timestamp, timestamp - first_timestamp);
                // fclose(recorded_file);
                // exit(0);
                if (timestamp - first_timestamp >= emulation_length)
                {
                    break;
                }
                total_error += interpolated_error(timestamp - first_timestamp, curvepoints);
                printf("running total of error: %Lf\n", total_error);
                fprintf(stderr, "Made it out of interpolated_error\ntimestamp: %lf\nfirst_timestamp: %lf\n", timestamp, first_timestamp);
                k += 1;
            }
            fclose(recorded_file);
            long double MSE = total_error / k;
            fprintf(stderr, "Average Mean Squared Error for every timestamp: %.17Lf\n\n", MSE); // actually just mean error right now
            exit(0);
        }
    }
}
