//
// Created by 林庚 on 2021/4/28.
//
#include <stdio.h>
#include <mpi.h>
int comm_sz=4;
int compute_partner(int phase,int my_rank){
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
int main(){
    int i;
    for (i = 0; i < comm_sz; ++i) {
        for (int j = 0; j < comm_sz; ++j) {
            int partner=compute_partner(i,j);
            printf("phase is %d,my_rank is%d ",partner);
        }

    }
}