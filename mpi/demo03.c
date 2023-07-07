//
// Created by 林庚 on 2021/4/20.
//
#include<stdio.h>
#include <mpi.h>
void vector_sum(const int local_x[], const int local_y[],int local_z[],int local_n);
int main(){
    int comm_sz;
    int my_rank;
    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
    int x[]={1,2,3,4,5,6,7,8,9,10,11,12};
    int y[]={6,5,4,3,2,1,1,2,3,4,5,6};
    int z[12]={0};
    vector_sum(x,y,z,my_rank*3);
    if (my_rank!=0){
        MPI_Send(z+my_rank*3,3,MPI_INT,0,0,MPI_COMM_WORLD);
    } else{
        MPI_Recv(z+3,3,MPI_INT,1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        MPI_Recv(z+6,3,MPI_INT,2,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        MPI_Recv(z+9,3,MPI_INT,3,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        for (int i = 0; i < 12; ++i) {
            printf("%d  ",*(z+i));
        }
        printf("\n");
    }
    MPI_Finalize();
    return 0;
}
void vector_sum(const int local_x[],const int local_y[],int local_z[],int local_n){
    for (int j = 0; j < 3; ++j) {
        local_z[j+local_n]=local_x[j+local_n]+local_y[j+local_n];
    }
}
