//
// Created by 林庚 on 2021/4/23.
//
#include<stdio.h>
#include<mpi.h>
int main(){
    int comm_sz;
    int my_rank;
    int flag;

    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
    flag=1;
    MPI_Send(&flag,1,MPI_INT,my_rank==0?comm_sz-1:my_rank-1,0,MPI_COMM_WORLD);
    MPI_Recv(&flag,1,MPI_INT,my_rank==comm_sz-1?0:my_rank+1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    printf("%d output\n",my_rank);

    MPI_Finalize();
}
