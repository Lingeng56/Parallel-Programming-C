//
// Created by 林庚 on 2021/5/18.
//
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <math.h>


/*-------------------------*
 *     Local Function      *
 *-------------------------*/
void* thread_work(void* rank);
double f(double x);
void Usage(char* pro_name);
void get_args(int argc,char* argv[]);


/*-------------------------*
 *     Global Variable     *
 *-------------------------*/
long thread_count;
double b;
double a;
const long long int N=1024;
pthread_mutex_t mutex;
double sum;

/*------------------------------------------------------------------- */
int main(int argc,char* argv[]){
    long thread;
    get_args(argc,argv);
    pthread_t* thread_handles=malloc(thread_count*sizeof(pthread_t));
    printf("input your beginning double num:\n");
    scanf("%lf",&a);
    printf("input your ending double num:\n");
    scanf("%lf",&b);

    for (thread = 0; thread <thread_count ; ++thread) {
        pthread_create(thread_handles+thread,NULL,thread_work,(void*)thread);
    }

    for (thread = 0; thread <thread_count ; ++thread) {
        pthread_join(*(thread_handles+thread),NULL);
    }

    printf("from %lf to %lf ,sum is %lf",a,b,sum);

    free(thread_handles);
}

/*-------------------------------------------------------------------
 * Function:        Usage
 * Purpose:         printf the usage of the program
 * Input args:      char* pro_name :name of program
 * In/out args:
 */
void Usage(char* pro_name){

    fprintf(stderr,"Usage : %s <thread_count>",pro_name);
    exit(1);

}

/*-------------------------------------------------------------------
 * Function:        get_args
 * Purpose:         get arguments from command line
 * Input args:      int argc:  num of command line arguments,  char* argv[] : contents of command line arguments
 * In/out args:
 */
void get_args(int argc,char* argv[]){
    if (argc!=2){
        Usage(argv[0]);
    }
    thread_count=strtol(argv[1],NULL,10);
}

/*-------------------------------------------------------------------
 * Function:        f
 * Purpose:         a simple function
 * Input args:      double x: the x of f(x)
 * In/out args:
 */
double f(double x){
    return sin(x);
}

/*-------------------------------------------------------------------
 * Function:        thread_work
 * Purpose:         calculate the integral of function f
 * Input args:      default parameters for pthread to create a thread
 * In/out args:
 */
void* thread_work(void* rank){
    long my_rank=(long) rank;
    double h=(b-a)/(double)N;
    double local_a=(double)my_rank*(b-a)/(double)thread_count;
    double local_b=local_a+h*(double)N/(double)thread_count;
    double my_sum=0.0;
    while (local_a<local_b){
        my_sum+=f(local_a+h)*h;
        local_a+=h;
    }
    pthread_mutex_lock(&mutex);
    sum+=my_sum;
    pthread_mutex_unlock(&mutex);
    return NULL;
}
