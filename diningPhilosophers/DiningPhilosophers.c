
/* 
 * File:   DiningPhilosophers
 * Author: Alexander Jakobsen, 16BITSEC, Studentnr: 473151
 * E-mail: alexajak@stud.ntnu.no
 * Based on sudokode form:
 * https://github.com/NTNUcourses/opsys/blob/master/05-ipc-concurrency/Tanenbaum%20figur%202-47%20(Dining%20philosophers).pdf
 * and some other code form:
 * https://github.com/NTNUcourses/opsys/blob/master/05-ipc-concurrency/thread-start-stop-semafor-hjelp1a.c 
 * 
 * Created on February 7, 2018, 12:48 PM
 * Created with NetBeans IDE in Ubuntu 16.04.3 LTS
 */

//bibliotek
#include <pthread.h>            /* POSIX tråer*/
#include <semaphore.h>          /* Semaphore datatype*/
#include <stdio.h>		/* printf */
#include <stdlib.h>		/* exit */
#include <unistd.h>		/* fork */

//conster
#define N 5
#define LEFT ((i+5-1)%N)
#define RIGHT ((i+1)%N)
#define THINKING 0
#define HUNGRY 1
#define EATING 2
//structer
struct argumenter { //argument til traa
    int p;
};
//globale variable
sem_t mutex;    //brukes til gaffler
sem_t s[N];     //brukes til spising
sem_t lock;     //brukes til aa laase "antEat"
int state[N];   //filosofers tilstand
int count[N];   //spiseteller
int antEat;     //delt ressurs. brukes til aa avslutte program
//pre deklererte funksjoner
void *philosophers(void *arg); //kontroll funksjon() for filosofer
void takeForks(int i);         // anskaff gaffler
void putForks(int i);          // gi fra gaffler
void test(int i);              // proov aa anskaff gaffler
void eat(int i );              // spis
void think();                  //tenk
int trueFalce();               //til arbeid paa "lock" og "antEat"

int main() {
// div variabler
    char temp[256];
    char *temp2;
    long sjekk=-50;
    struct argumenter *a[N];
    pthread_t traa[N];
    
//skaff brukerdata til antEat
    while(sjekk<1 || sjekk>10000) {
        printf("Hvor mange spiseomganger som skal gjennomføres totalt?\n");
        printf("skriv en tall mellom 1 - 10 000\n");
        scanf("%255s", temp);                   //les brukerdata
        sjekk = strtol(temp, &temp2, 10);       //konverter til "long" hvis mulig
    }
    antEat=sjekk; 
        
//intit sem_t mutex. brukes til arbeid på gaffeler
    if(sem_init(&mutex, 0,1) != 0){ 
        printf("some error form sem_init(mutex)\n");
        return 0;
    }
//intit sem_t lock. brukes til "int trueFalce()"
    if(sem_init(&lock, 0,1) != 0){ 
        printf("some error form sem_init(lock)\n");
        return 0;
    }
//intit sem_t s[N], struck, og spiseteller "count[N]"
    for(int i = 0; i < N; i++){
        if(sem_init(&s[i], 0,1) != 0){ 
            printf("some error form sem_init(s[%d])\n", i);
        return 0;
        }
        count[i]=0; //nullstill spiseteller
        a[i] = malloc(sizeof(struct argumenter));// Sett av minne
        a[i]->p=i;// brukes til å gi filosofer ID
    }
// skap N traaer 
    for(int j = 0; j<N; j++) {
        if(pthread_create(&traa[j], NULL, philosophers, a[j]) != 0  ){
            printf("feil fra pthred_create(%d)", j);
            return 0;
        }
    }       
//vent paa at alle traaer er avsluttet og print 
// spissetelleren "count[N]" for alle filosofer og frigi minne
    for(int g = 0; g<N; g++) {
        if(pthread_join(traa[g], NULL) !=0 ){
            printf("feil fra pthread_join lup omgang %d", g);
            return 0;
        }
        printf("filosof %d har spist ant. ganger = %d\n",g, count[g]);
        free(a[g]);                             //frigi minne
    }
  
    return (EXIT_SUCCESS);
}


