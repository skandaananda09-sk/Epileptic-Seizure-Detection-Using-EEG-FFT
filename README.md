# Epileptic Seizure-Related EEG Analysis Using FFT in C

## Project Overview
This project implements an EEG signal analysis system using C programming and Digital Signal Processing (DSP) techniques. It analyses EEG signals in the frequency domain using Fast Fourier Transform (FFT), calculates frequency-band power, and identifies patterns that may require further review.

## Objectives
- Analyse EEG signals in the time and frequency domains.
- Implement Radix-2 FFT in C.
- Apply Hann windowing to reduce spectral leakage.
- Calculate Delta, Theta, Alpha and Beta frequency-band power.
- Analyse changes in frequency-band power across consecutive windows.
- Generate CSV files for further analysis and visualisation.

## Technologies Used
- C Programming
- Digital Signal Processing (DSP)
- Fast Fourier Transform (FFT)
- GCC Compiler
- Visual Studio Code
- Microsoft Excel

## Methodology
1. EEG signal input and windowing.
2. Hann window application.
3. Radix-2 Cooley–Tukey FFT.
4. Frequency spectrum calculation.
5. Frequency-band power analysis.
6. Relative power and trend analysis.
7. Experimental REVIEW / NO_FLAG classification.

## Technical Specifications
| Parameter | Configuration |
|---|---|
| Sampling frequency | 512 Hz |
| Window size | 2048 samples |
| Window duration | 4 seconds |
| FFT resolution | 0.25 Hz |
| Window function | Hann |
| FFT algorithm | Radix-2 Cooley–Tukey |

## EEG Frequency Bands
| Band | Frequency Range |
|---|---|
| Delta | 0.5–4 Hz |
| Theta | 4–8 Hz |
| Alpha | 8–13 Hz |
| Beta | 13–30 Hz |

## Project Files
- `main.c` – EEG signal processing and FFT.
- `analysis.c` – Frequency-band power analysis.
- `trend.c` – Consecutive-window trend analysis.
- `run_project.bat` – Project execution script.
- `eeg_spectrum.csv` – FFT frequency and magnitude output.
- `eeg_results.csv` – Frequency-band power output.
- `eeg_analysis_final.csv` – Relative frequency-band analysis.
- `eeg_trend_final.csv` – Frequency-band trend output.

## Execution
1. Install GCC compiler.
2. Open the project folder in VS Code or Command Prompt.
3. Ensure the required input CSV file is available in the expected location.
4. Run `run_project.bat`.
5. View the generated CSV outputs in Microsoft Excel.

## Results and Visualisation
The generated CSV files can be used to create:
- Raw EEG time-domain plots.
- FFT frequency-domain spectrum.
- Frequency-band power distribution.
- Frequency-band trend analysis.

## Disclaimer
This is an experimental academic DSP project for EEG signal analysis. The REVIEW and NO_FLAG outputs are based on heuristic thresholds and are not clinically validated seizure diagnoses. This software must not be used for medical diagnosis or treatment.

## Author
Developed as an academic Digital Signal Processing project.

