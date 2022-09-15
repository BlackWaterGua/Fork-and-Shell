#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
using namespace std;
int main()
{
		int change = -1;
		cin >> change;
		if(change == 0)
		{
        		int exit_status;
        		pid_t PID = fork();
        		if(PID==0)
        		{
                		wait(NULL);
                		cout << "burst " << endl;
        		}
        		else if(PID>0)
        		{
                		cout << "star ";
        		}
        		else
        		{
        	        	cout << "error" << endl;
       			}
		}
		else if(change == 1)
    	{
            int exit_status;
            pid_t PID = fork();
            if(PID==0)
            {
                    cout << "burst ";
            }
            else if(PID>0)
            {
					wait(NULL);
                    cout << "star " << endl;
            }
            else
            {
                    cout << "error" << endl;
            }
    	}
}
