//
// Created by 林庚 on 2021/4/18.
//
#include<stdio.h>
#include<mpi.h>
void Get_Input(int my_rank,int comm_sz,double *a_p,double *b_p,int *n_p);
int main(){
    int comm_sz;
    int my_rank;
    double x=1.0;
    double y=2.0;
    int z=1;
    double *a=&x;
    double *b=&y;
    int *n=&z;

    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
        if (my_rank==0) {
            *a=3.0;
            *b=4.0;
            *n=5;}

        MPI_Bcast(a, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Bcast(b, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Bcast(n, 1, MPI_INT, 0, MPI_COMM_WORLD);


    if (my_rank!=0){
        printf("%.2f from %d\n",*a,my_rank);
        printf("%.2f from %d\n",*b,my_rank);
        printf("%d from %d\n",*n,my_rank);
    }
    MPI_Finalize();
    return 0;
}
void Get_Input(int my_rank,int comm_sz,double *a_p,double *b_p,int *n_p){

}
