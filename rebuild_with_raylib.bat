@echo off
echo ============================================
echo Rebuilding Blackjack with Raylib UI
echo ============================================
echo.

echo Closing any running instances...
taskkill /F /IM BlackjackUI.exe 2>nul

echo.
echo Cleaning build directories...
rd /s /q "D:\ISProjectV2\out" 2>nul
rd /s /q "D:\ISProjectV2\.vs" 2>nul

echo.
echo ============================================
echo Clean complete!
echo ============================================
echo.
echo Next steps:
echo 1. Open the project folder in Visual Studio
echo 2. Wait for CMake to configure
echo 3. Build and run (Ctrl+F5)
echo.
echo Your Raylib Blackjack UI is ready!
echo.
pause
