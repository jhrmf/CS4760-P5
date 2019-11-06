#include "sharedMem.h"
sem_t mutex, timeMutex;

float checkTime(){
    float currentTime;
    sem_init(&timeMutex, 1, 1);
    sem_wait(&timeMutex);
    currentTime = (float)getSeconds() + getNano();
    sem_post(&timeMutex);
    sem_destroy(&timeMutex);
    return currentTime;
}

int requestOrRelease(){
    int chance = getRandom(100, 0);
    if(chance < 30){
        return 0;
    }else{
        return 1;
    }
}

void threadFunc(float creationTime, float checkIfTerminate, int element){            //function that holds our semaphore
    float checkMe = creationTime + checkIfTerminate;    //float that holds time for checking if process should terminate
    int leaveItToChance;                                   //integer that gets a value between 0 and 100 to get a chance
    key_t dataKey = 65;
    int dataId = shmget(dataKey, 2048, 0666|IPC_CREAT);
    key_t decisionKey = 69;

    do {                                                                   //nice little do while loop for our semaphore

        if(checkTime() >= checkMe){            //check if the current time is equal to or greater than termination check
            leaveItToChance = getRandom(0, 100);                                                //get the value 0 to 100
            if(leaveItToChance < 10 ){                                             //if the value is 0-9 or a 10% chance
                resources = shmat(dataId, NULL, 0);                         //attach to shared memory for resource table
                resources[element].alive = 0;                                                     //mark it as not alive
                shmdt(resources);                                                            //detach from shared memory
                break;
            }
            else{
                sem_wait(&mutex);                                                               //enter critical section
                int decisionId = shmget(decisionKey, 2048, 0666|IPC_CREAT);
                if(requestOrRelease() == 1){
                    iShould = shmat(decisionId, NULL, 0);
                    iShould->requestOrRelease = 1;
                    iShould->element = element;
                    shmdt(resources);
                    shmdt(iShould);
                    //request
                }else{
                    iShould = shmat(decisionId, NULL, 0);
                    iShould->requestOrRelease = 0;
                    iShould->element = element;
                    shmdt(resources);
                    shmdt(iShould);
                    //release resource
                }
                checkMe += checkIfTerminate;                                   //if not between 0-9, get next check time
                sem_post(&mutex);                                                                      //end of critical section
            }
        }



    }while(1);
}

int main(int argc, char* argv[]){
    float creationTime = checkTime();
    int element = atoi(argv[1]);
    float checkIfTerminate = ((float)getRandom(250, 0)/1000);
    int maxClaimsTemp;

    key_t dataKey = 65;
    int dataId = shmget(dataKey, 2048, 0666|IPC_CREAT);
    resources = shmat(dataId, NULL, 0);
    resources[element].alive = 1;
    maxClaimsTemp = resources[element].requests + resources[element].allocations;
    resources[element].maxClaims = getRandom(0, maxClaimsTemp);
    shmdt(resources);

    sem_init(&mutex, 1, 1);
    threadFunc(creationTime, checkIfTerminate, element);                                       //call semaphore function
    sem_destroy(&mutex);                                                                         //destroy the semaphore

    exit(0);
}