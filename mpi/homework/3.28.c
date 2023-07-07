//
// Created by 林庚 on 2021/4/27.
//
#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#define LEN 8
/*-------------------------*
 *     Local Function      *
 *-------------------------*/
void getArray(double *);
void printArray(double *);
void mergeSort(double*);
void partSort(double *,int);
int compute_partner(int);
void keep_smaller(double*,double*,int);
void keep_bigger(double*,double*,int);
/*-------------------------*
 *     Global Variable     *
 *-------------------------*/
int comm_sz;
int my_rank;
int main(){
    MPI_Init(NULL,NULL);

    MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);


    double local_array[LEN/comm_sz];
    getArray(local_array);
    MPI_Barrier(MPI_COMM_WORLD);


    partSort(local_array,LEN/comm_sz);
    if (my_rank==0){
        printf("part sort successfully !\n");
    }
    MPI_Barrier(MPI_COMM_WORLD);
    mergeSort(local_array);
    MPI_Barrier(MPI_COMM_WORLD);



    MPI_Barrier(MPI_COMM_WORLD);
    if (my_rank==0){
        printf("%d threads all merge successfully!\n",comm_sz);
        printf("result:\n");
    }
    printArray(local_array);
    MPI_Finalize();
}
/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
void getArray(double *local_a){
    double* a=NULL;
    if(my_rank==0){
        a=malloc(LEN*sizeof(double));
        int i;
        printf("input your array ,it has %d args\n",LEN);
        for (i = 0; i < LEN; ++i) {
            scanf("%lf",a+i);
        }
        MPI_Scatter(a,LEN/comm_sz,MPI_DOUBLE,local_a,LEN/comm_sz,MPI_DOUBLE,0,MPI_COMM_WORLD);
    } else{
        MPI_Scatter(a,LEN/comm_sz,MPI_DOUBLE,local_a,LEN/comm_sz,MPI_DOUBLE,0,MPI_COMM_WORLD);
    }
}
/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
void mergeSort(double* local_a){
    if (my_rank==0){
        printf("begin to merge!\n");
    }
    int phase;
    int partner;
    double *temp;
    double *now;
    temp=malloc(LEN/comm_sz*sizeof(double));
    now=malloc(LEN/comm_sz*sizeof(double));
    int i;
    for (phase = 0;  phase<comm_sz ; phase++) {
        for (i = 0; i < LEN/comm_sz; ++i) {
            *(now+i)=*(local_a+i);
        }
        partner = compute_partner(phase);
        MPI_Sendrecv(now,LEN/comm_sz,MPI_DOUBLE,partner,0,temp,LEN/comm_sz,MPI_DOUBLE,partner,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            if (my_rank < partner && partner!=-2) {
                keep_smaller(local_a, temp, LEN / comm_sz);
            } else if (my_rank>partner && partner!=-2){
                keep_bigger(local_a, temp, LEN / comm_sz);
            }
            MPI_Barrier(MPI_COMM_WORLD);
    }
    free(temp);
    free(now);

}
/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
int compute_partner(int phase){
    if (phase%2==0){
        if (my_rank%2==0){
            return my_rank+1;
        } else{
            return my_rank-1;
        }
    } else{
        if (my_rank%2==0){
            if (my_rank==0){
                return MPI_PROC_NULL;
            }
            return my_rank-1;
        } else{
            if (my_rank+1==comm_sz){
                return MPI_PROC_NULL;
            }
            return my_rank+1;
        }
    }
}
/*-------------------------------------------------------------------
 * Function:    oddEvenSort
 * Purpose:     sort the sequence
 * Input args:  double a[],int n
 * In/out args:
 */
void partSort(double *a,int n){
    int phase,i;
    double temp;
    for (phase = 0; phase <n ; ++phase) {
        if (phase % 2 == 0) {
            for (i=1 ;i < n; i += 2) {
                if (a[i] < a[i - 1]) {
                    temp = a[i];
                    a[i] = a[i - 1];
                    a[i - 1] = temp;
                }
            }
        } else {
            for (i=1 ;i < n - 1; i += 2) {
                if (a[i] > a[i + 1]) {
                    temp = a[i];
                    a[i] = a[i + 1];
                    a[i + 1] = temp;
                }
            }
        }
    }

}
/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
void keep_smaller(double *local_a,double *temp,int n){
    double *change=NULL;
    int i;
    change=malloc(2*n*sizeof(double));
    for (i = 0; i < n; ++i) {
        *(change+i)=*(local_a+i);
        *(change+n+i)=*(temp+i);
    }
    partSort(change,2*n);
    for (i = 0; i < n; ++i) {
        *(local_a+i)=*(change+i);
    }
    free(change);


}
/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
void keep_bigger(double *local_a,double *temp,int n){
    double *change=NULL;
    int i;
    change=malloc(2*n*sizeof(double));
    for (i = 0; i < n; ++i) {
        *(change+i)=*(local_a+i);
        *(change+n+i)=*(temp+i);
    }
    partSort(change,2*n);
    for (i = 0; i < n; ++i) {
        *(local_a+i)=*(change+i+n);
    }
    free(change);
}
/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
void printArray(double *local_a){
    double *result=NULL;
    int i;
    if (my_rank==0){
        result=malloc(LEN*sizeof(double));
        MPI_Gather(local_a,LEN/comm_sz,MPI_DOUBLE,result,LEN/comm_sz,MPI_DOUBLE,0,MPI_COMM_WORLD);
        for (i = 0; i < LEN; ++i) {
            printf("%lf ",*(result+i));
        }
        free(result);
    } else{
        MPI_Gather(local_a,LEN/comm_sz,MPI_DOUBLE,result,LEN/comm_sz,MPI_DOUBLE,0,MPI_COMM_WORLD);
    }
}