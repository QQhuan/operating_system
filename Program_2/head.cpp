#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <assert.h>
#include <string.h>


#define n 5  //������
#define m 3  //��Դ��
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
	int name;		 //������ 
	int arriveTime;
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



linkList queue, waitQueue;       //�ȴ�����, ��������

void print(){
	cout << "�������У�" << endl; 
	Node *q = NULL;
	q = queue->head;
	while(q != NULL){
		cout << "���̣�" << "[" << q->data.name << "] ";
		cout << "��Ҫ����Դ��" << " "; 
		for(int i = 0; i < m; i++){
			cout << Need[q->data.name][i] << " ";
		}
		cout << "�ѷ������Դ��" << " "; 
		for(int i = 0; i < m; i++){
			cout << Allocation[q->data.name][i] << " ";
		}
		q = q->next;
	}
	cout << endl;
	cout << "�������У�" << endl; 
	q = waitQueue->head;
	while(q != NULL){
		cout << "���̣�" << "[" << q->data.name << "] ";
		cout << "��Ҫ����Դ��" << " "; 
		for(int i = 0; i < m; i++){
			cout << Need[q->data.name][i] << " ";
		}
		cout << "�ѷ������Դ��" << " "; 
		for(int i = 0; i < m; i++){
			cout << Allocation[q->data.name][i] << " ";
		}
		q = q->next;
	}
	cout << endl;
	
	cout << "����ɵĽ���:";
	for(int i = 0; i < n; i++){
		if(Finish[i] == 1)
			cout << "[" << i << "] ";
	} 
	cout << endl;
} 

//�жϽ����Ƿ���� 
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

//������Դ
void request(Node *running){
	int safe = 1;
	int fade[m] = {0};
	int flag = 1; //��ȫ 
	
	number0:
	//���������Դ 
	srand((unsigned)time(NULL));
	for(int i = 0; i < m; i++){
		fade[i] = rand()%(Need[running->data.name][i]+1);
	}
	if(fade[0] == 0 && fade[1] == 0 && fade[2] == 0)
		goto number0;
	
	cout << "����" << "[" << running->data.name << "]������Դ:" << fade[0] << " " << fade[1] << " " << fade[2] << " " << endl;
	//��Դ���� 
	for(int i = 0; i < m; i++){
		if(fade[i] > Available[i])
			safe = 0;
	} 
	if(!safe){
		cout << "������Դ���������������" << endl; 
		linkListPush(waitQueue, running->data);
		fgo = 0;
		return; 
	}
	
	//���Է�����Դ 
	for(int i = 0; i < m; i++){
		Available[i] -= fade[i]; 
		Need[running->data.name][i] -= fade[i];
		Allocation[running->data.name][i] += fade[i];
	} 
	
	int finish[n] = {0};
	//�ж��Ƿ�ȫ
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
		if(!finish[i] && list[i] == 1) //���ھ��������� 
		{
			flag=0;
			break;
		}
	}
	
	if(flag){
		cout << "��ȫ, ��÷�����Դ" << endl; 
		fgo = 1;
	}else{
		//��Դ���� 
		for(int i = 0; i < m; i++){
			Available[i] += fade[i]; 
			Need[running->data.name][i] += fade[i];
			Allocation[running->data.name][i] -= fade[i];
		}
		cout << "����ȫ, ��������Դ����ʱ��Ƭ���������������еȴ�" << endl; 
	}
	
} 

//ʱ��Ƭ��ת����
void RR(PCB p[]){
	int time = 0;
	int num = n;
	int pnum = 0;
	Node *running= NULL;  // �����������еĽ���  
	while(1){
		for(int i = 0;i < num; i++){
			if(p[i].arriveTime == time){  //ģ��ÿһ��ʱ��Ƭʱ�䣬����Ƿ��н��̵�� ����Ľ��̼���������� 
				linkListPush(queue, p[i]); 
				cout << time << "ʱ�̣�����[" << p[i].name << "]����" << endl;  
			}
		}
		cout << endl; 
		if(NULL == running){
			running = queue->head;
			cout << "ʱ��" << time << ":" << "�������[" << running->data.name << "]" << endl; 
			linkListPop(queue, running);  //�������г���	
			
			print();
			
		} 
		//������Դ 
		request(running); 
		
		//�����Ƿ������
		if(running != NULL && isFinish(running)){
			list[running->data.name] = 0; //��0��ʾ���谲ȫ��� 
			cout << "����[" << running->data.name << "]����ͷ���Դ" << endl; 
			Finish[running->data.name] = 1;
			pnum++;
			for(int i = 0; i < m; i++){
				Available[i] += Max[running->data.name][i]; //�ͷ���Դ 
			} 
			running = NULL;  
			
			//�����������, �������ڴ��Ľ��̵���������� 
			Node *up = waitQueue->head;
			while(up != NULL){
				if(Need[up->data.name][0] <= Available[0] && Need[up->data.name][1] <= Available[1] && Need[up->data.name][2] <= Available[2]){
					linkListPop(waitQueue, up);
					linkListPush(queue, up->data);
					cout << "����[" << up->data.name << "]���������̵����������" << endl;
					up = up->next;
				}
			} 
		}else if(running != NULL && !isFinish(running) && fgo == 1){
			//δ���, �����������̵���
			linkListPush(queue, running->data);
			running = NULL; 
		}
		
		//���̵������ 
		if(pnum == n){
			cout << "���̵������" << endl;
			break; 
		}	
		
		time++;  // time round 
	} 
} 

//������ 
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
