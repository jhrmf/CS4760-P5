#ifndef SHAREDMEM_H
#define SHAREDMEM_H

#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <pthread.h>

/*holy cow look at all these headers!*/

int getSeconds(){                               //function for retrieving the second in our time stored in shared memory
    key_t key = 66;
    int secID = shmget(key, 2048, 0444);
    char *tempTime = (char*) shmat(secID, (void*)0, 0);
    int seconds = atoi(tempTime);
    shmdt(tempTime);
    return seconds;
}
float getNano(){                            //function for retrieving the nanosecond our in time stored in shared memory
    key_t key = 67;
    int nanoID = shmget(key, 2048, 0444);
    char *tempTime = (char*) shmat(nanoID, (void*)0, 0);
    float nano = (float)(atoi(tempTime)) / 1000000000;
    shmdt(tempTime);
    return nano;
}

struct myTime{                                                                             //struct for our virtual time
    unsigned int seconds;
    unsigned int nanoseconds;

};

struct RESOURCE{

    int alive;

    int maxClaims;
    int requests;
    int allocations;
    int released;

};

struct WHATDO{
    int requestOrRelease;
    int element;
};

struct myTime virtual;                                                             //struct for holding the virtual time
struct RESOURCE *resources;
struct WHATDO *iShould;

const int getRandom(const int max, const int min){            //function for getting a random number between max and min
    int randomNumber = ((rand() % (max + 1 - min)) + min);
    return randomNumber;
}

#endif