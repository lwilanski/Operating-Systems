#include <string.h>
#include <mqueue.h>
#include <sys/mman.h>
#include <stdio.h>
#include <fcntl.h>
#include <semaphore.h>
#define ROZM_BLOKU 1024
#define L_SLOW 8

int main(void){//POSIX
  char slownik[L_SLOW][10]={"alfa","bravo","charlie","delta","echo","foxtrot","golf","hotel"};
  //stworz dwa zamkniete semafory o identyfikatorach id_sem0 i id_sem1

  //
  //
  //stworz pamiec wspoldzielona, okresl jej rozmiar na ROZM_BLOKU

  //
  //
  //dolacz segment pamieci do przestrzeni adresowej procesu,
  //wsk ma wskazywac na te pamiec

  //

  for(i=0; i<=L_SLOW; i++){
    sem_wait(id_sem1);
    if(i<L_SLOW) strcpy(wsk,slownik[i]);
    else         wsk[0]='!';
    sem_post(id_sem0);
  }

  munmap(wsk, ROZM_BLOKU);
  return 0;
}
//kompilacja -lrt -lpthread
