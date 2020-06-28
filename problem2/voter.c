#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include "def.h"
void * voter_in_slot(void * args)
{
  Voter * voter = (Voter*)args;
  Booth * booth_of_voter=voter->booth;
  pthread_mutex_t * mutex_ptr = &(booth_of_voter->mutex);
  pthread_cond_t * cv_1_ptr = &(booth_of_voter->cv_1);
  pthread_cond_t * cv_2_ptr = &(booth_of_voter->cv_2);
  pthread_mutex_lock(mutex_ptr);
  while(voter->evm->slot_done == 0)//till not done
    pthread_cond_wait(cv_1_ptr, mutex_ptr);
  (voter->evm->number_of_slots)--;
  voter->iterator++;
  pthread_cond_broadcast(cv_2_ptr);
  printf("%d booth %d evm %d voter has casted its vote.\n",booth_of_voter->num, voter->evm->num, voter->num);
  pthread_mutex_unlock(mutex_ptr);
}
void * voter_wait_for_evm(void *args)
{
  Voter * voter = (Voter*)args;
  Booth * booth_of_voter=voter->booth;
  pthread_mutex_t * mutex_ptr = &(booth_of_voter->mutex);
  pthread_cond_t * cv_1_ptr = &(booth_of_voter->cv_1);
  pthread_cond_t * cv_2_ptr = &(booth_of_voter->cv_2);
  pthread_mutex_lock(mutex_ptr);
  EVM * evm = voter->evm;
  voter->status = VOTER_WAITING;
  while(voter->status == VOTER_WAITING&&voter->status!=VOTER_ASSIGNED&&voter->status!=VOTER_DONE)
    pthread_cond_wait(cv_1_ptr, mutex_ptr);
  pthread_mutex_unlock(mutex_ptr);
}
void * voter_util(void * args)
{
  Voter * voter = (Voter*)args;
  sleep(2);
  voter_wait_for_evm(args);
  sleep(1);
  voter_in_slot(args);
}
