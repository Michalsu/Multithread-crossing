#include <ncurses.h>
#include <chrono>
#include <thread>
//#include <pthread.h>
using namespace std;
using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds

struct car{
    int x;
    int y;
    char id;
};

void* car_runner(void* arg){
    struct car *arg_struct = (struct car*) arg;
}

void printLetter(int x, int y, char letter){
    sleep_for(milliseconds(50));
    clear();
    mvprintw(x,y,"%c",letter);
    refresh();
}

int main(int argc, char** argv){
    initscr();
    srand( time(NULL) );

    
    int xmin =2, ymin=20;
    int xmax=20,ymax=70;
    int x, y;
    int dir,hv; // hv- horizontal/vertical
    if(hv=rand()%2){
        y=rand()%(ymax-ymin)+ymin;
        if(dir=rand()%2) x=xmax;
        else x=xmin;
    }else{
        x=rand()%(xmax-xmin)+xmin;
        if(dir=rand()%2) y=ymin;
        else y = ymax;
    }
    
    switch (dir+2*hv)
    {
    case 2: //prawo
        for(;y<ymax;y++){
        printLetter(x,y,'A');
        }
    case 0: //dol
        for(;x<xmax;x++){
            printLetter(x,y,'A');
        }
    case 3: //lewo
        for(;y>ymin;y--){
            printLetter(x,y,'A');
        }
    case 1: //gora
        for(;x>xmin;x--){
            printLetter(x,y,'A');
        } 
        break;
    
    default:
        break;
    }
    
    for(int i=0;i<5;i++){
       for(y=ymin;y<ymax;y++){
        printLetter(x,y,'A');
        }
        for(x=xmin;x<xmax;x++){
            printLetter(x,y,'A');
        }
        for(;y>ymin;y--){
            printLetter(x,y,'A');
        }
        for(;x>xmin;x--){
            printLetter(x,y,'A');
        } 
    }
    xmin=6;
    xmax=16;
    ymin=10;
    ymax=80;
    for(int i=0;i<3;i++){
       for(y=ymin;y<ymax;y++){
        printLetter(x,y,'A');
        }
        for(x=xmin;x<xmax;x++){
            printLetter(x,y,'A');
        }
        for(;y>ymin;y--){
            printLetter(x,y,'A');
        }
        for(;x>xmin;x--){
            printLetter(x,y,'A');
        } 
    }
    
    getch();
    endwin();

    return 0;
}


