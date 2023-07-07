//
// Created by 林庚 on 2021/5/15.
//
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/*-------------------------*
 *     Local Function      *
 *-------------------------*/
void get_args(int argc,char* argv[]);
void* test(void* rank);


/*-------------------------*
 *     Global Variable     *
 *-------------------------*/
long thread_count;
pthread_mutex_t* mutexes;
int flag=0;

int main(int argc,char* argv[]){
    long thread;
    get_args(argc,argv);
    mutexes=malloc(thread_count*sizeof(pthread_mutex_t));


    for (thread = 0; thread <thread_count ; ++thread) {
        pthread_mutex_t mutex;
        pthread_mutex_init(&mutex,NULL);
        mutexes[thread]=mutex;
    }



}


/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
void* test(void* rank){
    long my_rank=(long) rank;
    long send_goal=(my_rank+1)%thread_count;
    long recv_goal=(my_rank-1+thread_count)%thread_count;
    pthread_mutex_t mutex=my_rank%2==0?mutexes[my_rank]:mutexes[my_rank+1];
    while (1){
        pthread_mutex_lock(&mutex);
        if (my_rank%2!=0){
            if (flag) {
                printf("thread %ld received message from %ld\n", my_rank, recv_goal);
                pthread_mutex_unlock(&mutex);
                break;
            }
        } else{
            flag=1;
            printf("thread %ld sent message to %ld\n",my_rank,send_goal);
            pthread_mutex_unlock(&mutex);
            break;
        }
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

