//
// Created by 林庚 on 2021/4/23.
//
#include<stdio.h>
#include <mpi.h>
double f(double x);
double Trap(double local_a,double local_b,int n);
int main(){
    int comm_sz;
    int my_rank;
    int source;

    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);

    double a=0,b=10;
    int n=1025;
    int local_n;
    double local_a;
    double local_b;
    double size;
    if (n%comm_sz==0){
        local_n=n/comm_sz;
        size=local_n*(b-a)/n;
        local_a=a+my_rank*size;
        local_b=local_a+size;
    } else{
        if (my_rank!=0)
        {
            local_n=n/comm_sz;
            double size_of_master=(local_n+n%comm_sz)*(b-a)/n;
            size=local_n*(b-a)/n;
            local_a=a+(my_rank-1)*size;
            local_a+=size_of_master;
            local_b=local_a+size;
        }
        else{
            local_n=n/comm_sz+n%comm_sz;
            size=local_n*(b-a)/n;
            local_a=a;
            local_b=local_a+size;
        }
    }
    double sum=Trap(local_a,local_b,local_n);
    double total_sum=0;
    if (my_rank==0){
        total_sum+=sum;
        for ( source = 1; source <comm_sz ; ++source) {
        MPI_Recv(&sum,1,MPI_DOUBLE,source,1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        total_sum+=sum;
        }
        printf("%lf",total_sum);
    } else{
        MPI_Send(&sum,1,MPI_DOUBLE,0,1,MPI_COMM_WORLD);
    }


    MPI_Finalize();
    return 0;
}
double Trap(double local_a,double local_b,int local_n) {
    double h=(local_b-local_a)/local_n;
    int i;
    double sum=0.0;
    for (i = 1; i <=local_n ; ++i) {
        sum+=f(local_a+i*h)*h;
    }
    return sum;
}
double f(double x){
    return x;
}
