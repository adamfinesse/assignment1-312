/**
 * Simple program demonstrating shared memory in POSIX systems.
 *
 * Figure 3.16
 *
 * @author Gagne, Galvin, Silberschatz
 * Operating System Concepts  - Eighth Edition
 * Copyright John Wiley & Sons - 2008.
 */

  /*
 * Adam Harms
 * 2/22/2022
 * description:
 * This simple program exercises process synchronization using string and int shared memory location, the processes (a,b,c)
 * work using polling. Once each process writes "I am processx" and inputs a numer 1-3 associated with the process, the process
 * next process in line runs, finally all ending then detaching the memory locations. Similar to q1 but uses nested forks to utilize the shared memory 
 * of the parent and children rather than using a key to make a new one.
 *
 * Usage: run servernested after compilation and it will print out the processes in order.
 */

#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>

int
main()
{
    int           processb;
    int           processc;
    char *s;
    /* the identifier for the shared memory segment */
    int             segment_id;
    /* a pointer to the shared memory segment */
    char           *shared_memory="Adam Harms";
    /* the size (in bytes) of the shared memory segment */
    const int       segment_size = 4096;

    /** allocate  a shared memory segment */
    segment_id = shmget(IPC_PRIVATE, segment_size, S_IRUSR | S_IWUSR);

    /** attach the shared memory segment */
    shared_memory = (char *) shmat(segment_id, NULL, 0);
    
    	printf("shared memory segment %d attached at address %p\n", segment_id, shared_memory);


    //int memory location

    /* the identifier for the shared memory segment */
    int             segment_idInt;
    /* a pointer to the shared memory segment */
    int           *shared_memoryInt;
    /* the size (in bytes) of the shared memory segment */
    //const int       segment_size = 4096;

    /** allocate  a shared memory segment */
    segment_idInt = shmget(IPC_PRIVATE, segment_size, S_IRUSR | S_IWUSR);

    /** attach the shared memory segment */
    shared_memoryInt = (int *) shmat(segment_idInt, NULL, 0);
    
    	printf("shared memory segment %d attached at address %p\n", segment_idInt, shared_memoryInt);
    

    /* fork another process */

    processb = fork();

    if (processb == 0) {		/* child process */
        /** write a message to the shared memory segment   */

            while(*shared_memoryInt != 1){
            sleep(4);
        }


        // for( s = shared_memory; *s != (char) NULL; s++ )
        //     putchar(*s);
        // putchar('\n');

        char processB[] = "I am Process B";

        int c = 0;
        for(s = shared_memory; *s != (char) NULL; s++)
            *s = processB[c++];

        /*
        * Finally, change the first character of the 
        * segment to '*', indicating we have read 
        * the segment.
        */
        *shared_memoryInt = 2;
        sleep(4);
        processc = fork();

    if(processc == 0){
        while(*shared_memoryInt != 2){
        sleep(4);
    }

    //  for( s = shared_memory; *s != (char) NULL; s++ )
    //      putchar(*s);
    //  putchar('\n');
    
    char processC[] = "I am Process C";

    int c = 0;
    for(s = shared_memory; *s != (char) NULL; s++)
        *s = processC[c++];

    /*
     * Finally, change the first character of the 
     * segment to '*', indicating we have read 
     * the segment.
     */
    *shared_memoryInt = 3;
    sleep(2);
        exit(0);
    }else{
        wait();
        while(*shared_memoryInt !=3){
            sleep(1.8);
        }
            exit(0);
    }

    }
    if(processb>0){
        usleep(100);

        s = shared_memory;

        *shared_memoryInt = 1;
        int c;
        char processA[] = "I am Process A";
        for(c = 0; c < strlen(processA);c++ ){
            *s++ = processA[c];
            }
        *s = (char) NULL;

        for( s = shared_memory; *s != (char) NULL; s++ )
            putchar(*s);
        putchar('\n');
        sleep(2);
        /** now print out the string from shared memory */

    while( *shared_memoryInt != 2 )
    {
        sleep(1);
    }

        for( s = shared_memory; *s != (char) NULL; s++ )
            putchar(*s);
        putchar('\n');

    while( *shared_memoryInt != 3 ){
        sleep(10);
    }

    for( s = shared_memory; *s != (char) NULL; s++ )
        putchar(*s);
    putchar('\n');

    sleep(1);

    /** now detach the shared memory segment */
        if (shmdt(shared_memory) == -1) {
            fprintf(stderr, "Unable to detach\n");
        }
    /** now remove the shared memory segment */
        shmctl(segment_id, IPC_RMID, NULL);
        //return 0;
    //}

    /** now detach the shared memory segment */
        if (shmdt(shared_memoryInt) == -1) {
            fprintf(stderr, "Unable to detach\n");
        }
    /** now remove the shared memory segment */
        shmctl(segment_idInt, IPC_RMID, NULL);
        //return 0;
    
    printf("%s\n","GoodBye");

        return 0;
    }
}