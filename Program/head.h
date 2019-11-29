#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#define n 5
using namespace std;

typedef struct PCB{
	char name;		 //������ 
	int arriveTime;  //����ʱ�� 
	int serverTime;  //����ʱ�� 
	int runTime;	 //������ʱ�� 
	int startTime;   //��ʼʱ�� 
	int finishTime;  //���ʱ��
	int restTime;    //ʣ�����ʱ�� 
	int priority;	 //���ȼ� 
} PCB;

//�������� 
typedef struct Node{
	PCB data;
	struct Node *next;
}Node;

typedef struct {
    Node *head;
    Node *tail;
    int size;
}linklist, *linkList;

//��ʼ�� 
void linkListInit(linkList &plinklist)
{
	plinklist = (linkList)malloc(sizeof(linklist)*1);
    plinklist->head = NULL;
    plinklist->tail = NULL;
    plinklist->size = 0;    

}

//���� 
void linkListDestroy(linkList plinklist)
{
	Node *cur = NULL;
	Node *del = NULL;
	if (plinklist->head == NULL) {
		return;
	}
	cur = plinklist->head;
	while (cur != NULL) {
		del = cur;
		cur = cur->next;
		free(del);
		del = NULL;
	}
}
Node *CreatNode(PCB data)
{
	Node *ret = (Node *)malloc(sizeof(Node));
	if (ret == NULL) {
		exit(0);
	}
	ret->data = data;
	ret->next = NULL;
	return ret;
}
//��� 
void linkListPush(linkList plinklist, PCB data)
{
	Node *temp = CreatNode(data);
	if (plinklist->head == NULL) {
		plinklist->head = temp;
		plinklist->tail = temp;
		++(plinklist->size);
	}
	else {
		plinklist->tail->next = temp;
		plinklist->tail = temp;
		++(plinklist->size);
	}
}


//���� 
void linkListPop(linkList plinklist, Node * pnode)
{
	Node *cur = NULL;
	Node *pre = NULL;
	cur = plinklist->head;
	pre = plinklist->head;
	if ((plinklist->head) == NULL) {
		printf("����Ϊ��");
		return;
	}
    if (plinklist->head == pnode) {
    	//cout << "ͷ" << endl;
    	//��βָ��任 
    	if(pnode == plinklist->tail){
			plinklist->tail = NULL;
			plinklist->head = NULL;
		}else{
			plinklist->head = pnode->next;
		}
    		
        
//      free(pnode);
        //pnode = NULL;
        --(plinklist->size);
        return;
    }else
	    while(cur != NULL) {
	        if (cur == pnode) {
	        	//��βָ��任 
	        	//cout << "�ҵ�" << endl; 
	        	if(pnode == plinklist->tail){
					plinklist->tail = pre;
					pre->next = NULL; 
				}
	        	else{
	            	pre->next = pnode->next;
	            }
	          //  free(pnode);
	           // pnode = NULL;
	            --(plinklist->size);
	            break;
	        }
	        pre = cur;
	        cur = cur->next;
	    } 
}

//��ӡ���� 
void print(linkList p){
	Node *q = NULL;
	q = p->head;
	while(q != NULL){
		cout << q->data.name << "\t";
		q = q->next; 
	}
	cout << endl;
} 

//�ж����ִ�ӡ����
void select(int f, linkList p){
	if(1 == f){
		//�������� 
		cout << "************************************" << endl;
		cout << "\t�������н��̣�";
		print(p); 
	}else if(0 == f){
		//��ɶ���
		cout << "\t��ɶ��н��̣�";
		print(p); 
		cout << "************************************" << endl;
	}
}
 
//�ж϶ӿ� 
int islinkListEmpty(linkList plinkList)
{
	if(plinkList->head == NULL)
		return 1;
	return 0;
}

//���ض��д�С 
int linkListSize(linkList plinklist)
{
    return plinklist->size;
}
