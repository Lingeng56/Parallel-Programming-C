//
// Created by 林庚 on 2021/5/8.
//
#include <stdio.h>
#include <mpi.h>
#include <time.h>

/*-------------------------*
 *     Global Variable     *
 *-------------------------*/
int comm_sz;
int my_rank;
int main(){
    MPI_Init(NULL,NULL);
    MPI_Comm comm=MPI_COMM_WORLD;
    MPI_Comm_size(comm,&comm_sz);
    MPI_Comm_rank(comm,&my_rank);

    int a;
    double start,finish,start1,finish1;
    if (my_rank==0){
        start=clock();
        start1=MPI_Wtime();
        printf("thread 0 begin to set a as 1\n");
        a=1;
        MPI_Send(&a,1,MPI_INT,1,0,comm);
        printf("thread 0 send successfully!\n");
        MPI_Recv(&a,1,MPI_INT,1,0,comm,MPI_STATUS_IGNORE);
        printf("received a from thread 1 successfully,a is %d now\n",a);
        finish=clock();
        finish1=MPI_Wtime();
        printf("all program finish,cost %lf ms for clock(),cost %lf ms for MPI_Wtime()\n",finish-start,finish1-start1);
    } else{
        MPI_Recv(&a,1,MPI_INT,0,0,comm,MPI_STATUS_IGNORE);
        printf("received a from thread 0 successfully,a is %d now\n",a);
        printf("thread 1 begin to change a to be 2\n");
        a=2;
        MPI_Send(&a,1,MPI_INT,0,0,comm);
        printf("thread 1 send successfully!\n");
    }

    MPI_Finalize();

}
/*answer
 *
 * clock()返回的是程序的cpu时钟周期数！！！！！
 * MPI_Wtime()返回的是世界时间！！！！！
 *
 *
 * */