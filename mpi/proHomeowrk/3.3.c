//
// Created by 林庚 on 2021/5/1.
//
#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#define MAX_CONTRIB 20
/*-------------------------*
 *     Local Function      *
 *-------------------------*/
int global_sum(int,MPI_Comm);
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
    int local_sum = global_sum(local_num,comm);


    if (my_rank == 0) {
        all_num = malloc(comm_sz * sizeof(int));
        int i;
        MPI_Gather(&local_num, 1, MPI_INT, all_num, 1, MPI_INT, 0, comm);
        printf("have received %d numbers\n", comm_sz);
        for (i = 0; i < comm_sz; ++i) {
            printf("%d ", *(all_num + i));
        }
        printf("\nthe sum of them is :%d", local_sum);

    } else {
        MPI_Gather(&local_num, 1, MPI_INT, all_num, 1, MPI_INT, 0, comm);
    }

    MPI_Finalize();
}/* main */


/*-------------------------------------------------------------------
 * Function:    global_sum
 * Purpose:     get global sum
 * Input args:  int local_num,MPI_Comm comm
 * In/out args: sum
 */
int global_sum(int local_num,MPI_Comm comm){
    int partner,recv_temp,my_sum=local_num;
    unsigned mask=1;
    while (mask<comm_sz){
        partner=my_rank^mask;

        if (my_rank<partner){
            MPI_Recv(&recv_temp, 1, MPI_INT, partner, 0, comm,
                     MPI_STATUS_IGNORE);
            my_sum += recv_temp;
            mask<<=1;
        } else{
            MPI_Send(&my_sum, 1, MPI_INT, partner, 0, comm);
            break;
        }
    }
    return my_sum;

}