//
// Created by 林庚 on 2021/4/17.
//
#include<stdio.h>
#include <string.h>
#include <mpi.h>
const int MAX_STRING =100;
int main(void){
    char greeting[MAX_STRING];
    int  comm_sz;
    int  my_rank;
    int count;
//    告知mpi系统需要进行所有必要的初始化设置
    MPI_Init(NULL,NULL);
    MPI_Status status;
    MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);

    if (my_rank!=0) {
//        args1传送到args0中
        sprintf(greeting, "Greeting from process %d of %d!", my_rank, comm_sz);
        MPI_Send(greeting, strlen(greeting) + 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);

    } else{
        printf("Greetings from process %d of %d!\n",my_rank,comm_sz);
        for (int i = 1; i < comm_sz; ++i) {
            MPI_Recv(greeting,MAX_STRING,MPI_CHAR,MPI_ANY_SOURCE,0,MPI_COMM_WORLD,&status);
            MPI_Get_count(&status,MPI_CHAR,&count);
            printf("%s\n",greeting);
            printf("%d\n",count);
        }
    }

//    告知mpi系统程序结束，释放内存
    MPI_Finalize();
    return 0;
}
