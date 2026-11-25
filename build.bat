@echo off

if exist build rmdir /s /q build
mkdir build && cd build

echo "[INFO] Building 32-bit version..."

setlocal
set "PATH=C:\msys64\mingw32\bin;%PATH%"
cmake -DBUILD_32BIT=ON -DCMAKE_C_COMPILER=C:/msys64/mingw32/bin/gcc.exe ..
cmake --build .

@REM  if "%1"=="32" (
@REM      echo "[INFO] Building 32-bit version..."
@REM      set "PATH=C:\msys64\mingw32\bin;%PATH%"
@REM      cmake -DBUILD_32BIT=ON -DCMAKE_C_COMPILER=C:/msys64/mingw32/bin/gcc.exe ..
@REM  ) else (
@REM      echo "[INFO] Building 64-bit version (default)..."
@REM      cmake ..
@REM  )

@REM  cmake --build .
