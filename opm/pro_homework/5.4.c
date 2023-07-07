//
// Created by 林庚 on 2021/5/22.
//
#include <stdio.h>
#include <omp.h>
/*-------------------------*
 *     Local Function      *
 *-------------------------*/
void row_work();
void col_work();
int** A;
int* x;
int* b;
int m;
int main(){

}

/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
void row_work(){
    int i;
    int j;
    for (i = m-1; i >=0; --i) {
        *(x+i)=*(b+i);
        for (j = i+1; j <m ; ++j) {
            *(x+i)-=*(x+j)**(*(A+i)+j);
        }
        *(x+i)/=*(*(A+i)+i);
    }
}

/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
void col_work(){
    int i;
    int j;
    for (i = 0; i <m ; ++i) {
        *(x+i)=*(b+i);
    }
    for (i = 0; i <m ; ++i) {
        *(x+i)/=*(*(A+i)+i);
        for (j = 0; j <i ; ++j) {
            *(x+j)-=*(x+i)**(*(A+j)+i);
        }
    }
}