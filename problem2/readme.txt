To run the program :
1) make
2) ./start
There are 4 C program files
1) main.c
2) evm.c
3) booth.c
4) voter.c

In main.c file:
1) This file takes the input and creates all the booth type threads
2) When everything is over it is here that I join threads and end the program

In evm.c
1)In polling_ready_evm function we used on mutex.This is to lock all the booth variables so that other evm(s) don't change it at the same time.
  If an evm is free you randomly pick the voters assigned to that evm and let them vote.
  Voter now goes to voting phase
2)I have used one conditional variable in utility function which locks the mutex until the all the voters in that slot come out.

In voter.c
1) In voter_in_slot function I used a condition variable mutex to make the voter wait until everyone in that slot has finished. I lock the mutex until then
    In this function I also unlocked all the threads blocked by conditional variable used in evm utility function
2) In voter_wait_for_evm function I used another conditional variable to block(lock the mutex) the evm until everyone in that slot has finished

In booth.c
1) Here I create voters and evms and its corresponding threads.
2)Once all the threads end I join voters and evms.
3)If comes till here then that means that that booth is done with all the voters.
