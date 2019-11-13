#include "sharedMem.h"
sem_t mutex, timeMutex, deadMutex;

float checkTime(){                                                       //function for the getting current virtual time
    float currentTime;
    sem_init(&timeMutex, 1, 1);
    sem_wait(&timeMutex);
    currentTime = (float)getSeconds() + getNano();
    sem_post(&timeMutex);
    sem_destroy(&timeMutex);
    return currentTime;
}

int requestOrRelease(){                                  //function for getting a random number for a request or release
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
    int dataId = shmget(dataKey, 1024, 0666|IPC_CREAT);
    key_t decisionKey = 69;

    do {                                                                   //nice little do while loop for our semaphore

        if(checkTime() >= checkMe){            //check if the current time is equal to or greater than termination check
            sem_wait(&mutex);                                                               //enter critical section
            leaveItToChance = getRandom(100, 0);                                                //get the value 0 to 100
            if(leaveItToChance < 2 ){                                               //if the value is 0-2 or a 2% chance
                sem_wait(&deadMutex);                                                               //enter critical section
                int decisionId = shmget(decisionKey, 1024, 0666|IPC_CREAT);
                iShould = shmat(decisionId, NULL, 0);
                iShould->deadCount = 1;             //set deadCount to 1 so parent knows to add a dead process for count
                sem_post(&deadMutex);                                                              //end of critical section
                break;                                                                           //break out of the loop
            }
            else{

                int decisionId = shmget(decisionKey, 1024, 0666|IPC_CREAT);
                int whichResource = getRandom(19, 0);                                            //get a random resource
                if(requestOrRelease() == 1){                                 //if 1, we are gonna say we want to request
                    iShould = shmat(decisionId, NULL, 0);                 //set up our information struct for the parent
                    iShould->requestOrRelease = 1;
                    iShould->element = element;
                    iShould->resource = whichResource;
                    iShould->deadCount = 0;
                    shmdt(resources);
                    shmdt(iShould);
                    //request
                }else{                                                          //otherwise the process wants to release
                    iShould = shmat(decisionId, NULL, 0);                     //set up our information struct for parent
                    iShould->requestOrRelease = 0;
                    iShould->element = element;
                    iShould->resource = whichResource;
                    shmdt(resources);
                    shmdt(iShould);
                    //release resource
                }
                checkMe += checkIfTerminate;                                   //if not between 0-9, get next check time

            }
            sem_post(&mutex);                                                              //end of critical section
        }

    }while(1);

}

int main(int argc, char* argv[]){
    float creationTime = checkTime();                                                                //get creation time
    int element = atoi(argv[1]);                                                                   //save process number
    float checkIfTerminate = ((float)getRandom(250, 0)/1000);                           //get time for termination check

    int i;
    key_t dataKey = 65;
    int dataId = shmget(dataKey, 1024, 0666|IPC_CREAT);
    resources = shmat(dataId, NULL, 0);
    for(i = 0; i < 20; i++){                                //reinitialize the resource table, getting random max claims
        resources[element].maxClaims[i] = getRandom(maxResources[i], 0);
        resources[element].allocations[i] = 0;
    }
    shmdt(resources);

    sem_init(&mutex, 1, 1);                                                                       //initialize semaphore
    sem_init(&deadMutex, 1, 1);
    threadFunc(creationTime, checkIfTerminate, element);                                       //call semaphore function
    sem_destroy(&mutex);                                                                         //destroy the semaphore
    sem_destroy(&deadMutex);
    exit(0);
}