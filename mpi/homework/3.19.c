//
// Created by 林庚 on 2021/4/24.
//
#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

/*-------------------------*
 *     Local Function      *
 *-------------------------*/
//void read_vector();
//void print_vector();


/*-------------------------*
 *     Global Variable    *
 *-------------------------*/
int comm_sz;
int my_rank;
int main(){
    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);


    int n=3;
    MPI_Datatype type;
    int *b=NULL;
    int i;
    MPI_Datatype type_array[n];
    int length_array[n];
    MPI_Aint place_array[n];

    struct my_Data{
        int a;
        int b;
        int c;
    };
    struct my_Data data1;
    MPI_Aint a_addr;
    MPI_Aint b_addr;
    MPI_Aint c_addr;
    MPI_Get_address(&data1.a,&a_addr);
    MPI_Get_address(&data1.b,&b_addr);
    MPI_Get_address(&data1.c,&c_addr);
    place_array[0]=0;
    place_array[1]=b_addr-a_addr;
    place_array[2]=c_addr-a_addr;
    printf("place is:");
    for (i = 0; i < n; ++i) {
        type_array[i] = MPI_INT;
        length_array[i]=1;
        printf("%td ",place_array[i]);
    }
    printf("\n");
    MPI_Type_create_struct(n,length_array,place_array,type_array,&type);
    MPI_Type_commit(&type);

    if (my_rank==0) {
        printf("input your vector:\n");
        fflush(stdout);
        scanf("%d",&data1.a);
        scanf("%d",&data1.b);
        scanf("%d",&data1.c);
        MPI_Bcast(&data1,1,type,0,MPI_COMM_WORLD);
        printf("successfully send\n");
    } else{
        MPI_Bcast(&data1,1,type,0,MPI_COMM_WORLD);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    printf("thread %d receive data: %d %d %d\n",my_rank,data1.a,data1.b,data1.c);





    MPI_Type_free(&type);
    MPI_Finalize();
}