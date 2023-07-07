//
// Created by 林庚 on 2021/5/14.
//
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
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
pthread_mutex_t mutex;


int main(int argc,char* argv[]){
    long thread;
    pthread_t* thread_handles;
    double start, finish, elapsed;

    get_args(argc,argv);

    thread_handles=(pthread_t*) malloc(thread_count*sizeof(pthread_t));
    pthread_mutex_init(&mutex,NULL);
    sum=0.0;
    start=(double)clock()/CLOCKS_PER_SEC;

    for (thread = 0; thread <thread_count ; ++thread) {
        pthread_create(&thread_handles[thread],NULL,thread_sum,(void*)thread);
    }

    for (thread = 0; thread <thread_count ; ++thread) {
        pthread_join(thread_handles[thread],NULL);
    }
    sum*=4;

    finish=(double)clock()/CLOCKS_PER_SEC;

    elapsed=finish-start;
    printf("with %ld threads,it cost %lf seconds to calculate PI\n",thread_count,elapsed);
    printf("PI is %lf\n",sum);

    start=(double)clock()/CLOCKS_PER_SEC;
    sum=serial_pi(n);
    sum*=4;
    finish=(double)clock()/CLOCKS_PER_SEC;
    elapsed=finish-start;
    printf("with single thread,it cost %lf seconds to calculate PI\n",elapsed);
    printf("PI is %lf\n",sum);

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
    long my_rank=(long )rank;
    int my_begin_i=(int)(my_rank*n/thread_count);
    int my_end_i=(int)(my_begin_i+n/thread_count);
    if (my_begin_i%2!=0){
        factor=-1.0;
    } else{
        factor=1.0;
    }
    for (i = my_begin_i; i <my_end_i ; ++i) {
        pthread_mutex_lock(&mutex);
        sum+=(factor/(2*i+1));
        pthread_mutex_unlock(&mutex);
        factor=-factor;
    }



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