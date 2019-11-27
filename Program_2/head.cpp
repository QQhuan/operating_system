#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <assert.h>
#include <string.h>


#define n 5  //进程数
#define m 3  //资源数
#define round 2
using namespace std;
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

int Available[m]= {0};
int Max[n][m] = {0};
int Allocation[n][m] = {0};
int Need[n][m] = {0};
int Work[m] = {0};
int list[n] = {0};
int Finish[n] = {0, 0, 0, 0, 0};
int fgo = 1;

typedef struct PCB{
	int name;		 //进程名 
	int arriveTime;
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



linkList queue, waitQueue;       //等待队列, 就绪队列

void print(){
	cout << "就绪队列：" << endl; 
	Node *q = NULL;
	q = queue->head;
	while(q != NULL){
		cout << "进程：" << "[" << q->data.name << "] ";
		cout << "需要的资源：" << " "; 
		for(int i = 0; i < m; i++){
			cout << Need[q->data.name][i] << " ";
		}
		cout << "已分配的资源：" << " "; 
		for(int i = 0; i < m; i++){
			cout << Allocation[q->data.name][i] << " ";
		}
		q = q->next;
	}
	cout << endl;
	cout << "阻塞队列：" << endl; 
	q = waitQueue->head;
	while(q != NULL){
		cout << "进程：" << "[" << q->data.name << "] ";
		cout << "需要的资源：" << " "; 
		for(int i = 0; i < m; i++){
			cout << Need[q->data.name][i] << " ";
		}
		cout << "已分配的资源：" << " "; 
		for(int i = 0; i < m; i++){
			cout << Allocation[q->data.name][i] << " ";
		}
		q = q->next;
	}
	cout << endl;
	
	cout << "已完成的进程:";
	for(int i = 0; i < n; i++){
		if(Finish[i] == 1)
			cout << "[" << i << "] ";
	} 
	cout << endl;
} 

//判断进程是否完成 
int isFinish(Node *running){
	int x = running->data.name;
	int rbool = 1;
	for(int i = 0;i < m; i++){
		if(Need[x][i] != 0)
			rbool = 0;
	}
	if(rbool)
		return 1;
	else
		return 0;
}

//申请资源
void request(Node *running){
	int safe = 1;
	int fade[m] = {0};
	int flag = 1; //安全 
	
	number0:
	//随机申请资源 
	srand((unsigned)time(NULL));
	for(int i = 0; i < m; i++){
		fade[i] = rand()%(Need[running->data.name][i]+1);
	}
	if(fade[0] == 0 && fade[1] == 0 && fade[2] == 0)
		goto number0;
	
	cout << "进程" << "[" << running->data.name << "]申请资源:" << fade[0] << " " << fade[1] << " " << fade[2] << " " << endl;
	//资源不够 
	for(int i = 0; i < m; i++){
		if(fade[i] > Available[i])
			safe = 0;
	} 
	if(!safe){
		cout << "现有资源不足调入阻塞队列" << endl; 
		linkListPush(waitQueue, running->data);
		fgo = 0;
		return; 
	}
	
	//尝试分配资源 
	for(int i = 0; i < m; i++){
		Available[i] -= fade[i]; 
		Need[running->data.name][i] -= fade[i];
		Allocation[running->data.name][i] += fade[i];
	} 
	
	int finish[n] = {0};
	//判断是否安全
	for(int i = 0; i < m; i++){
		Work[i] = Available[i];
	}
	
	number:
	for(int i = 0; i < n; i++){
		if(1 == list[i]){
			int count = 0;
			for(int j = 0; j < m; j++){
				if(!finish[i] && Need[i][j] <= Work[j]){
					count++;
				}
				if(count == m){
					for(int k=0;k<m;k++)
					{
						Work[k]=Work[k]+Allocation[i][k];
					}
					finish[i]=true;
					goto number;
				}
			}
		} 
	} 
	for(int i=0;i<n;i++)
	{
		if(!finish[i] && list[i] == 1) //需在就绪队列中 
		{
			flag=0;
			break;
		}
	}
	
	if(flag){
		cout << "安全, 获得分配资源" << endl; 
		fgo = 1;
	}else{
		//资源回收 
		for(int i = 0; i < m; i++){
			Available[i] += fade[i]; 
			Need[running->data.name][i] += fade[i];
			Allocation[running->data.name][i] -= fade[i];
		}
		cout << "不安全, 不分配资源，等时间片运行完调入就绪队列等待" << endl; 
	}
	
} 

//时间片轮转调度
void RR(PCB p[]){
	int time = 0;
	int num = n;
	int pnum = 0;
	Node *running= NULL;  // 代表正在运行的进程  
	while(1){
		for(int i = 0;i < num; i++){
			if(p[i].arriveTime == time){  //模拟每一个时间片时间，检查是否有进程到达， 到达的进程加入就绪队列 
				linkListPush(queue, p[i]); 
				cout << time << "时刻：进程[" << p[i].name << "]到达" << endl;  
			}
		}
		cout << endl; 
		if(NULL == running){
			running = queue->head;
			cout << "时刻" << time << ":" << "调入进程[" << running->data.name << "]" << endl; 
			linkListPop(queue, running);  //就绪队列出队	
			
			print();
			
		} 
		//分配资源 
		request(running); 
		
		//进程是否已完成
		if(running != NULL && isFinish(running)){
			list[running->data.name] = 0; //标0表示无需安全检查 
			cout << "进程[" << running->data.name << "]完成释放资源" << endl; 
			Finish[running->data.name] = 1;
			pnum++;
			for(int i = 0; i < m; i++){
				Available[i] += Max[running->data.name][i]; //释放资源 
			} 
			running = NULL;  
			
			//检查阻塞队列, 将满足期待的进程调入就绪队列 
			Node *up = waitQueue->head;
			while(up != NULL){
				if(Need[up->data.name][0] <= Available[0] && Need[up->data.name][1] <= Available[1] && Need[up->data.name][2] <= Available[2]){
					linkListPop(waitQueue, up);
					linkListPush(queue, up->data);
					cout << "进程[" << up->data.name << "]从阻塞进程调入就绪队列" << endl;
					up = up->next;
				}
			} 
		}else if(running != NULL && !isFinish(running) && fgo == 1){
			//未完成, 进程其他进程调度
			linkListPush(queue, running->data);
			running = NULL; 
		}
		
		//进程调度完毕 
		if(pnum == n){
			cout << "进程调度完毕" << endl;
			break; 
		}	
		
		time++;  // time round 
	} 
} 

//主函数 
int main(){
	PCB *pcbs = (PCB *)malloc(sizeof(PCB)*n);
	memset(pcbs, 0, sizeof(PCB)*n);
	for(int i = 0; i < n; i++){
		pcbs[i].arriveTime = 0;
		pcbs[i].name = i;
	} 
	
	linkListInit(queue);
	linkListInit(waitQueue);
	Max[0][0] = 3;Max[0][1] = 5;Max[0][2] = 3;
	Max[1][0] = 3;Max[1][1] = 2;Max[1][2] = 2;
	Max[2][0] = 2;Max[2][1] = 0;Max[2][2] = 2;
	Max[3][0] = 2;Max[3][1] = 2;Max[3][2] = 2;
	Max[4][0] = 4;Max[4][1] = 3;Max[4][2] = 3;
	
	Allocation[0][0] = 0;Allocation[0][1] = 1;Allocation[0][2] = 3;
	Allocation[1][0] = 2;Allocation[1][1] = 0;Allocation[1][2] = 2;
	Allocation[2][0] = 1;Allocation[2][1] = 0;Allocation[2][2] = 0;
	Allocation[3][0] = 2;Allocation[3][1] = 1;Allocation[3][2] = 1;
	Allocation[4][0] = 0;Allocation[4][1] = 1;Allocation[4][2] = 1;
	
	Need[0][0] = 3;Need[0][1] = 4;Need[0][2] = 0;
	Need[1][0] = 1;Need[1][1] = 2;Need[1][2] = 0;
	Need[2][0] = 1;Need[2][1] = 0;Need[2][2] = 2;
	Need[3][0] = 0;Need[3][1] = 1;Need[3][2] = 1;
	Need[4][0] = 4;Need[4][1] = 2;Need[4][2] = 2;
	
	Available[0] = 5;Available[1] = 12;Available[2] = 5;
	RR(pcbs);
	return 0;
}
