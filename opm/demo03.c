//
// Created by 林庚 on 2021/5/20.
//
#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <math.h>
int main(int argc,char* argv[]){
    double pi=0.0;
    int n=102400;
    long thread_count=strtol(argv[1],NULL,10);
    int i;
    printf("there is %ld threads\n",thread_count);
#pragma omp parallel for num_threads(thread_count)\
    reduction(+: pi)
    for (i = 0; i <n ; ++i) {
        pi+=pow(-1,i)/(2*i+1);
    }
    pi*=4;
    printf("with %ld threads, we get PI = %lf\n last i is %d",thread_count,pi,i);
}

