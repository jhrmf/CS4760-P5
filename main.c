#include "sharedMem.h"


struct myTime updateClock(struct myTime virtual){     //function for updating the system clock's seconds and nanoseconds
    char temp1[10], temp2[11];                                               //two temp strings for holding time
    key_t secKey = 66;                                         //the key for the shared memory holding the seconds is 66
    key_t nanoKey = 67;                                    //the key for the shared memory holding the nanoseconds is 67

    int secID = shmget(secKey, 2048, 0666|IPC_CREAT);    //access the shared memory with file write and read permissions
    int nanoID = shmget(nanoKey, 2048, 0666|IPC_CREAT); //same as above, secID is for seconds, nanoID is for nanoseconds


    virtual.nanoseconds = virtual.nanoseconds + 40000;                       //increment nanoseconds by 40000 every call
    if(virtual.nanoseconds >= 1000000000){                          //if the nanoseconds exceeds or is equal to a second
        virtual.seconds++;                                                             //increment the seconds counter
        virtual.nanoseconds = virtual.nanoseconds - 1000000000;             //decrement the nanosecond count by a second
    }

    sprintf(temp1, "%d", virtual.seconds);                                               //store the seconds in a string
    sprintf(temp2, "%d", virtual.nanoseconds);                                       //store the nanoseconds in a string
    char *secStr = (char *) shmat(secID, (void *) 0, 0);                 //ptr to the shared memory location for seconds
    strcpy(secStr, temp1);                                              //copy the seconds to the shared memory location
    shmdt(secStr);                                                              //detach from the shared memory location
    char *nanoStr = (char *) shmat(nanoID, (void *) 0, 0);           //ptr to the shared memory location for nanoseconds
    strcpy(nanoStr, temp2);                                         //copy the nanoseconds to the shared memory location
    shmdt(nanoStr);                                                             //detach from the shared memory location
    return virtual;                                           //return the new time stored in the virtual time structure
}

static void myhandler(int s) {                                    //handler for the program to shut down at end of timer
    int errsave;
    errsave = errno;
    shmctl(shmget(66, 2048, 0666), IPC_RMID, NULL);                                               //delete shared memory
    shmctl(shmget(67, 2048, 0666), IPC_RMID, NULL);                                               //delete shared memory
    shmctl(shmget(65, 2048, 0666), IPC_RMID, NULL);                                               //delete shared memory
    shmctl(shmget(69, 2048, 0666), IPC_RMID, NULL);                                               //delete shared memory
    exit(0);                                                           //close program in its tracks after timer expires
    errno = errsave;
}
static int setupinterrupt(void) {                                                     /* set up myhandler for SIGPROF */
    struct sigaction act;
    act.sa_handler = myhandler;
    act.sa_flags = 0;
    return (sigemptyset(&act.sa_mask) || sigaction(SIGPROF, &act, NULL));
}
static int setupitimer(int n) {                                             /* set ITIMER_PROF for n-second intervals */
    struct itimerval value;
    value.it_interval.tv_sec = n;                //set the timer to however many seconds user enters or default 1 second
    value.it_interval.tv_usec = 0;
    value.it_value = value.it_interval;
    return (setitimer(ITIMER_PROF, &value, NULL));
}



