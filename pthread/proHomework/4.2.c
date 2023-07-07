//
// Created by 林庚 on 2021/5/18.
//
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>


/*-------------------------*
 *     Local Function      *
 *-------------------------*/
void get_args(int argc,char* argv[]);
void Usage(char* pro_name);
void* thread_work(void* rank);
double get_rand(long rank);
/*-------------------------*
 *     Global Variable     *
 *-------------------------*/
const int RADIUS=1;
long long  int N=2000000000;
double pi;
long thread_count;
long long int count;
pthread_mutex_t mutex;

/*-------------------------------------------------------------------*/
int main(int argc,char* argv[]){

    long thread;

    get_args(argc,argv);
    pthread_t* thread_handles=malloc(thread_count*sizeof(pthread_t));

    for (thread = 0; thread < thread_count; ++thread) {
        pthread_create(thread_handles+thread,NULL,thread_work,(void*)thread);
    }


    for (thread = 0; thread < thread_count; ++thread) {
        pthread_join(*(thread_handles+thread),NULL);
    }

    pi=4.0*count/N;
    printf("we calculated pi is : %lf",pi);

    free(thread_handles);



}

/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
void Usage(char* pro_name){
    fprintf(stderr,"Usage : ./%s <thread_count> \n",pro_name);
    exit(1);
}

/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
void get_args(int argc,char* argv[]){
    if (argc!=2){
        Usage(argv[0]);
    }
    thread_count=strtol(argv[1],NULL,10);
}

/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */


/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
void* thread_work(void* rank){
    long long int local_n=N/thread_count;
    long long int i;
    long long int my_count=0;
    double my_x,my_y;
    for (i = 0; i <local_n ; ++i) {
        srand(i%INT32_MAX);
        my_x=1.0*rand()/RAND_MAX *(RADIUS-0);
        my_y=1.0*rand()/RAND_MAX *(RADIUS-0);
        if (my_x*my_x+my_y*my_y<=RADIUS){
            my_count++;
        }
    }
    pthread_mutex_lock(&mutex);
    count+=my_count;
    pthread_mutex_unlock(&mutex);
    return NULL;
}
