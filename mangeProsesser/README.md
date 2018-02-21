 README #

### What is this repository for? ###
oblig 1 i OS
Oppgave fra kap 4  Lab exercises b) Lage nye prosesser og enkel synkronisering av disse

### How do I run program? ###

last ned repo

i terminalen: flytt deg til den mappen som har repoet

     kjør komando: gcc -Wall -o prosesser mangeProsesser
     kjør komando: ./prosesser


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
  
       gcc -Wall -pthread -o prosesser mangeProsesser.c 
       
  Resultat:
  
       alt ok
  
  Kjoorte foolgende:
  
       cppcheck --enable=all ./mangeProsesser.c
       
  Resultat:
  
       cppcheck klarer ikke aa finne noen som helst av included
       biblioteker.
	   
  Komentar: 
  
       Dette tror jeg er et problem med cppcheck og ikke
       koden min. 
   
  Kjoorte foolgende:
  
       clang-tidy-5.0 -checks='*' mangeProsesser.c -- -std=c11 
       
  Resultat:
  
  		1) klager(warning) på måten jeg har skrevet koden(leslighet)  
  
      	2) /usr/include/stdio.h:33:11: error: 'stddef.h' file not found [clang-diagnostic-error]
		# include <stddef.h>

  Komentar: 
  
  		1) clang-tidy sier at jeg ikke skel bruke "else" etter "return"
		dette er feil fra clang-tidy. der det klages bruker jeg ikke "else" men "else if".
		jeg må bruke "else if" der. så jeg endrer det ikke.
  
       2) Jeg includer ikke <stddef.h> selv. saa det kan vaare et bibliotek
       som includer det. Det foor jeg ikke gjort noe med
  
  
  Kjoorte foolgende:
  
       valgrind --leak-check=yes ./prosesser 
  Resultat:
  
       alt ok   
  
  Kjoorte foolgende: 
  
       valgrind --tool=helgrind ./prosesser 
   Resultat:
   
       alt ok   

### Who do I talk to? ###
Alexander Jakobsen, 16BITSEC, Studentnr: 473151, alexajak@stud.ntnu.no