int main(int argc, char *argv[]) {

    int opt = 0;
    int timeInSec = 10;                                                              //default timer is set to 5 seconds
    int maxChildren = 5;                                                       //default max child processes is set to 5
    char fileName[100] = "logFile";                                            //default log file name is set to logFile
    virtual.nanoseconds = 0;                                                            //nanosecond counter is set to 0
    virtual.seconds = 0;                                                                    //second counter is set to 0
    key_t dataKey = 65;
    int requestCount = 0;
    int grantedCount = 0;
    int deniedCount = 0;
    int lineCount = 0;
    int tableCount = 1;
    int deadProcesses = 0;
    int dataId = shmget(dataKey, 2048, 0666|IPC_CREAT);

    /*while ((opt = getopt(argc, argv,"hs:l:t:")) != -1) {                                              //GET OPT WOOOOO
        switch (opt) {                                          //start a switch statement for the commandline arguments
            case 'h' :                                                                              //print help message
                printf("Welcome to Log Parse!\n");
                printf("Invocation: ./logParse [-h] [-s maxChildren] [-l logFile] [-t timer]\n");
                printf("h : Display a help message to the user and exit \n");
                printf("s : Set max amount of children to spawn\n");
                printf("l : Use the given file name as the logfile\n");
                printf("t : Limit the program to n amount of seconds for execution\n");
                exit(0);
                break;
            case 's' :                                                             //set max amount of children to spawn
                maxChildren = atoi(optarg);
                break;
            case 'l' :                                                    //use the given file name as the log file name
                strcpy(fileName, "");
                strcpy(fileName, optarg);
                break;
            case 't' :                                                     //set the time entire program can execute for
                timeInSec = atoi(optarg);
                break;
            default:                                                              //user inputted something unrecognized
                perror("Command line argument not recognized.");
                exit(EXIT_FAILURE);
        }

    } */

    //setupinterrupt();                                                    //start the interrupt and timer for the program
    //setupitimer(timeInSec);

    int childCount;                                      //this is for a loop below to hold the count of child processes
    pid_t childPid;                                            //of course we need one of these guys for child processes
    remove("logFile");                                //remove whatever the default log file name just in case it exists
    int i, processNumber = 0;


    FILE *logptr = fopen(fileName, "w");                                               //initialize file ptr for writing

    float randomProcessMili = getRandom(2, 499);
    int checkSpace;
    do{
            int miliCheck = virtual.nanoseconds / 1000000;              //get current millisecond for process time check
            if(miliCheck == randomProcessMili && processNumber < 18){                    //if its time to fork a process

                char temp[2];
                sprintf(temp, "%d", processNumber);
                char *envp[] = {temp, NULL};

                processNumber++;
                childPid = fork();

                if(childPid == 0){
                    execve("./user", NULL, envp);
                }

                randomProcessMili = getRandom(2, 499);
            }
            virtual = updateClock(virtual);                                               //update the virtual user time
            key_t read = 69;                                               //the key for the shared memory for messaging
            if (shmget(read, 2048, 0444) != -1) {                    //if the memory location has something stored there
                char msg[150] = "\0";
                char tmp[100] = "\0";
                int n;

                int decisionId = shmget(read, 2048, 0666|IPC_CREAT);
                iShould = shmat(decisionId, NULL, 0);
                if(iShould->deadCount == 1){
                    deadProcesses++;
                    if(deadProcesses == 18){
                        break;
                    }
                }
                else {

                    int elementForUse = iShould->element;
                    int resourceForUse = iShould->resource;

                    resources = shmat(dataId, NULL, 0);
                    if (iShould->requestOrRelease == 1) {
                        int howManyResources = getRandom(resources[elementForUse].maxClaims[resourceForUse], 0);
                        int currentAloc = howManyResources;
                        for (n = 0; n < processNumber; n++) {
                            currentAloc += resources[n].allocations[resourceForUse];
                        }
                        if (currentAloc <= maxResources[resourceForUse]) {
                            resources[elementForUse].allocations[resourceForUse] += howManyResources;
                            if (lineCount < 100000) {
                                strcpy(msg, "Request approved for allocation of ");
                                sprintf(tmp, "%d", howManyResources);
                                strcat(msg, tmp);
                                strcat(msg, " resources | There are now ");
                                strcpy(tmp, "\0");
                                sprintf(tmp, "%d", resources[elementForUse].allocations[resourceForUse]);
                                strcat(msg, tmp);
                                strcat(msg, " allocations in resource ");
                                strcpy(tmp, "\0");
                                sprintf(tmp, "%d", resourceForUse);
                                strcat(msg, tmp);
                                strcat(msg, " for process ");
                                strcpy(tmp, "\0");
                                sprintf(tmp, "%d", elementForUse);
                                strcat(msg, tmp);
                                strcat(msg, "\n");
                                fputs(msg, logptr);
                            }
                            grantedCount++;
                            requestCount++;
                        } else {
                            if (lineCount < 100000) {
                                strcpy(msg, "Request for allocation of ");
                                sprintf(tmp, "%d", howManyResources);
                                strcat(msg, tmp);
                                strcat(msg, " resources has been denied as the max resources for resource ");
                                strcpy(tmp, "\0");
                                sprintf(tmp, "%d", resourceForUse);
                                strcat(msg, tmp);
                                strcat(msg, " is ");
                                strcpy(tmp, "\0");
                                sprintf(tmp, "%d", maxResources[resourceForUse]);
                                strcat(msg, tmp);
                                strcat(msg, " and there are already ");
                                strcpy(tmp, "\0");
                                sprintf(tmp, "%d", currentAloc);
                                strcat(msg, tmp);
                                strcat(msg, " allocations\n");
                                fputs(msg, logptr);
                            }
                            deniedCount++;
                        }
                    } else {
                        int howManyResources = getRandom(resources[elementForUse].maxClaims[resourceForUse], 0);
                        int n;
                        int freedMax = 0;
                        int ifNot = 0, howMuch = 0;
                        for (n = 0; n < processNumber; n++) {
                            if (resources[n].allocations[resourceForUse] >= howManyResources) {
                                freedMax = 1;
                                break;
                            }
                            if (resources[n].allocations[resourceForUse] > 0) {
                                ifNot = n;
                                howMuch = getRandom(resources[n].allocations[resourceForUse], 0);
                            }
                        }
                        if (freedMax == 1) {
                            resources[n].allocations[resourceForUse] -= howManyResources;
                            if (lineCount < 100000) {
                                strcpy(msg, "Request approved for release of ");
                                sprintf(tmp, "%d", howManyResources);
                                strcat(msg, tmp);
                                strcat(msg, " resources | Released from process ");
                                strcpy(tmp, "\0");
                                sprintf(tmp, "%d", n);
                                strcat(msg, tmp);
                                strcat(msg, " for resource ");
                                strcpy(tmp, "\0");
                                sprintf(tmp, "%d", resourceForUse);
                                strcat(msg, tmp);
                                strcat(msg, "\n");
                                fputs(msg, logptr);
                            }
                            requestCount++;
                            grantedCount++;
                        } else {
                            resources[n].allocations[resourceForUse] -= howMuch;
                            if (lineCount < 100000) {
                                strcpy(msg, "Request for release of ");
                                sprintf(tmp, "%d", howMuch);
                                strcat(msg, tmp);
                                strcat(msg, " was greater than the amount of other resource allocations, and so ");
                                strcpy(tmp, "\0");
                                sprintf(tmp, "%d", howMuch);
                                strcat(msg, tmp);
                                strcat(msg, " resources were removed from process ");
                                strcpy(tmp, "\0");
                                sprintf(tmp, "%d", ifNot);
                                strcat(msg, tmp);
                                strcat(msg, ", which now has ");
                                strcpy(tmp, "\0");
                                sprintf(tmp, "%d", resources[ifNot].allocations[resourceForUse]);
                                strcat(msg, tmp);
                                strcat(msg, " allocations.\n");
                                fputs(msg, logptr);
                            }

                            requestCount++;
                            grantedCount++;
                        }
                    }

                    if (requestCount == 20) {
                        int n, m;
                        int totals[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
                        for (m = 0; m < 20; m++) {

                            for (n = 0; n < 18; n++) {
                                totals[m] = resources[n].allocations[m] + totals[m];
                            }
                        }
                        printf("______________________________________________________________"
                               "______________________________________________________________\n");
                        printf("RESOURCE TABLE AFTER %d GRANTED REQUESTS\n", tableCount * 10);
                        printf("| R01 | R02 | R03 | R04 | R05 | R06 | R07 | R08 | R09 | R10 | R11 "
                               "| R12 | R13 | R14 | R15 | R16 | R17 | R18 | R19 | R20 |\n");
                        for (n = 0; n < 18; n++) {
                            for (m = 0; m < 20; m++) {
                                printf("| %-3ld ", resources[n].allocations[m]);
                            }
                            printf("| P%d\n", n);
                        }
                        for (n = 0; n < 20; n++) {
                            printf("| %-3ld ", totals[n]);
                        }
                        printf("| TOTALS\n");
                        printf("______________________________________________________________"
                               "______________________________________________________________\n");
                        printf("\n");
                        printf("< R01 | R02 | R03 | R04 | R05 | R06 | R07 | R08 | R09 | R10 | R11 "
                               "| R12 | R13 | R14 | R15 | R16 | R17 | R18 | R19 | R20 > ALL RESOURCES\n");
                        for (n = 0; n < 20; n++) {
                            if (n == 0) {
                                printf("< %-3ld ", maxResources[n]);
                            } else {
                                printf("| %-3ld ", maxResources[n]);
                            }
                        }
                        printf("> MAX RESOURCES\n");
                        tableCount++;
                        requestCount = 0;
                    }
                    shmdt(resources);
                }
                    shmdt(iShould);
                    shmctl(shmget(69, 2048, 0444), IPC_RMID, NULL);
                    lineCount++;
                    if (lineCount == 1000000) {
                        strcpy(msg, "\0");
                        strcpy(msg, "LogFile has stopped writing lines after 1000000 lines have been written.\n");
                        fputs(msg, logptr);
                        lineCount++;
                    }

            }

    } while (virtual.seconds < 100);   //this do while loop ends if seconds < 2 or 100 children exist
    printf("Finished | Printed %d lines | Runtime: %d virtual seconds | Live Processes: %d | Dead Processes: %d\n",
            lineCount, virtual.seconds, (processNumber - deadProcesses), deadProcesses);
    printf("Granted Requests: %d | Denied Requests: %d | Approval Rate: %f %\n", grantedCount, deniedCount,
            (float)((float)grantedCount /(float)(grantedCount + deniedCount)));
    fclose(logptr);                                                                                 //close the log file
    shmctl(shmget(66, 2048, 0444), IPC_RMID, NULL);                               //delete the shared memory for seconds
    shmctl(shmget(67, 2048, 0444), IPC_RMID, NULL);                           //delete the shared memory for nanoseconds
    shmctl(shmget(65, 2048, 0444), IPC_RMID, NULL);                             //delete the shared memory for resources
    shmctl(shmget(69, 2048, 0444), IPC_RMID, NULL);                             //delete the shared memory for resources
    exit(0);                                                                    // end the process, cause its the parent

}