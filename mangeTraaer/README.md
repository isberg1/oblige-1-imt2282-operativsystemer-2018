 README #

### What is this repository for? ###
oblig 1 i OS
Oppgave fra kap 5  Lab exercises a)Lage nye traader og enkel semafor-synkronisering av disse

### How do I run program? ###

last ned repo

i terminalen: flytt deg til den mappen som har repoet

     kjør komando: gcc -Wall -pthread -o traaer main.c
     kjør komando: ./traaer

### code quality ###
Til kvalitetstesing av koden har jeg kjoort 
alle testene som Erik H. henviser til paa siden:
 
https://github.com/NTNUcourses/opsys/blob/master/README-C-code-quality.txt

For å intalere disse programene bruk følgende komadoer:

sudo apt-get install clang-tidy-5.0

sudo apt-get install gcc

sudo apt-get install cppcheck

sudo apt-get install valgrind
  
  
  Kjoorte foolgende:
  
       gcc -Wall -pthread -o traaer mangeTraaer.c
       
  Resultat:
  
       alt ok
  
  Kjoorte foolgende:
  
       cppcheck --enable=all ./mangeTraaer.c
       
  Resultat:
  
       cppcheck klarer ikke aa finne noen som helst av included
       biblioteker.
	   
  Komentar: 
  
       Dette tror jeg er et problem med cppcheck og ikke
       koden min. 
   
  Kjoorte foolgende:
  
       clang-tidy-5.0 -checks='*' mangeTraaer.c -- -std=c11 
       
  Resultat:

          Error while processing /home/isberg/NetBeansProjects/mangeTråer/main.c.
          /usr/include/sched.h:28:10: error: 'stddef.h' file not found [clang-diagnostic-error]
          #include <stddef.h>


  Komentar: 
  
       Jeg includer ikke <stddef.h> selv. saa det kan vaare et bibliotek
       som includer det. Det foor jeg ikke gjort noe med
  
  
  Kjoorte foolgende:
  
      valgrind --leak-check=yes ./traaer
      
  Resultat:
  
       alt ok   
  
  Kjoorte foolgende: 
  
       valgrind --tool=helgrind ./traaer 
   Resultat:
   
       alt ok   

### Who do I talk to? ###
Alexander Jakobsen, 16BITSEC, Studentnr: 473151, alexajak@stud.ntnu.no