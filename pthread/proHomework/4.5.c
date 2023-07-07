//
// Created by 林庚 on 2021/5/18.
//

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "timer.h"
/*-------------------------*
 *    Local Data Struct    *
 *-------------------------*/

struct Queue_task{
    char* command;
    struct Queue_task* next;
    long value;
};

/*      for condition we regard 1 as busy and 0 as free     */
struct thread{
    pthread_t thread_handle;
    bool condition;
    struct thread* next;
    long rank;
};

struct List_node{
    long value;
    struct List_node* next;
    struct List_node* pre;
};



/*-------------------------*
 *     Local Function      *
 *-------------------------*/
void allocate_task(struct thread* thread,void* fun(void*),void* args);
void working();
void Usage(char* pro_name);
void enqueue(char* command,long value);
struct Queue_task* dequeue();
void* insert(void* value);
bool isEmptyList();
bool isEmptyQueue();
void* delete(void* value);
void* member(void* value);
struct thread* find_available();
void thread_init(struct thread* t,long i);
void start_all_work();
void create_thread();
void show_list();

/*-------------------------*
 *     Global Variable     *
 *-------------------------*/
long n=0;
long task_num=0;
long thread_count;
char** task_list;
struct List_node* head_node;
struct Queue_task* head_task;
struct Queue_task* last_task;
pthread_t* pthread_handles;
struct thread *head_thread;
pthread_mutex_t mutex;
bool member_flag=false;
bool delete_flag=false;

/*-----------------------------------------------*/
int main(int argc,char* argv[]) {
    char command[1024];
    task_list=malloc(1024*sizeof(char*));
    pthread_mutex_init(&mutex,NULL);

    printf("Dear Master,please input number of threads you want to start.\n");
    scanf("%ld",&thread_count);

    pthread_handles=malloc(thread_count*sizeof(pthread_t));
    create_thread();
    printf("ok,enter yes then cores will begin to work...\notherwise system will exit\n");
    scanf("%s",command);
    if (strcmp("yes",command)==0){
        printf("begin to work...\n");
        working();
    }
    printf("Dear Master,wish you a happy life.\nGood Bye!\n");



    free(task_list);
    free(pthread_handles);
}

/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
void working(){
    long i;
    char command[10];
    long value;

    while (1){
        printf("/*------------------------------------------------*/\n");
        printf("command list:\ninput 'insert' to enqueue the task<insert> \ninput 'delete' to enqueue the task<delete> \ninput 'member' to enqueue the task<member> \ninput 'start' to enqueue the start all tasks \ninput 'quit' to exit \ninput your command:\n");
        scanf("%s",command);
        if (strcmp("quit",command)==0){
            break;
        }
        else if (strcmp("insert",command)==0){
            printf("input the value you want to insert:\n");
            scanf("%ld",&value);
            *(task_list+task_num)=malloc(20*sizeof(char));
            sprintf(*(task_list+task_num),"*task%ld----------%s",task_num,command);
            printf("task has been added successfully\n\ntask list:\n");
            for (i = 0; i <=task_num ; ++i) {
                printf("%s\n",*(task_list+i));
            }
            printf("\n");
            enqueue("insert",value);
        }
        else if (strcmp("delete",command)==0){
            printf("input the value you want to delete:\n");
            scanf("%ld",&value);
            *(task_list+task_num)=malloc(20*sizeof(char));
            sprintf(*(task_list+task_num),"*task%ld----------%s",task_num,command);
            printf("task has been added successfully\n\ntask list:\n");
            for (i = 0; i <=task_num ; ++i) {
                printf("%s\n",*(task_list+i));
            }
            printf("\n");
            enqueue("delete",value);
        }
        else if (strcmp("member",command)==0){
            printf("input the value you want to member:\n");
            scanf("%ld",&value);
            *(task_list+task_num)=malloc(20*sizeof(char));
            sprintf(*(task_list+task_num),"*task%ld----------%s",task_num,command);
            printf("task has been added successfully\n\ntask list:\n");
            for (i = 0; i <=task_num ; ++i) {
                printf("%s\n",*(task_list+i));
            }
            printf("\n");
            enqueue("member",value);
        }
        else if (strcmp("start",command)==0){
            printf("begin to work...\n");
            start_all_work();
            show_list();
        }
        else if (strcmp("show",command)==0){
            *(task_list+task_num)=malloc(20*sizeof(char));
            sprintf(*(task_list+task_num),"*task%ld----------%s",task_num,command);
            printf("task has been added successfully\n\ntask list:\n");
            for (i = 0; i <=task_num ; ++i) {
                printf("%s\n",*(task_list+i));
            }
            printf("\n");
            enqueue("show",value);
        }
        else{
            printf("please input valid command.\n");
        }
    }
}

/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
bool isEmptyList(){
    return n==0;
}

/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
bool isEmptyQueue(){
    return task_num==0;
}

/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
void allocate_task(struct thread* thread,void* fun(void*),void* args){
    thread->condition=1;
    pthread_create(&thread->thread_handle,NULL,fun,args);
    pthread_join(thread->thread_handle,NULL);
    thread->condition=0;
}

/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
void Usage(char* pro_name){
    fprintf(stderr,"Usage : %s ",pro_name);
    exit(1);
}


