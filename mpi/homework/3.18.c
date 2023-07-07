//
// Created by 林庚 on 2021/4/24.
//
#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
void read_vector(int[],int,int,int);
void print_result(int[],int,int);
void read_n(int*);
void get_count_stride(int*,int*);


int comm_sz;
int my_rank;
int main(){
    int n;
    int count,stride;


    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);


    get_count_stride(&count,&stride);
    read_n(&n);
    int local_a[count];

    read_vector(local_a,n,count,stride);
    MPI_Barrier(MPI_COMM_WORLD);
    print_result(local_a,count,stride);


    MPI_Finalize();
}
/*-------------------------------------------------------------------*/
void read_n(int *n){
    if (my_rank==0){
        printf("input n:\n");
        scanf("%d",n);


        MPI_Bcast(n,1,MPI_INT,0,MPI_COMM_WORLD);
    }
}
/*-------------------------------------------------------------------*/
void read_vector(int local_a[],int n,int count,int stride){
    int* a=NULL;
    MPI_Datatype type;
    MPI_Type_vector(1,count,stride,MPI_INT,&type);
    MPI_Type_commit(&type);
    int local_i;
    a=malloc(n*sizeof(int));
    if (my_rank==0){

        printf("input your vec:\n");
        for (local_i = 0; local_i < n; ++local_i) {
            scanf("%d",a+local_i);
        }
        for (local_i = 1; local_i <comm_sz ; ++local_i) {
            MPI_Send(a+stride*local_i,1,type,local_i,0,MPI_COMM_WORLD);
        }
        for (local_i = 0; local_i < count; ++local_i) {
            *(local_a+local_i)=*(a+local_i);
        }
        printf("send successfully!\n");
    } else{
        printf("thread %d begin to receive!\n",my_rank);
        MPI_Recv(local_a,count,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        printf("thread %d received successfully!",my_rank);
    }
    printf("thread %d received:\n",my_rank);
    for ( local_i= 0; local_i <count ; local_i++) {
        printf("%d ",local_a[local_i]);
    }
    printf("\n");
    free(a);
}
/*-------------------------------------------------------------------*/
void get_count_stride(int*count,int*stride){
    if (my_rank==0){
    printf("input your count:\n");
    scanf("%d",count);


    printf("input your stride:\n");
    scanf("%d",stride);


    MPI_Bcast(count,1,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Bcast(stride,1,MPI_INT,0,MPI_COMM_WORLD);
    }
}
/*-------------------------------------------------------------------*/
void print_result(int local_result[],int count,int stride){
    int*b=NULL;
    MPI_Datatype type;
    MPI_Type_vector(1,count,stride,MPI_INT,&type);
    MPI_Type_commit(&type);
    int local_i;
    printf("begin to print....\n");

    if (my_rank==0){
        b=malloc(count*comm_sz*sizeof(int));
        for (local_i = 0; local_i <count ; ++local_i) {
            *(b+local_i)=*(local_result+local_i);
        }
        for (local_i = 1; local_i < comm_sz; ++local_i) {
            MPI_Recv(b,1,type,local_i,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        }
        printf("result:\n");
        for (local_i = 0; local_i < count*comm_sz; ++local_i) {
            printf("%d ",*(b+local_i));
        }
        free(b);
    } else{
        MPI_Send(local_result,count,MPI_INT,0,0,MPI_COMM_WORLD);
    }

}