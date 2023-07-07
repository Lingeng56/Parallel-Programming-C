//
// Created by 林庚 on 2021/5/21.
//
#include <stdio.h>
#include <omp.h>

int main(int argc,char* argv[]){
    long thread_count=strtol(argv[1],NULL,10);
    int i;
    int n=1024;
    int my_rank;
    int* min=malloc(thread_count*sizeof(int));
    int* max=malloc(thread_count*sizeof(int));
    for (i = 0; i <thread_count ; ++i) {
        *(min+i)=n;
        *(max+i)=0;
    }
#pragma omp parallel num_threads(thread_count)\
    default(none) shared(min,max,n,thread_count) private(my_rank,i)
    {
        my_rank=omp_get_thread_num();
#pragma omp for
        for (i = 0; i < n; ++i) {
            if (i < min[my_rank]) {
                *(min + my_rank) = i;
            }
            if (i > max[my_rank]) {
                *(max + my_rank) = i;
            }
        }
    }

    for (i = 0; i < thread_count; i++) {
        if (min[i] == n && max[i] == 0)
            printf("Th %d > No iterations\n", i);
        else if (min[i] != max[i])
            printf("Th %d > Iterations %d - %d\n", i, min[i], max[i]);
        else
            printf("Th %d > Iteration  %d\n", i, min[i]);
    }
    free(min);
    free(max);
}
