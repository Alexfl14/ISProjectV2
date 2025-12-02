@echo off
echo Cleaning CMake cache and build directories...
rd /s /q "D:\ISProjectV2\out" 2>nul
rd /s /q "D:\ISProjectV2\.vs" 2>nul
echo.
echo Done! Now reopen the project in Visual Studio.
echo Visual Studio will regenerate the CMake cache properly.
pause
