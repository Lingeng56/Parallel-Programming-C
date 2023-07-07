//
// Created by 林庚 on 2021/5/22.
//
#include <stdio.h>
#include <omp.h>
#include "timer.h"
#include <string.h>

/*-------------------------*
 *     Local Function      *
 *-------------------------*/
void get_args(int argc,char* argv[]);
void Usage(char* pro_name);
void para_count_sort();
void count_sort();
int comp(const void*a, const void*b) {
    return *(int*)a-*(int*)b;
}
/*-------------------------*
 *     Global Variable     *
 *-------------------------*/
long thread_count;
double start,finish,elapsed;
int n;
int* arr;
int* arr2;
/*-------------------------------------------------------------------*/
int main(int argc,char* argv[]){
    get_args(argc,argv);
    GET_TIME(start)
    para_count_sort(arr,n);
    GET_TIME(finish)
    elapsed=finish-start;
    printf("with %ld threads,it cost %lf seconds to sort\n",thread_count,elapsed);
    GET_TIME(start)
    count_sort();
    GET_TIME(finish)
    elapsed=finish-start;
    printf("with one thread,it cost %lf seconds to sort\n",elapsed);
    free(arr);
    free(arr2);
}

/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
void Usage(char* pro_name){
    fprintf(stderr,"Usage : %s \n OR \n Usage : %s <thread_count>\n",pro_name,pro_name);
    exit(0);
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
    } else if (argc==2){
        thread_count=strtol(argv[1],NULL,10);
    }else{
        printf("input your thread_count\n");
        scanf("%ld",&thread_count);
    }
    printf("input your length of arr\n");
    scanf("%d",&n);
    arr=malloc(n*sizeof(int));
    arr2=malloc(n*sizeof(int));
    for (int i = 0; i < n; ++i) {
        *(arr+i)=rand();
        *(arr2+i)=*(arr+i);
    }

}

/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
void count_sort(){
    int* temp=malloc(n*sizeof(int));
    int i;
    int j;
        for (i = 0; i < n; ++i) {
            int count = 0;
            for (j = 0; j < n; ++j) {
                if (arr2[j] < arr2[i] || (arr2[j] == arr2[i] && j < i)) {
                    count++;
                }
            }
            *(temp + count) = *(arr2 + i);
        }
    memcpy(arr2,temp,n);
    free(temp);
}
/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
void para_count_sort(){
    int* temp=malloc(n*sizeof(int));
    int i;
    int j;
#pragma omp parallel num_threads(thread_count)\
    default(none) shared(i,j,n,temp,arr)
    {
#pragma omp for
        for (i = 0; i < n; ++i) {
            int count = 0;
            for (j = 0; j < n; ++j) {
                if (arr[j] < arr[i] || (arr[j] == arr[i] && j < i)) {
                    count++;
                }
            }
            *(temp + count) = *(arr + i);
        }
#pragma omp for
        for (i = 0; i < n; ++i) {
            *(arr+i)=*(temp+i);
        }
    }
    free(temp);
}
