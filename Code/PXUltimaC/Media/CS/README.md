# C# (Programming language)

## Interacting with C/C++ DLLs
As the system itself is mostly written in C, we need to interact with it.<br>
There a two ways to do that.

### P/Invoke
The standard way to call functions from an external source like a DynamicLibrary.<br>
A very big downside is. that our program does not know about the required header information for these functions.<br>
Because of that, we are forced to make our own. Even in a sligtly bigger project, this is alot of boilerplate code that should not be required.
Sadly, as C# can't take a use of C/C++ header files, it's a painfully manual process.

### C++/CLI (Common Language Infrastructure)
This middle language is a very great way to mix C, C++ and even C# in the same file.<br>
Other than the P/Invoke aproach, we are directly in the C++ space and have access to the C++ version of the .NET framework.<br>
With this, we can create unmanaged memory and even garbage-collector-managed memory in the same function.<br>
Disappointingly, this language is not supported by other C# enviroments like Mono. Linux is another thing.<br>
At this point in time, this is a neat thing for Windows + VisualStudio only.

## Wrapper into a C-DLL



### Missing delcatations


#### Union
As there is no explicit union type in C#, yet we can still create them.<br>
To create a union, we simply have to do the flowing...

```CS
[StructLayout(LayoutKind.Explicit)] 
public struct UnionExample
{
  [FieldOffset(0)] public bool Flag;
  ...
}
```

