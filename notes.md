# philosophers - summary for personal use

• One or more philosophers sit at a round table.
There is a large bowl of spaghetti in the middle of the table.

• The philosophers take turns eating, thinking, and sleeping.
While they are eating, they are not thinking nor sleeping;
while thinking, they are not eating nor sleeping;
and, of course, while sleeping, they are not eating nor thinking.

• There are also forks on the table. There are as many forks as philosophers.

• Since eating spaghetti with just one fork is impractical, a philosopher must pick up

both the fork to their right and the fork to their left before eating.

• When a philosopher has finished eating, they put their forks back on the table and start sleeping. Once awake, they start thinking again. The simulation stops when a philosopher dies of starvation.

• Every philosopher needs to eat and should never starve.

• Philosophers do not communicate with each other.

• Philosophers do not know if another philosopher is about to die.

• Needless to say, philosophers should avoid dying!

---

## requirements.
- filename: Makefile, *.h, *.c, in directory philo/

- args: number_of_philosophers time_to_die time_to_eat
time_to_sleep
[number_of_times_each_philosopher_must_eat]

## permitted functions (14)

memset, printf, malloc, free, write, fork, kill,
exit, pthread_create, pthread_detach, pthread_join,
usleep, gettimeofday, waitpid, sem_open, sem_close,
sem_post, sem_wait, sem_unlink

---

# C Library Functions Reference

## 1. Memory and Standard I/O
* **memset**: `void *memset(void *s, int c, size_t n);`  
    Fills the first `n` bytes of the memory area pointed to by `s` with the constant byte `c`. Commonly used to initialize or clear structures and arrays.
* **printf**: `int printf(const char *format, ...);`  
    Prints formatted text to the standard output (screen).
* **malloc**: `void *malloc(size_t size);`  
    Allocates `size` bytes of uninitialized memory on the heap and returns a pointer to it.
* **free**: `void free(void *ptr);`  
    Deallocates the memory block pointed to by `ptr` which was previously allocated by `malloc`, `calloc`, or `realloc`.
* **write**: `ssize_t write(int fd, const void *buf, size_t count);`  
    A low-level system call that writes up to `count` bytes from the buffer `buf` to the file descriptor `fd`.

## 2. Timing and System
* **usleep**: `int usleep(useconds_t usec);`  
    Suspends execution of the calling thread for (at least) `usec` microseconds.
* **gettimeofday**: `int gettimeofday(struct timeval *tv, struct timezone *tz);`  
    Returns the current time since the Epoch (Jan 1, 1970) in seconds and microseconds.

## 3. POSIX Thread Management
* **pthread_create**: `int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);`  
    Starts a new thread in the calling process.
* **pthread_detach**: `int pthread_detach(pthread_t thread);`  
    Marks the thread identified by `thread` as detached. When a detached thread terminates, its resources are automatically released back to the system.
* **pthread_join**: `int pthread_join(pthread_t thread, void **retval);`  
    Waits for the thread specified by `thread` to terminate. If that thread has already terminated, then it returns immediately.

## 4. Mutexes (Synchronization)
* **pthread_mutex_init**: `int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr);`  
    Initializes a mutex object with specified attributes.
* **pthread_mutex_destroy**: `int pthread_mutex_destroy(pthread_mutex_t *mutex);`  
    Destroys a mutex object, freeing any resources it might be using.
* **pthread_mutex_lock**: `int pthread_mutex_lock(pthread_mutex_t *mutex);`  
    Locks the mutex. If the mutex is already locked, the calling thread blocks until the mutex becomes available.
* **pthread_mutex_unlock**: `int pthread_mutex_unlock(pthread_mutex_t *mutex);`  
    Releases the lock on the specified mutex so that other threads can acquire it.