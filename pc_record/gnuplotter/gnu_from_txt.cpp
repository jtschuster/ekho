#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <gsl/gsl_multifit.h>

int main(int argc, char **argv) // Create Main Function For Bringing It All Together
{
    if (argc != 3)
    {
        fprintf(stderr, "This program generates GNU plot file given the data file in format <timestamp> <voltage> <current>.\n");
        fprintf(stderr, "	Usage:   ./gnu_from_text <text_file> <gnu_file_name> \n");
        exit(0);
    }
    FILE *gnu_outfile;
    char gnu_file_name[255];
    char input_file_name[255];
    char output_img_file_name[255];
    strcpy(input_file_name, argv[1]);
    strcpy(gnu_file_name, argv[2]);

    strcpy(output_img_file_name, gnu_file_name);
    strcat(output_img_file_name, "_render");

    strcat(gnu_file_name, ".gnu");

    gnu_outfile = fopen(gnu_file_name, "w");

    fprintf(gnu_outfile, "set terminal png  transparent enhanced font \"arial,10\" fontscale 1.0 size 1600, 800\n");
    fprintf(gnu_outfile, "set output '%s.png'\n", output_img_file_name);
    fprintf(gnu_outfile, "unset border\n");
    fprintf(gnu_outfile, "set style fill   solid 1.00 noborder\n");
    fprintf(gnu_outfile, "set dummy u, v\n");
    fprintf(gnu_outfile, "unset key\n");
    fprintf(gnu_outfile, "set ticslevel 0\n");
    fprintf(gnu_outfile, "set view 45,85, 1.145, 1\n");
    fprintf(gnu_outfile, "set isosamples 75, 75\n");
    fprintf(gnu_outfile, "set ztics\n");
    fprintf(gnu_outfile, "set xrange [0:10.8]\n");
    fprintf(gnu_outfile, "set ylabel \"Time (s)\" \n");
    fprintf(gnu_outfile, "set xlabel \"Voltage (V)\" rotate parallel\n");
    fprintf(gnu_outfile, "set title \"\" \n");
    fprintf(gnu_outfile, "set urange [ -3.14159 : 3.14159 ] noreverse nowriteback\n");
    fprintf(gnu_outfile, "set vrange [ -3.14159 : 3.14159 ] noreverse nowriteback\n");
    fprintf(gnu_outfile, "set pm3d depthorder\n");
    fprintf(gnu_outfile, "set pm3d lighting primary 0.5 specular 0.6\n");
    fprintf(gnu_outfile, "set palette rgbformulae 8, 9, 7\n");
    fprintf(gnu_outfile, "splot '%s' using 2:1:3 with pm3d\n", input_file_name);
    fprintf(gnu_outfile, "# Time, Voltage, Current\n");

    fclose(gnu_outfile);
    return 0;
}
