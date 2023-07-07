//
// Created by 林庚 on 2021/5/1.
//
#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <math.h>
#define MAX_CONTRIB 20
/*-------------------------*
 *     Local Function      *
 *-------------------------*/
void global_sum(int*,MPI_Comm);
/*-------------------------*
 *     Global Variable     *
 *-------------------------*/
int comm_sz;
int my_rank;
int main() {
    MPI_Init(NULL, NULL);
    MPI_Comm comm = MPI_COMM_WORLD;
    MPI_Comm_size(comm, &comm_sz);
    MPI_Comm_rank(comm, &my_rank);


    srandom(my_rank - 1);
    int local_num = random() % MAX_CONTRIB;
    int *all_num = NULL;


    if (my_rank == 0) {
        all_num = malloc(comm_sz * sizeof(int));
        int i;
        MPI_Gather(&local_num, 1, MPI_INT, all_num, 1, MPI_INT, 0, comm);
        printf("have received %d numbers\n", comm_sz);
        for (i = 0; i < comm_sz; ++i) {
            printf("%d ", *(all_num + i));
        }
    } else {
        MPI_Gather(&local_num, 1, MPI_INT, all_num, 1, MPI_INT, 0, comm);
    }

    MPI_Barrier(comm);


    global_sum(&local_num,comm);

    printf("\nthread %d get sum of these num is: %d\n",my_rank,local_num);



    MPI_Finalize();
}/* main */


/*-------------------------------------------------------------------
 * Function:    global_sum
 * Purpose:     get global sum
 * Input args:  int local_num,MPI_Comm comm
 * In/out args: sum
 */
void global_sum(int *local_num,MPI_Comm comm){
    int partner,recv_temp;
    int i;
    int j;
    for (i = 0; i <pow(comm_sz,0.5) ; ++i) {
        int flag=0;
        for (j = 0; j <=pow(2,i)-1; ++j) {
            if ((my_rank-j)%(int)pow(2,i+1)==0){
                partner=my_rank+(int)pow(2,i);
                flag=1;
                break;
            }
        }
        if (flag==0){
            partner=my_rank-(int)pow(2,i);
        }
       MPI_Sendrecv(local_num,1,MPI_INT,partner,1,&recv_temp,1,MPI_INT,partner,1,comm,MPI_STATUS_IGNORE);
        *local_num+=recv_temp;
    }
}/* global sum */

