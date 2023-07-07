//
// Created by 林庚 on 2021/5/21.
//
#include <stdio.h>
#include <omp.h>
/*-------------------------*
 *     Local Function      *
 *-------------------------*/
void trap(double a,double b,int n);
void get_args(int argc,char* argv[]);
void Usage(char* pro_name);
double f(double x);
/*-------------------------*
 *     Global Variable     *
 *-------------------------*/
long thread_count;

/*------------------------------------------------------------------- */
int main(int argc,char* argv[]){

    get_args(argc,argv);
    double a,b;
    int n;
    printf("input your begin point and end point with format: begin-end\n");
    scanf("%lf-%lf",&a,&b);
    printf("input your n\n");
    scanf("%d",&n);
    trap(a,b,n);

    return 0;
}


/*-------------------------------------------------------------------
 * Function:        Usage
 * Purpose:         提示用户如何运行程序
 * Input args:      char* pro_name: 程序名
 * In/out args:
 */
void Usage(char* pro_name){
    fprintf(stderr,"Usage : %s <thread_count>",pro_name);
    exit(0);
}

/*-------------------------------------------------------------------
 * Function:        get_args
 * Purpose:         从命令行获取参数
 * Input args:      int argc:  命令行参数的个数  , char* argv[]:   命令行参数的内容
 * In/out args:
 */
void get_args(int argc,char* argv[]){
    if (argc!=2){
        Usage(argv[0]);
    }
    thread_count=strtol(argv[1],NULL,10);
}

/*-------------------------------------------------------------------
 * Function:        trap
 * Purpose:         计算函数f的积分
 * Input args:      double a:  范围左端 ,double b:  范围右端,int n:  分割块的数量
 * In/out args:
 */
void trap(double a,double b,int n){
    double h=(b-a)/n;
    int i;
    double my_sum=0.0;
    double sum=0.0;
    int* iteration=malloc(n*sizeof(int));
#pragma omp parallel num_threads(thread_count)\
    default(none) shared(i,h,n,iteration) private(my_sum) reduction(+: sum)
    {
#pragma omp for\
    schedule(runtime)
        for (i = 1; i <= n; ++i) {
            iteration[i]=omp_get_thread_num();
            my_sum += f(i * h) * h;
        }
        sum += my_sum;
    }
    printf("result is %lf\n",sum);
    printf("we can see the condition of thread using while iteration like this:\n");
    for (i = 0; i <n ; ++i) {
        printf("%d ",iteration[i]);
    }
}

/*-------------------------------------------------------------------
 * Function:        f
 * Purpose:         一个函数
 * Input args:      double x:  自变量
 * In/out args:
 */
double f(double x){
    return x;
}

