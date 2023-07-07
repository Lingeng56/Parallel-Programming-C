// Created by 林庚 on 2021/5/5.
//
#include <stdio.h>
#include <mpi.h>
/*-------------------------*
 *     Local Function      *
 *-------------------------*/
void getN(int*,MPI_Comm);
void get_vector(double*,int,MPI_Comm);
void get_matrix(double*,int,MPI_Comm);
void dot(double*,double*,double*,double*,int,MPI_Comm);
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


    int n;
    int i;
    getN(&n,comm);
    double vector[n];
    double matrix[n*n/comm_sz];
    double local_result[n/comm_sz];
    double result[n];
    MPI_Barrier(comm);


    get_vector(vector,n,comm);
    get_matrix(matrix,n,comm);
    dot(matrix,vector,local_result,result,n,comm);

    if (my_rank==0){
        printf("result:\n");
        for (i = 0; i <n ; ++i) {
            printf("%lf ",*(result+i));
        }
    }

    MPI_Finalize();
}

/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
void getN(int* n,MPI_Comm comm){
    if (my_rank==0){
        printf("please input your length of vector:\n");
        scanf("%d",n);
        fflush(stdin);
        MPI_Bcast(n,1,MPI_INT,0,comm);
    } else{
        MPI_Bcast(n,1,MPI_INT,0,comm);
    }
}


/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
void get_vector(double* vector,int n,MPI_Comm comm){
    if (my_rank==0){
        printf("please input your vector with %d args\n",n);
        fflush(stdout);
        int i;
        for (i = 0; i <n ; ++i) {
            scanf("%lf",vector+i);
        }
        MPI_Bcast(vector,n,MPI_DOUBLE,0,comm);
    } else{
        MPI_Bcast(vector,n,MPI_DOUBLE,0,comm);
    }
}

/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
void get_matrix(double *matrix,int n,MPI_Comm comm){
    double temp[n*n];
    int i;
    int j;
    if (my_rank==0){
        printf("please input your %dx%d matrix\n",n,n);
        for (i = 0; i <n ; ++i) {
            for (j = 0; j <n ; ++j) {
                scanf("%lf",&temp[j*n+i]);
            }
        }
        MPI_Scatter(temp,n*n/comm_sz,MPI_DOUBLE,matrix,n*n/comm_sz,MPI_DOUBLE,0,comm);
    } else{
        MPI_Scatter(temp,n*n/comm_sz,MPI_DOUBLE,matrix,n*n/comm_sz,MPI_DOUBLE,0,comm);
    }
}


/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
void dot(double *matrix,double *vector,double *local_result,double *result,int n,MPI_Comm comm){
    int i;
    int j;
    for (i = 0; i <n/comm_sz ; ++i) {
        double sum=0;
        for (j = 0; j <n ; ++j) {
            sum+=*(matrix+j+i*n)**(vector+j);
        }
        *(local_result+i)=sum;
    }
    MPI_Gather(local_result,n/comm_sz,MPI_DOUBLE,result,n/comm_sz,MPI_DOUBLE,0,comm);

}


