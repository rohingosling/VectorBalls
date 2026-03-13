@ECHO OFF
ECHO -------------------------------------------------------------------------------
ECHO  Building BALLS.EXE
ECHO -------------------------------------------------------------------------------
ECHO.

IF EXIST BALLS.EXE DEL BALLS.EXE
IF EXIST MAIN.EXE DEL MAIN.EXE

BCC -ml -1 main.c gfx13.c starfld.c vecballs.c > BUILD.LOG
IF ERRORLEVEL 1 GOTO ERROR
IF EXIST MAIN.EXE REN MAIN.EXE BALLS.EXE
IF NOT EXIST BALLS.EXE GOTO ERROR

TYPE BUILD.LOG
ECHO.
ECHO Build successful.
GOTO END

:ERROR
TYPE BUILD.LOG
ECHO.
ECHO *** Build FAILED. See BUILD.LOG for details. ***

:END
