//
// Created by 林庚 on 2021/5/22.
//
#include <stdio.h>
#include <omp.h>

/*-------------------------*
 *     Local Function      *
 *-------------------------*/
void thread_work();
void multi(int *row,int n);
void sub(int *row,int *_row);
void work(int i,int j);
/*-------------------------*
 *     Global Variable     *
 *-------------------------*/
int** A;
int m;
int main(){
    printf("input number of your x\n");
    scanf("%d",&m);
    A=malloc(m*sizeof(int*));
    int i;
    int j;
    for (i = 0; i <m ; ++i) {
        *(A+i)=malloc(sizeof(int)*(m+1));
    }
    printf("input your equations set matrix and you should add a column to enter the number on the right of = \n");
    for (i = 0; i <m ; ++i) {
        for (j = 0; j <m+1 ; ++j) {
            scanf("%d",*(A+i)+j);
        }
    }
    thread_work();
    printf("calculate finish, result is \n");
    for (i = 0; i < m; ++i) {
        for (j = 0; j <m+1 ; ++j) {
                printf("%4d ",*(*(A+i)+j));
        }
        printf("\n");
    }


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
    for (i = 0; i < m-1; ++i) {
        for (j = i+1; j <m; ++j) {
           work(i,j);
        }

    }
}

/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
void multi(int* row,int n){
    int i;
    for (i = 0; i <m+1 ; ++i) {
        *(row+i)*=n;
    }

}

/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
void sub(int* row,int* _row){
    int i;
    for (i = 0; i <m+1 ; ++i) {
        *(_row+i)-=*(row+i);
    }

}

/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
void work(int i,int j){
    int temp=*(*(A+i)+i);
    multi(*(A+i),*(*(A+j)+i));
    multi(*(A+j),temp);
    sub(*(A+i),*(A+j));
}
