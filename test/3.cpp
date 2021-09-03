/*
https://courses.cs.washington.edu/courses/cse410/00au/hw/hw6_mutex.html
getchar()影响调试
WaitForMultipleObjects()
    1). 按等待对象(数组等)顺序编号：0，1，2，3,...
    2). FALSE: 返回等待时间内触发对象的编号
        TRUE: 全部对象均触发后返回，返回值参考WaitForSingleObject()
WaitForSingleObject()与Mutex的搭配
*/

#include <windows.h>
#include <stdio.h>
#include <assert.h>

HANDLE mutex = NULL;

//
// Acquire the mutex, wait for 5 seconds, release the mutex, and then exit.
//
unsigned long __stdcall MutexThread(void *threadName)
{
    BOOL success;
    DWORD retVal;

    char *str = (char *)threadName;

    //
    // Acquire the mutex.
    //
    printf("Thread %s trying to acquire the mutex.\n", threadName);
    retVal = WaitForSingleObject(mutex, INFINITE);
    assert(retVal == WAIT_OBJECT_0); // always check for success

    printf("  Thread %s has acquired the mutex.\n", threadName);

    //
    // Sleep for 5 seconds.
    //
    Sleep(1000);

    //
    // Release the mutex.
    //
    printf("    Thread %s about to release the mutex.\n", threadName);
    success = ReleaseMutex(mutex);
    assert(success); // always check for success

    return 0;
}

//
// Main
//
int main()
{
    DWORD threadID;
    DWORD retVal;
    HANDLE threadHandles[4];

    //
    // Create the mutex
    //
    mutex = CreateMutex(NULL,  // no security attributes
                        FALSE, // this thread is not the initial owner
                        NULL   // no name
    );
    assert(mutex != NULL); // always check for success

    //
    // Create four threads that will all try to acquire mutex.
    //
    threadHandles[2] = CreateThread(NULL, 0, MutexThread, (void *)"THREE", 0, &threadID);
    assert(threadHandles[2] != NULL); // always check for success

    threadHandles[3] = CreateThread(NULL, 0, MutexThread, (void *)"FOUR", 0, &threadID);
    assert(threadHandles[3] != NULL); // always check for success

    threadHandles[0] = CreateThread(NULL, 0, MutexThread, (void *)"ONE", CREATE_SUSPENDED, &threadID);
    assert(threadHandles[0] != NULL); // always check for success

    threadHandles[1] = CreateThread(NULL, 0, MutexThread, (void *)"TWO", 0, &threadID);
    assert(threadHandles[1] != NULL); // always check for success

    

    //
    // WaitForMultipleObjects can accept at most MAXIMUM_WAIT_OBJECTS objects.
    //
    int numThreads = 4;
    assert(numThreads <= MAXIMUM_WAIT_OBJECTS);

    //
    // Wait for the four threads to finish
    //
    retVal = WaitForMultipleObjects(4,             // number of threads to wait for
                                    threadHandles, // handles for threads to wait for
                                    FALSE,         // wait for all of the threads
                                    2500           // wait forever
    );

    //
    // If successful, WaitForMultipleObjects returns a value between
    // WAIT_OBJECT_0 and ( WAIT_OBJECT_0 + numThreads - 1).
    //
    // This assert checks if the function was successful.
    //
    assert((retVal >= WAIT_OBJECT_0) && (retVal <= (WAIT_OBJECT_0 + numThreads - 1)));
    printf("%d\n", retVal);
    assert(retVal == WAIT_OBJECT_0);

    printf("\nAll threads have exited.  Press Enter to continue.\n");
    //getchar();
}