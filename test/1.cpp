/*
https://blog.csdn.net/LL596214569/article/details/81088862
WaitForSingleObject(thread,INFINITE) signal: active after thread stop
thread means always running: while(1)
*/

/*
UINT CFlushDlg::MyThreadProc( LPVOID pParam )
{
    WaitForSingleObject(g_event,INFINITE);
    For(;;)
    {
    ………….
    }
    return 0;
}

INT CFlushDlg::MyThreadProc( LPVOID pParam )
{
    while(WaitForSingleObject(g_event,MT_INTERVAL)!=WAIT_OBJECT_0)
    {
    ………………
    }
    return 0;
}
*/
//#include "stdafx.h"
#include "stdio.h"
#include "windows.h"
#include "iostream"
#include <time.h>
using namespace std;

int i = 0;
int timeStart = time(NULL);
int holdTime = 20;
DWORD WINAPI FuncProc1(HANDLE hThread);
DWORD WINAPI FuncProc2(HANDLE hThread);
DWORD WINAPI FuncProc3(HANDLE hThread);
HANDLE hThread1 = CreateThread(NULL, 0, FuncProc1, NULL, CREATE_SUSPENDED, NULL);
HANDLE hThread2 = CreateThread(NULL, 0, FuncProc2, NULL, CREATE_SUSPENDED, NULL);
HANDLE hThread3 = CreateThread(NULL, 0, FuncProc3, NULL, CREATE_SUSPENDED, NULL);

DWORD WINAPI FuncProc1(LPVOID lpParameter)
{
    cout << "thread1 start" << endl;
    while (WaitForSingleObject(hThread2, 500) != WAIT_OBJECT_0)
    {
        if (WaitForSingleObject(hThread3, 500) == WAIT_TIMEOUT)
        {
            cout << "等待 thread3 超时  " << time(NULL) << endl;            
        }
        if (WaitForSingleObject(hThread3, 5000) == WAIT_ABANDONED)
        {
            cout << "Abondoned" << endl;
        }
    }
    return 0;
}

DWORD WINAPI FuncProc2(LPVOID lpParameter)
{
    cout << "thread2 start" << endl;
    for (int i = 0; i < 10; i++)
    {
        WaitForSingleObject(hThread1, 5000);
    }
    // while (WaitForSingleObject(hThread1, 500) != WAIT_OBJECT_0)
    // {
    //     cout << "thread2" << endl;
    // }
    return 0;
}

DWORD WINAPI FuncProc3(LPVOID lpParameter)
{
    cout << "thread3 start" << endl;
    while (WaitForSingleObject(hThread2, 500) != WAIT_OBJECT_0)
    {
        if (WaitForSingleObject(hThread1, 500) == WAIT_TIMEOUT)
        {
            cout << "等待 thread1 超时  " << time(NULL) << endl;            
        }

        if (WaitForSingleObject(hThread1, 5000) == WAIT_ABANDONED)
        {
            cout << "Abondoned" << endl;
        }
    }
    return 0;
}

int main()
{
    cout << "start time:" << timeStart << endl;
    ResumeThread(hThread2);
    ResumeThread(hThread3);
    ResumeThread(hThread1);
    cout << WaitForSingleObject(hThread2, 1) << endl;
    cout << WaitForSingleObject(hThread2, 60000) << endl; 
    DWORD dwRet = 0;
    while (1)
        ;
    //DWORD dwRet = WaitForSingleObject(hThread1, INFINITE);

    if (dwRet == WAIT_TIMEOUT)
    {
        cout << "等待超时" << endl;
    }

    if (dwRet == WAIT_ABANDONED)
    {
        cout << "Abondoned" << endl;
    }
    CloseHandle(hThread1);
    CloseHandle(hThread2);
    CloseHandle(hThread3);
    //system("pause");
    return 0;
}