//
// Created by 林庚 on 2021/4/23.
//
#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
void read_vector(int n,int my_rank,int local_a[],int local_n,int local_b[]){
    int *a=NULL;
    int *b=NULL;
    if (my_rank==0){
        a=malloc(n*sizeof(int));
        b=malloc(n*sizeof(int));
        printf("input your vec a\n");
        int local_i;
        for (local_i = 0; local_i < n; ++local_i) {
            scanf("%d",a+local_i);
            fflush(stdin);
        }
        printf("input your vec b\n");
        for (local_i = 0; local_i <n ; ++local_i) {
            scanf("%d",b+local_i);
            fflush(stdin);
        }
        MPI_Scatter(a,local_n,MPI_INT,local_a,local_n,MPI_INT,0,MPI_COMM_WORLD);
        MPI_Scatter(b,local_n,MPI_INT,local_b,local_n,MPI_INT,0,MPI_COMM_WORLD);
        free(a);
        free(b);
    } else{
        MPI_Scatter(a,local_n,MPI_INT,local_a,local_n,MPI_INT,0,MPI_COMM_WORLD);
        MPI_Scatter(b,local_n,MPI_INT,local_b,local_n,MPI_INT,0,MPI_COMM_WORLD);
    }
}
void multi(int local_a[],int m,int local_n,int local_b[]){
    int local_i;
    for (local_i = 0; local_i <local_n ; ++local_i) {
        local_b[local_i]=m*local_a[local_i];
    }
}
void dot(int local_a[],int local_b[],int local_n,int local_result[]){
    int local_i;
    for (local_i = 0; local_i < local_n; ++local_i) {
        local_result[local_i]=local_a[local_i]*local_b[local_i];
    }
}
void print_vector(int my_rank,int local_b[],int local_n,int n,char vec_name[]){
    int *b=NULL;
    int local_i;
    if (my_rank==0){
        b=malloc(n*sizeof(int));
        MPI_Gather(local_b,local_n,MPI_INT,b,local_n,MPI_INT,0,MPI_COMM_WORLD);
        printf("\nresult of %s:\n",vec_name);
        for ( local_i = 0; local_i < n; ++local_i) {
            printf("%d ",b[local_i]);
            fflush(stdout);
        }
        free(b);
    } else{
        MPI_Gather(local_b,local_n,MPI_INT,b,local_n,MPI_INT,0,MPI_COMM_WORLD);
    }
}
int main(){
    int comm_sz;
    int my_rank;
    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
    int n=12;
    int m=3;
    int local_n=n/comm_sz;
    int local_a[local_n];
    int local_b[local_n];
    int local_result[local_n];
    read_vector(n, my_rank, local_a, local_n, local_b);
    multi(local_a, m, local_n, local_result);
    print_vector(my_rank,local_result,local_n,n,"multi");
    dot(local_a,local_b,local_n,local_result);
    print_vector(my_rank,local_result, local_n, n,"dot");
    MPI_Finalize();

    return 0;
}
