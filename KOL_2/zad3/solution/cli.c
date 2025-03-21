#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>

#define L_PYTAN 10

int main(void){
 int status,gniazdo,i;
 struct sockaddr_in ser,cli;
 char buf[200];
 char pytanie[]="abccbahhhh";

 //stworz gniazdo TCP, nawiaz polaczenie z IP 127.0.0.1 i usluga na porcie takim samym jak ustaliles w serwerze, zwroc status
 //...

 if (status<0) {printf("blad 01\n"); return 0;}
 for (i=0; i<L_PYTAN; i++){
  status = write(gniazdo, pytanie+i, 1);
  //odczytaj dane otrzymane z sieci, wpisz do tablicy 'buf' i wyswietl na standardowym wyjsciu (ekranie)
  //...

 }
 printf ("\n");

close(gniazdo);
printf("KONIEC DZIALANIA KLIENTA\n");
return 0;
}

