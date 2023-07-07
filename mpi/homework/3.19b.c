//
// Created by 林庚 on 2021/4/26.
//
#include <stdio.h>
#include <mpi.h>
/*-------------------------*
 *     Global Variable     *
 *-------------------------*/
int comm_sz;
int my_rank;
int main(){
    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);

    struct my_Data{
        int a;
        int b;
        int c;
    };


    struct my_Data data1;
    int n=3;
    MPI_Datatype datatype;
    int length_array[n];
    int place_array[n];
    int i;

    for (i = 0; i < n; ++i) {
        length_array[i]=1;
        place_array[i]=i;
    }

    MPI_Type_indexed(n,length_array,place_array,MPI_INT,&datatype);
    MPI_Type_commit(&datatype);

    if (my_rank==0) {
        printf("input your vector:\n");
        fflush(stdout);
        scanf("%d",&data1.a);
        scanf("%d",&data1.b);
        scanf("%d",&data1.c);
        MPI_Bcast(&data1,1,datatype,0,MPI_COMM_WORLD);
        printf("successfully send\n");
    } else{
        MPI_Bcast(&data1,1,datatype,0,MPI_COMM_WORLD);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    printf("thread %d receive data: %d %d %d\n",my_rank,data1.a,data1.b,data1.c);

    MPI_Type_free(&datatype);
    MPI_Finalize();


}