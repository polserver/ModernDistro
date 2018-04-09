@ECHO OFF

REM -- If a special path is needed to ecompile.exe set it here
REM -- Path is considered to be run from the root if started by starthere.bat
SET ECOMPILE_PATH=scripts\ecompile.exe
REM ----------

IF NOT EXIST scripts\ecompile.cfg (
    CLS
    ECHO !!!WARNING!!!
    ECHO ========================
    ECHO scripts\ecompile.cfg doesn't exist compiler may have issues.
    ECHO Please check ecompile.cfg.example for an example file!!
    ECHO .
    SET /p DUMMY=Hit ENTER to continue...
    CLS
)

GOTO :MENU()

REM -- MENU FUNCTION
:MENU()
CLS
ECHO Ecompile.bat by Austin
ECHO ========================
ECHO Command        Purpose
ECHO  [ a ] - Compile a specific script.
ECHO  [ b ] - Compile a directory.
ECHO  [ c ] - Compile all .src scripts.
ECHO  [ d ] - Compile all scripts and output to POL\log\ecompile.log
ECHO  [ e ] - Compile all scripts but halt on errors.
ECHO  [ f ] - Compile updated scripts only.
ECHO.
ECHO  [ x ] - Back

SET /p CMD=Command:

IF /i "%CMD%" == "a" GOTO :COMPILE_SCRIPT()
IF /i "%CMD%" == "b" GOTO :COMPILE_DIRECTORY()
IF /i "%CMD%" == "c" GOTO :COMPILE_ALL_SCRIPTS()
IF /i "%CMD%" == "d" GOTO :COMPILE_ALL_SCRIPTS_OPTXT()
IF /i "%CMD%" == "e" GOTO :COMPILE_ALL_SCRIPTS_WITH_HALT()
IF /i "%CMD%" == "f" GOTO :COMPILE_UPDATED_SCRIPTS()
IF /i "%CMD%" == "x" GOTO :QUIT()

ECHO.
ECHO Invalid command.
GOTO :RETURN_TO_MENU()

REM -- RETURN_TO_MENU() FUNCTION
:RETURN_TO_MENU()
PAUSE
GOTO :MENU()

REM -- COMPILE_SCRIPT() FUNCTION
:COMPILE_SCRIPT()
SET /p CMD="Path to script to compile:"
%ECOMPILE_PATH% %CMD%
GOTO RETURN_TO_MENU()

REM -- COMPILE_DIRECTORY() FUNCTION
:COMPILE_DIRECTORY()
SET /p CMD="Path to DIRECTORY:"
%ECOMPILE_PATH% -b -r %CMD%
GOTO RETURN_TO_MENU()

REM -- COMPILE_ALL_SCRIPTS() FUNCTION
:COMPILE_ALL_SCRIPTS()
%ECOMPILE_PATH% -b -A
GOTO RETURN_TO_MENU()

REM -- COMPILE_ALL_SCRIPTS_OPTXT() FUNCTION
:COMPILE_ALL_SCRIPTS_OPTXT()
%ECOMPILE_PATH% -b -A > log\ecompile.log
GOTO RETURN_TO_MENU()

REM -- COMPILE_ALL_SCRIPTS_WITH_HALT() FUNCTION
:COMPILE_ALL_SCRIPTS_WITH_HALT()
%ECOMPILE_PATH% -A
GOTO RETURN_TO_MENU()

REM -- COMPILE_UPDATED_SCRIPTS() FUNCTION
:COMPILE_UPDATED_SCRIPTS()
%ECOMPILE_PATH% -Au
GOTO RETURN_TO_MENU()

REM -- QUIT FUNCTION
:QUIT()
