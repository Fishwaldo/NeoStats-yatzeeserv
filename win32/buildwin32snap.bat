@echo off
IF EXIST win32\YahtzeeServ-Setup*.exe del win32\YahtzeeServ-Setup*.exe
"C:\Program Files\Pantaray\QSetup\Composer.exe" win32\YahtzeeServ.qsp /Compile /Exit
move win32\YahtzeeServ-Setup.exe win32\YahtzeeServ-Setup-3-0-1-%1.exe
