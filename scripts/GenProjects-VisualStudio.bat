@echo off
echo Option 1: vs2017
echo Option 2: vs2019
echo Option 3: vs2022
set /p version="Choose Option: "

pushd ..\
if %version%==1 (CALL vendor\bin\premake\premake5.exe vs2017)
if %version%==2 (CALL vendor\bin\premake\premake5.exe vs2019)
if %version%==3 (CALL vendor\bin\premake\premake5.exe vs2022)
popd

PAUSE