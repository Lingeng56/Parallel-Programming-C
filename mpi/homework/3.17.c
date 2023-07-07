//
// Created by 林庚 on 2021/4/21.
//
#include<stdlib.h>
#include<stdio.h>
#include<mpi.h>
void parallel_vector_sum(
        double  local_x[],
        double  local_y[],
        double  local_z[],
        int     local_n);
void read_vector(
        double  local_a[],
        int     local_n,
        int     n,
        char    vec_name[],
        int     my_rank,
        MPI_Comm comm       );
void print_vector(
        double  local_b[],
        int     local_n,
        int     n,
        char    title[],
        int     my_rank,
        MPI_Comm comm
);
int main(){
    int comm_sz=0;
    int my_rank=0;

    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);

    int n=12;
    double x[n];
    double y[n];
    double z[n];
    read_vector(x,n/comm_sz,n,"X",my_rank,MPI_COMM_WORLD);
    read_vector(y,n/comm_sz,n,"Y",my_rank,MPI_COMM_WORLD);

    parallel_vector_sum(x,y,z,n/comm_sz);

    print_vector(z,n/comm_sz,n,"result",my_rank,MPI_COMM_WORLD);

    MPI_Finalize();
    return 0;
}
/*-------------------------------------------------------------------*/
void read_vector(
        double  local_a[],
        int     local_n,
        int     n,
        char    vec_name[],
        int     my_rank,
        MPI_Comm comm       ){
    double * a=NULL;
    MPI_Datatype type;
    PMPI_Type_contiguous(local_n,MPI_DOUBLE,&type);
    MPI_Type_commit(&type);
    int i;
    if (my_rank == 0){
        a = malloc(n*sizeof(double));
        printf("enter the vector %s",vec_name);
        fflush(stdout);
        for (i = 0; i < n; i++) {
            scanf("%lf",&a[i]);
            fflush(stdin);
        }
        MPI_Scatter(a,1,type,local_a,local_n,MPI_DOUBLE,0,comm);
        free(a);
    } else{
        MPI_Scatter(a,1,type,local_a,local_n,MPI_DOUBLE,0,comm);
        fflush(stdin);
    }
}
/*-------------------------------------------------------------------*/
void parallel_vector_sum(
        double  local_x[],
        double  local_y[],
        double  local_z[],
        int     local_n){
    int local_i;
    for (local_i = 0; local_i < local_n; ++local_i) {
        local_z[local_i]=local_x[local_i]+local_y[local_i];
    }
}
/*-------------------------------------------------------------------*/
void print_vector(
        double  local_b[],
        int     local_n,
        int     n,
        char    title[],
        int     my_rank,
        MPI_Comm comm
){
    MPI_Datatype type;
    PMPI_Type_contiguous(local_n,MPI_DOUBLE,&type);
    MPI_Type_commit(&type);
    double *b=NULL;
    int i;
    if (my_rank==0){
        b=malloc(n*sizeof(double ));
        MPI_Gather(local_b,1,type,b,local_n,MPI_DOUBLE,0,comm);
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
        MPI_Gather(local_b,1,type,b,local_n,MPI_DOUBLE,0,comm);
    }
}