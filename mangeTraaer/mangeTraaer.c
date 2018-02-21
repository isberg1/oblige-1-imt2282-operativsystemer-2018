
/* Oppgave: kap 5. lab exercices a)
 * 
 * File:   mangeTråer.c
 * Author: Alexander Jakobsen, 16BITSEC, Studentnr: 473151
 * E-mail: alexajak@stud.ntnu.no
 * Based on template form:
 * https://github.com/NTNUcourses/opsys/blob/master/05-ipc-concurrency/thread-start-stop-semafor-hjelp1a.c 
 * Created on January 31, 2018, 11:27 AM
 * Created with NetBeans IDE in Ubuntu 16.04.3 LTS
 */
#include <pthread.h>            /* POSIX tråer*/
#include <stdio.h>		/* printf */
#include <stdlib.h>		/* exit */
#include <unistd.h>		/* fork */

#include <semaphore.h>          /* Semaphore datatype*/

#define SHARED 0

sem_t sem[6];

// En datastruktur til å gi info til trådene
struct threadargs {
	int id;         // Trådens ID
	int sec;        // Hvor lenge skal tråden kjøres
	int signal[6];  // Hvem skal tråden gi signal til
                  // når den er ferdig?
};

void* tfunc(void *arg) {
	struct threadargs *targs=arg;
        int returVerdi =0;
        
  // Vent på lov til å kjøre (tell ned egen semafor)
        returVerdi = sem_wait(&sem[targs->id]);  //busy waiting 
        if(returVerdi != 0) {
            // feilmelding og retur hvis sem_post feilet
            printf("feil fra sem_wait for traa %d\n", targs->id);
            return (EXIT_SUCCESS);
        }    
  printf("Tråd %d har startet\n", targs->id); // Print at tråden starter
  sleep(targs->sec);   // Vent noen sekunder
  // Print at tråden er ferdig
   printf("Tråd %d er ferdig. Den har kjøret i %d sekund\n", targs->id, targs->sec );
  // For i in 0->5:
   for( int i=0; i < 6; i++ )
   {
   // Hvis "jeg" skal signalisere i: , Tell opp semafor til tråd i
       if( targs->signal[i] == 1 ) { 
           returVerdi = sem_post(&sem[i]); 
           if( (returVerdi != 0 ) ) {   // feilmelding hvis sem_post feilet             
               printf("feil fra sem_post for traa %d\n", i);
           }
       }         
   }  
  return (EXIT_SUCCESS);
};

int main(void) {
  int i, j;                    // Tellere til løkker
  struct threadargs* targs[6]; // Pekere til argumentene vi gir til trådene.
  pthread_t tid[6];            // Variabler til å holde trådinformasjon

  // Initialiser argumentene vi gir til trådene.
  for(j = 0; j < 6; j++) {
    targs[j] = malloc(sizeof(struct threadargs)); // Sett av minne, og
    targs[j]->id = 0;                             // sett alle variablene
    targs[j]->sec = 0;                            // til 0.
    for(i = 0; i < 6; i++) {
      targs[j]->signal[i] = 0;
    }
  }    
  // Sett opp data til alle strukter
  targs[0]->id = 0;
  targs[0]->sec = 1;
  targs[0]->signal[1] = 1;
  targs[0]->signal[4] = 1;
  targs[1]->id = 1;
  targs[1]->sec = 2;
  targs[1]->signal[3] = 1;
  targs[2]->id = 2;
  targs[2]->sec = 3;
  targs[3]->id = 3;
  targs[3]->sec = 2;
  targs[4]->id = 4;
  targs[4]->sec = 3;
  targs[4]->signal[5] = 1;
  targs[5]->id = 5;
  targs[5]->sec = 3;
// Start T0 og T2. og sjekk returverdi, eventuelt gi feilmelding og avbryt program
  for(int b = 0; b < 3; b+=2) {                    
      // sjekk returverdi, eventuelt gi feilmelding og avbryt program
      if(sem_init(&sem[b], SHARED, 1) != 0){
        printf("feil fra sem_init traa %d",b); return 0;
      }  
      // sjekk returverdi, eventuelt gi feilmelding og avbryt program
      if(pthread_create(&tid[b], NULL, tfunc, targs[b]) != 0){
        printf("feil fra sem_create traa %d",b); return 0;
      }      
  }  
// Start T1 og sjekk returverdi, eventuelt gi feilmelding og avbryt program
  if(sem_init(&sem[1], SHARED, 0) != 0){
        printf("feil fra sem_init traa 1"); return 0;
    }  
  if(pthread_create(&tid[1], NULL, tfunc, targs[1]) != 0){
        printf("feil fra sem_create traa 1"); return 0;
    }     
  
// Start T3 til T5 og sjekk returverdi, eventuelt gi feilmelding og avbryt program
  for(int c = 3; c < 6; c++) {      
      if(sem_init(&sem[c], SHARED, 0) != 0){
        printf("feil fra sem_init traa %d",c); return 0;
      }  
      if(pthread_create(&tid[c], NULL, tfunc, targs[c]) != 0){
        printf("feil fra sem_create traa %d",c); return 0;
      }      
  }  
// Vent på alle tråder, frigi minne 
  for(int a = 0; a < 6; a++){
      if( pthread_join(tid[a], NULL) != 0  ) {
          printf("feil fra sem_join traa %d", a);  
      }            
      free(targs[a]); //frigi minne
  }    
  return 0;
}
