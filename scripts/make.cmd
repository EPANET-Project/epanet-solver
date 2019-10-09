::
::  make.cmd - builds epanet
::
::  Date Created: 9/18/2019
::  Date Updated: 10/9/2019
::
::  Author: Michael E. Tryby
::          US EPA - ORD/NRMRL
::
::  Requires:
::    Build Tools for Visual Studio download:
::      https://visualstudio.microsoft.com/downloads/
::
::    CMake download:
::      https://cmake.org/download/
::
::  Environment Variables:
::    BUILD_HOME - defaults to build
::
::  Optional Arguments:
::    /g ("GENERATOR") defaults to "Visual Studio 15 2017"
::    /t builds and runs unit tests (requires Boost)
::

:: set global defaults
set "BUILD_HOME=build"
set "TEST_HOME=nrtests"
set "PLATFORM=win32"


::echo off
setlocal EnableDelayedExpansion


:: check for requirements
where cmake > nul
if %ERRORLEVEL% NEQ 0 ( echo "ERROR: cmake not installed" & exit /B 1 )


:: determine project directory
set "CUR_DIR=%CD%"
set "SCRIPT_HOME=%~dp0"
cd %SCRIPT_HOME%
cd ..


echo INFO: Building epanet  ...


:: set local defaults
set "GENERATOR=Visual Studio 15 2017"
set "TESTING=0"

:: process arguments
:loop
if NOT [%1]==[] (
  if "%1"=="/g" (
    set "GENERATOR=%~2"
    shift
  )
  if "%1"=="/t" (
    set "TESTING=1"
  )
  shift
  goto :loop
)

:: process args
::if [%1]==[] ( set "GENERATOR=Visual Studio 15 2017"
::) else ( set "GENERATOR=%~1" )


:: if generator has changed delete the build folder
if exist %BUILD_HOME% (
  for /F "tokens=*" %%f in ( 'findstr CMAKE_GENERATOR:INTERNAL %BUILD_HOME%\CmakeCache.txt' ) do (
    for /F "delims=:= tokens=3" %%m in ( 'echo %%f' ) do (
      set CACHE_GEN=%%m
      if not "!CACHE_GEN!" == "!GENERATOR!" ( rmdir /s /q %BUILD_HOME% & mkdir %BUILD_HOME% )
    )
  )
) else (
  mkdir %BUILD_HOME%^
  & if %ERRORLEVEL% NEQ 0 ( echo "ERROR: unable to make %BUILD_HOME% dir" & exit /B 1 )
)


:: perform the build
cd %BUILD_HOME%
if %TESTING% EQU 1 (
  cmake -G"%GENERATOR%" -DBUILD_TESTS=ON -DBOOST_ROOT=C:\local\boost_1_67_0 ..^
  && cmake --build . --config Debug^
  & echo. && ctest -C Debug --output-on-failure
) else (
  cmake -G"%GENERATOR%" -DBUILD_TESTS=OFF ..^
  && cmake --build . --config Release --target package^
  && move epanet-solver*.zip %PROJECT_PATH%
)


endlocal


:: determine platform from CmakeCache.txt file
for /F "tokens=*" %%f in ( 'findstr CMAKE_SHARED_LINKER_FLAGS:STRING %BUILD_HOME%\CmakeCache.txt' ) do (
  for /F "delims=: tokens=3" %%m in ( 'echo %%f' ) do (
    if "%%m" == "X86" ( set "PLATFORM=win32" ) else if "%%m" == "x64" ( set "PLATFORM=win64" )
  )
)
if not defined PLATFORM ( echo "ERROR: PLATFORM could not be determined" & exit /B 1 )


:: return to users current dir
cd %CUR_DIR%
