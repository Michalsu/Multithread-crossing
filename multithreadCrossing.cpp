#include <ncurses.h>
#include <chrono>
#include <thread>
#include <pthread.h>
using namespace std;
using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds

std::chrono::milliseconds delay=milliseconds(100);

struct car{
    int x;
    int y;
    char id='A';
};



void printLetter(int x, int y, char letter){
    sleep_for(delay);
    // clear();
    // mvprintw(x,y,"%c",letter);
    // refresh();
}

void* car_runner(void* arg){
    struct car *arg_struct = (struct car*) arg;


    int xmin =2, ymin=20;
    int xmax=20,ymax=70;
    int arg_structx, arg_structy;
    int dir,hv; // hv- horizontal/vertical
    if(hv=rand()%2){
        arg_struct->y=rand()%(ymax-ymin)+ymin;
        if(dir=rand()%2) arg_struct->x=xmax;
        else arg_struct->x=xmin;
    }else{
        arg_struct->x=rand()%(xmax-xmin)+xmin;
        if(dir=rand()%2) arg_struct->y=ymin;
        else arg_struct->y = ymax;
    }
    
    switch (dir+2*hv)
    {
    case 2: //prawo
        for(;arg_struct->y<ymax;arg_struct->y++){
        sleep_for(delay);
        }
    case 0: //dol
        for(;arg_struct->x<xmax;arg_struct->x++){
            sleep_for(delay);
        }
    case 3: //lewo
        for(;arg_struct->y>ymin;arg_struct->y--){
            sleep_for(delay);
        }
    case 1: //gora
        for(;arg_struct->x>xmin;arg_struct->x--){
            sleep_for(delay);
        } 
        break;
    
    default:
        break;
    }
    
    for(int i=0;i<5;i++){
       for(arg_struct->y=ymin;arg_struct->y<ymax;arg_struct->y++){
        sleep_for(delay);
        }
        for(arg_struct->x=xmin;arg_struct->x<xmax;arg_struct->x++){
            sleep_for(delay);
        }
        for(;arg_struct->y>ymin;arg_struct->y--){
            sleep_for(delay);
        }
        for(;arg_struct->x>xmin;arg_struct->x--){
            sleep_for(delay);
        }
    }


}

int main(int argc, char** argv){
    initscr();
    srand( time(NULL) );

    int numOfCars=3;

    struct car args[numOfCars];
    
    pthread_t tids[numOfCars];
    for(int i=0; i<numOfCars;i++){
        args[i].id='A'+i;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_create(&tids[i],&attr, car_runner, &args[i]);
    }

    while(true){
        clear();
        for(int j=0;j<numOfCars;j++){
            //printf("%d %d %c \n",args[j].x,args[j].y,args[j].id);
            mvprintw(args[j].x,args[j].y,"%c",args[j].id);
        }
        refresh();
        sleep_for(milliseconds(50));
    }
    
    
    // xmin=6;
    // xmax=16;
    // ymin=10;
    // ymax=80;
    // for(int i=0;i<3;i++){
    //    for(y=ymin;y<ymax;y++){
    //     printLetter(x,y,'A');
    //     }
    //     for(x=xmin;x<xmax;x++){
    //         printLetter(x,y,'A');
    //     }
    //     for(;y>ymin;y--){
    //         printLetter(x,y,'A');
    //     }
    //     for(;x>xmin;x--){
    //         printLetter(x,y,'A');
    //     } 
    // }
    
    getch();
    endwin();

    return 0;
}


