#include "clkUtilities.h"
#include "queueUtilities.h"
#include<vector>
#include<algorithm>
#include <queue>
#include <bits/stdc++.h>
#include <string>
using namespace std;
int _id; // recieving msgs

void signal_callback_handler(int signum) {

    printf("Caught signal %d\n", signum);

    // Cleanup and close up stuff here
    // Terminate program
    exit(signum);

}

// queue which i store processes in it.
priority_queue<processData>ready_queue;
// variable to strore data when transporting it from queue1 to queue2 to do operations on it.
struct processData temp;
// flage of scheduale alarming
bool x = false;


//void operator=(const processData& leftside , const processData& rightside){
//leftside.arrivaltime=rightside.arrivaltime;
//    leftside.priority=rightside.priority;
//    leftside.runningtime=rightside.runningtime;
//    leftside.id=rightside.id;
//    leftside.pid=rightside.pid;
//}

//void algo_hpf();
//void read_msg();

//vector<struct processData> ready_queue;
processData therunning_process;
//bool scheduler_sleep=false;
//bool lastmsgrecived=false;
//bool HPF_check(struct processData a,struct processData b);
//void HPF();
void SRT();
void HPF();
void RR();
void handle(int sigNum);
void handle2(int sigNum);
//void handler(int signo);
//pid_t create_child(string path,int);
//int indexno=0;
struct processData current_process;

int algo_n;

int main(int argc, char* argv[]) {
    cout << "Scheduler starting..." << endl;

    initQueue(false);
    initClk();

    //TODO: implement the scheduler :)
    //string algo_id= stoi(*argv[0]);
    //signal(SIGCHLD,handler);
    int algo_n = stoi(argv[0]);

    switch (algo_n) {
        case 1: HPF();
            break;
        case 2:
            signal(SIGUSR1, handle);  // schedular talks to processess
           signal(SIGCHLD, handle2); // after termination
            SRT();
            printf("right choice");

            break;
        case 3: RR();
            break;
        default: printf("error number");
            break;
    }
    /*
        while (!ready_queue.empty()||!lastmsgrecived){
            read_msg();
            algo_hpf();
            if (scheduler_sleep){
            pause();
            }
        }
     */




    //===================================
    //Preimplemented Functions examples

    /////Toget time use the following function
    //int x= getClk();
    //printf("current time is %d\n",x);
    /*
    struct processData pD;
      int pr=  Recmsg(pD); //returns -1 on failure; 1 on the end of processes, 0 no processes available yet

            while(pr!=1){
            //call the read function that read msg if exist
            // call the algorithm fn



            if(pr==0){
            ready_queue.push(pD);
    //is_new=true;
            printf("here%d\n",pD.id);
             //  printf("current received data %d\n",pD.id);
            }
          HPF();

             pr=  Recmsg(pD);

            }
        //////To receive something from the generator, for example  id 2

    cout<<"size"<<ready_queue.size();
     */

    return 0;

}

bool operator<(const processData& leftside, const processData& rightside) {

    if (algo_n == 2)
        return leftside.runningtime > rightside.runningtime;
    else if (algo_n == 1)
        return leftside.priority > rightside.priority;

}

void HPF() {



}

void RR() {




}

void handle(int sigNum) {
    x = false;
}

void handle2(int sigNum) {

    int status;
    waitpid(-1, &status, WUNTRACED | WCONTINUED);
    if (!WIFSTOPPED(status) && !WIFCONTINUED(status)) {
        ready_queue.pop();
        x = false;
    } else {
        x = true;
    }

}

/*------------------------------------------------------------------SRF -------------------------------------------------------*/
void SRT() {
    struct processData pd;
    struct processData current;
    pid_t _id, id;
    int status;
    int cl1, cl2;
    // while reieving msgs
    int st;
    while (true) {

        //------------------------------------------------------------------- terminte process after execution----------------------------------------------------------------//
        if (ready_queue.size() > 0) {
            priority_queue<processData>ready_queue2;
            kill(ready_queue.top().pid, 19); // stopping the current process
            cl2 = getClk();
            temp = ready_queue.top();
            temp.remaningtime -= cl2 - cl1;
            ready_queue2.push(temp);
            ready_queue.pop();
            cout<<"pop at clock"<<getClk();
            // adding waiting time for the rest processes
            while (ready_queue.size() != 0) {
                temp = ready_queue.top();
                temp.wait += cl2 - cl1;
                ready_queue2.push(temp);
                ready_queue.pop();
            }
            ready_queue = ready_queue2; // after adding the waiting time
            // getting the remainning time
        }

        // *---------------------------------------------------------------wait to recieve message -----------------------------------------------------------------------*//
        while (Recmsg(pd) == 0) {
            // if it did not finish and it was the first time
            ready_queue.push(pd);
            cout << "receiving message";
        }
        /*--------------------------------------------------------stopping after receiving all messages-------------------------------------------*/
        if (Recmsg(pd) == 1 && ready_queue.size() == 0) {
            break;
        }
        //-------------------------------------------------------------------------- execute process------------------------------------------------------------------*//
        if (ready_queue.size() > 0) {
            printf("here");
            current = ready_queue.top(); // current process i will work on

            if (current.pid == -3) {
                printf("here");
                id = fork();
                printf("\n time arrival : %d \n", current.arrivaltime);
                if (id == -1)
                    perror("fork error");
                else if (id == 0) {
                    //child code
                    cout << "fork here";
                    execl("process.out", "process.out", std::to_string(pd.runningtime).c_str(), (char *) NULL); // run the command
                    cl1 = getClk();
                } else {
                    // parent
                    current.pid = id;
                }
            } else if (current.pid != -3) {
                cl1 = getClk();
                kill(current.pid, 18); // continue executing the process
            }
            // ---------------------------------------------pausing the schedule until receiving a new process----------------------------------------------------
            x = false;
            do {
                pause();
            } while (x == true);

        }
    }
}
/*--------------------------------------------- SRT end ----------------------------------------*/
/*
bool HPF_check(struct processData  a,struct processData  b)
{
return a.priority>=b.priority;
}
 */
/*
void HPF()
{
if(is_new)
sort (ready_queue.begin(),ready_queue.end(),HPF_check);


if(ready_queue.size()>0 && indexno< ready_queue.size()){
current_process=ready_queue.at(indexno);
current_process.pid=create_child("./process.out",current_process.runningtime);

}

is_new=false;
}
 */


/*
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
 */

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
     const char* st=(to_string(rtime)).c_str();
        execl(s,st, (char *) 0); // run the command


    }
    else if(child_id==1){

    }
    return child_id;
}


/*
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

 */

