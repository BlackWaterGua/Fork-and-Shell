#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<cstring>
#include<vector>
#include<string>
#include<algorithm>
using namespace std;

void type_prompt(void)
{
    /*
    string path = getenv("HOME");
    cout << path << ">>> ";
    */
    char *path = NULL;
    path = getcwd(NULL,0);
    cout << path << " -> ";
    free(path);
}

int main()
{
    while(true)
    {
        type_prompt();
        string input;
        getline(cin, input);
        int gotoSystem = 0;
        for(int i=0;i<input.length();i++)
        {
            if(input.at(i) == '>' || input.at(i) == '<')
            {
                gotoSystem = 1;
            }
        }
        int cutCount = 0, cutHead = 0;
        vector<string> cut;
        for(int i=0;i<input.length();i++)
        {
            char tmp = input.at(i);
            if(tmp == ' ')
            {
                string temp = input.substr(cutHead, cutCount);
                cutCount = 0;
                cutHead = i+1;
                cut.push_back(temp);
            }
            else if(i == input.length()-1)
            {
                cutCount++;
                string temp = input.substr(cutHead, cutCount);
                cut.push_back(temp);
            }
            else
            {
                cutCount++;
            }
        }
        char *command[cut.size() + 1];
        char tmp[256];
        for(int i=0;i<cut.size();i++)
        {
            strcpy(tmp, cut.at(i).c_str());
            command[i] = strdup(tmp);
        }
        command[cut.size()] = nullptr;

        // for(int i=0;i<cut.size();i++)
        // {
        //     cout << cut.at(i) << endl;
        // }

        //int status;
        if(gotoSystem == 1)
        {
            char* temp = &input.at(0);
            system(temp);
        }
        else if(strcmp(command[0], "exit") == 0)
        {
            break;
        }
        else if(strcmp(command[0], "cd") == 0)
        {
            chdir(command[1]);
        }
        //pipe task
        else
        {
            int p1[2];
            int p2[2];
            int stdin_copy = dup(STDIN_FILENO);
            int stdout_copy = dup(STDOUT_FILENO);
            int this_vbar = 0;
            int last_vbar = 0;
            char pipe_buff[100000] = {0};
            vector<int> vbar_pos;
            vbar_pos.push_back(-1);
            for(int i=0;i<cut.size(); i++)
            {
                if(cut.at(i) == "|"){
                    vbar_pos.push_back(i);   /*store the pos of "|" in vector vbar_pos*/
                }
            }
            for(int i=0; i<cut.size(); i++)
            {
                if(cut.at(i) == "|" || i==cut.size()-1)
                {
                    if(cut.at(i) == "|")
                    {
                        this_vbar = find(vbar_pos.begin(), vbar_pos.end(), i) - vbar_pos.begin();
                        last_vbar = this_vbar-1;  /*this_vbar, last_vbar are indexes in vbar_pos*/
                    }
                    if(pipe(p1)<0)
                        cout << "create pipe1 error" << endl;
                    if(pipe(p2)<0)
                        cout << "create pipe2 error" << endl;
                    if(fork()==0)
                    { /*child process: call execvp if cmd.at(i) is "|" or is the last word*/
                        if(i==cut.size()-1)
                        {
                            close(p1[1]);
                            dup2(p1[0],STDIN_FILENO); /*p1 will close after STDIN receive EOF*/
                            close(p1[0]);
                            close(p2[0]);
                            close(p2[1]);
                            dup2(stdout_copy,STDOUT_FILENO);
                            char* arg[100000] = {0};
                            for(int j=0 ; j<(i-vbar_pos.at(this_vbar)); j++)
                            {
                                arg[j] = strdup(cut.at(vbar_pos.at(this_vbar)+j+1).c_str());
                            }
                            arg[i-vbar_pos.at(this_vbar)] = NULL;
                            execvp(arg[0],arg);
                            exit(0);
                        }
                        else
                        {
                            close(p1[1]);
                            dup2(p1[0],STDIN_FILENO); /*p1 will close after STDIN receive EOF*/
                            close(p1[0]);
                            close(p2[0]);
                            dup2(p2[1],STDOUT_FILENO);
                            close(p2[1]);
                            char* arg[100000] = {0};
                            for(int j=0 ; j<(i-vbar_pos.at(last_vbar)-1); j++)
                            {
                                arg[j] = strdup(cut.at(vbar_pos.at(last_vbar)+j+1).c_str());
                            }
                            arg[i-vbar_pos.at(last_vbar)-1] = NULL;
                            execvp(arg[0],arg);
                            exit(0);
                        }
                    }
                    else
                    { /*parent process: pass old data to the new child and store new data*/
                        close(p1[0]);
                        dup2(p1[1],STDOUT_FILENO);
                        close(p1[1]);
                        cout << pipe_buff << flush;
                        dup2(stdout_copy,STDOUT_FILENO); /*p1 write end isn't used anymore, send EOF*/
                        close(p2[1]);
                        if(i<cut.size()-1){
                            memset(pipe_buff, 0, sizeof(pipe_buff));
                            read(p2[0],pipe_buff, sizeof(pipe_buff));
                        }
                        close(p2[0]);  /*p2 close here*/
                        wait(NULL);
                    }
                }
            }
        }
        /*
        else if(fork()!=0)
        {
            //parent code
            waitpid(-1, &status, 0);
        }
        else
        {
            //child code
            if(execvp(command[0], command) == -1)
            {
                cout << "error: " << command[0] << endl;
            }

        }
        */

    }
}
