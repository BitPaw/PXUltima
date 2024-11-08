# FireWall

## Code
### Linux
```
sudo apt-get install iptables-dev
```
```C
#include <libiptc/libiptc.h>

#pragma comment(lib, "iptc.so")
```
### Windows
```C
#include <netfw.h>

// Does not have a library, because this is an interface fetched by "CoCreateInstance()" 
```


## Links
[Microsoft.com](https://learn.microsoft.com/en-us/previous-versions/windows/desktop/ics/c-adding-a-lan-rule)
