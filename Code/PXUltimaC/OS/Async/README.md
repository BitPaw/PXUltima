# Asynchronous
## Info
Asynchronous code execution (ASYNC) is a term used to define code that can run in parralel to the main executing program flow.
This does not need to mean that those paths are actually running at the same time,
they can finish at different times as they are independed while executing their own code.
This is how computers with one single processor core can still multitask, even though it is 
actually not at the same time. With more cores we can actually do stuff in parralel and each code can do the same and scale even more.

## Threads
Threads, the lines of parralel programm execution paths allow programms to 
execute slow code beside the main thread to reduce latency from the main program loop.

## Locks (Semaphore, Mutex, CriticalSection)
A Threading-Lock is a mechanism to prevent parralel execution by exclusion.<br>
Only one thread may execute the code in a given section at a time.<br>
When a thread enters a section, every other thread that tryes to enter this section will be suspended until the first thread leaves the section.
Then, the secound thread will proceed, one at a time. 
This way of waiting is also called a spinlock. An obvious downside is the time we wait, in this time we 
can't do anything and just IDLE: Because of this, locks should always used spareingly and with care about this aspect.
The difference in locks
|Name|Overhead|When to use..|
|:-|:-|:-|
|Semaphore|Higher| Can be called at any time from any place. Bigger overhead.|
|Mutex|Moderate|Can only be used on the thread it was created from (Managed locking).<br>Use when multible processes are involved.|
|CriticalSection|Lower|Can only be used inside a procees not across processes, faster because of it.|

## Threadpools
A Threadspool can be seen as an array of threads, a fixed amount of Threads, that are created once and can 
be used for a task. After executing, the thread is not destroyed but marked as "free" to be used for another task.
The advantage is in recyceling the objects, instead of creating and destroying them repeatetly.
### Design
- An array of threads. The initial size is the amount of processor cores.
- Tasks need to have a binding to a spesific thread. Only that thread may execute that task. Some APIs enforce this behaviour.
- Tasks can be compleated but also a repeating task. The tasks stays in queue as long as desired.
- The mein thread needs to be part of this pool, jet shall not count to the amount. As some tasks might be only callable from the main thread.
- If workload is low, threads shall be suspended to not waste CPU cycles.
