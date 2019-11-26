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
void handler(int signo);
pid_t create_child(string path,int);

processData therunning_process;
bool scheduler_sleep=false;
bool lastmsgrecived=false;
//ofstream scheduler_log;


struct processData current_process;

int main(int argc, char* argv[]) {
    initQueue(false);
    initClk();
    cout<<"scheduler"<<endl;

    signal(SIGCHLD,handler);


//	scheduler_log.open("scheduler.log");
	//scheduler_log<< "#At time x process y state arr w total z remain y wait k"<<endl;

    while (!ready_queue.empty()||!lastmsgrecived){


        read_msg();

        algo_hpf();


        if (scheduler_sleep){
        pause();
        }




    }

   // scheduler_log.close();
    printf( "done\n");
    return 0;

}

void handler(int signo)
{
int status;
int d= waitpid(therunning_process.pid, &status, WNOHANG );
printf("handler%d\n",d);

if (d>0)
{/*
therunning_process.finished=getClk();
int ta=therunning_process.finished-therunning_process.started;
float wta=ta/therunning_process.runningtime;
scheduler_log<< "At time "<<getClk()<<" process "<<therunning_process.id<<" stopped"<<" arr "<<therunning_process.arrivaltime<<" total "<<therunning_process.runningtime<<" remain "<<therunning_process.remaningtime<<" wait "<<therunning_process.wait<<" TA "<<ta<<" WTA "<<wta<<endl;
*/
cout<<"process p\back at "<<getClk()<<endl;
    scheduler_sleep=false;

}

else if(d==0)
{
pause();
}
if(d==-1){
cout<<"error"<<endl;
}

}


pid_t create_child(string path,int rtime)
{


    const char*s= path.c_str();
    pid_t child_id = fork(); // create child process
   // cout<<child_id<<endl;


    if(child_id==-1)
    {

        perror("fork");
    }
    else if (child_id==0)
    {

    // therunning_process.started=getClk();
   //  scheduler_log<< "At time "<<getClk()<<" process "<<therunning_process.id<<" started"<<" arr "<<therunning_process.arrivaltime<<" total "<<therunning_process.runningtime<<" remain "<<therunning_process.remaningtime<<" wait "<<therunning_process.wait<<endl;
       const char* qi=(to_string(rtime)).c_str();
      // cout<<"process started at "<<getClk()<<endl;
        execl(s,qi, (char *) 0); // run the command


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
 else if (ret==0){
    cout<<"2na 2rait mn 2l zft pg"<<endl;
    ready_queue.push(pp);
cout<<"push id "<<pp.id<<" arr "<<pp.arrivaltime<<" rt "<<pp.runningtime<<endl;
}


    }
      return;

}


void algo_hpf(){

    if (!ready_queue.empty()){
    therunning_process=ready_queue.top();
    ready_queue.pop();
    create_child("./process.out",therunning_process.runningtime);
    scheduler_sleep=true;


    }




}



