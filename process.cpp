
#include <iostream>
#include <unistd.h>
#include<string>
using namespace std;

/* Modify this file as needed*/
int remainingtime =0;


int main(int agrc, char* argv[]) {
//printf("process%d\n",getpid());
    //if you need to use the emulated clock uncomment the following line
    //initClk();

    //TODO: it needs to get the remaining time from somewhere


    remainingtime=atoi(argv[0]);
     cout<<"rrrrrrrrrrrrrrrrrrrrrrr "<<argv[0]<<" rt "<<remainingtime<<endl;

    while(remainingtime>0) {
       sleep(1);
       remainingtime--;
    }
    //if you need to use the emulated clock uncomment the following line
    //destroyClk(false);
   // cout<<"returning pid= "<<getpid()<<endl;

    return 0;
}
