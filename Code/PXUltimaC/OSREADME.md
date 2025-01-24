# Operating system (OS)

## Accessing a 32-Bit library from a 64-bit application
Loading a 32-Bit library directly into a 64-Bit application is not generally supported by operating systems due to the difference in architecture.
One solutions to this is to have a seperate 32-Bit application that acts as a proxy.
It would recieve instuctions what library to load and which functions to fetch from our 64-Bit application.
The transport (IPC) of this is often done with a pipe or socket to send and recieve.
An additional way is to use the virtual memory space itself. 
Ofcourse it is importent to remember that 32-Bit applications are only able to use 4GB with a given adress.
This is where we can map our virtual memory to be explicitly be allocated in the lower 2GB region  of memory.


## Windows
The Windows API is a bigger collection of functions that evolved from its beginnings and has not beeing subsceeded by any new format.
Windows exists in 3 main eras

|Bit|Name|
|:-:|:-:|
|16-Bit|3.x|
|32-Bit|NT|
|64-Bit|modern|

## Naming conventions
### Prefix
|Prefix | Name | Example function |
|:-:|:-|:-|
|Cm|Configuration manager|CmRegisterCallbackEx|
|Ex|Executive|ExAllocatePool|
|Hal|Hardware abstraction layer|HalGetAdapter|
|Io|I/O manager|IoAllocateIrp|
|Ke|Kernel core|KeSetEvent|
|Mm|Memory manager|MmUnlockPages|
|Ob|Object manager|ObReferenceObject|
|Po|Power manager|PoSetPowerState|
|Tm|Transaction manager|TmCommitTransaction|
|Nt| New technolegy|NtCreateFile|
|Zw| Does not mean anything.<br>letters used to indicate difference from Nt functions| ZwCreateFile|



### Kernel function name [Nt___() & Zw___()](https://learn.microsoft.com/en-us/windows-hardware/drivers/kernel/using-nt-and-zw-versions-of-the-native-system-services-routines)
Those functions run in KERNEL mode. <br>
The difference between those functions is the source of the input data.<br>
Safetychecks have cost of computational time. If data can be trusted to be valid, a saftly check should be ommited.<br>
Here we use the [PreviousMode](https://learn.microsoft.com/en-us/windows-hardware/drivers/kernel/previousmode) field to indicate from what source we came from.

| | |
|:-|:-|
|Nt___()| The data is from a user and will be checked |
|Zw___()|Data is from a trusted KERNEL source, no checking is done|


## Linux
