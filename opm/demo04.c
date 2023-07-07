//
// Created by 林庚 on 2021/5/20.
//
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "timer.h"
#include <string.h>


/*-------------------------*
 *     Local Function      *
 *-------------------------*/
void get_args(int argc,char* argv[]);
void Usage(char* pro_name);
void omp_odd_even_sort(double *arr,int n);
void omp_odd_even_sort_sch(double *arr,int n,char* command);
void normal_odd_even_sort(double *arr,int n);
void show_arr(double *arr,int n);
/*-------------------------*
 *     Global Variable     *
 *-------------------------*/
long thread_count;
/*-------------------------------------------------------------------*/
int main(int argc,char* argv[]){
    if (argc!=2){
        Usage(argv[0]);
    }
    int n;
    double *arr;
    double *a;
    double start;
    double finish;
    double avr_time=0;
    printf("input your length of array\n");
    scanf("%d",&n);
    a=malloc(n*sizeof(double ));
    arr=malloc(n*sizeof(double ));
    int i,j;
    for (i = 0; i <n ; ++i) {
        *(a+i)=(double)(rand()%(2*n));
    }

    for (j = 0; j <10 ; ++j) {

        for (i = 0; i < n; ++i) {
            *(arr + i) = *(a + i);
        }
        GET_TIME(start);
        normal_odd_even_sort(arr, n);
        GET_TIME(finish);
        avr_time+=(finish-start);
    }
    avr_time*=0.1;
    printf("without parallel , program cost %lf seconds averagely\n",avr_time);


    avr_time=0.0;
    for (j = 0; j <10 ; ++j) {

        for (i = 0; i < n; ++i) {
            *(arr + i) = *(a + i);
        }
        GET_TIME(start);
        omp_odd_even_sort(arr, n);
        GET_TIME(finish);
        avr_time+=(finish-start);
    }
    avr_time*=0.1;
    printf("without scheduler , program cost %lf seconds averagely\n",avr_time);


    avr_time=0.0;
    for (j = 0; j <10 ; ++j) {

        for (i = 0; i < n; ++i) {
            *(arr + i) = *(a + i);
        }
        GET_TIME(start);
        omp_odd_even_sort_sch(arr, n,"static");
        GET_TIME(finish);
        avr_time+=(finish-start);
    }
    avr_time*=0.1;
    printf("with scheduler static , program cost %lf seconds averagely\n",avr_time);


    avr_time=0.0;
    for (j = 0; j <10 ; ++j) {

        for (i = 0; i < n; ++i) {
            *(arr + i) = *(a + i);
        }
        GET_TIME(start);
        omp_odd_even_sort_sch(arr, n,"dynamic");
        GET_TIME(finish);
        avr_time+=(finish-start);
    }
    avr_time*=0.1;
    printf("with scheduler dynamic , program cost %lf seconds averagely\n",avr_time);


    avr_time=0.0;
    for (j = 0; j <10 ; ++j) {

        for (i = 0; i < n; ++i) {
            *(arr + i) = *(a + i);
        }
        GET_TIME(start);
        omp_odd_even_sort_sch(arr, n,"guided");
        GET_TIME(finish);
        avr_time+=(finish-start);
    }
    avr_time*=0.1;
    printf("with scheduler guided , program cost %lf seconds averagely\n",avr_time);






}


/*-------------------------------------------------------------------
 * Function:        Usage
 * Purpose:         提示用户如何使用本程序
 * Input args:      char* pro_name: 程序名
 * In/out args:
 */
void Usage(char* pro_name){
    fprintf(stderr,"Usage : %s <thread_count> \n",pro_name);
    exit(0);
}

/*-------------------------------------------------------------------
 * Function:        get_args
 * Purpose:         从命令行获取线程数两
 * Input args:      int argc: 命令行参数的数量  , char* argv[]: 命令行参数的数组
 * In/out args:
 */
void get_args(int argc,char* argv[]){
    thread_count=strtol(argv[1],NULL,10);
}

/*-------------------------------------------------------------------
 * Function:        omp_odd_even_sort
 * Purpose:         对数组进行并行奇偶排序
 * Input args:      double *arr:  要排序的数组的指针,  int n:  数组长度
 * In/out args:
 */
