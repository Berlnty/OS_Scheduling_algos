#include "clkUtilities.h"
#include "queueUtilities.h"
#include <bits/stdc++.h>
#include<iostream>
#include<queue>
#include<string>
#include<sstream>
using namespace std;
void ClearResources(int);
queue<struct processData> read_processes(int);
pid_t create_child(string path,int);
int main() {

    initQueue(true);
    //TODO:
    // 1-Ask the user about the chosen scheduling Algorithm and its parameters if exists.
    cout<< "choose prefered  algorithm:  1-HPF   2-SRN   3-RR";
	cout<<"\n";
    char algo_id;
    cin>> algo_id;
int quantum=0;
queue<struct processData> processes=read_processes(algo_id);




    // 2-Initiate and create Scheduler and Clock processes.
    string path;

    if(algo_id=='1')
    path="./sch.out";

    else if(algo_id=='2')
    path="./schsrtn.out";

    else if(algo_id=='3')
    {cout<<"Enter quantum: ";
    cin>>quantum;
    path="./schrr.out";
    }

       cout<<path<<endl;
       pid_t sch_id = create_child(path,quantum);


       path ="./clock.out";
       pid_t clk_id=create_child(path,quantum);

    // 3-use this function AFTER creating clock process to initialize clock, and initialize MsgQueue
    initClk();



    //TODO:  Generation Main Loop
    //4-Creating a data structure for process  and  provide it with its parameters
    //5-Send & Notify the information to  the scheduler at the appropriate time
    //(only when a process arrives) so that it will be put it in its turn.

    //===================================
    //Preimplemented Functions examples

    /////Toget time use the following function
	while(processes.size()>0){
    int x= getClk();


	if(processes.front().arrivaltime==x){
	Sendmsg(processes.front());

 //cout<<"pt"<<processes.front().id<<"  "<<x<<endl;
    processes.pop();

	}
	}
    //////Tosend something to the scheduler, for example send id 2

    //no more processes, send end of transmission message
    lastSend();

     //waiting for the last process to be terminated
    int stat;
    int d=waitpid(sch_id,&stat,WNOHANG);
    while(d<=0){

    d=waitpid(sch_id,&stat,WNOHANG);}
    cout<<"d "<<d<<endl;


    //////////To clear all resources
    ClearResources(0);
    //======================================

    return 0;
}

void ClearResources(int){
    msgctl(qid, IPC_RMID, (struct msqid_ds*)0);
    destroyClk(true);
    exit(0);
}

queue<struct processData> read_processes(int type){

queue <struct processData>temp;

ifstream processes_file("processes.txt");
if(processes_file.is_open())
    {
        struct processData temp_process;
string line;
getline(processes_file,line);
while(getline(processes_file,line))
{
    std::istringstream iss(line);
    iss>>temp_process.id>>temp_process.arrivaltime>>temp_process.runningtime>>temp_process.priority;
    temp_process.remaningtime=temp_process.runningtime;
      temp_process. started=0;
     temp_process. finished=0;
     temp_process.wait=0;
        temp_process. stopped=0;
    temp.push(temp_process);
    //cout<< temp_process.id<<' '<<temp_process.arrivaltime<<' '<<temp_process.runningtime<<' '<<temp_process.priority<<' '<<temp_process.runningtime<<endl;

}

}

return temp;
}





pid_t create_child(string path,int q){


    const char*s= path.c_str();
    //const char *qi=to_string(q).c_str();

    pid_t child_id = fork(); // create child process
    cout<<child_id<<endl;

    if(child_id==-1)
    {

        perror("fork");
    }
    else if (child_id==0)
    {
    cout<<"path"<<s;
    stringstream ss;
    ss<<q;
    const char *qi=ss.str().c_str();
       // cout<<"before exec sch"<<endl;
       execl(s,qi, (char *) 0); // run the command


    }
    return child_id;



}












