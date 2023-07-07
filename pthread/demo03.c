//
// Created by 林庚 on 2021/5/11.
//
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

#define MSG_MAX 100
/*-------------------------*
 *     Local Function      *
 *-------------------------*/
void* send_msg(void* rank);
void Usage(char* pro_name);

/*-------------------------*
 *     Global Variable     *
 *-------------------------*/
long thread_count;
char** messages=NULL;
char** semaphores_names;
sem_t** semaphores;


int main(int argc,char* argv[]){
    long thread;
    pthread_t* thread_handles;
    if (argc!=2)Usage(argv[0]);
    thread_count=strtol(argv[1],NULL,10);
    if (thread_count<=0 ) Usage(argv[0]);

    thread_handles=(pthread_t*)malloc(thread_count*sizeof(pthread_t));
    messages=(char**)malloc(thread_count*sizeof(char*));
    for (thread = 0; thread <thread_count ; ++thread) {
        messages[thread]=NULL;
    }

    semaphores=(sem_t**)malloc(thread_count*sizeof(sem_t *));
    semaphores_names=(char**)malloc(thread_count*sizeof(char*));


    for (thread = 0; thread < thread_count; ++thread) {
        semaphores_names[thread]=malloc(10*sizeof(char));
        sprintf(semaphores_names[thread],"/sem%ld",thread);
        semaphores[thread]=sem_open(semaphores_names[thread],O_CREAT,0777,0);
    }

    for (thread = 0; thread < thread_count; ++thread) {
        pthread_create(&thread_handles[thread],NULL,send_msg,(void*)thread);
    }

    for (thread = 0; thread <thread_count ; ++thread) {
        pthread_join(thread_handles[thread],NULL);
    }

    for (thread = 0; thread < thread_count; ++thread) {
        sem_unlink(semaphores_names[thread]);
        sem_close(semaphores[thread]);
        free(messages[thread]);
        free(semaphores_names[thread]);
    }

    free(semaphores);
    free(messages);
    free(thread_handles);
    return 0;
}

/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
void* send_msg(void* rank){
    long my_rank=(long )rank;
    long dest=(my_rank+1)%thread_count;
    long source=(my_rank+thread_count-1)%thread_count;
    char* my_msg=malloc(MSG_MAX*sizeof(char));

    sprintf(my_msg,"hello to %ld from %ld",dest,my_rank);
    messages[dest]=my_msg;

    sem_post(semaphores[dest]);


    sem_wait(semaphores[my_rank]);
    if (messages[my_rank]!=NULL){
    printf("Thread %ld > %s\n",my_rank,messages[my_rank]);
    } else{
        printf("Thread %ld > No message from %ld",my_rank,source);
    }

    return NULL;
}

/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
void Usage(char* pro_name){
    fprintf(stderr,"Usage: %s <number of thread>\n",pro_name);
    exit(0);
}
