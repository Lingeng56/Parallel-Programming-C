//
// Created by 林庚 on 2021/4/23.
//
#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
void parallel_get_prefix_sum(int my_rank,int num[],int *result){
    int i;
    for ( i = 0; i < my_rank+1; ++i) {
        *result+=num[i];
    }
}
void print_result(int result,int my_rank,int comm_sz){
    int i;
    int *b=NULL;
    if (my_rank==0){
        b=malloc(comm_sz*sizeof(int));
        b[0]=result;
        for (i = 1; i < comm_sz; ++i) {
            MPI_Recv(&result,1,MPI_INT,i,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            b[i]=result;
        }
        for (i = 0; i < comm_sz; ++i) {
            printf("%d ",b[i]);
        }
        free(b);
    } else{
        MPI_Send(&result,1,MPI_INT,0,0,MPI_COMM_WORLD);
    }

}
void get_prefix_sum(int num[],int prefix_sum[],int n){
    int i;
    int j;
    for ( i = 0; i <n ; ++i) {
        int sum=0;
        for (j = 0; j <i ; ++j) {
            sum+=num[j];
        }
        prefix_sum[i]=sum;
    }
}
int main(){
    int comm_sz;
    int my_rank;
    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
    int n=comm_sz;
    int num[n];
    int local_result=0;
    if (my_rank==0){
        printf("input your nums");
        fflush(stdout);
        for (int i = 0; i <n ; ++i) {
            scanf("%d",num+i);
            fflush(stdin);
        }
        for (int i = 1; i < comm_sz; ++i) {
            MPI_Send(num,i+1,MPI_INT,i,0,MPI_COMM_WORLD);
        }
    } else{
        MPI_Recv(num,my_rank+1,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    }
    parallel_get_prefix_sum(my_rank,num,&local_result);
    print_result(local_result,my_rank,comm_sz);
    MPI_Finalize();
}