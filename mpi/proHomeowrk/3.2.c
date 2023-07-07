//
// Created by 林庚 on 2021/5/1.
//
#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

/*-------------------------*
 *     Local Function      *
 *-------------------------*/
void get_num(long long int*,long long int*,MPI_Comm);
void test(long long int,long long int*,long long int*,MPI_Comm);
/*-------------------------*
 *     Global Variable     *
 *-------------------------*/
int comm_sz;
int my_rank;
int main(){
    MPI_Init(NULL,NULL);
    double start=MPI_Wtime();
    MPI_Comm comm=MPI_COMM_WORLD;
    MPI_Comm_size(comm,&comm_sz);
    MPI_Comm_rank(comm,&my_rank);
    long long int all_num,local_num,local_num_in,total_num_in=0;
    get_num(&all_num,&local_num,comm);
    test(local_num,&local_num_in,&total_num_in,comm);
    MPI_Barrier(comm);
    double finish=MPI_Wtime();
    if (my_rank==0){
        printf("result is:\n%f\ncost time:\n%lf",4.0*(double)total_num_in/(double)all_num,finish-start);
    }
    MPI_Finalize();
    return 0;
}
void get_num(long long int* all_num,long long int* local_num,MPI_Comm comm){
    if (my_rank==0){
        printf("input your total num:\n");
        scanf("%lld",all_num);
        *local_num=*all_num/comm_sz;
        MPI_Bcast(local_num,1,MPI_INT,0,comm);
    } else{
        MPI_Bcast(local_num,1,MPI_INT,0,comm);
    }
}
void test(long long int local_num,long long int* local_num_in,long long int* total_num_in,MPI_Comm comm){
    int local_i;
    double test_num_x,test_num_y;
    for (local_i = 0; local_i < local_num; ++local_i) {
        test_num_x=1.0*rand()/RAND_MAX *(1-0);
        test_num_y=1.0*rand()/RAND_MAX *(1-0);
        if (test_num_y*test_num_y+test_num_x*test_num_x<=1){
            (*local_num_in)++;
        }
    }
    MPI_Reduce(local_num_in,total_num_in,1,MPI_INT,MPI_SUM,0,comm);

}

