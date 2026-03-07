
# Event Demuxer

In a Program, often we have a basic/core loop where we 
poll updates ot check states in a loop. 
A Major Problem with this is, that the CPU will poll this
data as fast as it can. With modern pattern detection of 
hardware and Software, this is reduced and does not bring 
the CPU to 100%. This does not hold true to older Hardware!
Modern Hardware can Hide this flaw but the Problem this 
persists, adding a fix is always a good idea. 

## Solution A - Sleep(x)
A first thought solution would be to add a `Sleep(0)` to 
yield execution to other threads, very commonly there
will be no thread Ready at that time, so a `Sleep(1)` 
or more would be required. But this does not quite
sole the Problem, addiionally, while in Sleep our
System cant react, this adds unavoidable latency, not good. 

## Solution B  - WaitForMultipleObjects
A next solution would be to use waitable Events, using an 
Event driven aproach. Windows introduces the function 
[`WaitForMultipleObjects`](https://learn.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-waitformultipleobjects), 
sadly this function is hardcoeded
to a Limit of 64 Elements which makes it impractical 
for higher scaling Problems. Why 64? A design choice,
using a fixed amount avoids dynamic Memory and does
avoid the attached Problems with it. A solution to
this Problem is just solving in 64 sized blocks, which
is not a good solution.

## Solution C - Build-In Threadpool
Was added in `Windows Vista`, `Windows XP` and older aswell as Linux 
dont have it. `Windows 8` fixed Performance choices. 

## Solution D - I/O Compleation ports
`IOCP` was added in `Windows NT 3.5` aswell as [`WaitForMultipleObjects`](https://learn.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-waitformultipleobjects)
and is not limted to anything. More complicated API but
Unlimited scaling. 