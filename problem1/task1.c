#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define numthreads 10
void *warmUp(void*);
void *adjustEquipment(void*);
int countplayers=0;
int countreferee=0;
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex1=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2=PTHREAD_MUTEX_INITIALIZER;
pthread_t thread_p[2];
pthread_t thread_r[1];
void startgame(int refereenum)
{
  printf("Referee%d starts the game\n",refereenum);
  pthread_mutex_unlock(&mutex);
  printf("The organiser is free\n");
}
void enterCourt(int player1num,int player2num,int refereenum)
{
  int *refereenumptr=&refereenum;
  int *player1numptr=&player1num;
  int *player2numptr=&player2num;
  printf("Referee%d, player%d and player%d enter the court\n",refereenum,player1num,player2num);
  pthread_create(&thread_p[0],NULL,warmUp,(void *)player1numptr);
  pthread_create(&thread_p[1],NULL,warmUp,(void *)player2numptr);
  pthread_create(&thread_r[0],NULL,adjustEquipment,(void *)refereenumptr);
  pthread_join(thread_r[0],NULL);
  pthread_join(thread_p[0],NULL);
  pthread_join(thread_p[1],NULL);
  startgame(refereenum);
}
void meetOrganiser(int player1num,int player2num,int refereenum)
{
  enterCourt(player1num,player2num,refereenum);
}
void enterAcademy(int player1num,int player2num,int refereenum)
{
  pthread_mutex_lock(&mutex1);
  countplayers-=2;
  pthread_mutex_unlock(&mutex1);
  pthread_mutex_lock(&mutex2);
  countreferee-=1;
  pthread_mutex_unlock(&mutex2);
  pthread_mutex_lock(&mutex);
  printf("The group meets the organiser\n");
  meetOrganiser(player1num,player2num,refereenum);
}
int main()
{
  int n;
  scanf("%d",&n);
  int playernum=2*n;
  int refereenum=n;
  while(playernum!=0||refereenum!=0)
  {
    int wait=rand()%3 + 1;
    if(playernum>=refereenum)
    {
      sleep(wait);
      printf("Player%d picked\n",2*n-playernum+1);
      playernum--;
      pthread_mutex_lock(&mutex1);
      countplayers++;
      pthread_mutex_unlock(&mutex1);
    }
    else
    {
      sleep(wait);
      refereenum--;
      printf("Referee%d picked\n",n-refereenum);
      pthread_mutex_lock(&mutex2);
      countreferee++;
      pthread_mutex_unlock(&mutex2);
    }
    if(countplayers>=2&&countreferee>=1)
    {
      printf("A group is formed\n");
      enterAcademy(2*(n-refereenum)-1,(2*(n-refereenum)),n-refereenum);
    }
  }
  printf("SIMULATION IMPLEMENTED\n");
  return 0;
}
void *warmUp(void *dummyPtr)
{
  int * playerptr= (int *)dummyPtr;
  printf("Player%d is warming up\n",*playerptr);
  sleep(1);
}
void *adjustEquipment(void *dummyPtr)
{
  int * refereenumptr=(int *)dummyPtr;
  printf("Referee%d adjusting equipments\n",*refereenumptr);
  sleep(1);
}
