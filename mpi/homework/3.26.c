//
// Created by 林庚 on 2021/4/27.
//
#include <stdio.h>
/*-------------------------*
 *     Local Function      *
 *-------------------------*/
int isSorted(double [],int );
void oddEvenSort(double [],int);
/*-------------------------*
 *     Global Variable     *
 *-------------------------*/
int comm_sz;
int my_rank;
int main(){
    int i;
    double a[10]={10,9,8,7,6,5,4,3,2,1};
    oddEvenSort(a,10);
    for (i = 0; i <10 ; ++i) {
        printf("%2.lf ",*(a+i));
    }
    return 0;

}
/*-------------------------------------------------------------------
 * Function:    isSorted
 * Purpose:     check if the sequence has been sorted
 * Input args:      double a[],int length
 * In/out args:
 */
int isSorted(double *a,int length){
    int flag=1;
    int i;
    for (i = 0; i <length-1 ; ++i) {
        if (a[i]<a[i+1]){
            flag=0;
            break;
        }
    }
    return flag;
}
/*-------------------------------------------------------------------
 * Function:    oddEvenSort
 * Purpose:     sort the sequence
 * Input args:  double a[],int n
 * In/out args:
 */
void oddEvenSort(double a[],int n){
    int phase,i;
    double temp;
    int index=1;
    for (phase = 0; phase <n ; ++phase) {
        if (phase % 2 == 0) {
            for ( i=1;i < n; i += 2) {
                if (a[i] < a[i - 1]) {
                    temp = a[i];
                    a[i] = a[i - 1];
                    a[i - 1] = temp;
                }
            }
        } else {
            for (i=1 ;i < n - 1; i += 2) {
                if (a[i] > a[i + 1]) {
                    temp = a[i];
                    a[i] = a[i + 1];
                    a[i + 1] = temp;
                }
            }
        }
    }


}
