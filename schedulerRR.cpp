#include "clkUtilities.h"
#include "queueUtilities.h"
#include<vector>
#include<algorithm>
#include <queue>
#include <bits/stdc++.h>
#include <string>
#include<sstream>
using namespace std;



void read_msg();
void childhandler(int signo);
void alarmhandler(int signo);
void handler (int signo);
void ALGO_RR();
pid_t create_child(string path,int q);

//long var=0;
vector<int>wta_vec;
queue<processData>ready_queue;
ofstream scheduler_log,scheduler_brief;
processData current_process;
bool lastmsgrecived;
int quantum;
bool scheduler_sleep=false;
float total_waiting=0;
float total_wta=0;
float total_runing=0;
int processes_no=0;
int finish_time=0;
int first_time=-1;

int main(int argc, char* argv[]) {

    initQueue(false);
    initClk();

    //intializing
    quantum=atoi(argv[0]);
    lastmsgrecived=false;
    signal(SIGALRM,alarmhandler);
    signal(SIGCHLD,childhandler);
    signal(SIGINT,handler);

    scheduler_log.open("schedulerRR.log");
	scheduler_log<< "#At time x process y state arr w total z remain y wait k"<<endl;


    cout<<"RR_scheduler"<<endl;
    cout<<"quantum "<<argv[0]<<" q "<<quantum<<endl;


    //TODO: implement the scheduler :)

    while (!ready_queue.empty() || !lastmsgrecived){

       read_msg();
       ALGO_RR();
       if(scheduler_sleep){
   
       pause();
       }
   }


    scheduler_log.close();
float sum_wta=0;
   for(int i=0;i<wta_vec.size();++i){
    sum_wta+=pow((total_waiting/processes_no)-wta_vec[i],2);
   }
    sum_wta=sqrt(sum_wta/processes_no);


   scheduler_brief.open("schedulerRR.brief");
   scheduler_brief<<"CPU utilization= "<<std::fixed << std::setprecision(2)<<(total_runing/(finish_time-first_time)*100)<<'%'<<endl;
   scheduler_brief<<"Avg WTA= "<<std::fixed << std::setprecision(2)<<(total_wta/processes_no)<<endl;
   scheduler_brief<<"Avg Waiting= "<<std::fixed << std::setprecision(2)<<(total_waiting/processes_no)<<endl;
   scheduler_brief<<"Std WTA= "<<sum_wta<<endl;
    scheduler_brief.close();

    return 0;

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

    else if (ret==-1) return;


    else if(ret==0){
     
        ready_queue.push(pp);
        cout<<"push "<<pp.id<<" "<<pp.arrivaltime<<"  "<<pp.remaningtime<<endl;
        }

    }
    return;
    }



void alarmhandler(int signo){
    cout<<"hi alarm ";
    alarm(0);
    int stat;
    int d=waitpid(current_process.pid,&stat,WNOHANG);
    int y= max(0,current_process.remaningtime-quantum);
    cout<<"d= "<<d<<" y= "<<y<<endl;
if(y>0){
 if(d==0){
 current_process.stopped=getClk();
 current_process.remaningtime=y;

 scheduler_log<< "At time "<<getClk()<<" process "<<current_process.id<<" stopped"<<" arr "<<current_process.arrivaltime<<" total "<<current_process.runningtime<<" remain "<<current_process.remaningtime<<" wait "<<current_process.wait<<endl;

    kill(current_process.pid,SIGSTOP);
    scheduler_sleep=false;
    cout<<"d=0 sends SIGSTOP pre rtime= "<<current_process.remaningtime<<" current rtime= "<<y<< " pid= "<<current_process.pid<<endl;


    }
    /*
else if(d>0){

cout<<"///////d5l hna abl el child handler id="<<current_process.id<<endl;

}*/
}
else{
cout<<"else alarm"<<endl;
if(!ready_queue.empty()){
cout<<"pause here"<<endl;
pause();
}

else scheduler_sleep=false;

}
return;
}