void omp_odd_even_sort(double *arr,int n){
    int phase;
    int i;
    double temp;
#pragma omp parallel num_threads(thread_count)\
    default(none) shared(arr,n) private(i,temp,phase)
    {
    for (phase = 0; phase < n; ++phase) {
        if (phase % 2 == 0) {
#pragma omp  for
            for (i = 0; i < n - 1; ++i) {
                if (arr[i] > arr[i + 1]) {
                    temp = arr[i];
                    *(arr + i) = *(arr + i + 1);
                    *(arr + i + 1) = temp;
                }
            }
        } else {
#pragma omp  for
            for (i = 1; i < n - 1; ++i) {
                if (arr[i] > arr[i + 1]) {
                    temp = arr[i];
                    *(arr + i) = *(arr + i + 1);
                    *(arr + i + 1) = temp;
                }
            }
        }
    }
    }

}

/*-------------------------------------------------------------------
 * Function:        show_arr
 * Purpose:         将数组打印出来
 * Input args:      double *arr:  要打印的数组的指针,  int n:  数组的长度
 * In/out args:
 */
void show_arr(double *arr,int n){
    int i;
    for (i = 0; i <n ; ++i) {
        printf("%lf ",*(arr+i));
    }
}

/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
void normal_odd_even_sort(double *arr,int n){
    int phase;
    int i;
    double temp;
    for (phase = 0; phase < n; ++phase) {
        if (phase%2==0){
            for (i = 0; i <n-1 ; ++i) {
                if (arr[i]>arr[i+1]){
                    temp=arr[i];
                    *(arr+i)=*(arr+i+1);
                    *(arr+i+1)=temp;
                }
            }
        } else{
            for (i = 1; i <n-1 ; ++i) {
                if (arr[i]>arr[i+1]){
                    temp=arr[i];
                    *(arr+i)=*(arr+i+1);
                    *(arr+i+1)=temp;
                }
            }
        }

    }
}

/*-------------------------------------------------------------------
 * Function:        omp_odd_even_sort
 * Purpose:         对数组进行并行奇偶排序
 * Input args:      double *arr:  要排序的数组的指针,  int n:  数组长度
 * In/out args:
 */
void omp_odd_even_sort_sch(double *arr,int n,char* command){
    int phase;
    int i;
    double temp;
    if (strcmp(command,"static")==0) {
#pragma omp parallel num_threads(thread_count)\
    default(none) shared(arr, n) private(i, temp, phase)
        {
            for (phase = 0; phase < n; ++phase) {
                if (phase % 2 == 0) {
#pragma omp  for\
    schedule(static, 2)
                    for (i = 0; i < n - 1; ++i) {
                        if (arr[i] > arr[i + 1]) {
                            temp = arr[i];
                            *(arr + i) = *(arr + i + 1);
                            *(arr + i + 1) = temp;
                        }
                    }
                } else {
#pragma omp  for\
    schedule(static, 2)
                    for (i = 1; i < n - 1; ++i) {
                        if (arr[i] > arr[i + 1]) {
                            temp = arr[i];
                            *(arr + i) = *(arr + i + 1);
                            *(arr + i + 1) = temp;
                        }
                    }
                }

            }
        }
    } else if (strcmp(command,"dynamic")==0){
#pragma omp parallel num_threads(thread_count)\
    default(none) shared(arr, n) private(i, temp, phase)
        {
            for (phase = 0; phase < n; ++phase) {
                if (phase % 2 == 0) {
#pragma omp  for\
    schedule(dynamic, 2)
                    for (i = 0; i < n - 1; ++i) {
                        if (arr[i] > arr[i + 1]) {
                            temp = arr[i];
                            *(arr + i) = *(arr + i + 1);
                            *(arr + i + 1) = temp;
                        }
                    }
                } else {
#pragma omp  for\
    schedule(dynamic, 2)
                    for (i = 1; i < n - 1; ++i) {
                        if (arr[i] > arr[i + 1]) {
                            temp = arr[i];
                            *(arr + i) = *(arr + i + 1);
                            *(arr + i + 1) = temp;
                        }
                    }
                }

            }
        }
    } else if (strcmp(command,"guided")==0) {
#pragma omp parallel num_threads(thread_count)\
    default(none) shared(arr, n) private(i, temp, phase)
        {
            for (phase = 0; phase < n; ++phase) {
                if (phase % 2 == 0) {
#pragma omp  for\
    schedule(guided, 2)
                    for (i = 0; i < n - 1; ++i) {
                        if (arr[i] > arr[i + 1]) {
                            temp = arr[i];
                            *(arr + i) = *(arr + i + 1);
                            *(arr + i + 1) = temp;
                        }
                    }
                } else {
#pragma omp  for\
    schedule(guided, 2)
                    for (i = 1; i < n - 1; ++i) {
                        if (arr[i] > arr[i + 1]) {
                            temp = arr[i];
                            *(arr + i) = *(arr + i + 1);
                            *(arr + i + 1) = temp;
                        }
                    }
                }

            }
        }
    }

}
