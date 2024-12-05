# NET-Framework (.NET)
## Info
Framework used to compile and run C#, VisualBasic.NET and CLI/C++ scripts or librarys.

## Linux
As the .NET Framework is inheritally a Windows only element, due to its popularity there are versions for linux.
Although it beeing very confusing, as there are many different versions.

## Windows
Under "C:/Windows/System32/" we have the "MSCorEE.dll", if the .NET Framework is installed.<br>
We can link (static or dynamic) to this file by using [LoadLibrary](https://learn.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-loadlibrarya)
and call [CLRCreateInstance](https://learn.microsoft.com/en-us/dotnet/framework/unmanaged-api/hosting/clrcreateinstance-function) 
and recieve a [ICLRMetaHost](https://learn.microsoft.com/en-us/dotnet/framework/unmanaged-api/hosting/iclrmetahost-interface) object from it.
The documentation states that CLRCreateInstance and CreateInstance is the same, they are just an alias. 

```C
#include <metahost.h>
#pragma comment(lib, "mscoree.lib")

or

const HMODULE hModule = LoadLibrary(L"mscoree.dll");
const CreateInterfaceFnPtr createInterface = (CreateInterfaceFnPtr)GetProcAddress(hModule, "CreateInterface");
const HRESULT resA = createInterface(CLSID_CLRMetaHost,       IID_ICLRMetaHost,       (LPVOID*)&pMetaHost);
const HRESULT resB = createInterface(CLSID_CLRMetaHostPolicy, IID_ICLRMetaHostPolicy, (LPVOID*)&pMetaHostPolicy);
const HRESULT resC = createInterface(CLSID_CLRDebugging,      IID_ICLRDebugging,      (LPVOID*)&pCLRDebugging);
```

- Next we call the [GetRuntime](https://learn.microsoft.com/en-us/dotnet/framework/unmanaged-api/hosting/iclrmetahost-getruntime-method) method 
of the object to get the [ICLRRuntimeInfo](https://learn.microsoft.com/en-us/dotnet/framework/unmanaged-api/hosting/iclrruntimeinfo-interface) object.<br>
We can check whether the version is loadable by calling [ICLRRuntimeInfo.IsLoadable](https://learn.microsoft.com/en-us/dotnet/framework/unmanaged-api/hosting/iclrruntimeinfo-isloadable-method)
- Call the [GetInterface](https://learn.microsoft.com/en-us/dotnet/framework/unmanaged-api/hosting/iclrruntimeinfo-getinterface-method) method
from the ICLRRuntimeInfo to get the [ICorRuntimeHost](https://learn.microsoft.com/en-us/dotnet/framework/unmanaged-api/hosting/iclrruntimehost-interface)
- Call the [Start](https://learn.microsoft.com/en-us/dotnet/framework/unmanaged-api/hosting/iclrruntimehost-start-method) method
of the ICorRuntimeHost object
- Call the [GetDefaultDomain]() method
from the ICorRuntimeHost object to get the IAppDomain object
- Then you can load libraries using IAppDomain.Load_2.
- If you want to load .NET DLLs from network shares it is more complex, because
you need to call UnsafeLoadFrom, which is not available in IAppDomain. But this is also possible.


## Alternative
Because of the missing implementation on linux in the beginnning, a company created a whole .NET framework that is platform independed.
[mono-project](https://www.mono-project.com/)


## Links
- [codeproject - C++ to C#](https://www.codeproject.com/KB/cs/unmanagedtomanaged.aspx)
- [microsoft - CLRCreateInstance()](https://learn.microsoft.com/de-de/dotnet/framework/unmanaged-api/hosting/clrcreateinstance-function)
- [github - example](https://gist.github.com/xpn/e95a62c6afcf06ede52568fcd8187cc2)
- [medium - lewiscomstive - C# in C/C++](https://medium.com/@lewiscomstive/how-to-embed-c-scripting-into-your-c-application-782b2e57245a)
