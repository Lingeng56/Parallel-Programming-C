//
// Created by 林庚 on 2021/5/5.
//
#include <stdio.h>
#include <mpi.h>
#include <math.h>
/*-------------------------*
 *     Local Function      *
 *-------------------------*/
void getN(int*,MPI_Comm);
void get_vector(double*,int,MPI_Comm);
void get_matrix(double*,int,MPI_Comm);
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

    int local_begin_row,local_begin_column,n,i,j;
    getN(&n,comm);
    double result[comm_sz*n/(int)pow(comm_sz,0.5)],vector[n],matrix[n*n];
    double local_result[n/(int)pow(comm_sz,0.5)];
    local_begin_row=my_rank%(int)pow(comm_sz,0.5)*n/(int)pow(comm_sz,0.5);
    local_begin_column=my_rank/(int)pow(comm_sz,0.5)*n/(int)pow(comm_sz,0.5);

    get_vector(vector,n,comm);
    get_matrix(matrix,n,comm);

    /*     dot     */
    printf("thread %d begin to dot...\nbegin of row is %d,begin of column is %d\n",my_rank,local_begin_row,local_begin_column);
    for (i = 0; i <n/(int)pow(comm_sz,0.5) ; ++i) {
        *(local_result+i)=0;
    }
    for (j = 0; j < n / (int) pow(comm_sz, 0.5); ++j) {
        for (i = 0; i < n / (int) pow(comm_sz, 0.5); ++i) {
            *(local_result+j)+=*(vector+i+local_begin_column)**(matrix+i+local_begin_row+j*n*local_begin_column);
        }
    }
    MPI_Gather(local_result,n / (int) pow(comm_sz, 0.5),MPI_DOUBLE,result,n / (int) pow(comm_sz, 0.5),MPI_DOUBLE,0,comm);


    /*    print result    */
    if (my_rank==0) {
        double final_result[n];
        for (i = 0; i < n; ++i) {
            *(final_result + i) = 0;
            for (j = 0; j < n / (int) pow(comm_sz, 0.5); j++) {
                *(final_result + i) += *(result + j * n / (int) pow(comm_sz, 0.5));
            }
        }
        printf("result:\n");
        for (i = 0; i < n; ++i) {
            printf("%lf ", *(final_result + i));
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
    int i;
    if (my_rank==0){
        printf("please input your %dx%d matrix\n",n,n);
        for (i = 0; i < n*n; ++i) {
            scanf("%lf",matrix+i);
        }
        MPI_Bcast(matrix,n*n,MPI_DOUBLE,0,comm);
    } else{
        MPI_Bcast(matrix,n*n,MPI_DOUBLE,0,comm);
    }
}




