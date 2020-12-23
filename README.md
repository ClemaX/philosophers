## POSIX Threads
A thread is a procedure that is executed in parallel to the process's main thread.

By default, processes that are cloned using fork() or clone() do not share the same memory space.
On the other hand, threads remain within the main process and share the same memory space, which enables parallel computing.

## Shared Memory
Each thread can be passed a pointer to some (potentially private) data.
This could be the thread's identity or a pointer to some shared memory space, which contains data to be computed concurrently.

![Shared Memory][shared-mem]

Sharing memory is very efficient, but writing to a shared memory space can be a bit tedious.

If the memory is accessed and written to by multiple threads at the same time, the data being read is in an undefined state, since memory changes are not perfectly instantaneous.

## Mutexes
Mutexes provide mutual exclusion over shared resources, as they can be locked and unlocked.

Locking guarantees that nobody else can access the resource at the same time. This means that if two threads try to lock the same mutex concurrently, one of them will have to wait until the other thread has unlocked the mutex again.

If multiple mutexes are to be acquired by a thread, a global order has to be provided in order to prevent deadlocks.

### Deadlocks
Deadlocks happen when multiple threads try to acquire multiple locks at the same time.

[shared-mem]: https://computing.llnl.gov/tutorials/pthreads/images/sharedMemoryModel.gif "Shared Memory in POSIX threads"