/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
void enqueue(char* command,long value){
    if (isEmptyQueue()){
    head_task=malloc(sizeof(long)+10*sizeof(char)+sizeof(struct Queue_task*));
    head_task->command=command;
    head_task->value=value;
    head_task->next=NULL;
    last_task=head_task;
    } else{
        struct Queue_task* current_task=malloc(sizeof(long)+10*sizeof(char)+sizeof(struct Queue_task*));
        current_task->command=command;
        current_task->value=value;
        last_task->next=current_task;
        current_task->next=NULL;
        last_task=current_task;
    }
    task_num++;
}

/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
struct Queue_task* dequeue(){
    if (!isEmptyQueue()){
        struct Queue_task* result_task=head_task;
        head_task=head_task->next;
        task_num--;
        return result_task;
    }
    return NULL;
}


/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
void* delete(void* value){
    pthread_mutex_lock(&mutex);
    struct List_node* current_node=head_node;
    int i;
    bool flag=false;
    for (i = 0; i <n ; ++i) {
        if (current_node->value==(long)value){
            flag=true;
            break;
        }
        current_node=current_node->next;
    }
    if (flag) {
        current_node->pre->next = current_node->next;
        delete_flag=true;
        free(current_node);
        flag=false;
    }
    pthread_mutex_unlock(&mutex);
    return NULL;
}

/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
void* insert(void* value){
    pthread_mutex_lock(&mutex);
    head_node=malloc(sizeof(long)+2*sizeof(struct List_node*));
    struct List_node* current_node=head_node;
    struct List_node* new_node=malloc(sizeof(long)+2*sizeof(struct List_node*));
    new_node->value=(long)value;
    int i;
    bool flag=false;
    if (isEmptyList()){
        head_node=new_node;
        head_node->next=NULL;
        head_node->pre=NULL;
    } else {
        for (i = 0; i < n; ++i) {
            if (current_node->value >= (long) value) {
                flag=true;
                break;
            }
            current_node = current_node->next;
        }
        if (flag) {
            new_node->pre = current_node->pre;
            new_node->next = current_node;
            current_node->pre = new_node;
            if (current_node == head_node) {
                head_node = new_node;
            }
            flag=false;
        }
        else{
            current_node=new_node;
            new_node->pre=current_node->pre;
            new_node->next=NULL;
        }
    }
    n++;
    pthread_mutex_unlock(&mutex);
    return NULL;
}


/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
void* member(void* value){
    pthread_mutex_lock(&mutex);
    struct List_node* current_node=head_node;
    int i;
    bool flag=false;
    for (i = 0; i <n ; ++i) {
        if (current_node->value==(long)value){
            flag=true;
            break;
        }
        current_node=current_node->next;
    }
    if (flag){
        member_flag=true;
        flag=false;
    }
    pthread_mutex_unlock(&mutex);
    return NULL;
}

/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
struct thread* find_available(){
    struct thread* current_thread=head_thread;
    long i;
    bool flag=false;
    for (i = 0; i <thread_count ; ++i) {
        if (current_thread->condition==0){
            flag=true;
            break;
        }
        current_thread=current_thread->next;
    }
    if (flag) {
        flag=false;
        return current_thread;
    } else{
        return NULL;
    }
}

/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
void thread_init(struct thread* t,long i){
    t->thread_handle=pthread_handles[i];
    t->condition=0;
}

/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
void start_all_work(){
    printf("enter start function successfully...\n");
    while (!isEmptyQueue()){
        struct Queue_task* current_task=dequeue();
        if (strcmp(current_task->command,"insert")==0){
            struct thread* t=find_available();
            if (t!=NULL) {
                allocate_task(t, insert, (void *) current_task->value);
                printf("thread %ld insert %ld successfully...\n",t->rank,current_task->value);
            }
        }
        else if (strcmp(current_task->command,"delete")==0){
            struct thread* t=find_available();
            if (t!=NULL) {
                allocate_task(t, delete, (void *) current_task->value);
                if (delete_flag) {
                    printf("thread %ld delete %ld successfully...\n", t->rank, current_task->value);
                } else{
                    printf("there is no node's value is %ld\n",current_task->value);
                }
            }
        }
        else if (strcmp(current_task->command,"member")==0){
            struct thread* t=find_available();
            if (t!=NULL) {
                allocate_task(t, member, (void *) current_task->value);
                if (member_flag) {
                    printf("thread %ld member %ld successfully...\n", t->rank, current_task->value);
                }else{
                    printf("there is no node's value is %ld\n",current_task->value);
                }
            }
        }
    }
}

/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
void create_thread(){
    long i;
    /*      create first thread     */
    head_thread = malloc(sizeof(long)+sizeof(pthread_t)+sizeof(bool)+2*sizeof(struct thread*));
    thread_init(head_thread,0);
    struct thread *current_thread = head_thread;
    head_thread->rank=0;

    /*      create other threads    */
    for (i = 1; i <thread_count ; ++i) {
        struct thread *new_thread =malloc(sizeof(pthread_t)+sizeof(bool)+2*sizeof(struct thread*));
        thread_init(new_thread,i);
        new_thread->rank=i;
        current_thread->next=new_thread;
        current_thread=new_thread;
    }
    current_thread->next=NULL;


    printf("\n");
    printf("\n");
}


/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
void show_list(){
    int i;
    struct List_node* current_node=head_node;
    for (i = 0; i <n ; ++i) {
        printf("%ld ",current_node->value);
        current_node=current_node->next;
    }
    printf("\n");
}

