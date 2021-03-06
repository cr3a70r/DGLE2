@echo off

:: Global paths

set BIN_WIN_32_PATH=.\..\..\bin\win
set BIN_WIN_64_PATH=.\..\..\bin\win\x64
set SRC_PATH=.\..\..\src

::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: Don't forget to remove junk for every new project! ::
::::::::::::::::::::::::::::::::::::::::::::::::::::::::

:: Engine

del .\engine\DGLE2.APS
call :removeCPPJunk "" ".\engine"

:: Tests

call :removeCPPJunk "tests" ".\tests\dev"

:: Tools

del /A:H .\..\common\tools\*.docstates.suo
del /A:A .\..\common\tools\*.suo
del .\..\common\tools\*.userprefs

call :removeCSharpJunk "tools" "tools\gui_widgets"
call :removeCSharpJunk "tools" "tools\template"

:: Function will clear all temporary Visual Studio files for C++ project.
:removeCPPJunk

set arg1=%1
set arg2=%2

del %BIN_WIN_32_PATH%\%arg1%\*.pdb
del %BIN_WIN_32_PATH%\%arg1%\*.exp
del %BIN_WIN_32_PATH%\%arg1%\*.lib
del %BIN_WIN_32_PATH%\%arg1%\*.ilk
del %BIN_WIN_32_PATH%\%arg1%\log.txt

del %BIN_WIN_64_PATH%\%arg1%\*.pdb
del %BIN_WIN_64_PATH%\%arg1%\*.exp
del %BIN_WIN_64_PATH%\%arg1%\*.lib
del %BIN_WIN_64_PATH%\%arg1%\*.ilk
del %BIN_WIN_64_PATH%\%arg1%\log.txt

del %arg2%\*.sdf
del /A:H %arg2%\*.docstates.suo
del /A:A %arg2%\*.suo
del %arg2%\*.user
del %arg2%\log.txt
rd /s /q %arg2%\ipch
rd /s /q %arg2%\Debug
rd /s /q %arg2%\Release
rd /s /q %arg2%\x64\Debug
rd /s /q %arg2%\x64\Release
rd %arg2%\x64

goto :eof

:: Function will clear all temporary files for C# project.
:removeCSharpJunk

set arg1=%1
set arg2=%2

del %BIN_WIN_32_PATH%\%arg1%\*.pdb
del %BIN_WIN_32_PATH%\%arg1%\*.vshost.exe
del %BIN_WIN_32_PATH%\%arg1%\*.manifest
del %BIN_WIN_32_PATH%\%arg1%\log.txt

del %BIN_WIN_64_PATH%\%arg1%\*.pdb
del %BIN_WIN_64_PATH%\%arg1%\*.vshost.exe
del %BIN_WIN_64_PATH%\%arg1%\*.manifest
del %BIN_WIN_64_PATH%\%arg1%\log.txt

rd /s /q %SRC_PATH%\%arg2%\obj

goto :eof