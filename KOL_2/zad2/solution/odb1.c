#include <string.h>
#include <mqueue.h>
#include <sys/mman.h>
#include <stdio.h>
#include <fcntl.h>
#include <semaphore.h>
#define ROZM_BLOKU 1024

int main(void){//POSIX
  //stworz dwa zamkniete semafory o identyfikatorach id_sem0 i id_sem1
  sem_t *id_sem0 = sem_open("nazwa_sem0", O_CREAT, 0644, 0);
  sem_t *id_sem1 = sem_open("nazwa_sem1", O_CREAT, 0644, 0);
  //stworz pamiec wspoldzielona, okresl jej rozmiar na ROZM_BLOKU
  int shm_fd = shm_open("/pamiec_wspolna", O_CREAT | O_RDWR, 0644);
  ftruncate(shm_fd, ROZM_BLOKU);
  //dolacz segment pamieci do przestrzeni adresowej procesu;
  //wsk ma wskazywac na te pamiec
  char *wsk = mmap(0, ROZM_BLOKU, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
  
  wsk[0]=0;
  while(wsk[0]!='!'){
    sem_post(id_sem1);
    sem_wait(id_sem0);
    if(wsk[0]!='!') printf("%s\n", wsk);
  }
  munmap(wsk, ROZM_BLOKU);
  sem_unlink("nazwa_sem0");
  sem_unlink("nazwa_sem1");
  return 0;
}
//kompilacja -lrt -lpthread
