//
// Created by 林庚 on 2021/5/10.
//
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
/*-------------------------*
 *     Global Variable     *
 *-------------------------*/
int thread_count;

/*-------------------------*
 *     Local Function      *
 *-------------------------*/

void* hello(void* rank);

int main(int argc,char* argv[]){
    long thread;
    pthread_t* thread_handles;

    thread_count=strtol(argv[1],NULL,10);

    thread_handles=malloc(thread_count*sizeof(pthread_t ));

    for (thread = 0; thread <thread_count ; ++thread) {
        pthread_create(&thread_handles[thread],NULL,hello,(void *) thread);
    }

    free(thread_handles);
    return 0;
}

/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
void* hello(void* rank){
    long my_rank=(long )rank;


    printf("hello world from %ld of %d\n",my_rank,thread_count);

    return NULL;
}