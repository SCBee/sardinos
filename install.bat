@echo off
setlocal enableextensions enabledelayedexpansion

rem Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.

rem VC_INSTALL_PATH must point to the Microsoft Visual C++ installation directory
set VC_INSTALL_PATH="C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC"
if not exist %VC_INSTALL_PATH% (
   set VC_INSTALL_PATH="C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC"
)
call %VC_INSTALL_PATH%\bin\nmake.exe install || goto :error
rem C:\ProgramData\LMCDL\vcsi is filled in by the installer at installation time
copy messageDefinitions\*.xml C:\ProgramData\LMCDL\vcsi\sma || goto :error
XCopy "..\bin\modules\qml\" "C:\Program Files\LMCDL\vcsi\bin\modules\qml" /S /y || goto :error

echo Build Passed
goto :end

:error 
echo Failed with error %ERRORLEVEL%
exit /B %ERRORLEVEL%

:end
endlocal
