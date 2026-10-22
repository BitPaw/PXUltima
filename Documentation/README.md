# Documentation

## Banned functions
### Linux
|Avoid|Alternative|Context|
|-:|-:|-:|
|backtrace_symbols() __glibc__| dladdr() <dlfcn.h>| backtrace_symbols is a simpler version but is less precise because of it. |