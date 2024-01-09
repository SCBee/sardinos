@echo off
setlocal enableextensions enabledelayedexpansion

rem Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.

if not defined QTBASE (
   echo 'Please set QTBASE before you run this script.'
   set ERRORLEVEL=1
   goto :error
)

rem VC_INSTALL_PATH must point to the Microsoft Visual C++ installation directory
set VC_INSTALL_PATH="C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC"
if not exist %VC_INSTALL_PATH% (
   set VC_INSTALL_PATH="C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC"
)
if not exist %VC_INSTALL_PATH% (
   echo ERROR: Could not find Visual Studio 2019 installation
   set ERRORLEVEL=1
   goto :error
)

rem PYTHON_BIN_PATH must point to where python.exe lives
set PYTHON_BIN_PATH=C:\Python27

rem Add Python to path if it is not already
where python >nul 2>nul
if not %errorlevel% == 0 (
   if not exist %PYTHON_BIN_PATH% (
      echo Python not found in PATH or at "%PYTHON_BIN_PATH%;" please update build script with Python location
      goto :error
   )

   set PATH=!PATH!;%PYTHON_BIN_PATH%
)

set buildDir=%~dp0
cd ..\messageGeneration
call python -m pip install . || goto :error
cd %buildDir%

call python generate_messages.py || goto :error
call %VC_INSTALL_PATH%\Auxiliary\Build\vcvarsall.bat x86_amd64 || goto :error
call %QTBASE%\bin\qmake.exe || goto :error
call %VC_INSTALL_PATH%\Tools\MSVC\14.29.30133\bin\Hostx86\x64\nmake.exe || goto :error

echo Build Passed
goto :end

:error
echo Failed with error %ERRORLEVEL%
exit /B %ERRORLEVEL%

:end
endlocal
