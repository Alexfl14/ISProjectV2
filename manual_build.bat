@echo off
echo Building BlackjackUI...
cd /d D:\ISProjectV2\out\build\x64-Debug
cmake --build . --target BlackjackUI --config Debug -v
echo.
echo Build complete. Check output above for errors.
pause