void childhandler(int signo){
  cout<<"hi handler";
   int stat;
    int d=waitpid(current_process.pid,&stat,WNOHANG);
    cout<<" id= "<<d<<" pid"<<current_process.pid<<endl;
    if(d==0){
    if(scheduler_sleep)
    pause();

    }

    else if(d>0){
    alarm(0);
    finish_time=current_process.finished=getClk();
current_process.remaningtime=0;
total_waiting+=current_process.wait;
float ta=current_process.finished-current_process.arrivaltime;
current_process.wait=ta-current_process.runningtime;
float wta=ta/current_process.runningtime;
wta_vec.push_back(wta);
total_wta+=(current_process.runningtime==0)?0:wta;
total_runing+=current_process.runningtime;
    cout<<"child supposed to be terminated id ="<<current_process.id<<endl;
    scheduler_log<< "At time "<<getClk()<<" process "<<current_process.id<<" finished"<<" arr "<<current_process.arrivaltime<<" total "<<current_process.runningtime<<" remain "<<current_process.remaningtime<<" wait "<<current_process.wait<<" TA "<<ta<<" WTA "<<std::fixed << std::setprecision(2)<<(wta)<<endl;

    scheduler_sleep=false;
}

    else if(d==-1 && ready_queue.empty()){
      finish_time=current_process.finished=getClk();
current_process.remaningtime=0;
total_waiting+=current_process.wait;
float ta=current_process.finished-current_process.arrivaltime;
current_process.wait=ta-current_process.runningtime;
float wta=ta/current_process.runningtime;
wta_vec.push_back(wta);
total_wta+=(current_process.runningtime==0)?0:wta;
total_runing+=current_process.runningtime;

    scheduler_log<< "At time "<<getClk()<<" process "<<current_process.id<<" finished"<<" arr "<<current_process.arrivaltime<<" total "<<current_process.runningtime<<" remain "<<current_process.remaningtime<<" wait "<<current_process.wait<<" TA "<<ta<<" WTA "<<std::fixed << std::setprecision(2)<<(wta)<<endl;

    cout<<"last message here"<<endl;
}


return;
}


void ALGO_RR(){

 if(current_process.remaningtime>0){
    ready_queue.push(current_process);
    cout<<"push current back in queue with remaning time "<<current_process.remaningtime<<" id "<<current_process.id<<endl;
    }

 if(!ready_queue.empty()){
    current_process=ready_queue.front();
    ready_queue.pop();
    cout<<"pop current new in queue with remaning time "<<current_process.remaningtime<<" id "<<current_process.id<<endl;
    if(current_process.remaningtime == current_process.runningtime){
    cout<<" fork process rt= "<<current_process.remaningtime<<" id "<<current_process.id<<endl;

     current_process.started=getClk();
     current_process.wait=getClk()-current_process.arrivaltime;
    processes_no+=(current_process.runningtime==0)?0:1;

     current_process.pid=create_child("./process.out",current_process.runningtime);
     cout<<"fffffffprocess id= "<<current_process.id<<" pid= "<<current_process.pid<<endl;
     }
     else {
      cout<<" send SIGCONT rt= "<<current_process.remaningtime<<" id "<<current_process.id<<endl;
      current_process.wait+=getClk()-current_process.stopped;
      scheduler_log<< "At time "<<getClk()<<" process "<<current_process.id<<" resumed"<<" arr "<<current_process.arrivaltime<<" total "<<current_process.runningtime<<" remain "<<current_process.remaningtime<<" wait "<<current_process.wait<<endl;

     kill(current_process.pid,SIGCONT);
     }
    alarm(quantum);
    scheduler_sleep=true;
    cout<<"paused"<<endl;


    }


 return;
}



pid_t create_child(string path,int q){


    const char*s= path.c_str();
    //const char *qi=to_string(q).c_str();

    pid_t child_id = fork(); // create child process


    if(child_id==-1)
    {

        perror("fork");
    }
    else if (child_id==0)
    {

    scheduler_log<< "At time "<<getClk()<<" process "<<current_process.id<<" started"<<" arr "<<current_process.arrivaltime<<" total "
    <<current_process.runningtime<<" remain "<<current_process.remaningtime<<" wait "<<current_process.wait<<endl;

    stringstream ss;
    ss<<q;
    const char *qi=ss.str().c_str();
    cout<<"qi= "<<qi<<endl;
       // cout<<"before exec sch"<<endl;
       execl(s,qi, (char *) 0); // run the command

    }
    return child_id;



}


void handler(int signo){

return;
}
