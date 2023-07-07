//
// Created by 林庚 on 2021/5/22.
//
#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include "my_rand.c"
#include "timer.h"
/*-------------------------*
 *     Local Function      *
 *-------------------------*/
void get_args(int argc,char* argv[]);
void Usage(char* pro_name);
void calculate_pi();
/*-------------------------*
 *     Global Variable     *
 *-------------------------*/
long thread_count;

/*------------------------------------------------------------------- */
int main(int argc,char* argv[]){
    get_args(argc,argv);
    calculate_pi();
    return 0;
}


/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
void get_args(int argc,char* argv[]){
    if (argc>2){
        Usage(argv[0]);
    }
    else if (argc==2){
        thread_count=strtol(argv[1],NULL,10);
    } else{
        printf("please input your thread_count\n");
        scanf("%ld",&thread_count);
    }
}

/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
void Usage(char* pro_name){
    fprintf(stderr,"you can run the program by:\n Usage : %s OR Usage: %s <thread_count>\n",pro_name,pro_name);
    exit(0);

}

/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
void calculate_pi(){
    int i;
    long long int my_in_count=0;
    long long int in_count=0;
    long long int n=2000000000;
    double start,finish;
    GET_TIME(start);
#pragma omp parallel num_threads(thread_count)\
    default(none) shared(i,n) private(my_in_count) reduction(+:in_count)
    {
        int my_rank=omp_get_thread_num();
        unsigned  seed=my_rank+1;
#pragma omp for
        for (i = 0; i <n ; ++i) {
            double x=my_drand(&seed)-1;
            double y=my_drand(&seed)-1;
            if((x*x+y*y)<=1){
                my_in_count++;
            }
        }
        in_count+=my_in_count;
        printf("thread %d get %lld \n",my_rank,my_in_count);

    }
    GET_TIME(finish);
    double pi=(4.0*in_count)/(1.0*n);
    printf("calculate pi is %lf ,cost %lf seconds\n",pi,finish-start);

}

