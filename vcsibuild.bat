call vcvarsall.bat amd64 || goto :error
call qmake.exe %1 || goto :error
call nmake.exe || goto :error

echo Build Passed
goto :end

:error
echo Failed with error 
exit /B

:end
endlocal
