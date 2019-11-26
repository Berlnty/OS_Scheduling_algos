#include "clkUtilities.h"
#include "queueUtilities.h"
#include<vector>
#include<algorithm>
#include <queue>
#include <bits/stdc++.h>
#include <string>
using namespace std;


priority_queue<processData>ready_queue;

bool operator< (const processData& leftside , const processData& rightside){

    return leftside.priority>rightside.priority;


}


void algo_hpf();
void read_msg();
void algo_rr();

processData therunning_process;
bool scheduler_sleep=false;
bool lastmsgrecived=false;
bool HPF_check(struct processData a,struct processData b);

void handler(int signo);
pid_t create_child(string path,int);
//int indexno=0;
struct processData current_process;

int main(int argc, char* argv[]) {
    initQueue(false);
    initClk();
cout<<"scheduler"<<endl;
    //TODO: implement the scheduler :)
	//string algo_id= stoi(*argv[0]);
    signal(SIGCHLD,handler);




    while (!ready_queue.empty()||!lastmsgrecived){


        read_msg();

        algo_rr();


        if (scheduler_sleep){
        pause();
        }




    }





    //===================================
    //Preimplemented Functions examples

    /////Toget time use the following function
    //int x= getClk();
    //printf("current time is %d\n",x);


    return 0;

}

void handler(int signo)
{
int status;
int d= waitpid(therunning_process.pid, &status, WNOHANG );
printf("handler%d\n",status);

if (d>0)
{
    scheduler_sleep=false;
    pause();
}



}


pid_t create_child(string path,int rtime)
{


    const char*s= path.c_str();
    pid_t child_id = fork(); // create child process
  

    if(child_id==-1)
    {

        perror("fork");
    }
    else if (child_id==0)
    {
     const char* st=(to_string(rtime)).c_str();
        execl(s,st, (char *) 0); // run the command


    }
    else if(child_id==1){

    }
    return child_id;



}




void read_msg(){

    while(!lastmsgrecived){
    processData pp;

    int ret=Recmsg(pp);

    if (ret==1){
        lastmsgrecived=true;
        cout <<"the last msg recived"<<endl;
    return;

    }
   else if (ret==-1)
    return;

    cout<<"2na 2rait mn 2l zft pg"<<endl;
    ready_queue.push(pp);
cout<<"push"<<pp.id<<" "<<pp.arrivaltime<<endl;
    return;






    }

}

void algo_hpf(){
    if (!scheduler_sleep)
    return;
    if (!ready_queue.empty()){
    therunning_process=ready_queue.top();
    ready_queue.pop();
    create_child("./process.out",therunning_process.runningtime);
    scheduler_sleep=true;


    }




}



