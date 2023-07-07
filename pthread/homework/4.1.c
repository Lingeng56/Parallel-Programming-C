//
// Created by 林庚 on 2021/5/11.
//
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

/*-------------------------*
 *     Local Function      *
 *-------------------------*/
void Usage(char* pro_name);
void* operator(void* rank);

/*-------------------------*
 *     Global Variable     *
 *-------------------------*/
long thread_count;
int *x;
int *y;
int **a;
int m;
int n;
int main(int argc,char* argv[]){

    if (argc!=2){
        printf("Usage %s <number of thread>",argv[0]);
        exit(1);
    }


    printf("input your size of matrix:\n");
    int i;
    int j;
    long thread;
    thread_count=strtol(argv[1],NULL,10);
    pthread_t* thread_handles;


    scanf("%dx%d",&m,&n);
    x=malloc(n*sizeof(int));
    y=malloc(n*sizeof(int));
    a=malloc(m*(sizeof(int*)));
    thread_handles=malloc(thread_count*sizeof(pthread_t));
    printf("input your vector whose length is %d\n",n);
    for (i = 0; i <n ; ++i) {
        scanf("%d",x+i);
    }
    for (i = 0; i <m ; ++i) {
        *(a+i)=malloc(n*sizeof(int));
    }
    printf("input your matrix whose size is %dx%d\n",m,n);
    for (i = 0; i <m ; ++i) {
        for (j = 0; j <n ; ++j) {
            scanf("%d",*(a+i)+j);
        }
    }

    for (thread = 0; thread <thread_count ; ++thread) {
        pthread_create(thread_handles+thread,NULL,operator,(void*)thread);
    }

    for (thread = 0; thread <thread_count ; ++thread) {
        pthread_join(*(thread_handles+thread),NULL);
    }

    printf("result:\n");
    for (i = 0; i <n ; ++i) {
        printf("%d ",*(y+i));
    }


    free(x);
    free(y);
    for (i = 0; i <m ; ++i) {
        free(*(a+i));
    }
    free(a);
    free(thread_handles);
}

/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
void* operator(void* rank){
    long my_rank=(long )rank;
    int my_begin_row=(int)my_rank*m/(int)thread_count;
    int my_end_row=my_begin_row+m/(int)thread_count;
    int i;
    int j;
    for (i = my_begin_row; i <my_end_row; ++i) {
        for (j = 0; j < n; ++j) {
            *(y+i)+=*(*(a+i)+j)**(x+j);
        }
    }

    return NULL;
}

