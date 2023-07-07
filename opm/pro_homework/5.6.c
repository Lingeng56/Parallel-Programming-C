//
// Created by 林庚 on 2021/5/22.
//
#include <stdio.h>
#include <omp.h>
#include <string.h>
#include <stdbool.h>
#define MAX_FILE_NAME 1024
#define MAX_MESSAGE_LEN 100
/*-------------------------*
 *    Local Data Struct    *
 *-------------------------*/
struct task{
    struct task* next;
    long thread_id;
    char* message;
};
/*-------------------------*
 *     Local Function      *
 *-------------------------*/
void read_task();
void work();
void Usage(char* pro_name);
void get_args(int argc,char* argv[]);
/*      function of struct      */
void enqueue(long thread_id,char* message);
void dequeue();
bool isEmpty();
void finish_memory();
/*-------------------------*
 *     Global Variable     *
 *-------------------------*/
struct task* first_task;
struct task* last_task;
FILE **files;
int in_queue=0;
int finish=0;
long thread_count;
int work_thread;
int num_task=0;
/*-------------------------------------------------------------------*/
int main(int argc,char* argv[]){
    get_args(argc,argv);
    read_task();
    work();
}

/*-------------------------------------------------------------------
 * Function:        read_task
 * Purpose:         获取任务
 * Input args:
 * In/out args:
 */
void read_task(){
    char* file_name=malloc(MAX_FILE_NAME*sizeof(char));
    files=malloc(1024*sizeof(FILE*));
    printf("input your file_name <one name one line>:\n (enter EOF to finish input) \n");
    int i=0;
        while (gets(file_name) && strcmp(file_name,"EOF")!=0) {
            FILE *fp = NULL;
            fp=fopen(file_name, "r");
            if (fp == NULL) {
                fprintf(stderr, "File: %s not found\n", file_name);
                fprintf(stderr, "Quitting....\n");
                exit(0);
            }
            printf("input %s successfully...\n",file_name);
            fflush(stdout);


            num_task++;
            *(files + i) = fp;
            i++;
        }
        printf("%d tasks enter the queue\n",i);

}

/*-------------------------------------------------------------------
 * Function:        get_args
 * Purpose:         获取参数，得到线程数
 * Input args:      int argc:   命令行参数的个数    char* argv[]:   命令行参数的内容
 * In/out args:
 */
void get_args(int argc,char* argv[]){
    if (argc>2){
        Usage(argv[0]);
    } else if (argc==2){
        thread_count=strtol(argv[1],NULL,10);
    }else {
        printf("input your thread_count\n");
        scanf("%ld", &thread_count);
    }
}

/*-------------------------------------------------------------------
 * Function:        Usage
 * Purpose:         提示用户如何运行本程序
 * Input args:      char* pro_name: 程序的名字
 * In/out args:
 */
void Usage(char* pro_name){
    fprintf(stderr,"Usage : %s OR %s <thread_count>\n",pro_name,pro_name);
    exit(0);
}
/*-------------------------------------------------------------------
 * Function:        isEmpty
 * Purpose:         检查任务队列是否为空
 * Input args:
 * In/out args:
 */
bool isEmpty(){
    return in_queue==0;
}

/*-------------------------------------------------------------------
 * Function:        enqueue
 * Purpose:         将任务加进队列
 * Input args:      long thread_id:     进行加入操作的进程号  char* message:      任务携带的信息
 * In/out args:
 */
void enqueue(long thread_id,char* message){
    int length_of_message=(int)strlen(message)+1;
    struct task* new_task=malloc(length_of_message*sizeof(char)+sizeof(long)+sizeof(struct task*));
    new_task->message=message;
    new_task->next=NULL;
    new_task->thread_id=thread_id;
    if (isEmpty()){
        first_task=new_task;
        last_task=new_task;
    }
    last_task->next=new_task;
    last_task=new_task;
    in_queue++;
    printf("thread %ld adds new task...\n",thread_id);
}

/*-------------------------------------------------------------------
 * Function:        dequeue
 * Purpose:         将队列头部的任务移出队列
 * Input args:
 * In/out args:
 */
void dequeue(){
    if (isEmpty()){
        fprintf(stderr,"Queue is empty ...\n");
        exit(0);
    }
    struct task* old_first=first_task;
    struct task* new_first_task=first_task->next;
    first_task=new_first_task;
    in_queue--;
    free(old_first);
}

/*-------------------------------------------------------------------
 * Function:        work
 * Purpose:         线程在这个函数进行添加任务，结束任务的工作
 * Input args:
 * In/out args:
 */
void work(){
    printf("begin to work\n");
    int i;
#pragma omp parallel num_threads(thread_count)
    {
#pragma omp for
        for (i = 0; i < num_task; ++i) {
            int my_rank = omp_get_thread_num();
            FILE *f = files[i];
            char *message = malloc(MAX_MESSAGE_LEN * sizeof(char));
            fgets(message, MAX_MESSAGE_LEN, f);
#pragma omp critical(two)
            {
            enqueue(my_rank, message);
            }
        }
#pragma omp for
        for (i = 0; i < num_task; ++i) {
            int my_rank = omp_get_thread_num();
            struct task *task = first_task;
#pragma omp critical(one)
            dequeue();
            char *message = task->message;
            printf("thread %d finish the message: %s\n", my_rank, message);
        }
    }
}

/*-------------------------------------------------------------------
 * Function:        finish_memory
 * Purpose:         清空内存
 * Input args:
 * In/out args:
 */
void finish_memory(){
    int i;
    for (i = 0; i <num_task ; ++i) {
        fclose(files[i]);
    }
}