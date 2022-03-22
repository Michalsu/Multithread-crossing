#include <ncurses.h>
#include <chrono>
#include <thread>
#include <pthread.h>
#include <vector>
#include <stdlib.h>
using namespace std;
using namespace std::this_thread;
using namespace std::chrono; 


bool finish = false;

/*
    kończenie watkow (nasluchiwanie klawisza) czxekanie x wyjsciem az sie skonczy rysowanie
    i wzszystkie watki
    pthread cancel
*/

/*
    zadanie 2
    zatrzymywanie sie samochodow na torze B gdzy
    samochod z toru A znajduje sie w poblizu skrzyzowania
*/

/**
 * Struktura zawierajaca pozycję,
 * opoznienie miedzy krokami(predkosc) oraz 
 * symbol pojazdu 
 */
struct car{
    int x;
    int y;
    milliseconds delay=milliseconds(50);
    char id='A';
};

vector<car*> carsB;
vector<pthread_t*> threadsB;

/**
 * Funkcja symulująca ruch pojazdów po torze A
 * 
 * @param arg instancja samochodu przekazana do watku
 * @return void* 
 */
void* car_runner_A(void* arg){
    //nadanie odpowiedniego typu dla parametru
    struct car *arg_struct = (struct car*) arg;

    //zmienne kontrolujace zakres toru
    int xmin =2, ymin=25;
    int xmax=22,ymax=55;

    //losowanie pozycji poczatkowej i ustalanie
    //kierunku ruchu
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
    
    // funkcja switch plynnie pozycjonujaca samochod
    // do pozycji startowej
    switch (dir+2*hv)
    {
    case 2: //prawo
        for(;arg_struct->y<ymax && !finish;arg_struct->y++){
        sleep_for(arg_struct->delay);
        }
    case 0: //dol
        for(;arg_struct->x<xmax && !finish;arg_struct->x++){
        sleep_for(arg_struct->delay);
        }
    case 3: //lewo
        for(;arg_struct->y>ymin && !finish;arg_struct->y--){
        sleep_for(arg_struct->delay);
        }
    case 1: //gora
        for(;arg_struct->x>xmin && !finish;arg_struct->x--){
        sleep_for(arg_struct->delay);
        } 
        break;
    default:
        break;
    }
    //pętla poruszajaca pojazd po torze
    while(!finish){
       for(arg_struct->y=ymin;arg_struct->y<ymax && !finish;arg_struct->y++){
        sleep_for(arg_struct->delay);
        }
        for(arg_struct->x=xmin;arg_struct->x<xmax && !finish;arg_struct->x++){
        sleep_for(arg_struct->delay);
        }
        for(;arg_struct->y>ymin && !finish;arg_struct->y--){
        sleep_for(arg_struct->delay);
        }
        for(;arg_struct->x>xmin && !finish;arg_struct->x--){
        sleep_for(arg_struct->delay);
        }
    }

    return NULL;
}

/**
 * Funkcja symulująca ruch pojazdów po torze B
 * 
 * @param arg instancja samochodu przekazana do watku
 * @return void* 
 */
void* car_runner_B(void* arg){
    struct car *arg_struct = (struct car*) arg;
    // czas po jakim samochod wjedzie na tor
    int time = rand()%60;
    for(int i=0;i<time && !finish;i++){
        sleep_for(seconds(1));
    }
    

    //zmienne kontrolujace zakres toru i pozycje poczatkowa
    int xmin=6, ymin=10;
    int xmax=16,ymax=70;
    arg_struct->y=0;
    arg_struct->x=xmin;

    //pętla poruszajaca pojazd po torze przez 3 okrazenia
    for(int i=0;i<3 && !finish;i++){
       for(;arg_struct->y<ymax && !finish;arg_struct->y++){
        sleep_for(arg_struct->delay);
        }
        for(;arg_struct->x<xmax && !finish;arg_struct->x++){
        sleep_for(arg_struct->delay);
        }
        for(;arg_struct->y>ymin && !finish;arg_struct->y--){
        sleep_for(arg_struct->delay);
        }
        // opuszczenie toru
        if(i==2) break;
        for(;arg_struct->x>xmin && !finish;arg_struct->x--){
        sleep_for(arg_struct->delay);
        }
    }
    // odjazd samochodu do garazu
    for(;arg_struct->y>0 && !finish;arg_struct->y--){
        sleep_for(arg_struct->delay);
    }
    return NULL;
}

void printTracks(){
    for(int y=25;y<=55;y++){
            mvprintw(2,y,"%c",'-');
            mvprintw(22,y,"%c",'-');
        }
        for(int x=3;x<22;x++){
            mvprintw(x,25,"%c",'|');
            mvprintw(x,55,"%c",'|');
        }
        for(int y=10;y<=70;y++){
            mvprintw(6,y,"%c",'-');
            mvprintw(16,y,"%c",'-');
        }
        for(int x=7;x<16;x++){
            mvprintw(x,10,"%c",'|');
            mvprintw(x,70,"%c",'|');
        }
        mvprintw(6,25,"%c",'+');
        mvprintw(6,55,"%c",'+');
        mvprintw(16,25,"%c",'+');
        mvprintw(16,55,"%c",'+');
}

void* car_starter_B(void* arg){
    char* id_ptr=(char*)(arg);
    char id= *id_ptr;
    int i=0;
    int rc;
    
    while(!finish){
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
    return NULL;
}

int main(int argc, char** argv){
    WINDOW* win = initscr();
    srand( time(NULL) );


    // inicjalizacja watkow dla toru A
    int numOfCarsA=3;
    struct car args[numOfCarsA];
    pthread_t tids[numOfCarsA];
    for(int i=0; i<numOfCarsA;i++){
        args[i].id='A'+i;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_create(&tids[i],&attr, car_runner_A, &args[i]);
    }

    // inicjalizacja watkow dla toru B
    int numOfCarsB=10;
    struct car argsB[numOfCarsB];
    pthread_t tidsB[numOfCarsB];
    for(int i=0; i<numOfCarsB;i++){
        argsB[i].id='D'+i;
        argsB[i].delay=milliseconds(rand()%190+10);
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_create(&tidsB[i],&attr, car_runner_B, &argsB[i]);
    }

    // wyswietlanie obiektow na ekranie
    curs_set(0);
    noecho();
    nodelay(win ,true);
    
    while(!finish){
        clear();
        printTracks();
        for(int j=0;j<numOfCarsA;j++){
            mvprintw(args[j].x,args[j].y,"%c",args[j].id);
        }
        for(int j=0; j<numOfCarsB;j++){
            if(argsB[j].y!=0)
            mvprintw(argsB[j].x,argsB[j].y,"%c",argsB[j].id);
        }
        refresh();
        sleep_for(milliseconds(50));
        if(getch()=='q') finish=true;
    }
    for(int i = 0; i< numOfCarsA;i++) pthread_join(tids[i], NULL);
    for(int i=0;i<numOfCarsB;i++)pthread_join(tidsB[i], NULL);
    endwin();

    return 0;
}
