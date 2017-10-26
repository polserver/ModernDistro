@ECHO OFF

REM -- If a special path is needed to the batch files set it here
SET BATCH_PATH=batchfiles\
REM ----------

GOTO :INTRODUCTION()

GOTO :MENU()

REM -- INTRODUCTION FUNCTION
:INTRODUCTION()
ECHO THE 099 DISTRO IS CURRENTLY IN DEVELOPMENT.
ECHO THIS IS NOT A FINISHED PROJECT.
ECHO.
ECHO POST ANY ISSUES YOU HAVE TO THE DISTRO FORUM AT
ECHO HTTP://WWW.POLSERVER.COM
PAUSE

REM -- MENU FUNCTION
:MENU()
CLS
ECHO StartHere.bat by Austin
ECHO ========================
ECHO Command        Purpose
ECHO  [ a ] - RealmGen menu        (Realm building tools)
ECHO  [ b ] - Ecompiler menu       (Ecompile tools)
ECHO  [ c ] - Cleanup menu         (File removal tools)
ECHO.
ECHO  [ d ] - Start POL.exe        (Returns to menu on exit)
ECHO  [ e ] - Keep POL.exe running (Restarts when it exits. Use CTRL+C to stop)
ECHO.
ECHO  [ x ] - Quit

SET /p CMD=Command:

IF /i "%CMD%" == "a" GOTO :REALM_GEN()
IF /i "%CMD%" == "b" GOTO :ECOMPILE()
IF /i "%CMD%" == "c" GOTO :CLEANUP()
IF /i "%CMD%" == "d" GOTO :POL()
IF /i "%CMD%" == "e" GOTO :POL_LOOP()
IF /i "%CMD%" == "x" GOTO :QUIT()

ECHO.
ECHO Invalid command.
GOTO :RETURN_TO_MENU()

REM -- RETURN_TO_MENU() FUNCTION
:RETURN_TO_MENU()
GOTO :MENU()

REM -- REALM_GEN() FUNCTION
:REALM_GEN()
CALL %BATCH_PATH%realmgen.bat
GOTO RETURN_TO_MENU()

REM -- ECOMPILE() FUNCTION
:ECOMPILE()
CALL %BATCH_PATH%ecompile.bat
GOTO :RETURN_TO_MENU()

REM -- CLEANUP() FUNCTION
:CLEANUP()
CALL %BATCH_PATH%cleanup.bat
GOTO :RETURN_TO_MENU()

REM -- POL() FUNCTION
:POL()
CALL %BATCH_PATH%pol.bat
GOTO :RETURN_TO_MENU()

REM -- ECOMPILE() FUNCTION
:POL_LOOP()
CALL %BATCH_PATH%loopPOL.bat
GOTO :RETURN_TO_MENU()

REM -- QUIT FUNCTION
:QUIT()