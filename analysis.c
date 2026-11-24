
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    FILE *input = fopen("eeg_results.csv", "r");

    if (input == NULL) {
        perror("Cannot open eeg_results.csv");
        return 1;
    }

    FILE *output = fopen("eeg_analysis_final.csv", "w");

    if (output == NULL) {
        perror("Cannot create eeg_analysis.csv");
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
        "Window,Start_Time,Delta_Percent,"
        "Theta_Percent,Alpha_Percent,Beta_Percent,Status\n");

    int processed = 0;
    int review_count = 0;
    int no_flag_count = 0;

    while (fgets(line, sizeof(line), input) != NULL) {
        int window;
        double time, delta, theta, alpha, beta;

        if (sscanf(line, "%d,%lf,%lf,%lf,%lf,%lf",
                   &window, &time, &delta,
                   &theta, &alpha, &beta) != 6)
            continue;

        double total = delta + theta + alpha + beta;

        if (total <= 0.0)
            continue;

        double d = delta / total * 100.0;
        double t = theta / total * 100.0;
        double a = alpha / total * 100.0;
        double b = beta / total * 100.0;

        const char *status =
            (b > 45.0 || d > 80.0)
            ? "REVIEW" : "NO_FLAG";

        if (status[0] == 'R')
            review_count++;
        else
            no_flag_count++;

        fprintf(output,
            "%d,%.6f,%.2f,%.2f,%.2f,%.2f,%s\n",
            window, time, d, t, a, b, status);

        processed++;
    }

    fclose(input);
    fclose(output);

    printf("\nEEG BAND ANALYSIS COMPLETED\n");
    printf("Windows analysed: %d\n", processed);
    printf("REVIEW: %d\n", review_count);
    printf("NO_FLAG: %d\n", no_flag_count);
    printf("Output: eeg_analysis.csv\n");

    return 0;
}
