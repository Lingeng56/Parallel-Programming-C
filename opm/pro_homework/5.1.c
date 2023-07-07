//
// Created by 林庚 on 2021/5/21.
//
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

/*-------------------------*
 *     Local Function      *
 *-------------------------*/
void Usage(char* pro_name);
void get_args(int argc,char* argv[]);
void get_data();
void thread_work();
void print_result();
/*-------------------------*
 *     Global Variable     *
 *-------------------------*/
long thread_count;
int bin_count;
double* data;
int num;
int* bin;
/*-------------------------------------------------------------------*/
int main(int argc,char* argv[]){
    get_args(argc,argv);
    get_data();
    thread_work();
    print_result();

}

/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
void get_data(){
    int i;

    printf("input your upper bound of data\n");
    scanf("%d",&bin_count);

    printf("input the amount of number you prepare to input\n");
    scanf("%d",&num);

    bin=malloc(bin_count*sizeof(int));
    for (i = 0; i < bin_count; ++i) {
        *(bin+i)=0;
    }

    data=malloc(num*sizeof(double));
    printf("input your data ,the amount of data is %d",num);
    for (i = 0; i < num; ++i) {
        scanf("%lf",data+i);
    }

}

/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
void Usage(char* pro_name){
    fprintf(stderr,"Usage : %s <thread_count> ",pro_name);
    exit(0);
}

/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
void get_args(int argc,char* argv[]){
    if (argc!=2){
        Usage(argv[0]);
    }
    thread_count=strtol(argv[1],NULL,10);
}

/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
void thread_work(){
    int i;
    int j;
    double temp;
#pragma omp parallel num_threads(thread_count)\
    default(none) shared(num,data,bin,bin_count) private(i,j,temp)
    {
#pragma omp for
        for (i = 0; i <num ; ++i) {
            temp=*(data+i);
            for (j = 0; j <bin_count ; ++j) {
                if (temp<j+1){
                    *(bin+j)+=1;
                    break;
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
void print_result(){
    int i;
    for (i = 0; i <bin_count ; ++i) {
        printf("in bin %d ,there is %d element\n",i+1,*(bin+i));
    }
}