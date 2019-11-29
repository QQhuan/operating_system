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
	char name;		 //进程名 
	int arriveTime;  //到达时间 
	int serverTime;  //服务时长 
	int runTime;	 //已运行时间 
	int startTime;   //开始时间 
	int finishTime;  //完成时间
	int restTime;    //剩余服务时长 
	int priority;	 //优先级 
} PCB;

//进程链表 
typedef struct Node{
	PCB data;
	struct Node *next;
}Node;

typedef struct {
    Node *head;
    Node *tail;
    int size;
}linklist, *linkList;

//初始化 
void linkListInit(linkList &plinklist)
{
	plinklist = (linkList)malloc(sizeof(linklist)*1);
    plinklist->head = NULL;
    plinklist->tail = NULL;
    plinklist->size = 0;    

}

//销毁 
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
//入队 
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


//出队 
void linkListPop(linkList plinklist, Node * pnode)
{
	Node *cur = NULL;
	Node *pre = NULL;
	cur = plinklist->head;
	pre = plinklist->head;
	if ((plinklist->head) == NULL) {
		printf("队列为空");
		return;
	}
    if (plinklist->head == pnode) {
    	//cout << "头" << endl;
    	//队尾指针变换 
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
	        	//队尾指针变换 
	        	//cout << "找到" << endl; 
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

//打印队列 
void print(linkList p){
	Node *q = NULL;
	q = p->head;
	while(q != NULL){
		cout << q->data.name << "\t";
		q = q->next; 
	}
	cout << endl;
} 

//判断区分打印队列
void select(int f, linkList p){
	if(1 == f){
		//就绪队列 
		cout << "************************************" << endl;
		cout << "\t就绪队列进程：";
		print(p); 
	}else if(0 == f){
		//完成队列
		cout << "\t完成队列进程：";
		print(p); 
		cout << "************************************" << endl;
	}
}
 
//判断队空 
int islinkListEmpty(linkList plinkList)
{
	if(plinkList->head == NULL)
		return 1;
	return 0;
}

//返回队列大小 
int linkListSize(linkList plinklist)
{
    return plinklist->size;
}
