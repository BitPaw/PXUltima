# X86 / X64 - Assembly

## Info
Assembly language mainly used in desktop conputers.

## Structure
<p>
Instructions have a varriable length.<br>
Read the first Byte and then use a lookuptable to decode the correct logic to execute.<br>
If a prefix code is read, we can read the next Byte to decode with another Table.<br>
This allows us to have an unlimited instuction set that can be extended at any point in time.<br>
Ofcourse, Only a limited set is defined to this point.
</p>


# Links
[Intel® 64 and IA-32 Architectures Software
Developer’s Manual](https://cdrdv2.intel.com/v1/dl/getContent/671200)
