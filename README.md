# (WIP) ChangeLockscreen

A Windows daemon to change lockscreen every time the computer is locked and start up.  
This repo is being tested and developed on Windows 11 64 bit with [winlibs MinGW-w64](https://github.com/brechtsanders/winlibs_mingw).  
Download [Windows SDK](https://developer.microsoft.com/en-us/windows/downloads/windows-sdk/) to contribute.  
  
## Start-up

You should run the daemon on start-up. Here's how you can do it.   

Put the daemon in somewhere persistent.  
Create a shortcut (symlink) to the daemon.  
Press `Win + R` and run `shell:startup`. A folder should be opened in explorer.  
Put the shortcut in the said folder.  

## Build

```powershell
cd change-lockscreen
g++ src/main.cpp src/ErrorMessageBox.cpp src/ChangeLockscreenDataBuilder.cpp -o out/change-lockscreen.exe --std=c++23 -lwtsapi32 -municode -mwindows
```

## Run
```powershell
.\run.bat
```