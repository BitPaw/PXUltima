# Debugging

## Debugging under Linux
### Librarys
```C
#include <sys/ptrace>
```

### Using gdb
...



## Debugging under Windows
### Librarys
```C
#include <windows.h> // Important! You need to include "windows.h" before other windows headers.
#include <dbghelp.h>
```


### Links
- Linux pthread : https://man7.org/linux/man-pages/man2/ptrace.2.html
- Windows dbghelp.h - https://learn.microsoft.com/de-de/windows/win32/debug/dbghelp-functions
