//
// Created by 林庚 on 2021/4/18.
//
#include<stdio.h>
#include<mpi.h>
double Trap(double ,double ,int,double );
double f(double );
int main(void){
    int my_rank,comm_sz,n=1024,local_n;
    double a=0.0,b=3.0,h,local_a,local_b;
    double local_int, total_int = 0;
    double start,finish;
    MPI_Init(NULL,NULL);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
    MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);

    start=MPI_Wtime();

    h=(b-a)/n;
    local_n=n/comm_sz;

    local_a=a+my_rank*local_n*h;
    local_b=local_a+local_n*h;
    local_int=Trap(local_a,local_b,local_n,h);

    if (my_rank!=0){
        MPI_Send(&local_int,1,MPI_DOUBLE,0,0,MPI_COMM_WORLD);
    } else{
        MPI_Recv(&local_int,1,MPI_DOUBLE,MPI_ANY_SOURCE,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        total_int+=local_int;
    }
    if (my_rank==0){
        printf("With n = %d trapezoids, our estimate\n",n);
        printf("of the integral from %f to %f = %.15e\n",a,b,total_int);
    }
    finish=MPI_Wtime();
    printf("thread %d cost %lf ms:",my_rank,finish-start);
    MPI_Finalize();
    return 0;
}
double Trap(double a,double b,int n,double h){
    double estimate,x;
    int i;
    estimate=(f(a)+f(b))/2.0;
    for (i = 0; i <n-1 ; i++) {
        x=a+i*h;
        estimate+=f(x);
    }
    estimate=estimate*h;
    return estimate;
}
double f(double x){
    return x;
}
