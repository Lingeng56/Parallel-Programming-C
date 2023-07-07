//
// Created by 林庚 on 2021/5/18.
//
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "timer.h"
/*-------------------------*
 *     Local Function      *
 *-------------------------*/
void* test(void* rank);

/*-------------------------*
 *     Global Variable     *
 *-------------------------*/
long thread_count;
int main(int argc,char* argv[]){
    thread_count=strtol(argv[1],NULL,10);
    pthread_t* thread_handles=malloc(thread_count*sizeof(pthread_t));
    long thread;
    double start,finish,elapsed;

    GET_TIME(start);
    for (thread = 0; thread <thread_count ; ++thread) {
        pthread_create(thread_handles+thread,NULL,test,(void*)thread);
    }
    GET_TIME(finish);
    elapsed=finish-start;
    printf("average time of creating a thread is : %lf\n",elapsed/(double)thread_count);


    GET_TIME(start);
    for (thread = 0; thread <thread_count ; ++thread) {
        pthread_join(*(thread_handles+thread),NULL);
    }
    GET_TIME(finish);
    elapsed=finish-start;
    printf("average time of ending a thread is : %lf\n",elapsed/(double)thread_count);

    free(thread_handles);



}

/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
void* test(void* rank){
    return NULL;
}