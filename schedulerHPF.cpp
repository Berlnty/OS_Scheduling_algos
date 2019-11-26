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

    return leftside.priority<rightside.priority;


}


void algo_hpf();
void read_msg();
void handler(int signo);
pid_t create_child(string path,int);


vector<int>wta_vec;
processData therunning_process;
bool scheduler_sleep=false;
bool lastmsgrecived=false;
ofstream scheduler_log,scheduler_brief;
float total_waiting=0;
float total_wta=0;
float total_runing=0;
int processes_no=0;
int finish_time=0;
int first_time=-1;
struct processData current_process;

int main(int argc, char* argv[]) {
    initQueue(false);
    initClk();
    cout<<"scheduler"<<endl;

    signal(SIGCHLD,handler);


	scheduler_log.open("schedulerHPF.log");
	scheduler_log<< "#At time x process y state arr w total z remain y wait k"<<endl;

    while (!ready_queue.empty()||!lastmsgrecived){


        read_msg();

        algo_hpf();


        if (scheduler_sleep){
        pause();
        }




    }

   scheduler_log.close();
    float sum_wta=0;
   for(int i=0;i<wta_vec.size();++i){
    sum_wta+=pow((total_waiting/processes_no)-wta_vec[i],2);
   }
    sum_wta=sqrt(sum_wta/processes_no);


   scheduler_brief.open("schedulerHPF.brief");
   scheduler_brief<<"CPU utilization= "<<std::fixed << std::setprecision(2)<<(total_runing/(finish_time-first_time)*100)<<'%'<<endl;
   scheduler_brief<<"Avg WTA= "<<std::fixed << std::setprecision(2)<<(total_wta/processes_no)<<endl;
   scheduler_brief<<"Avg Waiting= "<<std::fixed << std::setprecision(2)<<(total_waiting/processes_no)<<endl;
   scheduler_brief<<"Std WTA= "<<sum_wta<<endl;
    scheduler_brief.close();


    printf( "done\n");
    return 0;

}

void handler(int signo)
{


int status;
int d= waitpid(therunning_process.pid, &status, WNOHANG );
printf("handler%d\n",d);

if (d>0)
{
finish_time=therunning_process.finished=getClk();
therunning_process.remaningtime=0;
total_waiting+=therunning_process.wait;
float ta=therunning_process.finished-therunning_process.arrivaltime;
therunning_process.wait=ta-therunning_process.runningtime;
float wta=ta/therunning_process.runningtime;
wta_vec.push_back(wta);
total_wta+=(therunning_process.runningtime==0)?0:wta;
total_runing+=therunning_process.runningtime;
scheduler_log<< "At time "<<getClk()<<" process "<<therunning_process.id<<" finished"<<" arr "<<therunning_process.arrivaltime<<" total "<<therunning_process.runningtime<<" remain "<<therunning_process.remaningtime<<" wait "<<therunning_process.wait<<" TA "<<ta<<" WTA "<<std::fixed << std::setprecision(2)<<(wta)<<endl;

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


    scheduler_log<< "At time "<<getClk()<<" process "<<therunning_process.id<<" started"<<" arr "<<therunning_process.arrivaltime<<" total "<<therunning_process.runningtime<<" remain "<<therunning_process.remaningtime<<" wait "<<therunning_process.wait<<endl;
       const char* qi=(to_string(rtime)).c_str();
       cout<<"process started at "<<therunning_process.started<<endl;
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
  
    ready_queue.push(pp);
    if(first_time==-1)first_time=pp.arrivaltime;
cout<<"push id "<<pp.id<<" arr "<<pp.arrivaltime<<" rt "<<pp.runningtime<<endl;
}


    }
      return;

}


void algo_hpf(){

    if (!ready_queue.empty()){
    therunning_process=ready_queue.top();
    ready_queue.pop();
     therunning_process.started=getClk();
     therunning_process.wait=getClk()-therunning_process.arrivaltime;
     processes_no+=(therunning_process.runningtime==0)?0:1;
  therunning_process.pid=  create_child("./process.out",therunning_process.runningtime);
    scheduler_sleep=true;


    }




}



