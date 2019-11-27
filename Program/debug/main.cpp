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
    	
    	//��βָ��任 
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
        	
        	//��βָ��任 
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
//�̽������ȵ��� 
void SJF(PCB p[], int num){
	int time = 0;
	int newPcb = 0; 
	int processNum = num;
	Node *running= NULL;  // �����������еĽ��� 
	Node *mid = NULL; 
	linkList queue;       //�������� 
	linkListInit(queue);
	while(1){
		for(int i = 0;i < num; i++){
			if(p[i].arriveTime == time){  //ģ��ÿһ���ʱ�䣬����Ƿ��н��̵�� ����Ľ��̼���������� 
				linkListPush(queue, p[i]); 
				newPcb = 1;
				cout << time << "ʱ�̣�����" << p[i].name << "����" << endl; 
				processNum--; 
			}
		}
		print(queue);
		//if(running != NULL)
	    //	cout << time <<"ʱ�̣�" << running->data.name << endl; 
		if(islinkListEmpty(queue) && processNum == 0){
			cout << "���̵������" << endl;
			break; 
		}
		
		//�������еĽ������н��� 
		if (running != NULL && running->data.restTime == 0) {
            cout << time << "ʱ��: ����" << running->data.name << "����" << endl; 
            linkListPop(queue, running);
//            cout << "����:" << queue->head->data.name << endl;
            running = NULL;
        } 
		//�״����л���ĳ���������ѽ��� 
		if(!islinkListEmpty(queue) && running == NULL){
			mid = queue->head;
			while(mid != NULL){
				//��������� , �Ӷ�����ѡ��һ������ʱ����̵� 
				if(running == NULL && queue->size > 1 && mid->data.serverTime < mid->next->data.serverTime) 
					running = mid;
				else if(running == NULL && queue->size > 1 && mid->data.serverTime > mid->next->data.serverTime)
					running = mid->next;
				//�״����� 
				else if(running == NULL && queue->size == 1)
					running = mid;
				//�ҳ������е���̽��̣���ô����
				//�������д���������ͬʱ��Ľ���ʱ������ǰ�����һ����ô����
				//���������еĽ���ʣ�����ʱ�����������С����ʱ���������ʱ�����������л� 
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
				//�½��̵����ж��Ƿ��л����� 
				if(mid->data.arriveTime == time && mid->data.serverTime < running->data.restTime){
					running = mid; 
				}
				mid = mid->next;
			}
			newPcb = 0;
		}
        //��ǰ����ʱ�� ���� 
        if (running != NULL) {
            --(running->data.restTime);
        }
		time++;
	} 
} 

int main(){
	PCB *pcbs = (PCB *)malloc(sizeof(PCB)*n);
	memset(pcbs, 0, sizeof(PCB)*n);
	
//	char name;		 //������ 
//	int arriveTime;  //����ʱ�� 
//	int serverTime;  //����ʱ�� 
//	int runTime;	 //������ʱ�� 
//	int startTime;   //��ʼʱ�� 
//	int finishTime;  //���ʱ��
//	int restTime;    //ʣ�����ʱ�� 
//	int priority;	 //���ȼ� 
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
