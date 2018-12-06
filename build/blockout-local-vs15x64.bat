@echo off
REM ~ Warning: Hardcoded paths! Change to your own if you use this
REM ~
REM ~ This file is temporarily here.
REM ~ Later I will devise a way to properly generate the projects from a script
if not exist vs15x64 mkdir vs15x64
cd vs15x64
cmake ^
    ../.. ^
    -G "Visual Studio 15 2017 Win64" ^
    -DWITH_LOCAL_MAGNUM=1 ^
    -DWITH_SDL2APPLICATION=1 ^
    -DCMAKE_PREFIX_PATH="D:/Projects/magnum/vendor/SDL2-2.0.9" ^
    -DCMAKE_RUNTIME_OUTPUT_DIRECTORY="D:/Projects/magnum/ibob/bin"
