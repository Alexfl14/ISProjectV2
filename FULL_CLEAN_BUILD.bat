@echo off
echo ============================================
echo Complete Clean Rebuild for BlackjackUI
echo ============================================
echo.

echo Step 1: Closing Visual Studio instances...
taskkill /F /IM devenv.exe 2>nul
timeout /t 2 /nobreak >nul

echo Step 2: Cleaning build directories...
rd /s /q "D:\ISProjectV2\out" 2>nul
rd /s /q "D:\ISProjectV2\.vs" 2>nul

echo Step 3: Creating .vs directory...
mkdir "D:\ISProjectV2\.vs"

echo.
echo ============================================
echo Clean complete!
echo ============================================
echo.
echo NEXT STEPS:
echo 1. Open Visual Studio
echo 2. File ^> Open ^> Folder... ^> Select D:\ISProjectV2
echo 3. Wait for CMake to finish configuring
echo 4. In the toolbar, select "BlackjackUI.exe" as the startup item
echo 5. Press Ctrl+F5 to Build and Run
echo.
echo If you still get the error:
echo - Go to Build ^> Build All (Ctrl+Shift+B)
echo - Then try running again
echo.
pause
