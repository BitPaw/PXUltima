# Memory
## Info
### Allignment - The 4-Byte rule
Current processors can work more effectivly when data is alligned on memory that is a multible of 4 Bytes.<br>
This has something to do with how the processor internally uses adresses and how it calculates offsets with them. 

### Allignment - Memory pages
The same way code and data is better alligned, the same goes for the main memory.<br>
Here the magic number is 2^12 Bytes, 4096 Bytes, 4KB.<br>
So if you want to allocate some memory, it could happen that even a 1 byte allocation can trigger 4KB to be reserved, as this is the smalest element the memory can reserve for you. Ofcourse this is wasted space, you can still utilize every single byte on that page. Be aware that one byte more than this page will trigger another full page to be loaded.

### The paging file
The paging file is a file sitting on the main harddrive but can also be on others or even disabled.<br>
When we understand memory pages, the paging file seems very simple.<br>
Pages where the entire page of memory blocks are not used can be moved into this file. This also applyes if the physical memory is too smal and we need to swap pages from or to the drive when we dont need them right now. This is not a fast solution but can greatly prevent a system from crashing because we run out of memory. With this solution, the system would slow down imensly but would still be running, as we can utilize the harddrive as memory buffer. 

### Bigger memory pages
Each entry of a normal memory page will cost you in internal page management.<br>
The cost of managing a handful of pages is irrelevant but given big enough memory blocks, there can be a better way.
Large pages were invented to boost performance and reduce redundand page information and resulting managing of them.
Internally, we combine 512 Pages that are 4KB each into a 2MB large page. The performance gain here comes from the reduced management overhead. As we only care about one single page instead of 512, we now can predice with certenty that you will probably use those exact 512 pages. If we didnt have one, the processor might move out needed memory out of the processors cache by mistake. This will result in cache misses and can slow down the program.<br><br>
Additionlly, there is even a bigger page than a large page, a huge page.<br>
Those are, the same scaling like large pages, now 512 times bigger than large pages. This results in a size of 1GB.<br>
Considering such a big size, the use of such large memory blocks might be not for every usecase but looking at every program that might need to shew through multible gigabytes of raw data, this might help. Examples could be Video editing, 3D modeling of very complex structures or even citys or big databases.

## Functions
|Name|Description|Windows|Linux(POSIX)|
|:-|:-|:-:|:-:|
|Memory allocate|Claim a block of memory| HeapAlloc | malloc|
|Memory allocate cleared|Claim a block of memory and clear the data to zero| HeapAlloc* | calloc|
|Memory reallocate|Resize a previous allocated block. If the space is not suficent, data is moved to a different place| HeapReAlloc | realloc|
|Memory free|Release a block of memory| HeapFree | free|
|Memory allocations list all|List all current allocations residing on the heap| HeapWalk | -|
|Memory Clear| Set a given range to a zero byte. | ZeroMemory | memset|
|Memory Set|Set a given range to a specified byte.| FillMemory | memset|
|Memory Compare|Compares a given range of bytes.| - |memcmp|
| Memory Copy|Copys a given range of bytes into another.<br>Note, this function will corrupt the data if the buffers overlap.<br>If you use uverlapping buffers or copying in the same buffer, use the MemoryMove() function.|CopyMemory | memcpy|
|Memory Move|Copys a given range of bytes into another while working like you'd coped them into a temporal buffer first.|MoveMemory  | memmove|
|Virtual Memory|Many operating systems noticed problems using direct physical memory adressing.<bs>A solution was, to create a virtal memory space for each process, so that other processes interfeere less with our own memory space.<br>Additionally, we got the bonus that shared memory mostly of librarys can reduce physical memory usage by alot.| VirtualAlloc |mmap|
|Memory virtual release| Releases the virtual memory block|VirtualFree| unmap|
|Memory protect|Set perimissions on a memory block. Can be used to prevent unwated read, writes or executes. May even alow it| VirtualProtect | madvice|
|Memory mapping|Load a file into the virtual memory space. Allows to load files vastly bigger than the actual physical memory but still access them liniarly as an array. The Operating system handles the file I/O behind the scenes | CreateFileMapping/CreateFileMapping | mmap|
|Memory unmapping|Closes the mapping| CloseHandle/UnmapViewOfFile | unmmap|

