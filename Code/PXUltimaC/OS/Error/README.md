# Action Result (errno)<br>(POSIX)
## Info
<p>
Most modern programmers use exceptions to handle faults in the code.<br>
But if not those, if we dont have them to use, we use errorIDs.
</p>

<p>
  Those errorIDs are just an interger that define some spesific problem.<br>
  The disadvantage is, that only a hard defined value can be used, where exceptions can hold ald yield vastly more detailed info.
</p>

## Links
### Linux
- [Error Get - errno](https://www.man7.org/linux/man-pages/man3/errno.3.html)
- [Error Message - strerror](https://www.man7.org/linux/man-pages/man3/strerror.3.html)

### Windows
- [GetLastError](https://learn.microsoft.com/en-us/windows/win32/api/errhandlingapi/nf-errhandlingapi-getlasterror)
- [FormatMessage](https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-formatmessagea)
