@echo off

:: Check for optional flags set in the batch file call 
:: Adapted from dbenham answer http://stackoverflow.com/questions/3973824/windows-bat-file-optional-argument-parsing
setlocal enableDelayedExpansion
set "options=-build:Blank"
for %%O in (%options%) do for /f "tokens=1,* delims=:" %%A in ("%%O") do set "%%A=%%~B"
:loop
if not "%~1"=="" (
	set "test=!options:*%~1:=! "
	if "!test!"=="!options! " (
		echo Error: Invalid option %~1
	) else if "!test:~0,1!"==" " (
		echo Hey
		set "%~1=1"
	) else (
		setlocal disableDelayedExpansion
		set "val=%~2"
		call :escapeVal
		setlocal enableDelayedExpansion
		for /f delims^=^ eol^= %%A in ("!val!") do endlocal&endlocal&set "%~1=%%A" !
		shift /1
	)
	shift /1
	goto :loop
)
goto :endArgs
:escapeVal
set "val=%val:^=^^%"
set "val=%val:!=^!%"
exit /b
:endArgs

:: query registry to see if windows is 32-bit or 64-bit
reg Query "HKLM\Hardware\Description\System\CentralProcessor\0" | find /i "x86" > NUL && set OS=Win32 || set OS=Win64

:: Check for Visual Studio 2005
reg query "HKEY_CLASSES_ROOT\VisualStudio.DTE.8.0" >nul 2>nul
if %ERRORLEVEL% EQU 0 set vsVer=8
:: Check for Visual Studio 2008
reg query "HKEY_CLASSES_ROOT\VisualStudio.DTE.9.0" >nul 2>nul
if %ERRORLEVEL% EQU 0 set vsVer=9
:: Check for Visual Studio 2010
reg query "HKEY_CLASSES_ROOT\VisualStudio.DTE.10.0" >nul 2>nul
if %ERRORLEVEL% EQU 0 set vsVer=10
:: Check for Visual Studio 2012
reg query "HKEY_CLASSES_ROOT\VisualStudio.DTE.11.0" >nul 2>nul
if %ERRORLEVEL% EQU 0 set vsVer=11
:: Check for Visual Studio 2013
reg query "HKEY_CLASSES_ROOT\VisualStudio.DTE.12.0" >nul 2>nul
if %ERRORLEVEL% EQU 0 set vsVer=12 
:: Check for Visual Studio 2015
reg query "HKEY_CLASSES_ROOT\VisualStudio.DTE.14.0" >nul 2>nul
if %ERRORLEVEL% EQU 0 set vsVer=14
:: Check for Visual Studio 2017
reg query "HKEY_CLASSES_ROOT\VisualStudio.DTE.15.0" >nul 2>nul
if %ERRORLEVEL% EQU 0 set vsVer=15

:: Set platform target to pass to cmake
:: TODO(JR): let user pass it debug vs release build?
set "platform=Visual Studio %vsVer% %OS%"


:: http://stackoverflow.com/questions/24460486/cmake-build-type-not-being-used-in-cmakelists-txt
set cmakeTarget="Debug;MinSizeRel;Release;RelWithDebInfo"
if %-build% EQU Debug (set cmakeTarget=Debug)
if %-build% EQU Release (set cmakeTarget=Release) 
@echo on
cmake -G "%platform%" -H. -B_build -DCMAKE_CONFIGURATION_TYPES=%cmakeTarget%
@echo off

tasklist /FI "IMAGENAME eq devenv.exe" 2>NUL | find /I /N "devenv.exe">NUL
if not "%ERRORLEVEL%"=="0" (
	START "" devenv.exe %~dp0\_build\OpenGL_NoLib_Demo.sln
)

@echo on