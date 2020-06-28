#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#define VOTER_CAME 0
#define VOTER_WAITING 1
#define VOTER_ASSIGNED 2
#define VOTER_DONE 3

typedef struct Booth Booth;

typedef struct EVM
{
  int num;
  pthread_t evm_thread_id;
  Booth * booth;
  int number_of_slots;
  int util;
  int slot_done;
  int voter_num_ongoing;
  int iterator;
} EVM;

typedef struct Voter
{
  int num;
  pthread_t voter_thread_id;
  Booth * booth;
  EVM * evm;
  int status;
  int iterator;
} Voter;

typedef struct Booth
{
  int num;
  pthread_t booth_thread_id;
  int number_of_evms;
  int max_slots_in_evm;
  int number_of_voters;
  int left_voters;
  EVM ** evms;
  Voter ** voters;
  pthread_mutex_t mutex;
  pthread_cond_t cv_1;
  pthread_cond_t cv_2;
} Booth;

Booth* booth_init( Booth * booth, int num,int max_slots_in_evm, int number_of_voters);
void * booth_util(void * args);
void* booth_free(Booth * booth);

//EVM* evm_init(Booth * booth, int num, EVM * evm,int init1,int init2);
void * evm_util(void * args);

//Voter* voter_init( Booth * booth,Voter * voter,int num,int init1,int init2);
void * voter_util(void * args);
