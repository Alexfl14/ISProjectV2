@echo off
echo ============================================
echo Complete CMake Clean for BlackjackUI Project
echo ============================================
echo.

echo Closing any running instances...
taskkill /F /IM BlackjackUI.exe 2>nul

echo.
echo Deleting build outputs...
rd /s /q "D:\ISProjectV2\out" 2>nul
rd /s /q "D:\ISProjectV2\.vs" 2>nul
rd /s /q "D:\ISProjectV2\build" 2>nul

echo.
echo Deleting CMake cache files...
del /s /q "D:\ISProjectV2\CMakeCache.txt" 2>nul
del /s /q "D:\ISProjectV2\cmake_install.cmake" 2>nul
del /s /q "D:\ISProjectV2\CMakeFiles" 2>nul

echo.
echo ============================================
echo Clean complete!
echo ============================================
echo.
echo Next steps:
echo 1. Close Visual Studio completely
echo 2. Reopen the project
echo 3. Wait for CMake to configure
echo 4. Try Ctrl+F5 again
echo.
pause
