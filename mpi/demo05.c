//
// Created by 林庚 on 2021/4/21.
//
#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>
void print_vector(
        double  local_b[],
        int     local_n,
        int     n,
        char    title[],
        int     my_rank,
        MPI_Comm comm
){

    double *b=NULL;
    int i;
    if (my_rank==0){
        b=malloc(n*sizeof(double ));
        MPI_Gather(local_b,local_n,MPI_DOUBLE,b,local_n,MPI_DOUBLE,0,comm);
        printf("%s\n",title);
        fflush(stdout);
        for (i = 0; i <n ;i ++) {
            printf("%f ",b[i]);
            fflush(stdout);
        }
        printf("\n");
        fflush(stdout);
        free(b);
    } else{
        MPI_Gather(local_b,local_n,MPI_DOUBLE,b,local_n,MPI_DOUBLE,0,comm);
    }
}
int main(){
    int comm_sz;
    int my_rank;
    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
    int a[12]={1,2,3,4,5,6,7,8,9,10,11,12};
    int x[3]={1,2,3};
    int y[4]={0};
    int n=3;
    int* result=NULL;
    MPI_Scatter(a,n,MPI_INT,a,n,MPI_INT,0,MPI_COMM_WORLD);
    for (int i = 0; i <n ; ++i) {
        y[my_rank]+=x[i]*a[i];
    }
    if (my_rank==0) {
        result=malloc(4*sizeof(int));
        MPI_Gather(y,1,MPI_INT,result,1,MPI_INT,0,MPI_COMM_WORLD);
        for (int i = 0; i < 4; ++i) {
            printf("%d ", result[i]);
        }
        free(result);
        printf("\n");
    } else{
        MPI_Gather(y+my_rank,1,MPI_INT,result+my_rank,1,MPI_INT,0,MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}
