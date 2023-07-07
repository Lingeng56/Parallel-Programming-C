//
// Created by 林庚 on 2021/5/8.
//
#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <math.h>
/*-------------------------*
 *     Local Function      *
 *-------------------------*/
void read_n(int*,MPI_Comm);
void read_array(int*,int);
void merge_sort(int*,int,int);
void merge(int*,int,int,int);
void merge_all(int*,int,MPI_Comm comm);
int compute_partner(int);
/*-------------------------*
 *     Global Variable     *
 *-------------------------*/
int comm_sz;
int my_rank;
int rem[1024]={100};
int rem_index=0;
int main(){
    MPI_Init(NULL,NULL);
    MPI_Comm comm=MPI_COMM_WORLD;
    MPI_Comm_size(comm,&comm_sz);
    MPI_Comm_rank(comm,&my_rank);
    int n;
    read_n(&n,comm);
    int local_array[n];
    int i;

    read_array(local_array,n);
    merge_sort(local_array,0,n/comm_sz-1);
    merge_all(local_array,n,comm);

    MPI_Barrier(comm);
    if (my_rank==0){
        printf("merge sort successfully!\n");
        printf("result:\n");
        for (i = 0; i < n; ++i) {
            printf("%d ",*(local_array+i));
        }
    }


    MPI_Finalize();

}/* main */


/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
void read_n(int* n,MPI_Comm comm){
    if (my_rank==0){
        printf("input your length of array\n");
        scanf("%d",n);
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
void read_array(int* local_array,int n){
   int i;
   srandom(my_rank+random());
    for (i = 0; i <n/comm_sz ; ++i) {
        *(local_array+i)=(int)random()%(n+10)+1;
    }
}


/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
void merge(int* local_array,int lo,int mid,int hi){
    int arr[hi-lo+1];
    int i=lo;
    int j=mid+1;
    int p=lo;
    while (i<=mid && j<=hi) {
        if (*(local_array + i) < *(local_array + j)) {
            arr[p++] = local_array[i++];
        } else {
            arr[p++] = local_array[j++];
        }
    }
    while (i<=mid){
        arr[p++]=local_array[i++];
    }
    while (j<=hi){
        arr[p++]=local_array[j++];
    }

    for (int k = lo; k <=hi; ++k) {
        *(local_array+k)=arr[k];
    }
}


/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */

void merge_sort(int* local_array,int lo,int hi){
    if (lo>=hi){
        return;
    }
    int mid=lo+(hi-lo)/2;
    merge_sort(local_array,lo,mid);
    merge_sort(local_array,mid+1,hi);
    merge(local_array,lo,mid,hi);
}


/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
void merge_all(int *local_array,int n,MPI_Comm comm){
    int i;
    int j;
    int partner;
    int *trans=NULL;
    for (i = 0; i <pow(comm_sz,0.5); ++i) {
        int length=n/comm_sz*(int)pow(2,i);
        trans=malloc(length*sizeof(int));
        partner=compute_partner(i);
        if (my_rank<partner){
             MPI_Recv(trans,length,MPI_INT,partner,0,comm,MPI_STATUS_IGNORE);
            for (j = 0; j <length ; ++j) {
                *(local_array+length+j)=*(trans+j);
            }
            int lo=0;
            int hi=2*length-1;
            int mid=lo+(hi-lo)/2;
            merge(local_array,lo,mid,hi);
        } else if (my_rank>partner){
            rem[rem_index++]=my_rank;
            for (j = 0; j <length ; ++j) {
                *(trans+j)=*(local_array+j);
            }
            MPI_Send(trans,length,MPI_INT,partner,0,comm);
        }
        MPI_Barrier(comm);
    }
}


/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
int compute_partner(int phase){
    unsigned bitmask=1;
    int i;
    for (i = 0; i <phase ; ++i) {
        bitmask <<= 1;
    }
    for (i = 0; i <rem_index ; ++i) {
        if (rem[i]==my_rank){
            return MPI_PROC_NULL;
        }
    }

    return my_rank ^ bitmask;
}