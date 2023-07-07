//
// Created by 林庚 on 2021/4/26.
//
#include <stdio.h>
#include <mpi.h>
#define PACK_BUF_SZ 100
/*-------------------------*
 *     Local Function      *
 *-------------------------*/
double f(double x);
double trap(double ,double ,double );
void get_input(double *,double *,int *);
/*-------------------------*
 *     Global Variable     *
 *-------------------------*/
int comm_sz;
int my_rank;
int main(){
    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);

    double a,b,local_a,local_b,h,local_sum=0,total_sum=0,start,finish;
    int n,local_n;
    start=MPI_Wtime();
    get_input(&a,&b,&n);
    h=(b-a)/n;
    local_n=n/comm_sz;
    local_a=a+local_n*my_rank*h;
    local_b=local_a+local_n*h;
    local_sum=trap(local_a,local_b,local_n);

    printf("in thread %d ,local_a is %lf ,local_b is %lf,local_n is %d.local_result is %lf\n",my_rank,local_a,local_b,local_n,local_sum);


    MPI_Barrier(MPI_COMM_WORLD);
    if (my_rank==0){
        total_sum+=local_sum;
        int i;
        for (i = 1; i <comm_sz ; ++i) {
            MPI_Recv(&local_sum,1,MPI_DOUBLE,i,1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            total_sum+=local_sum;
        }
        printf("result is: %lf\n",total_sum);
        finish=MPI_Wtime();
        printf("program cost %lf ms\n",finish-start);
    } else{
        printf("receive the part_result: %lf from thread %d successfully:\n",local_sum,my_rank);
        MPI_Send(&local_sum,1,MPI_DOUBLE,0,1,MPI_COMM_WORLD);
    }



    MPI_Finalize();
}
/*-------------------------------------------------------------------
 * Function:        get_input
 * Purpose:         get a,n,b from thread 0,and send local_sum to thread 0
 * Input args:      none
 * In/out args:     position,a,b,n
 */
void get_input( double *a, double *b, int *n){
    char pack_buf[100];
    int position=0;

    if (my_rank==0){
        printf("input left:\n");
        scanf("%lf",a);
        printf("input right:\n");
        scanf("%lf",b);
        printf("input size of split:\n");
        scanf("%d",n);
        MPI_Pack(a,1,MPI_DOUBLE,pack_buf,100,&position,MPI_COMM_WORLD);
        MPI_Pack(b,1,MPI_DOUBLE,pack_buf,100,&position,MPI_COMM_WORLD);
        MPI_Pack(n,1,MPI_DOUBLE,pack_buf,100,&position,MPI_COMM_WORLD);
    }
    MPI_Bcast(pack_buf,100,MPI_PACKED,0,MPI_COMM_WORLD);
    if (my_rank!=0){
        MPI_Unpack(pack_buf, 100, &position, a, 1, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(pack_buf, 100, &position, b, 1, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(pack_buf, 100, &position, n, 1, MPI_DOUBLE, MPI_COMM_WORLD);
    }
}

/*-------------------------------------------------------------------
 * Function:        trap
 * Purpose:         calculate integral
 * Input args:      local_a,local_b,local_n
 * In/out args:     sum of integral
 */
double trap(double local_a,double local_b,double local_n){
    double h=(local_b-local_a)/local_n;
    double sum=0;
    int i;

    for (i = 0; i < local_n; ++i) {
        sum+=f(local_a+(i+1)*h)*h;
    }


    return sum;
}

/*-------------------------------------------------------------------
 * Function:        f
 * Purpose:     original function
 * Input args:      x
 * In/out args:     y
 */
double f(double x){
    return x;
}