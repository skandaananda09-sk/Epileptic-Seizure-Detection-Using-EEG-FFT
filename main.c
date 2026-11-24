
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define FS 512
#define N 2048
#define PI 3.14159265358979323846

typedef struct {
    double re;
    double im;
} Complex;

double samples[N];
Complex fft[N];

/* Read time and EEG amplitude from a two-column CSV */
int read_eeg_row(char *line, double *time, double *value)
{
    char *end;
    char *p = line;

    double t = strtod(p, &end);
    if (end == p) return 0;

    p = end;
    while (*p == ' ' || *p == '\t') p++;

    if (*p != ',') return 0;
    p++;

    double v = strtod(p, &end);
    if (end == p) return 0;

    *time = t;
    *value = v;

    return 1;
}

/* Radix-2 Cooley-Tukey FFT with Hann window */
void calculate_fft(void)
{
    int bits = 0;

    while ((1 << bits) < N)
        bits++;

    for (int i = 0; i < N; i++) {
        int j = 0;
        int x = i;

        for (int b = 0; b < bits; b++) {
            j = (j << 1) | (x & 1);
            x >>= 1;
        }

        double window = 0.5 - 0.5 *
            cos(2.0 * PI * i / (N - 1));

        fft[j].re = samples[i] * window;
        fft[j].im = 0.0;
    }

    for (int len = 2; len <= N; len <<= 1) {
        double angle = -2.0 * PI / len;
        double wr0 = cos(angle);
        double wi0 = sin(angle);

        for (int i = 0; i < N; i += len) {
            double wr = 1.0;
            double wi = 0.0;

            for (int j = 0; j < len / 2; j++) {
                Complex u = fft[i + j];

                Complex v = {
                    fft[i + j + len / 2].re * wr -
                    fft[i + j + len / 2].im * wi,

                    fft[i + j + len / 2].re * wi +
                    fft[i + j + len / 2].im * wr
                };

                fft[i + j].re = u.re + v.re;
                fft[i + j].im = u.im + v.im;

                fft[i + j + len / 2].re = u.re - v.re;
                fft[i + j + len / 2].im = u.im - v.im;

                double next_wr = wr * wr0 - wi * wi0;
                double next_wi = wr * wi0 + wi * wr0;

                wr = next_wr;
                wi = next_wi;
            }
        }
    }
}

/* Calculate squared FFT magnitude within a frequency band */
double band_power(double low, double high)
{
    double power = 0.0;

    for (int k = 1; k < N / 2; k++) {
        double frequency = (double)k * FS / N;

        if (frequency >= low && frequency < high) {
            double re = fft[k].re;
            double im = fft[k].im;

            power += re * re + im * im;
        }
    }

    return power;
}

int main(void)
{
    char filename[512];
    char line[4096];

    printf("EEG DSP PROCESSING\n");
    printf("Enter EEG CSV file path: ");

    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        printf("Invalid input.\n");
        return 1;
    }

    filename[strcspn(filename, "\r\n")] = '\0';

    FILE *input = fopen(filename, "r");

    if (input == NULL) {
        perror("Cannot open EEG input");
        return 1;
    }

    FILE *output = fopen("eeg_results.csv", "w");

    if (output == NULL) {
        perror("Cannot create eeg_results.csv");
        fclose(input);
        return 1;
    }

    FILE *spectrum = fopen("eeg_spectrum.csv", "w");

    if (spectrum == NULL) {
        perror("Cannot create eeg_spectrum.csv");
        fclose(input);
        fclose(output);
        return 1;
    }

    fprintf(output,
        "Window,Start_Time,Delta,Theta,Alpha,Beta\n");

    fprintf(spectrum,
        "Window,Start_Time,Frequency_Hz,Magnitude\n");

    double buffer[N];
    double first_time = 0.0;

    int count = 0;
    int windows = 0;
    int skipped = 0;

    while (fgets(line, sizeof(line), input) != NULL) {
        double time, value;

        if (!read_eeg_row(line, &time, &value)) {
            skipped++;
            continue;
        }

        if (count == 0)
            first_time = time;

        buffer[count++] = value;

        if (count == N) {

            for (int i = 0; i < N; i++)
                samples[i] = buffer[i];

            /* Perform FFT */
            calculate_fft();

            /* Export individual positive-frequency bins */
            for (int k = 1; k < N / 2; k++) {

                double frequency = (double)k * FS / N;

                double magnitude = sqrt(
                    fft[k].re * fft[k].re +
                    fft[k].im * fft[k].im
                );

                fprintf(spectrum,
                    "%d,%.6f,%.3f,%.6f\n",
                    windows + 1,
                    first_time,
                    frequency,
                    magnitude);
            }

            /* Calculate EEG frequency-band powers */
            double delta = band_power(0.5, 4.0);
            double theta = band_power(4.0, 8.0);
            double alpha = band_power(8.0, 13.0);
            double beta  = band_power(13.0, 30.0);

            fprintf(output,
                "%d,%.6f,%.6f,%.6f,%.6f,%.6f\n",
                windows + 1,
                first_time,
                delta,
                theta,
                alpha,
                beta);

            windows++;
            count = 0;
        }
    }

    fclose(input);
    fclose(output);
    fclose(spectrum);

    printf("\nProcessing completed!\n");
    printf("Sampling frequency: %d Hz\n", FS);
    printf("FFT window size: %d samples\n", N);
    printf("Window duration: %.2f seconds\n", (double)N / FS);
    printf("Frequency resolution: %.2f Hz\n", (double)FS / N);
    printf("Windows processed: %d\n", windows);
    printf("Incomplete samples: %d\n", count);
    printf("Invalid rows skipped: %d\n", skipped);

    printf("\nGenerated output files:\n");
    printf("1. eeg_results.csv\n");
    printf("2. eeg_spectrum.csv\n");

    return 0;
}
