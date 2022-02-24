#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h> /* for exit */


/* 
 * shm-server - not sure of the origin of code segment.
 * Old source, not sure of the origin
 *  possibly: David Marshalls course : http://www.cs.cf.ac.uk/Dave/C/CE.html
 * or Steve Holmes : http://www2.its.strath.ac.uk/courses/c/
 */

 /*
 * Adam Harms
 * 2/22/2022
 * description:
 * This simple program exercises process synchronization using string and int shared memory location, the processes (a,b,c)
 * work using polling. Once each process writes "I am processx" and inputs a numer 1-3 associated with the process, the process
 * next process in line runs, finally all ending then detaching the memory locations.
 *
 * Usage: run processa first, then run c, then run b to run in the optimal order. The programs will wait and execute after this is complete.
 * Because of cpu scheduling, you may have to run the program more than once at various start speeds (slow or fast, starting c before b etc) 
 * because one process may be polling for a process thats scheduling was already completed by the scheduler. I have attached a picture of the program working on my end.
 */



#define SHMSZ     27


int 
main()
{
    char c;
    int shmid;
    key_t key;
    char *shm, *s;

    int shmidInt;
    key_t keyInt;
    int *shmInt, *sint;

    /*
     * We'll name our shared memory segment
     * "5678".
     */
    key = 1337;
    keyInt = 7331;

    /*
     * Create the segment.
     */
    if( (shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0 )
    {
        perror("shmget");
        exit(1);
    }

        if( (shmidInt = shmget(keyInt, SHMSZ, IPC_CREAT | 0666)) < 0 )
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
     * Now put some things into the memory for the
     * other process to read.
     */
    s = shm;
    sint = shmInt;

    //*sint++ = 1; need ++ for char location since its basically an array
    *sint = 1;

    char processA[] = "I am Process A";
    for(c = 0; c < strlen(processA);c++ ){
        *s++ = processA[c];
    }
    *s = (char) NULL;

       for( s = shm; *s != (char) NULL; s++ )
        putchar(*s);
    putchar('\n');

    /*
     * Finally, we wait until the other process 
     * changes the first character of our memory
     * to '*', indicating that it has read what 
     * we put there.
     */
    while( *shmInt != 2 )
    {
        sleep(1.2);
        //printf("waiting on b\n");
    }

    for( s = shm; *s != (char) NULL; s++ )
        putchar(*s); 
    putchar('\n');

    //sleep(2);

    while( *shmInt != 3 ){
        sleep(4);
        //printf("waiting on c\n");
    }

    sleep(2);


    for( s = shm; *s != (char) NULL; s++ )
        putchar(*s);
    putchar('\n');

    /** now detach the shared memory segment */
        if (shmdt(shm) == -1) {
            fprintf(stderr, "Unable to detach\n");
        }
    /** now remove the shared memory segment */
        shmctl(shmid, IPC_RMID, NULL);
    //}

    /** now detach the shared memory segment */
        if (shmdt(shmidInt) == -1) {
            fprintf(stderr, "Unable to detach\n");
        }
    /** now remove the shared memory segment */
        shmctl(shmidInt, IPC_RMID, NULL);
    
    printf("%s\n","GoodBye");
    return 0;
}
