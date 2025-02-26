# Microsoft Visual Compiler (MSVC)
## Info
This compiler is installed with Visual Studio and used by default.

## Locations/Directorys
|Path| Description|
|:-|:-|
|C:/Program Files/Microsoft Visual Studio/2022/\*/|Default installation folder|
|C:/Program Files (x86)/Windows Kits/10/Include/\*/|Common C Headers|
|C:/Program Files (x86)/Windows Kits/10/Lib/\*/um/\*/|Common C Librarys|

## Commands
As far as i've looked, there is no way to directly use the compiler except to call it as a programm over the [command line (CMD)](). <br>
To execute those command inside a function, you can either call ```system()```, ```popen```  or ```CreateProcess()```. 

Compile (Code to Objects)
```CMD
cl.exe /I"LibrayA.lib" /I"LibrayB.lib" /c FileC.C FileD.C
```

Compile (Code/Objects to executable (EXE)
```CMD
cl.exe /I"LibrayA.lib" /I"LibrayB.lib" /c FileC.C FileD.C /FeMyExecutableName.exe
```

Compile (Code/Objects to dynamic library (DLL))
```CMD
cl.exe /LD /I"LibrayA.lib" /I"LibrayB.lib" /c FileC.C FileD.C /FeMyExecutableName.exe
```

Link (Code/Objects to static library (LIB))
```CMD
lib.exe /out:MyLibraryName.lib /I"LibrayA.lib" /I"LibrayB.lib"
```
