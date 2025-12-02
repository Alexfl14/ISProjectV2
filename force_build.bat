@echo off
echo Attempting to build BlackjackUI directly...
echo.

cd /d "D:\ISProjectV2"

echo Reconfiguring CMake...
cmake -B out/build/x64-Debug -S . -DCMAKE_BUILD_TYPE=Debug

echo.
echo Building BlackjackUI target...
cmake --build out/build/x64-Debug --target BlackjackUI --config Debug

echo.
echo Checking if executable exists...
if exist "out\build\x64-Debug\bin\BlackjackUI.exe" (
    echo SUCCESS: BlackjackUI.exe found!
    echo Location: out\build\x64-Debug\bin\BlackjackUI.exe
) else (
    echo ERROR: BlackjackUI.exe was not created
    echo.
    echo Checking build directory...
    dir /s /b out\build\x64-Debug\*.exe
)

echo.
pause
