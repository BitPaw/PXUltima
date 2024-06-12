# Hardware
## Sensors and performance counters
### Linux

<p>
Linux stores sensor data inside fake files.<br>For the user it looks like files but they are generated on opening by the kernel.<br>
This is why, when reading the file, we are forced to open, read and close the file repeatitly to generate the data we require.
</p>

> Warning, these paths are not defined as standard. They change frequently.

CPU Temperature can be read from a pipe located at:
```Bash
cat /sys/class/termal/.../type
```
### Windows

Since Windows 2000 we have the [Windows Management Instrumentation (WMI)](https://learn.microsoft.com/en-us/windows/win32/wmisdk/wmi-start-page).<br>
This is a database that holds all data that the system collects.
[Tutorial](https://learn.microsoft.com/en-us/windows/win32/wmisdk/accessing-performance-data-in-c--)

```C
#include <wbemidi.h>
#include <wbemcli.h>
#pragma comment(lib "wbemuuid.lib")
```

---





## HardDrive
### Linux
### Windows
Since the DOS days, we use letters to specify drives.<br>This will result in a maximum of 26 drives (A-Z).

---
