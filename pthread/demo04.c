//
// Created by 林庚 on 2021/5/11.
//
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include<stdint.h>

struct link_node{
    pthread_mutex_t mutex;
    struct link_node* next;
    int data;
};

/*-------------------------*
 *     Local Function      *
 *-------------------------*/
int member(int value);
int insert(int value);
int delete(int value);

/*-------------------------*
 *     Global Variable     *
 *-------------------------*/
struct link_node* head_p;
pthread_mutex_t head_p_mutex;
int head_p_value;
int main(){


}

/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
int member(int value){
    struct link_node* temp_p;
    pthread_mutex_lock(&head_p_mutex);
    temp_p=head_p;
    while (temp_p!=NULL && temp_p->data<value){
        if (temp_p->next!=NULL){
            pthread_mutex_lock(&(temp_p->next->mutex));
        }
        if (temp_p==head_p){
            pthread_mutex_unlock(&(head_p_mutex));
        }
        pthread_mutex_unlock(&(temp_p->mutex));
        temp_p=temp_p->next;
    }

    if (temp_p == NULL || temp_p->data>value){
        if (temp_p==head_p){
            pthread_mutex_unlock(&(head_p_mutex));
        }
        if (temp_p!=NULL){
            pthread_mutex_unlock(&(temp_p->mutex));
        }
        return 0;
    } else{
        if (temp_p==head_p){
            pthread_mutex_unlock(&(head_p_mutex));
        }
        pthread_mutex_unlock(&(temp_p->mutex));
        return 1;
    }


}

/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
int insert(int value){
    struct link_node* curr_p=head_p;
    struct link_node* pre_p=NULL;
    struct link_node *tmp_p;


    while (curr_p!=NULL && curr_p->data<value){
        pre_p=curr_p;
        curr_p=curr_p->next;
    }
    if (curr_p!=NULL && curr_p->data>value) {
        tmp_p = malloc(sizeof(struct link_node));
        tmp_p->data = value;
        tmp_p->next = curr_p;
        if (pre_p == NULL) {
            head_p = tmp_p;
        } else {
            pre_p->next=tmp_p;
        }
        return 1;
    } else{
        return 0;
    }
}

/*-------------------------------------------------------------------
 * Function:
 * Purpose:
 * Input args:
 * In/out args:
 */
int delete(int value){
    struct link_node* curr_p=head_p;
    struct link_node* pre_p=NULL;

    while (curr_p!=NULL && curr_p->data<value){
        pre_p=curr_p;
        curr_p=curr_p->next;
    }

    if (curr_p!=NULL && curr_p->data ==value){
        if (pre_p==NULL){
            head_p=curr_p->next;
            free(curr_p);
        } else {
            curr_p = curr_p->next;
            pre_p->next = curr_p;
            free(curr_p);
        }
        return 1;
    } else{
        return 0;
    }
}