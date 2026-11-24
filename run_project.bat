
@echo off
cd /d "%~dp0"

echo ===== EEG DSP PROJECT =====

echo Compiling main.c...
gcc src/main.c/main.c -o eeg_project.exe -lm
if errorlevel 1 goto error

echo Compiling analysis.c...
gcc reesults/analysis.c -o analysis.exe
if errorlevel 1 goto error

echo Compiling trend.c...
gcc reesults/trend.c -o trend.exe -lm
if errorlevel 1 goto error

echo Running EEG processing...
echo dataset/O1_eeg.csv | .\eeg_project.exe
if errorlevel 1 goto error

echo Running analysis...
.\analysis.exe
if errorlevel 1 goto error

echo Running trend analysis...
.\trend.exe
if errorlevel 1 goto error

echo.
echo ===== ALL PROCESSING COMPLETED =====
pause
exit /b 0

:error
echo.
echo ERROR: Compilation or execution failed.
pause
exit /b 1
