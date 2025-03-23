# C# or C-Sharp (Programming language)

## Interacting with C/C++ DLLs
As the system itself is mostly written in C, we need to interact with it.<br>
There a two ways to do that.

### P/Invoke (Platform invoke)
The standard way to call functions from an external source like a DynamicLibrary.<br>
A very big downside is. that our program does not know about the required header information for these functions.<br>
Because of that, we are forced to make our own. Even in a sligtly bigger project, this is alot of boilerplate code that should not be required.
Sadly, as C# can't take a use of C/C++ header files, it's a painfully manual process.

### C++/CLI (Common Language Infrastructure)
This middle language is a very great way to mix C, C++ and even C# in the same file.<br>
Other than the P/Invoke aproach, we are directly in the C++ space and have access to the C++ version of the .NET framework.<br>
With this, we can create unmanaged memory and even garbage-collector-managed memory in the same function.<br>
Disappointingly, this language is not supported by other C# enviroments like Mono. Linux is another thing.<br>
At this point in time, this is a neat thing for Windows in VisualStudio only.

## Wrapper into a C-DLL with P/Invoke
### Calling functions (C# -> C) (P/Invoke)
<p>
To call a C-Function from a pure C# enviromen, you are required to already have a C-DLL to link to.<br>
Additionally, you need to rewrite most of the header information from the C files.<br>
As these can't be generated easily, you need to write them by hand or write a generator for it.
</p>
<p>
 Best practise is, to wrap all C-Function-Calls into a class and make them private or internal if they are needed in this project somewhere else.  
 </p>
 
```CS
using System.Runtime.InteropServices; // To use 'DllImport'

public class MyCFunctionWrapper
{ 
  [DllImport("MyCDLL.dll", CallingConvention = CallingConvention.StdCall)]
  private static extern int MyFunctionFromC(int x);
  
  public int MyFunctionFromCS(int x)
  {
    return MyFunctionFromC(x);
  }
}
```

### Calling functions (C# <- C) (reverse P/Invoke)
<p>
Same as the above example with P/Invoke, a C-DLL is required.<br>
To make the C-DLL able to call a C#-function we dont need to change the C-DLL at all.<br>
All we need to do is to create a C#-Delegate, that is tecnically just a more powerful function pointer.<br>
Create one with the EXACT same signature, this is very important, as a smal error can lead to crashes.
</p>

As a delegate is a 'managed' object and NOT a function pointer, we have to extract the real function pointer.<br>
We need to use the [Marshal.GetFunctionPointerForDelegate()](https://learn.microsoft.com/en-us/dotnet/api/system.runtime.interopservices.marshal.getfunctionpointerfordelegate?view=net-7.0) function.


```CS
// Example delegate
[UnmanagedFunctionPointer(CallingConvention.StdCall)] 
internal delegate void MyDelegate(int number);

// Requires no additional 'unsafe' signature
IntPtr adress = Marshal.GetFunctionPointerForDelegate<***DELEGATE***>(***FUNCTION***);

// Requires additional 'unsafe' signature
void* adress = Marshal.GetFunctionPointerForDelegate<***DELEGATE***>(***FUNCTION***).ToPointer();
```


---


### Structs (C# -> C)
As C, like C++ and other similar compiled languages, has no runtime information about structs or classes, there is no direct way to pass the structure of a struct or class to C.
To still get simmilar behavour, you would need to build a reflection system into the C-DLL to then use it and pass to it by created functions and defined structs in the C-DLL.

### Structs (C# <- C)
Sequential size, can be used to have an 'empty' object with a fixed size
```CS
[StructLayout(LayoutKind.Sequential, Size = 160)]
private unsafe struct MyCStruct
{
  // Nothing delcared here
}
```
C# does not know the layout but C does. Functions will work as expected.<br>
The downside is, that you can only interact over functions like getters and setters.

Explicit size, required for UNIONs.
```CS
[StructLayout(LayoutKind.Explicit, Size = 420)]
internal unsafe struct MyCStruct
{
  // offset 0, first element
  [FieldOffset(0)] public ulong Size; // ulong = 64Bit = 8 Byte
  
  // offset 8, from the field before.
  // both have the same offset, meaning they are placed on the same adress
  // this is the functionality of a C-Union
  [FieldOffset(8)] public byte* TextASCII;
  [FieldOffset(8)] public char* TextUNICODE;
}
```
---

### Passing structs (C# -> C) 
If we want to pass a struct to a function, in C we would solve this by taking the address of this object.<br>
Here in C#, this is not quite as easy, as we would need to mark every 'DllImport' as unsafe.<br> 
Also, for every function, we would need to use the 'fixed' keyword to be allowed to take addresses.<br>
This using of fixed would add the unsafe keyword for every function. Luckily, there is a better option.

Instead of taking the adress, we can singify with the keywork 'ref' for reference that we want to pass it by reference.<br>
This will be compiled to a 'void*' as it is tecnically the same, although it is 'safer' as a forced to provide the object, not just any adress.
```CS
// Declaration with 'ref' instead of 'void*'
[DllImport("MyCDLL.dll")] private static extern void DoSomethingWithStruct(ref MyStruct myStruct);

// Declaration
MyStruct myStruct = new MyStruct(); 

// Call, addtional 'ref' needed instead of '&' to take address
DoSomethingWithStruct(ref myStruct);
```

### Passing structs (C# <- C) 
As you pass structs via poniter, the only requirement is that the function and types have matching signatures.

---

### String (C# -> C) 
```CS
// Note, as we use the 'fixed' keyword to take an adress, we need to mark this function as 'unsafe'.
public unsafe ActionResult SendData(string message) 
{
  // As a char* is 16-Bit in C#, we need to keep in mind that this format is UNICODE not ASCII.
  fixed (char* messageAdress = message.ToCharArray())
  {
    int totalByteSize = message.Length * sizeof(char); // Length * 2, as sizeof(char) = 2
    
    // use 'messageAdress' and 'totalByteSize' here
  }
}
```


### String (C# <- C) 
```CS
// Allocate on the stack or dynamically create with new
sbyte* buffer = stackalloc sbyte[64];

// Fill up buffer with function
int howManyBytesWritten = MyStringFunctionFromCDLL(buffer, 64);       

// Construct C#-string with C-string buffer
string myConvertedString = new string(buffer, 0, howManyBytesWritten);
```

---
