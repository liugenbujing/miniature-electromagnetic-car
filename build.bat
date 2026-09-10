@echo off
rem ============================================================
rem  Generic Keil C251 build script
rem  Place at any project root; auto-discovers *.uvproj under
rem  the current directory tree and builds it with UV4.
rem  Usage: build.bat           incremental build
rem         build.bat rebuild   full rebuild
rem ============================================================
setlocal
set "MODE=-b"
if /i "%1"=="rebuild" set "MODE=-r"

set "UV4=D:\Dev\Keil5\Core\UV4\UV4.exe"
if not exist "%UV4%" (
    echo [ERROR] UV4.exe not found: %UV4%
    exit /b 1
)

set "PROJ="
for /f "delims=" %%f in ('dir /s /b *.uvproj 2^>nul') do (
    if not defined PROJ set "PROJ=%%~ff"
)
if not defined PROJ (
    echo [ERROR] no .uvproj found under %CD%
    exit /b 1
)

echo [INFO] project: %PROJ%
echo [INFO] mode:    %MODE%
rem Redirect UV4 output to nul so it cannot hold the pipe
rem handle and stall the VSCode task panel / terminal.
start /wait "" "%UV4%" %MODE% "%PROJ%" -j0 -o "%~dp0uv4_build.log" >nul 2>&1
set "ERR=%ERRORLEVEL%"
type "%~dp0uv4_build.log"
exit /b %ERR%
