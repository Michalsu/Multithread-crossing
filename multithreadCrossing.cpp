#include <ncurses.h>
#include <chrono>
#include <thread>
#include <pthread.h>
#include <vector>
using namespace std;
using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds





struct car{
    int x;
    int y;
    milliseconds delay=milliseconds(20);
    char id='A';
};


vector<car*> carsB;
vector<pthread_t*> threadsB;



void printLetter(int x, int y, char letter, milliseconds delay){
    sleep_for(delay);
    // clear();
    // mvprintw(x,y,"%c",letter);
    // refresh();
}

void* car_runner_A(void* arg){
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
        sleep_for(arg_struct->delay);
        }
    case 0: //dol
        for(;arg_struct->x<xmax;arg_struct->x++){
        sleep_for(arg_struct->delay);
        }
    case 3: //lewo
        for(;arg_struct->y>ymin;arg_struct->y--){
        sleep_for(arg_struct->delay);
        }
    case 1: //gora
        for(;arg_struct->x>xmin;arg_struct->x--){
        sleep_for(arg_struct->delay);
        } 
        break;
    
    default:
        break;
    }
    
    while(true){
       for(arg_struct->y=ymin;arg_struct->y<ymax;arg_struct->y++){
        sleep_for(arg_struct->delay);
        }
        for(arg_struct->x=xmin;arg_struct->x<xmax;arg_struct->x++){
        sleep_for(arg_struct->delay);
        }
        for(;arg_struct->y>ymin;arg_struct->y--){
        sleep_for(arg_struct->delay);
        }
        for(;arg_struct->x>xmin;arg_struct->x--){
        sleep_for(arg_struct->delay);
        }
    }


}

void* car_runner_B(void* arg){
    struct car *arg_struct = (struct car*) arg;


    int xmin=6, ymin=16;
    int xmax=10,ymax=80;
    
    arg_struct->y=ymin;
    arg_struct->x=xmin;
    for(int i=0;i<3;i++){
       for(;arg_struct->y<ymax;arg_struct->y++){
        sleep_for(arg_struct->delay);
        //printf("%d\n",arg_struct->y);
        }
        for(;arg_struct->x<xmax;arg_struct->x++){
        sleep_for(arg_struct->delay);
        }
        for(;arg_struct->y>ymin;arg_struct->y--){
        sleep_for(arg_struct->delay);
        }
        for(;arg_struct->x>xmin;arg_struct->x--){
        sleep_for(arg_struct->delay);
        }
    }
    carsB.erase(carsB.begin());
    threadsB.erase(threadsB.begin());
    //printf("%d\n",arg);
    return 0;
}

void* car_starter_B(void* arg){
    char* id_ptr=(char*)(arg);
    char id= *id_ptr;
    int i=0;
    int rc;
    

    while(true){
        sleep_for(seconds(rand()%8));
        car new_car;
        new_car.id=id;
        id++;
        new_car.delay=milliseconds(rand()%190+10);
        pthread_t new_thread;
        pthread_attr_t attr;
        rc = pthread_create(&new_thread, &attr,car_runner_B, &new_car);
        carsB.push_back(&new_car);
        threadsB.push_back(&new_thread);
        if(id=='Z'+1) id='a';
        if(id='z'+1) id='D';
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
        pthread_create(&tids[i],&attr, car_runner_A, &args[i]);
    }

    char id='A'+numOfCars;
    pthread_t tid;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create(&tid,&attr, car_starter_B, &id);
    
    while(true){
        clear();
        for(int j=0;j<numOfCars;j++){
           // printf("%d %d %c \n",args[j].x,args[j].y,args[j].id);
            mvprintw(args[j].x,args[j].y,"%c",args[j].id);
        }
        for(int j=0; j<carsB.size();j++){
            mvprintw(carsB[j]->x,carsB[j]->y,"%c",carsB[j]->id);
           // printf("%d %d %c %d\n",carsB[j].x,carsB[j].y,carsB[j].id, carsB[j].delay);
        }
        
        move(0,0);
        printw("%d",carsB.size());
        move(1,0);
        printw("%d",threadsB.size());
        refresh();
        sleep_for(milliseconds(50));
    }

    getch();
    endwin();

    return 0;
}




