# (WIP) ChangeLockscreen

A Windows daemon to change lockscreen every time the computer is locked and start up.  
  
This repo is being tested and developed on Windows 11.  
Download [Windows SDK](https://developer.microsoft.com/en-us/windows/downloads/windows-sdk/) to contribute.  

## Build

```powershell
cd change-lockscreen
g++ main.cpp -o main.exe -fdiagnostics-color=always --std=c++23 -lwtsapi32 -lOle32 -municode
```

## Run
```powershell
run
```