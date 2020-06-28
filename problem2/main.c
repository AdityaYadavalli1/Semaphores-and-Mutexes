#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include "def.h"
int main()
{
  printf("Enter the number of booths - ");
  int number_of_booths;
  scanf("%d", &number_of_booths);
  int * number_of_evms = (int*)malloc(sizeof(int)*number_of_booths);
  int * number_of_voters = (int*)malloc(sizeof(int)*number_of_booths);
/* assigning */
  for(int i=0; i<number_of_booths; i++)
  {
    printf("Booth %d number of voters and number of evms ",i+1);
    scanf("%d%d",&number_of_voters[i], &number_of_evms[i]);
  }
  printf("Election process initiated.\n");
  Booth ** booths = (Booth**)malloc(sizeof(Booth*)*number_of_booths);
  /* booth init */
  int iterator=0;
  while(iterator<number_of_booths)
  {
    booths[iterator] = booth_init( booths[iterator], iterator+1, number_of_evms[iterator], number_of_voters[iterator]);
    iterator++;
  }
  iterator=0;
  /* booth thread start */
  while(iterator<number_of_booths)
  {
    pthread_create(&(booths[iterator]->booth_thread_id), NULL, booth_util, booths[iterator]);
    iterator++;
  }
  iterator=0;
  /* booth thread joined */
  while(iterator<number_of_booths)
  {
    pthread_join(booths[iterator]->booth_thread_id, 0);
    iterator++;
  }
  printf("Election done\n");
  /* freeing alloted memory */
  free(number_of_voters);
  free(number_of_evms);
  iterator=0;
  while(iterator<number_of_booths)
  {
    free(booths[iterator]);
    iterator++;
  }
  free(booths);
  return 0;
}
