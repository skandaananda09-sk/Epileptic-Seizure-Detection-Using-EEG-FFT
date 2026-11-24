
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(void)
{
    FILE *input = fopen("eeg_analysis_final.csv", "r");

    if (input == NULL) {
        perror("Cannot open eeg_analysis.csv");
        return 1;
    }

    FILE *output = fopen("eeg_trend_final.csv", "w");

    if (output == NULL) {
        perror("Cannot create eeg_trend.csv");
        fclose(input);
        return 1;
    }

    char line[512];

    if (fgets(line, sizeof(line), input) == NULL) {
        printf("Input file is empty.\n");
        fclose(input);
        fclose(output);
        return 1;
    }

    fprintf(output,
        "Window,Start_Time,Delta_Change,"
        "Theta_Change,Alpha_Change,Beta_Change,Status\n");

    int window, previous_window = 0;
    int processed = 0;
    int review_count = 0;
    int no_flag_count = 0;

    double time, d, t, a, b;
    double pd = 0.0, pt = 0.0, pa = 0.0, pb = 0.0;

    while (fgets(line, sizeof(line), input) != NULL) {
        if (sscanf(line, "%d,%lf,%lf,%lf,%lf,%lf",
                   &window, &time, &d, &t, &a, &b) != 6)
            continue;

        if (previous_window == 0) {
            pd = d;
            pt = t;
            pa = a;
            pb = b;
            previous_window = window;
            continue;
        }

        double cd = fabs(d - pd);
        double ct = fabs(t - pt);
        double ca = fabs(a - pa);
        double cb = fabs(b - pb);

        int changes = 0;

        if (cd > 20.0) changes++;
        if (ct > 15.0) changes++;
        if (ca > 15.0) changes++;
        if (cb > 15.0) changes++;

        const char *status =
            (changes >= 2) ? "REVIEW" : "NO_FLAG";

        if (status[0] == 'R')
            review_count++;
        else
            no_flag_count++;

        fprintf(output,
            "%d,%.6f,%.2f,%.2f,%.2f,%.2f,%s\n",
            window, time, cd, ct, ca, cb, status);

        processed++;

        pd = d;
        pt = t;
        pa = a;
        pb = b;
        previous_window = window;
    }

    fclose(input);
    fclose(output);

    printf("\nEEG TREND ANALYSIS COMPLETED\n");
    printf("Windows compared: %d\n", processed);
    printf("REVIEW: %d\n", review_count);
    printf("NO_FLAG: %d\n", no_flag_count);
    printf("Output: eeg_trend.csv\n");

    return 0;
}
