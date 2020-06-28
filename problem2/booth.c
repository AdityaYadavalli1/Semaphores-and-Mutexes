#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include "def.h"
void * booth_free(Booth * booth)
{
  int i=0;
  while(i<booth->number_of_voters)
  {
    free(booth->voters[i]);
    i++;
  }
  i=0;
  while(i<booth->number_of_evms)
  {
    free(booth->evms[i]);
    i++;
  }
  free(booth->evms);
  free(booth->voters);
}
void * booth_util(void* args)
{
  Booth * booth = (Booth*)args;
  /* initialise voters and evms init */
  int i=0;
  while(i<booth->number_of_voters)
  {
    //booth->voters[i] = voter_init( booth,booth->voters[i],i+1,0,0);//for now it is 0,0
    Voter *voter = (Voter*)malloc(sizeof(Voter));
    voter->num = i+1;
    voter->booth = booth;
    voter->status = VOTER_CAME;
    voter->evm = NULL;
    voter->iterator=0;
    booth->voters[i]=voter;
    i++;
  }
  i=0;
  while(i<booth->number_of_evms)
  {
      //booth->evms[i] = evm_init(booth, i+1, (booth->evms[i]),1,0);
      EVM *evm =(EVM *)malloc(sizeof(EVM));
      evm->num=i+1;
      evm->booth = booth;
      evm->number_of_slots = 0;
      evm->util=1;
      evm->slot_done = 0;
      evm->voter_num_ongoing = 0;
      evm->iterator= 0;
      booth->evms[i]=evm;
      i++;
  }
  i=0;
  while(i<booth->number_of_voters)
  {
    pthread_create(&(booth->voters[i]->voter_thread_id),NULL, voter_util, booth->voters[i]);
    i++;
  }
  i=0;
  while(i<booth->number_of_evms)
  {

    pthread_create(&(booth->evms[i]->evm_thread_id),NULL, evm_util, booth->evms[i]);
    i++;
  }
  /* evms and voters threads joined */
  while(i<booth->number_of_voters)
  {
    pthread_join(booth->voters[i]->voter_thread_id, 0);
    i++;
  }
  i=0;
  while(i<booth->number_of_evms)
  {
    pthread_join(booth->evms[i]->evm_thread_id, 0);
    i++;
  }

  printf("%d booth is about to close.\n", booth->num);
  /* freeing memory */
  booth_free(booth);
}
Booth* booth_init( Booth * booth, int num,int number_of_evms, int number_of_voters)
{
  booth = (Booth*)malloc(sizeof(Booth));
  pthread_mutex_init(&(booth->mutex), NULL);
  pthread_cond_init(&(booth->cv_1), NULL);
  pthread_cond_init(&(booth->cv_2), NULL);
  booth->evms = (EVM**)malloc(sizeof(EVM*)*number_of_evms);
  booth->num = num;
  booth->number_of_evms = number_of_evms;
  booth->max_slots_in_evm = 10;
  booth->voters = (Voter**)malloc(sizeof(Voter*)*number_of_voters);
  booth->number_of_voters = number_of_voters;
  booth->left_voters = number_of_voters;
  return booth;
}
