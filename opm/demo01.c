//
// Created by 林庚 on 2021/5/19.
//
#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
int main(int argc,char* argv[]) {
    long thread_count = strtol(argv[1], NULL, 10);
#pragma omp parallel num_threads(thread_count)
#pragma omp parallel for
    for (int i = 0; i < 10; ++i) {
        printf("%d enter circle\n",omp_get_num_threads());
    }


    return 0;
}
