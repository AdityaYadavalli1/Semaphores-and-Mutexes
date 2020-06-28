#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include "def.h"
int random_voter_assignment(int mod)
{
  int random=rand();
  int ret = random%mod;
  return ret;
}
int polling_ready_evm(void * args,int left,int allocated_slots,int iterator2)
{
  EVM *evm = (EVM*)args;
  Booth *booth = evm->booth;
  pthread_mutex_t *mutex_ptr = &(booth->mutex);//cv1->evm_wait
  int j = 0, i;
  while(left!=0)
  {
     iterator2 = random_voter_assignment(booth->number_of_voters);//rand()%booth->number_of_voters;
     pthread_mutex_lock(mutex_ptr);
     if(booth->left_voters == 0)
     {
       pthread_mutex_unlock(mutex_ptr);
       return (allocated_slots-left);
     }
     if(booth->voters[iterator2]->status == VOTER_WAITING&&booth->voters[iterator2]->status!=VOTER_ASSIGNED&&booth->voters[iterator2]->status!=VOTER_DONE)
     {
       booth->voters[iterator2]->status = VOTER_ASSIGNED;
       booth->voters[iterator2]->evm = evm;
       left--;
       (booth->left_voters)--;
       printf("%d booth %d evm %d voter is alloted.\n", booth->num, evm->num, iterator2+1);
       if(booth->left_voters == 0)
       {
         pthread_mutex_unlock(mutex_ptr);
         return (allocated_slots-left);
       }
     }
     pthread_mutex_unlock(mutex_ptr);
   }
   return (allocated_slots-left);
}
void * evm_util(void * args)
{
  EVM * evm = (EVM*)args;
  Booth * booth = evm->booth;
  pthread_mutex_t * mutex_ptr = &(booth->mutex);
  while(evm->util)
  {
    pthread_mutex_lock(mutex_ptr);
      if(booth->left_voters == 0)
      {
        pthread_mutex_unlock(mutex_ptr);
        break;
      }
    pthread_mutex_unlock(mutex_ptr);

    /* evm getting free slots and waiting for voters to get assigned */
    /* polling_ready_evm */
    int j = 0;
    int allocated_slots = rand()%(booth->max_slots_in_evm) + 1;
    evm->number_of_slots = allocated_slots;
    pthread_mutex_lock(mutex_ptr);
    evm->slot_done = 0;
    pthread_mutex_unlock(mutex_ptr);
    printf("%d booth %d evm %d slots free.\n", booth->num, evm->num, allocated_slots);
    pthread_cond_t * evm_wait = &(booth->cv_1);
    if(allocated_slots != 0)
    {
      j=polling_ready_evm(args,allocated_slots,allocated_slots,0);
      sleep(1);
    }
    else
      break;
    /* evm executing voting phase. */ /* voter_in_slot */
    pthread_cond_t * voter_wait = &(booth->cv_2);
    printf("%d booth %d evm has started voting phase.\n", booth->num, evm->num);
    pthread_mutex_lock(mutex_ptr);
    evm->number_of_slots = j;
    evm->slot_done = 1;
    pthread_cond_broadcast(evm_wait);
    for(evm->iterator=0;evm->number_of_slots!=0;evm->iterator++)
    {
      pthread_cond_wait(voter_wait, mutex_ptr);
    }
    pthread_mutex_unlock(mutex_ptr);

    printf("%d booth %d evm has finished voting phase.\n", booth->num, evm->num);
  }
  printf("%d booth %d evm signing off.\n", booth->num, evm->num);
}
