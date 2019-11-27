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
    	
    	//队尾指针变换 
    	if(pnode == plinklist->tail)
    		plinklist->tail = pnode->next;
        plinklist->head = pnode->next;
        free(pnode);
        pnode = NULL;
        --(plinklist->size);
        return;
    }
    while(cur != NULL) {
        if (cur->next == pnode) {
        	
        	//队尾指针变换 
        	if(pnode == plinklist->tail)
        		plinklist->tail = pre;
            cur->next = pnode->next;
            free(pnode);
            pnode = NULL;
            --(plinklist->size);
            break;
        }
        pre = cur;
        cur = cur->next;
    } 
}
void print(linkList p){
	Node *q = NULL;
	q = p->head;
	while(q != NULL){
		cout << q->data.name << "-_-";
		q = q->next; 
	}
	cout << endl;
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
//短进程优先调度 
void SJF(PCB p[], int num){
	int time = 0;
	int newPcb = 0; 
	int processNum = num;
	Node *running= NULL;  // 代表正在运行的进程 
	Node *mid = NULL; 
	linkList queue;       //就绪队列 
	linkListInit(queue);
	while(1){
		for(int i = 0;i < num; i++){
			if(p[i].arriveTime == time){  //模拟每一秒的时间，检查是否有进程到达， 到达的进程加入就绪队列 
				linkListPush(queue, p[i]); 
				newPcb = 1;
				cout << time << "时刻：进程" << p[i].name << "到达" << endl; 
				processNum--; 
			}
		}
		print(queue);
		//if(running != NULL)
	    //	cout << time <<"时刻：" << running->data.name << endl; 
		if(islinkListEmpty(queue) && processNum == 0){
			cout << "进程调度完毕" << endl;
			break; 
		}
		
		//正在运行的进程运行结束 
		if (running != NULL && running->data.restTime == 0) {
            cout << time << "时刻: 进程" << running->data.name << "结束" << endl; 
            linkListPop(queue, running);
//            cout << "队列:" << queue->head->data.name << endl;
            running = NULL;
        } 
		//首次运行或者某进程运行已结束 
		if(!islinkListEmpty(queue) && running == NULL){
			mid = queue->head;
			while(mid != NULL){
				//处理机空闲 , 从队列中选出一个服务时间最短的 
				if(running == NULL && queue->size > 1 && mid->data.serverTime < mid->next->data.serverTime) 
					running = mid;
				else if(running == NULL && queue->size > 1 && mid->data.serverTime > mid->next->data.serverTime)
					running = mid->next;
				//首次运行 
				else if(running == NULL && queue->size == 1)
					running = mid;
				//找出队列中的最短进程，获得处理机
				//当队列中存在两个相同时间的进程时，排在前面的那一个获得处理机
				//当正在运行的进程剩余服务时间跟队列中最小服务时长进程相等时，不作进程切换 
				else if(running != NULL && mid->data.serverTime < running->data.restTime){
					running = mid; 
				}
				mid = mid->next;
			}
		}
		//cout << queue->head->data.arriveTime << endl;
		if(newPcb && running != NULL){
			mid = queue->head;
			while(mid != NULL){
				//新进程到达判断是否切换进程 
				if(mid->data.arriveTime == time && mid->data.serverTime < running->data.restTime){
					running = mid; 
				}
				mid = mid->next;
			}
			newPcb = 0;
		}
        //当前进程时间 —— 
        if (running != NULL) {
            --(running->data.restTime);
        }
		time++;
	} 
} 

int main(){
	PCB *pcbs = (PCB *)malloc(sizeof(PCB)*n);
	memset(pcbs, 0, sizeof(PCB)*n);
	
//	char name;		 //进程名 
//	int arriveTime;  //到达时间 
//	int serverTime;  //服务时长 
//	int runTime;	 //已运行时间 
//	int startTime;   //开始时间 
//	int finishTime;  //完成时间
//	int restTime;    //剩余服务时长 
//	int priority;	 //优先级 
	pcbs[0].name = '1';pcbs[0].arriveTime = 0;pcbs[0].runTime = 0;pcbs[0].startTime = 0;
	pcbs[0].finishTime = 0;pcbs[0].serverTime = 3; pcbs[0].restTime = 3;pcbs[0].priority = 0;
	
	pcbs[1].name = '2';pcbs[1].arriveTime = 2;pcbs[1].runTime = 0;pcbs[1].startTime = 0;
	pcbs[1].finishTime = 0;pcbs[1].serverTime = 2; pcbs[1].restTime = 2;pcbs[1].priority = 0;
	
	pcbs[2].name = '3';pcbs[2].arriveTime = 5;pcbs[2].runTime = 0;pcbs[2].startTime = 0;
	pcbs[2].finishTime = 0;pcbs[2].serverTime = 5; pcbs[2].restTime = 5;pcbs[2].priority = 0;
	
	pcbs[3].name = '4';pcbs[3].arriveTime = 3;pcbs[3].runTime = 0;pcbs[3].startTime = 0;
	pcbs[3].finishTime = 0;pcbs[3].serverTime = 1; pcbs[3].restTime = 1;pcbs[3].priority = 0;
	
	pcbs[4].name = '5';pcbs[4].arriveTime = 9;pcbs[4].runTime = 0;pcbs[4].startTime = 0;
	pcbs[4].finishTime = 0;pcbs[4].serverTime = 10; pcbs[4].restTime = 10;pcbs[4].priority = 0;
	
	SJF(pcbs, n);
	return 0;
} 
