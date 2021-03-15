%~d0
cd %~dp0
cd ..
call vendor\bin\premake\premake5.exe vs2019 --hot-reload
PAUSE