if not defined DevEnvDir (
    call vcvarsall.bat amd64
)

call qmake.exe %1.pro || goto :error
call nmake.exe || goto :error

echo Build Passed

echo Installing to VCSi

if exist "C:\Program Files\LMCDL\vcsi\bin\plugins\vcsi\%1.dll" (
	del "C:\Program Files\LMCDL\vcsi\bin\plugins\vcsi\%1.dll"
)

move "..\build\%1.dll" "C:\Program Files\LMCDL\vcsi\bin\plugins\vcsi"

goto :end

:error
echo Failed with error 
exit /B

:end
endlocal
