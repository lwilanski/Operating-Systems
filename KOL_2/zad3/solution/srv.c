#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>

#define L_SLOW 8

int main(void){
 int status,gniazdo,dlugosc,gniazdo2,lbajtow,i;
 struct sockaddr_in ser,cli;
 char buf[200];
 char pytanie[L_SLOW]="abcdefgh";
 char odpowiedz[L_SLOW][10]={"alfa","bravo","charlie","delta","echo","foxtrot","golf","hotel"};

 //stworz gniazdo TCP, przypisz mu numer portu oraz interfejs sieciowy 127.0.0.1, zwroc status
 //utwórz kolejkê oczekujacych równa 10
 while (1){
  //czekaj na przychodzace po³aczenie; gdy przyjdzie przypisz mu 'gniazdo2'
  //...

  if (gniazdo2==-1) {printf("blad 03\n"); return 0;}
  lbajtow=1;
  while(lbajtow>0){
    lbajtow = read (gniazdo2, buf, sizeof buf);
    if (lbajtow>0){
      for(i=0; i<L_SLOW && pytanie[i]!=buf[0]; i++);
      if (i<L_SLOW) write (gniazdo2, odpowiedz[i], strlen(odpowiedz[i]));
    }
  }
  close (gniazdo2);
 }
 close(gniazdo);
 printf("KONIEC DZIALANIA SERWERA\n");
 return 0;
}

