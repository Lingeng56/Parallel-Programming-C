//
// Created by 林庚 on 2021/5/10.
//
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
/*-------------------------*
 *     Local Function      *
 *-------------------------*/
void* multi_matrix_vector(void* rank);
/*-------------------------*
 *     Global Variable     *
 *-------------------------*/
int n;
int m;
int* x=NULL;
int* y=NULL;
int* a=NULL;
int thread_count;

int main(int argc,char* argv[]){
    long thread;
    pthread_t* thread_handles;
    int i;

    thread_count=strtol(argv[1],NULL,10);

    thread_handles=malloc(thread_count*sizeof(pthread_t));

    printf("thread main begin...\n");
    printf("input your size of matrix:(mxn)\n");
    scanf("%dx%d",&m,&n);
    x=malloc(n*sizeof(int));
    y=malloc(n*sizeof(int));
    a=malloc(m*n*sizeof(int));
    printf("input your vector:\n");
    for (i = 0; i < n; ++i) {
        scanf("%d",x+i);
    }
    printf("input your matrix:\n");
    for (i = 0; i <m*n ; ++i) {
        scanf("%d",a+i);
    }

    for (thread = 0; thread <thread_count ; ++thread) {
        pthread_create(&thread_handles[thread],NULL,multi_matrix_vector,(void *) thread);
        printf("thread %ld have received the task\n",thread);
    }

    for (thread = 0; thread < thread_count; ++thread) {
        pthread_join(thread_handles[thread],NULL);
    }

    printf("result:\n");
    for (i = 0; i <n ; ++i) {
        printf("%d ",*(y+i));
    }



    free(thread_handles);
    free(x);
    free(a);
    free(y);
    return 0;

}


/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
void* multi_matrix_vector(void* rank){
    long my_rank=(long) rank;
    int i;
    int j;
    int local_m=m/thread_count;
    int my_begin_row=(int)my_rank*local_m;
    int my_end_row=my_begin_row+local_m;

    printf("thread %ld begin to calculate!\n",my_rank);

    for (i = my_begin_row; i <my_end_row ; ++i) {
        for (j = 0; j <n ; ++j) {
            int sum=*(x+j)**(a+i*n+j);
            *(y+i)+=sum;
        }
    }
    printf("thread %ld finished calculating!\n",my_rank);

    return NULL;

}
