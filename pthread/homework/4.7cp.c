//
// Created by 林庚 on 2021/5/15.
//
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>


/*-------------------------*
 *     Local Function      *
 *-------------------------*/
void Usage(char* pro_name);
void* thread_work(void* rank);

/*-------------------------*
 *     Global Variable     *
 *-------------------------*/
const int MAX_STRING = 99;

int thread_count;
int msg=0;
char* message;
pthread_mutex_t mutex;

int main(int argc,char* argv[]){
    long thread;
    pthread_t* thread_handles;
    message=malloc(MAX_STRING*sizeof(char));

    if (argc != 1){
        Usage(argv[0]);
    }

    thread_count =2;

    thread_handles=malloc(thread_count*sizeof(pthread_t));

    pthread_mutex_init(&mutex,NULL);


    for (thread = 0; thread <thread_count ; ++thread) {
        pthread_create(&thread_handles[thread],NULL,thread_work,(void*) thread);
    }

    for (thread = 0; thread <thread_count ; ++thread) {
        pthread_join(thread_handles[thread],NULL);
    }

    pthread_mutex_destroy(&mutex);
    free(message);
    free(thread_handles);

    return 0;
}


/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
void* thread_work(void* rank){
    long my_rank=(long )rank;

    while (1){
        pthread_mutex_lock(&mutex);
        if (my_rank==1){
            if (msg){
                printf("thread %ld > message = %s\n",my_rank,message);
                pthread_mutex_unlock(&mutex);
                break;
            }
        } else{
            sprintf(message,"hello world\n");
            printf("thread %ld > create message\n",my_rank);
            msg=1;
            pthread_mutex_unlock(&mutex);
            break;
        }
        pthread_mutex_unlock(&mutex);
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

    fprintf(stderr,"Usage: %s \n",pro_name);
    exit(0);
}



