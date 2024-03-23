# (WIP) ChangeLockscreen

A Windows daemon to change lockscreen every time the computer is locked and start up.  
This repo is being tested and developed on Windows 11 64 bit with [winlibs MinGW-w64](https://github.com/brechtsanders/winlibs_mingw).  
Download [Windows SDK](https://developer.microsoft.com/en-us/windows/downloads/windows-sdk/) to contribute.  

## Build

```powershell
cd change-lockscreen
g++ main.cpp -o out/main.exe --std=c++23 -lwtsapi32 -municode -mwindows
```

## Run
```powershell
.\run
```