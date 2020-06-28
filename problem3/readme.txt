size  concurrent merge sort     normal merge sort  thread merge sort
1000  0.016s                    0.006s              0.012s
10000 0.102s                    0.012s              0.148s

Clearly, we can see that normal merge sort is faster than concurrent merge sort. This is could be due to transferring the process variables
from parent process to child process. This overhead outweighs any performance improvement due to the concurrency. Also the in the normal merge
sort, the OS optimises some things so that it could get faster. 
 
Ideally, thread merge sort should be faster than concurrent merge sort cause accessing the shared memory using threads is faster but due to some macOS optimisations 
concurrent merge sort is faster than thread merge sort. 
