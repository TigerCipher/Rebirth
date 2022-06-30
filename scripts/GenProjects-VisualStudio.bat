@echo off
@REM echo Option 1: vs2017
@REM echo Option 2: vs2019
@REM echo Option 3: vs2022
@REM set /p version="Choose Option: "

pushd ..\
@REM if %version%==1 (CALL vendor\premake\bin\premake5.exe vs2017)
@REM if %version%==2 (CALL vendor\premake\bin\premake5.exe vs2019)
@REM if %version%==3 (CALL vendor\premake\bin\premake5.exe vs2022)
CALL vendor\premake\bin\premake5.exe vs2022
popd
PAUSE