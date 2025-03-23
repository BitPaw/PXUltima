# Debugging

## Debugging under Linux
### Librarys
```C
#include <sys/ptrace>

long ptrace(enum __ptrace_request request, pid_t pid, void *addr, void *data);
```

### Using gdb
...



## Debugging under Windows
### Librarys
```C
#include <windows.h> // Important! You need to include "windows.h" before other windows headers.
#include <dbghelp.h>
```

## Functions
|Name| Windows | linux | Use |
|:-|-|-|:-|
|Debugging Attach| DebugActiveProcess() | ptrace(PTRACE_SEIZE  , ) |
|Debugging Dettach| xxxx| ptrace(PTRACE_DETACH  , ) |
|Debugging Start| xxxx| xxxx|
|Debuger Stop| DebugActiveProcessStop() | ptrace(PTRACE_INTERRUPT    , ) |
|Wait for event| WaitForDebugEvent() | ptrace(PTRACE_GETEVENTMSG   , | Use this in a loop to detect events| 
|Debugging Continue| ContinueDebugEvent() | ptrace(PTRACE_CONT   , )|
|Debug Break| DebugBreak() | xxxx| If there is problem, call this function. The debugger will stop there| 
|DataRead| xxxx| xxxx|
|DataWrite| xxxx| ptrace(PTRACE_POKEDATA, ) |
|DebugStopReasonGet| xxxx| ptrace(PTRACE_GETSIGINFO , ) |
|DebugStopReasoSetn| xxxx| ptrace(PTRACE_SETSIGINFO   , ) |
|Debug Event info| xxxx| ptrace(PTRACE_GET_SYSCALL_INFO    , ) |
|Debuger StepSingle| xxxx| ptrace(PTRACE_SINGLESTEP   , ) |
|StackTrace| StackWalk64() | backtrace()<br>backtrace_symbols() |

### Links
- [Linux pthread](https://man7.org/linux/man-pages/man2/ptrace.2.html)
- [valgrind](https://valgrind.org/)
- [Windows dbghelp.h](https://learn.microsoft.com/de-de/windows/win32/debug/dbghelp-functions)
