@echo off
echo Deleting CMake cache to force reconfiguration...
echo.

cd /d D:\ISProjectV2

echo Deleting cache files...
del /s /q CMakeCache.txt 2>nul
del /s /q cmake_install.cmake 2>nul
rd /s /q out 2>nul

echo.
echo Done! Now:
echo 1. Close and reopen Visual Studio
echo 2. Open Folder: D:\ISProjectV2
echo 3. Visual Studio will reconfigure CMake automatically
echo 4. Wait for "CMake generation finished" in the Output window
echo 5. Then try building
echo.
pause
