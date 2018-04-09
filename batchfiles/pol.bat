@ECHO OFF

REM -- If a special path is needed to pol.exe set it here
SET POL_PATH=pol.exe
REM ----------

CLS
ECHO POL.bat by Austin
ECHO ========================
ECHO Starting %POL_PATH%
eCHO ========================

%POL_PATH%

ECHO.
ECHO.
ECHO ========================
ECHO POL has finished running
eCHO ========================
ECHO.
PAUSE