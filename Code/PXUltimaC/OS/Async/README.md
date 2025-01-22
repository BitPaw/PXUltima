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

## Threadpools
Threadspools are just a "pool", a fixed amount of Threads, that are created beforehand and can 
be used directly for a task. After executing the thread is not destroyed but marked as free to be used for another task.
The advantage is in recyceling the objects instead of creating and destroying them repeatetly.
