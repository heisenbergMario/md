/*
https://zhuanlan.zhihu.com/p/353502657
https://blog.csdn.net/luliyuan/article/details/40542489?utm_medium=distribute.pc_relevant_t0.none-task-blog-2%7Edefault%7EBlogCommendFromMachineLearnPai2%7Edefault-1.essearch_pc_relevant&depth_1-utm_source=distribute.pc_relevant_t0.none-task-blog-2%7Edefault%7EBlogCommendFromMachineLearnPai2%7Edefault-1.essearch_pc_relevant
WaitForSingleObject() return -1: WAIT_FAILED
*/

#include <windows.h>
#include <iostream>
//#include <synchapi.h>
#include <tchar.h> // ??   Mbctype.h and Mbstring.h  https://docs.microsoft.com/en-us/previous-versions/5z097dxa(v=vs.140)
#include <Mbctype.h>
#include <Mbstring.h>
//#include <wtypes.h> // wtypes.h
//#include <avrt.h>

DWORD WINAPI ThreadProc1(LPVOID lpParameter);
DWORD WINAPI ThreadProc2(LPVOID lpParameter);
WCHAR *cacheMutexName = L"HelloWorldMutex";
int ticket = 500;
HANDLE handle = NULL;

DWORD WINAPI ThreadProc1(LPVOID lpParameter)
{
    handle = ::CreateMutexW(nullptr, FALSE, cacheMutexName);
    while (true)
    {
        WaitForSingleObject(handle, INFINITE);
        if (ticket > 0)
        {
            Sleep(1);
            printf("thread1 sale the ticket id is: %d\n", ticket--);
        }
        else
        {
            break;
        }
        //ReleaseMutex(handle);
    }
    return 0;
}

DWORD WINAPI ThreadProc2(LPVOID lpParameter)
{
    while (TRUE)
    {
        WaitForSingleObject(handle, INFINITE);
        if (ticket > 0)
        {
            Sleep(1);
            printf("thread2 sale the ticket id is: %d\n", ticket--);
        }
        else
        {
            break;
        }
        //ReleaseMutex(handle);
    }
    return 0;
}

int main(int argc, _TCHAR *argv[])
{
    /*wchar_t is a unicode datatype, which is different then the ASCII function you are specifying.
        Just define LoadingStream with a const char*, or use CreateFileW(...)
        https://docs.microsoft.com/en-us/previous-versions/5z097dxa(v=vs.140)
        https://docs.microsoft.com/en-us/previous-versions/2dax2h36(v=vs.140)?redirectedfrom=MSDN
    */
    //char *cacheMutexName = "HelloWorldMutex";
    // handle = ::CreateMutexW(nullptr, TRUE, cacheMutexName);
    //handle = ::CreateMutex(nullptr, FALSE, cacheMutexName);

    HANDLE handleThread1 = CreateThread(NULL, 0, ThreadProc1, NULL, 0, NULL);
    HANDLE handleThread2 = CreateThread(NULL, 0, ThreadProc2, NULL, 0, NULL);
    Sleep(1);
    DWORD result = WaitForSingleObject(handle, INFINITE);
    printf("%d\r\n", result);
    switch (result)
    {
    case WAIT_OBJECT_0:
        printf("The thread got ownership of the mutex.\r\n");
        ReleaseMutex(handle);
        printf("Mutex released.\r\n");
        break;
    case WAIT_ABANDONED:
        printf("The thread got ownership of an abandoned mutex.\r\n");
        break;
    }

    // Countdown.

    for (int i = 10; i > 0; i--)
    {
        wprintf(L"%d ", i);
        Sleep(1000);
    }
    wprintf(L"0\r\n");

    // ReleaseMutex(handle);
    // printf("Mutex released.\r\n");

    return 0;
}