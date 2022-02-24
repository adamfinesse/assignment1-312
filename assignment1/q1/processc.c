
/*
 * shm-client - client program to demonstrate shared memory.
 * shm-client - not sure of the origin of these code segments.
 * possibly: David Marshalls course : http://www.cs.cf.ac.uk/Dave/C/CE.html
 * or Steve Holmes : http://www2.its.strath.ac.uk/courses/c/
 */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>

#define SHMSZ     27

int
main()
{
    int shmid;
    key_t key;
    char *shm, *s;

    int shmidInt;
    key_t keyInt;
    int *shmInt, *sint;

    /*
     * We need to get the segment named
     * "5678", created by the server.
     */
    key = 1337;
    keyInt = 7331;

    /*
     * Locate the segment.
     */
    if( (shmid = shmget(key, SHMSZ, 0666)) < 0 )
    {
        perror("shmget");
        exit(1);
    }

    if( (shmidInt = shmget(keyInt, SHMSZ, 0666)) < 0 )
    {
        perror("shmget");
        exit(1);
    }

    /*
     * Now we attach the segment to our data space.
     */
    if( (shm = shmat(shmid, NULL, 0)) == (char *) -1 )
    {
        perror("shmat");
        exit(1);
    }

    if( (shmInt = shmat(shmidInt, NULL, 0)) == (int *) -1 )
    {
        perror("shmat");
        exit(1);
    }

    /*
     * Now read what the server put in the memory.
     */

     sleep(3);

    while(*shmInt != 2){
        sleep(2);
    }

    sleep(6);

    char processC[] = "I am Process C";
    int c = 0;
    for(s = shm; *s != (char) NULL; s++){
        *s = processC[c++];
    }

    /*
     * Finally, change the first character of the 
     * segment to '*', indicating we have read 
     * the segment.
     */
    *shmInt = 3;
    sleep(6);

    /** now detach the shared memory segment */
        if (shmdt(shm) == -1) {
            fprintf(stderr, "Unable to detach\n");
        }

    /** now detach the shared memory segment */
        if (shmdt(shmidInt) == -1) {
            fprintf(stderr, "Unable to detach\n");
        }
    return 0;
}