//kontrollfunksjon for fillosofer
void *philosophers(void *arg){
    struct argumenter *a = arg;    
        
    while( trueFalce() > 0) {   //kjor til antEat er 0
    //samme som i sudokoden
        think();
        takeForks(a->p);
        eat(a->p);
        putForks(a->p);        
    }
    return(EXIT_SUCCESS);
}

void takeForks(int i){
    //hele funksjonen er samme som sudokode
    //men med test paa returverdier for sem_ funksjoner
    int sjekkTilstand;          //husk tilstand
    if(sem_wait(&mutex) !=0 ){  //laas mutex
        printf("feil fra sem_wait(&mutex) filosof: %d", i);
        exit(0);
    }    
    state[i]=HUNGRY;
    sjekkTilstand=state[i];     //husk tilstand
    test(i);                    //skaff gaffler
     if(sem_post(&mutex) !=0 ){ //laas opp mutex
        printf("feil fra sem_post(&mutex) filosof: %d", i);
        exit(0);
    }     
    if(sjekkTilstand == HUNGRY ){//hvis jeg ikke fikk gaffler
        if(sem_wait(&s[i]) !=0 ){//laas s[i]
            printf("feil fra sem_wait(&s[i]) filosof: %d", i);
            exit(0);
        }
    }       
}

void putForks(int i){
    //hele funksjonen er samme som sudokode
    //men med test paa returverdier for sem_ funksjoner
    if(sem_wait(&mutex) !=0 ){  //laas mutex
        printf("feil fra sem_wait(&mutex) filosof: %d", i);
        exit(0);
    }
    state[i] = THINKING;        //endre tilstand
    test(LEFT);                 //sjekk om nabo vil spise
    test(RIGHT);                //sjekk om nabo vil spise
    if(sem_post(&mutex) !=0 ){  //laas opp mutex
        printf("feil fra sem_post(&mutex) filosof: %d", i);
        exit(0);
    }
}


void test(int i){
//hele funksjonen er samme som sudokode
//men med test paa returverdier for sem_ funksjoner
    if(state[i]== HUNGRY            //sjekk min tilstand
       && state[LEFT] != EATING     //sjekk naboens tilstand
       && state[RIGHT] != EATING){  //sjekk naboens tilstand
        
       state[i]=EATING;             //endre tilstand
       if(sem_post(&s[i]) !=0 ){    //laas opp s[i]
        printf("feil fra sem_post(&s[i]) filosof: %d", i);
        exit(0);
        }       
    }
}

void eat(int i ){
//sov litt, print melding, tell opp spiseteller
    usleep(500);
    printf("philosopher nr: %d eat\n", i);
    count[i]++;
}

void think(){ 
    usleep(500);                    //sov litt
}

int trueFalce() {
// brukes til å avslutte programmet basert paa brukerinputt lagret i "antEat"
    /*
     sudokode er foolgende:
     * down(lock)
     * if(antEat < 1) {
     *   up(lock)
     *   return 0 "falce"
     * }
     * antEat-- 
     * up(lock)
     * return 1 "true" 
     */  
    
    if(sem_wait(&lock) !=0 ){       //laas lock
        printf("feil fra sem_wait(&lock)");
        exit(0);
        }
    if(antEat < 1){                 //sjekk antal spiseomganger
        if(sem_post(&lock) !=0 ){
            printf("feil fra sem_post(&lock)");
            exit(0);
        }
        return 0;
    }    
    antEat--;                       //tell ned antall spiseomganger
    if(sem_post(&lock) !=0 ){       //laas opp lock
        printf("feil fra sem_post(&lock)");
        exit(0);
        }
    return 1;
}
