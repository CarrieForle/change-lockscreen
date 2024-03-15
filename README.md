# ChangeLockscreen

A Windows daemon to change lockscreen every time the computer is locked and start up.  
This repo is being tested and developed on Windows 11.

## Build

```powershell
cd ChangeLockscreen
g++ main.cpp -o main.exe -fdiagnostics-color=always --std=c++23 -lwtsapi32 -lOle32 -municode
```

## Run
```powershell
.\main.exe
```