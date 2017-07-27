@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\Tools\VsDevCmd.bat"
set compilerflags=/Od /Zi /EHsc /TC
set linkerflags=/OUT:..\assignment6.exe
cl.exe %compilerflags% ..\*.c /link %linkerflags%