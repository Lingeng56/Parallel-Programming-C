//
// Created by 林庚 on 2021/4/23.
//
#include <stdio.h>
#include <mpi.h>
int main(){
    int comm_sz;
    int my_rank;
    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
    int n=comm_sz;
    int a[n];
    int local_x;
    int local_result=0;
    if (my_rank==0){
        printf("input your nums\n");
        for (int i = 0; i < n; ++i) {
            scanf("%d",a+i);
            fflush(stdin);
        }

        for (int i = 1; i < n; ++i) {
            local_x=a[i];
            MPI_Send(&local_x,1,MPI_INT,i,1,MPI_COMM_WORLD);
        }
        local_x=a[0];
    } else{
        MPI_Recv(&local_x,1,MPI_INT,0,1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    }
    if (my_rank==0){
        local_result=local_x;
        MPI_Send(&local_result,1,MPI_INT,my_rank==comm_sz-1?0:my_rank+1,1,MPI_COMM_WORLD);
    } else{
    MPI_Recv(&local_result,1,MPI_INT,my_rank==0?comm_sz-1:my_rank-1,1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    local_result+=local_x;
    MPI_Send(&local_result,1,MPI_INT,my_rank==comm_sz-1?0:my_rank+1,1,MPI_COMM_WORLD);}
    printf("%d ",local_result);
    MPI_Finalize();
}