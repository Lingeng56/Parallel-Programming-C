//
// Created by 林庚 on 2021/4/23.
//
#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
void read_vector(int ,int[],int [],int);
void get_slides(int[],int[],int);
void dot(int[],int[],int[],int);
void print_result(int[],int,int);
void get_n(int*,int*);
int comm_sz;
int my_rank;
MPI_Comm comm;
int main(){
    MPI_Init(NULL,NULL);
    comm=MPI_COMM_WORLD;


    MPI_Comm_size(comm,&comm_sz);
    MPI_Comm_rank(comm,&my_rank);


    int n;
    int local_num;
    get_n(&n,&local_num);
    int local_a[local_num];
    int local_b[local_num];
    int local_result[local_num];


    read_vector(n,local_a,local_b,local_num);
    dot(local_a,local_b,local_result,local_num);
    print_result(local_result,n,local_num);


    MPI_Finalize();

}
/*-------------------------------------------------------------------*/
void read_vector(int n,int local_a[],int local_b[],int local_num){
    int local_i;
    int *a=NULL;
    int *b=NULL;
    int *slides=NULL;
    int *counts=NULL;
    if (my_rank==0) {
        a = malloc(n * sizeof(int));
        b = malloc(n * sizeof(int));
        slides = malloc(comm_sz * sizeof(int));
        counts = malloc(comm_sz * sizeof(int));
        get_slides(slides, counts, n);
        printf("please input vec a\n");
        for (local_i = 0; local_i < n; ++local_i) {
            scanf("%d", a + local_i);
            fflush(stdin);
        }
        printf("please input vec b\n");
        for (local_i = 0; local_i < n; ++local_i) {
            scanf("%d", b + local_i);
            fflush(stdin);
        }
        MPI_Scatterv(a, counts, slides, MPI_INT, local_a, local_num, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Scatterv(b, counts, slides, MPI_INT, local_b, local_num, MPI_INT, 0, MPI_COMM_WORLD);
        printf("sent successfully\n");
        free(a);
        free(b);
        free(slides);
        free(counts);
    } else{
        MPI_Scatterv(a, counts, slides, MPI_INT, local_a, local_num, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Scatterv(b, counts, slides, MPI_INT, local_b, local_num, MPI_INT, 0, MPI_COMM_WORLD);
    }
    MPI_Barrier(MPI_COMM_WORLD);
}
/*-------------------------------------------------------------------*/
void get_slides(int slides[],int counts[],int n){
    int local_i;
    int offset=0;
    for (local_i = 0; local_i < comm_sz; ++local_i) {
        if (local_i==comm_sz-1){
            counts[local_i]=n/comm_sz+n%comm_sz;
            slides[local_i]=offset;
            break;
        }
        counts[local_i]=n/comm_sz;
        slides[local_i]=offset;
        offset+=counts[local_i];
    }
}
/*-------------------------------------------------------------------*/
void get_n(int *n,int *local_num){
    if (my_rank==0){
    printf("input n\n");
    scanf("%d",n);}
    MPI_Bcast(n,1,MPI_INT,0,MPI_COMM_WORLD);

    if (my_rank!=comm_sz-1){
        *local_num=*n/comm_sz;
    } else{
        *local_num=*n/comm_sz+*n%comm_sz;
    }
    printf("local_num of thread%d is %d\n",my_rank,*local_num);
}
/*-------------------------------------------------------------------*/
void dot(int local_a[],int local_b[],int local_result[],int local_num){
    int local_i;
    for ( local_i = 0; local_i < local_num; ++local_i) {
        local_result[local_i]=local_a[local_i]*local_b[local_i];
    }
    printf("calculating of %d has finished... result is %d\n",my_rank,*local_result);
}
/*-------------------------------------------------------------------*/
void print_result(int local_result[],int n,int local_num){
    int* a=NULL;
    int* counts=NULL;
    int* slides=NULL;
    int local_i;
    if (my_rank==0){
        a=malloc(n*sizeof(int));
        slides=malloc(comm_sz*sizeof(int));
        counts=malloc(comm_sz * sizeof(int));
        get_slides(slides, counts, n);
        MPI_Gatherv(local_result, local_num, MPI_INT, a, counts, slides, MPI_INT, 0, MPI_COMM_WORLD);
        for (local_i = 0; local_i < n; ++local_i) {
            printf("%d ",a[local_i]);
        }
        free(a);
        free(slides);
        free(counts);
    } else{
        MPI_Gatherv(local_result, local_num, MPI_INT, a, counts, slides, MPI_INT, 0, MPI_COMM_WORLD);
    }
}