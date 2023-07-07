//
// Created by 林庚 on 2021/5/16.
//
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"



/*-------------------------*
 *     Local Function      *
 *-------------------------*/
void get_args(int argc,char* argv[]);
void Usage(char* pro_name);
void* thread_work(void* rank);

/*-------------------------*
 *     Global Variable     *
 *-------------------------*/
long thread_count=4;
const int MAX_THREAD=1024;
double **matrix;
double *vector;
double *result;
int m;
int n;
double start;
double finish;
double elapsed;
int flag=0;


/*-------------------------------------------------------------------*/
int  main(int argc,char* argv[]){
    long thread;
    int i;
    int j;
    pthread_t* thread_handles;

//    get_args(argc,argv);

    thread_handles=(pthread_t*)malloc(thread_count*sizeof(pthread_t));

    printf("Input your size of matrix mxn:\n");
    scanf("%dx%d",&m,&n);
    if (m%thread_count!=0) {
        Usage(argv[0]);
    }

    vector=malloc(n*sizeof(double ));
    result=malloc(m*sizeof(double ));
    matrix=malloc(m*sizeof(double *));
    for (i = 0; i <m ; ++i) {
        *(matrix+i)=malloc(sizeof(double ));
    }
    printf("Input your vector:\n");
    for (i = 0; i <n ; ++i) {
        scanf("%lf",vector+i);
    }
    printf("input your matrix:\n");
    for (i = 0; i <m ; ++i) {
        for (j = 0; j <n ; ++j) {
            scanf("%lf",*(matrix+i)+j);
        }
    }

    printf("Data have prepared successfully...\n");
    printf("Begin to calculate with %ld threads...\n",thread_count);


    /*      work       */
    GET_TIME(start);
    for (thread = 0; thread <thread_count ; ++thread) {
        pthread_create(thread_handles + thread, NULL, thread_work, (void *) thread);
    }

    for (thread = 0; thread <thread_count ; ++thread) {
        pthread_join(*(thread_handles+thread),NULL);
    }

    elapsed=finish-start;

    printf("Calculation has finished....\nIt cost %lf seconds.\nresult:\n",elapsed);
    for (i = 0; i <m ; ++i) {
        printf("%lf ",*(result+i));
    }

    free(matrix);
    free(vector);
    free(result);
    free(thread_handles);

}


/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
void Usage(char* pro_name){
    fprintf(stderr,"Usage ./%s <thread_count>\nthread_count should be divisible by number of matrix row and less than 1024.\n",pro_name);
    exit(1);
}


/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
void get_args(int argc,char* argv[]){
    if (argc!=2 || thread_count>MAX_THREAD){
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
void* thread_work(void* rank){
    long my_rank=(long) rank;
    int my_begin_row=(int)(m/thread_count*my_rank);
    int my_end_row=my_begin_row+(int)(m/thread_count);
    int i;
    int j;
    for (i = my_begin_row; i <my_end_row ; ++i) {
        for (j = 0; j <n ; ++j) {
            *(result+i)+=*(*(matrix+i)+j)**(vector+j);
        }
    }
    flag++;
    while (flag<thread_count){}
    GET_TIME(finish);
    return NULL;
}
