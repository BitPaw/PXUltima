# Action Result
## Info
<p>
Most modern programmers use exceptions to handle faults in the code.<br>
But if not those, if we dont have them to use, we use errorIDs.
</p>

<p>
  Those errorIDs are just an interger that define some spesific problem.<br>
  The disadvantage is, because the ID is just a number, that only a hard defined value<br> 
  can be used, where exceptions are more expressive and can yield vastly more detailed info.<br>
  This can be done by creating custom exceptions or adding strings or other elements to improve the context of that particular problem. 
</p>

## Links
### Linux
- [Error Get - errno](https://www.man7.org/linux/man-pages/man3/errno.3.html) (POSIX)
- [Error Message - strerror](https://www.man7.org/linux/man-pages/man3/strerror.3.html)

### Windows
- [Error Get - GetLastError](https://learn.microsoft.com/en-us/windows/win32/api/errhandlingapi/nf-errhandlingapi-getlasterror)
- [Error Message - FormatMessage](https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-formatmessagea)
- [Windows extended error codes](https://learn.microsoft.com/en-us/windows/win32/debug/system-error-codes)
