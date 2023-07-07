//
// Created by 林庚 on 2021/5/14.
//
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "timer.h"
#include <semaphore.h>
/*-------------------------*
 *     Local Function      *
 *-------------------------*/
void Usage(char* pro_name);
void* thread_sum(void* rank);
void get_args(int argc,char* argv[]);
double serial_pi(long long num);
/*-------------------------*
 *     Global Variable     *
 *-------------------------*/
const int MAX_THREADS=1024;
long thread_count;
long long n;
double sum;
sem_t** semaphores;
char** semaphore_names;


int main(int argc,char* argv[]){
    long thread;
    pthread_t* thread_handles;
    double start, finish, elapsed,elapsed1;

    get_args(argc,argv);


    semaphores=malloc(thread_count*sizeof(sem_t*));
    semaphore_names=malloc(thread_count*sizeof(char*));
    thread_handles=(pthread_t*) malloc(thread_count*sizeof(pthread_t));
    sum=0.0;
    for (thread = 0; thread <thread_count ; ++thread) {
        semaphore_names[thread]=malloc(10*sizeof(char));
        sprintf(semaphore_names[thread],"/sem%ld",thread);
        semaphores[thread]=sem_open(semaphore_names[thread],O_CREAT,0777,0);
    }

    GET_TIME(start);

    for (thread = 0; thread <thread_count ; ++thread) {
        pthread_create(&thread_handles[thread],NULL,thread_sum,(void*)thread);
    }

    for (thread = 0; thread <thread_count ; ++thread) {
        pthread_join(thread_handles[thread],NULL);
    }
    sum*=4;

    GET_TIME(finish);

    elapsed=finish-start;
    printf("with %ld threads,it cost %lf seconds to calculate PI\n",thread_count,elapsed);
    printf("PI is %lf\n",sum);

    for (thread = 0; thread <thread_count ; ++thread) {
        sem_unlink(semaphore_names[thread]);
        sem_close(semaphores[thread]);
        free(semaphore_names[thread]);
    }

    free(semaphore_names);
    free(semaphores);

    GET_TIME(start);
    sum=serial_pi(n);
    sum*=4;
    GET_TIME(finish);
    elapsed1=finish-start;
    printf("\n");
    printf("with single thread,it cost %lf seconds to calculate PI\n",elapsed1);
    printf("PI is %lf\n",sum);
    printf("**********************************************************************\n");
    printf("**********************************************************************\n");
    printf("**********************************************************************\n");
    printf("Linear acceleration ratio is %lf\n",elapsed1/elapsed);

}

/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
void Usage(char* pro_name){
    fprintf(stderr,"************************************************************************\n");
    fprintf(stderr,"*    command line should be:%s <thread_count> <calculate precision>* \n",pro_name);
    fprintf(stderr,"*                                                                      * \n");
    fprintf(stderr,"*                                                                      * \n");
    fprintf(stderr,"*                                                                      * \n");
    fprintf(stderr,"*    thread_count should be divisible by calculate precision.          *\n");
    fprintf(stderr,"************************************************************************\n");
    exit(1);
}


/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
void get_args(int argc,char* argv[]){
    if (argc!=3 || strtol(argv[1],NULL,10)>MAX_THREADS || strtol(argv[2],NULL,10)%strtol(argv[1],NULL,10)!=0){
        Usage(argv[0]);
    }
    thread_count=strtol(argv[1],NULL,10);
    n=strtol(argv[2],NULL,10);
}

/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
void* thread_sum(void* rank){
    int i;
    double factor;
    double my_sum=0.0;
    long my_rank=(long )rank;
    long dest=my_rank!=0?my_rank-1:thread_count-1;
    int my_begin_i=(int)(my_rank*n/thread_count);
    int my_end_i=(int)(my_begin_i+n/thread_count);
    if (my_begin_i%2!=0){
        factor=-1.0;
    } else{
        factor=1.0;
    }
    for (i = my_begin_i; i <my_end_i ; ++i) {
        my_sum+=(factor/(2*i+1));
        factor=-factor;
    }
    sem_post(semaphores[dest]);
    sem_wait(semaphores[my_rank]);
    sum+=my_sum;

    return NULL;
}

/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
double serial_pi(long long num){
    int i;
    double factor;
    double result=0.0;
    for (i = 0; i <num ; ++i) {
        if (i%2!=0){
            factor=-1.0;
        } else{
            factor=1.0;
        }
        result+=(factor/(2*i+1));
    }
    return result;
}