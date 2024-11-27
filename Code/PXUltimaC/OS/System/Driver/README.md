# Driver
## Info
A driver is most often a kernel level software that comunicates directly with a device on a very low level.<br>
Most interactions over a driver happen with I/O requests or simple fetching or writing of signals to comunicate with the device.

Why does a driver have such high permissions? <br>
There are some processor instructions that can only be executed in KERNEL mode.<br>
If an application would execute those instucions, even in admin/sudo mode, the processor would block the execution and stop the process.

---

### Linux
ToDo...

---

### Windows
Windows drivers consist of 3 types of files:
- .dll, the actual code
- .inf, info file on how to install the driver with addtitional details
- .cat, verification of the driver (optional security)

So, a driver is a mix of a library and a program that is running in the background (service).

#### Links
[WinRing0](https://github.com/GermanAizek/WinRing0/tree/master)

---
