#include <stdio.h> /* printf */
#include <stdlib.h> /* exit */
#include <sys/types.h> /* pid_t */
#include <sys/wait.h> /* waitpid */
#include <unistd.h> /* fork */



/*

0 start
2 start

1 start
4 start
o slutt

3 start
2 slutt
1 slutt

5 start
4 slutt

3 slutt

5 slutt

*/

void process(int number, int time) {
    printf("Prosess %d kjÃļrer\n", number);
    sleep(time);
    printf("Prosess %d kjÃļrte i %d sekunder\n", number, time);
}

int main(void){

 int p0, p1, p2, p3, p4, p5;

 p0=fork(); //childprosess 0
 if(p0 < 0){printf("feil fra p0\n"); return 0; } //printer feilmeding
 else if(p0 == 0){process(0,1); exit(0); } //kjÃļr prosess 0
 else {  // fremdeles i opprinelig prosess

        p1 = fork(); //childprosess 2
        if(p1 < 0) { printf("feil fra p1\n"); return 0;} //printer feilmeding
        else if( p1 == 0 ) { process(2,3); exit(0);} //kjÃļr prosess 2
        else { // fremdeles i opprinelig prosess 

               waitpid(p0,NULL,0); //vent pÃĨ prosess 0
               p3=fork(); //childprosess 1
               if(p3 < 0 ) { printf("feil fra p3\n"); return 0; } //printer feilmeding
               else if(p3 == 0) { process(1,2); exit(0); } //kjÃļr prosess 1
               else {  // fremdeles i opprinelig prosess

                      waitpid(p0,NULL,0); //vent pÃĨ prosess 0
                      p2 = fork(); //childprosess 4
                      if(p2 < 0) { printf("feil fra p2\n"); return 0; } //printer feilmeding
                      else if(p2 == 0) { process(4,3); exit(0); } //kjÃļr prosess 4
                      else {  // fremdeles i opprinelig prosess

                             waitpid(p1,NULL,0); //vent pÃĨ prosess 2
                             p4 =fork(); //childprosess 3
                             if(p4 < 0) { printf("feil fra p4"); return 0; } //printer feilmeding
                             else if (p4 == 0) { process(3,2); exit(0); } //kjÃļr prosess 3
                             else {   // fremdeles i opprinelig prosess

                                    waitpid(p2,NULL,0); //vent pÃĨ prosess 4
                                    p5 = fork(); //childprosess 5
                                    if(p5 < 0) { printf("feil fra p5"); return 0; } //printer feilmeding
                                    else if(p5 == 0) { process(5,3); exit(0); } //kjÃļr prosess 5                  
                                   }
                            }
                     }
             }
      }
waitpid(p5,NULL,0); // venter pÃĨ at siste prosses(nr. 5) skal bli ferdig.
return 0;
}          




