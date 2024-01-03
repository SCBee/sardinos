if not defined DevEnvDir (
    call vcvarsall.bat
)

call qmake.exe %1.pro || goto :error
call nmake.exe || goto :error

echo Build Passed

echo Installing to VCSi

move "..\build\%1.dll" "C:\Program Files\LMCDL\vcsi\bin\plugins\vcsi"

goto :end

:error
echo Failed with error 
exit /B

:end
endlocal
