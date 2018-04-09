@ECHO OFF

GOTO :MENU()

REM -- MENU FUNCTION
:MENU()
CLS
ECHO CleanUp.bat by Austin
ECHO ========================
ECHO Command        Purpose
ECHO  [ a ] - Remove *.ecl files. (Will need to recompile scripts)
ECHO  [ b ] - Remove *.bak files
ECHO  [ c ] - Remove *.dep files
ECHO  [ d ] - Remove *.log files
ECHO  [ e ] - Remove *.lst files
ECHO  [ f ] - Remove *.dbg files
ECHO  [ g ] - Remove *.dbg.txt
ECHO  [ h ] - Remove other
ECHO.
ECHO  [ x ] - Back

SET /p CMD=Command:

SET REMOVE_TYPE=
IF /i "%CMD%" == "a" SET REMOVE_TYPE=*.ecl
IF /i "%CMD%" == "b" SET REMOVE_TYPE=*.bak
IF /i "%CMD%" == "c" SET REMOVE_TYPE=*.dep
IF /i "%CMD%" == "d" SET REMOVE_TYPE=*.log
IF /i "%CMD%" == "e" SET REMOVE_TYPE=*.lst
IF /i "%CMD%" == "f" SET REMOVE_TYPE=*.dbg
IF /i "%CMD%" == "g" SET REMOVE_TYPE=*.dbg.txt
IF /i "%CMD%" == "h" GOTO DELETE_CUSTOM()
IF /i "%CMD%" == "x" GOTO :QUIT()

IF NOT "%REMOVE_TYPE%"=="" GOTO REMOVE()

ECHO.
ECHO Invalid command.
GOTO :RETURN_TO_MENU()

REM -- RETURN_TO_MENU() FUNCTION
:RETURN_TO_MENU()
PAUSE
GOTO :MENU()

REM -- REMOVE FUNCTION()
:REMOVE()
IF NOT "%REMOVE_TYPE%"=="" DEL /S %REMOVE_TYPE%
GOTO RETURN_TO_MENU()

REM -- DELETE_CUSTOM FUNCTION()
:DELETE_CUSTOM()
SET /p REMOVE_TYPE=File type to remove (e.g. *.bak or orphans.txt):
GOTO REMOVE()

REM -- QUIT() FUNCTION
:QUIT()