#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
using namespace std;
int main()
{
        pid_t PID = fork();
	// 位於子程序時
        if(PID==0)
        {
                cout << "child pid is " << getpid() << endl;

        }
	// 位於父程序時
        else if(PID>0)
        {
                cout << "parent pid is " << getpid() << endl;
        }
	// 錯誤時
        else
        {
                cout << "error" << endl;
        }
}

