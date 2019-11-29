#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "head.h"
#include "function.h"
#define Round 2   //ʱ��Ƭ 
using namespace std;

//��¼����
void record(PCB p[], char ch, int time){
	switch(ch){
		case '1':
			p[0].finishTime = time;
			break;
		case '2':
			p[1].finishTime = time;
			break;
		case '3':
			p[2].finishTime = time;
			break;
		case '4':
			p[3].finishTime = time;
			break;
		case '5':
			p[4].finishTime = time;
			break;
		default:
			break;
	}
} 

//����ƽ����תʱ�� 
float calculate(PCB p[]){
	int num = 0;
	for(int i = 0; i < n; i++){
		num += p[i].finishTime-p[i].arriveTime;
	}	
	float result;
	result = (float)num/(float)n;
} 

//������Ӧ��
float Pb(int time, int start, int rest){
	int up = time - start+rest;
	return (float)up/(float)rest;
}

//�ж�ʱ��Ƭ�����ж�
int isChange(int num){
	return num == Round?1:0;
} 



//�̽������ȵ��� 
void SJF(PCB p[], int num){
	int time = 0;
	int newPcb = 0; 
	int processNum = num;
	Node *running= NULL;  // �����������еĽ��� 
	Node *mid = NULL; 
	linkList queue, finish;       //�������� 
	linkListInit(queue);
	linkListInit(finish);
	while(1){
		for(int i = 0;i < num; i++){
			if(p[i].arriveTime == time){  //ģ��ÿһ���ʱ�䣬����Ƿ��н��̵�� ����Ľ��̼���������� 
				linkListPush(queue, p[i]); 
				newPcb = 1;
				cout << time << "ʱ�̣�����" << p[i].name << "����" << endl; 
				processNum--; 
			}
		}
		//print(queue);
		 
		if(islinkListEmpty(queue) && processNum == 0){
			cout << "���̵������" << endl;
			cout << "ƽ����תʱ�䣺" << calculate(p) <<endl; 
			break; 
		}
		
		//�������еĽ������н��� 
		if (running != NULL && running->data.restTime == 0) {
            cout << time << "ʱ��: ����" << running->data.name << "����" << endl;
			record(p, running->data.name, time); 
		//	cout << running->data.name << endl;
            //linkListPop(queue, running);
            linkListPush(finish, running->data); //������ɶ��� 
            running = NULL;
        }
        
		//�״����л���ĳ���������ѽ��� 
		if(!islinkListEmpty(queue) && running == NULL){
			mid = queue->head;
			
			while(mid != NULL){
				//��������� , �Ӷ�����ѡ��һ������ʱ����̵� 
				if(running == NULL && queue->size > 1 && mid->data.restTime <= mid->next->data.restTime) 
					running = mid;
				else if(running == NULL && queue->size > 1 && mid->data.restTime > mid->next->data.restTime)
					running = mid->next;
				//�״����� 
				else if(running == NULL && queue->size == 1)
					running = mid;
				//�ҳ������е���̽��̣���ô����
				//�������д���������ͬʱ��Ľ���ʱ������ǰ�����һ����ô����
				//���������еĽ���ʣ�����ʱ�����������С����ʱ���������ʱ�����������л� 
				else if(running != NULL && mid->data.restTime < running->data.restTime){
					running = mid; 
				}
				mid = mid->next;
			}
			linkListPop(queue, running);  //�ý����Ƴ��������� 
			newPcb = 0;
		}
		//cout << queue->head->data.arriveTime << endl;
		if(newPcb && running != NULL){
			mid = queue->tail;
			//�½��̵����ж��Ƿ��л�����, ������ռ 
			if(mid->data.arriveTime == time && mid->data.restTime < running->data.restTime){
				cout << "������ռ" << endl; 
				linkListPush(queue, running->data);
				linkListPop(queue, mid); 
				running = mid;
			}
			newPcb = 0;
		}
		cout << endl;
		if(running != NULL)
	    	cout << "\t\t" << time <<"ʱ��" << endl;
	    select(1, queue);
	    cout << "\t�������еĽ��̣�" << running->data.name << endl; 
	    select(0, finish);
        //��ǰ����ʱ�� ���� 
        if (running != NULL) {
            --(running->data.restTime);
        }
		time++;
	} 
} 

//����Ӧ�ȵ���
void HRRN(PCB p[], int num){
	int time = 0;
	int newPcb = 0;
	int processNum = num;
	Node *running = NULL;  //�����������еĽ��� 
	Node *mid = NULL; 
	linkList queue, finish;       //�������� 
	linkListInit(queue);
	linkListInit(finish);
	while(1){
		for(int i = 0;i < num; i++){
			if(p[i].arriveTime == time){  //ģ��ÿһ���ʱ�䣬����Ƿ��н��̵�� ����Ľ��̼���������� 
				linkListPush(queue, p[i]);
				newPcb = 1;
				cout << time << "ʱ�̣�����" << p[i].name << "����" << endl; 
				processNum--; 
			}
		}
		//print(queue);
		
		if(islinkListEmpty(queue) && processNum == 0){
			cout << "���̵������" << endl;
			cout << "ƽ����תʱ�䣺" << calculate(p) <<endl; 
			break; 
		}
		
		//�������еĽ������н��� 
		if (running != NULL && running->data.restTime == 0) {
            cout << time << "ʱ��: ����" << running->data.name << "����" << endl;
			record(p, running->data.name, time); 
            //linkListPop(queue, running);
            linkListPush(finish, running->data); 
            running = NULL;
        } 
        
		//�״����л���ĳ���������ѽ��� 
		if(!islinkListEmpty(queue) && running == NULL){
			mid = queue->head;
			while(mid != NULL){
				Node *q = mid->next; 
				//��������� , �Ӷ�����ѡ��һ��x���ȼ���ߵĽ��� 
				if(NULL == q && queue->size != 1){
					if(Pb(time, mid->data.arriveTime, mid->data.restTime) > Pb(time, running->data.arriveTime, running->data.restTime)){
						running = mid;
						break;
					}
				} 
				if(running == NULL && queue->size > 1 && Pb(time, mid->data.arriveTime, mid->data.restTime) >= Pb(time, q->data.arriveTime, q->data.restTime)) 
					running = mid;
				else if(running == NULL && queue->size > 1 && Pb(time, mid->data.arriveTime, mid->data.restTime) < Pb(time, q->data.arriveTime, q->data.restTime))
					running = q;
				//�״����� 
				else if(running == NULL && queue->size == 1)
					running = mid;
				//�ҳ������е���̽��̣���ô����
				//�������д���������ͬʱ��Ľ���ʱ������ǰ�����һ����ô����
				//���������еĽ���ʣ�����ʱ�����������С����ʱ���������ʱ�����������л� 
				else if(running != NULL && Pb(time, mid->data.arriveTime, mid->data.restTime) > Pb(time, running->data.arriveTime, running->data.restTime)){
					running = mid; 
				}
				mid = mid->next;
			}
			linkListPop(queue, running);
		}
		//cout << queue->head->data.arriveTime << endl;
		
		cout << endl;
		if(running != NULL)
	    	cout << "\t\t" << time <<"ʱ��" << endl;
	    select(1, queue);
	    cout << "\t�������еĽ��̣�" << running->data.name << endl; 
	    select(0, finish);
        //��ǰ����ʱ�� ���� 
        if (running != NULL) {
            --(running->data.restTime);
        }
		time++;
	} 
} 

//ʱ��Ƭ��ת����, ʱ��Ƭ�� 2 
void RR(PCB p[], int num){
	int time = 0;
	int cal = 0; 
	int processNum = num;
	Node *running= NULL;  // �����������еĽ��� 
	Node *mid = NULL; 
	linkList queue, finish;       //�������� 
	linkListInit(queue);
	linkListInit(finish);
	while(1){
		for(int i = 0;i < num; i++){
			if(p[i].arriveTime == time){  //ģ��ÿһ���ʱ�䣬����Ƿ��н��̵�� ����Ľ��̼���������� 
				linkListPush(queue, p[i]); 
				cout << time << "ʱ�̣�����" << p[i].name << "����" << endl; 
				processNum--; 
			}
		}
		
		
		if(islinkListEmpty(queue) && processNum == 0 && running->data.restTime == 0){
			cout << "���̵������" << endl;
			cout << "ƽ����תʱ�䣺" << calculate(p) <<endl; 
			break; 
		}
		
        //�״� 
        if(running == NULL){
        	running = queue->head;
			linkListPop(queue, running);
			cal = 0;
		} 
		if(running != NULL && running->data.restTime == 0 && !isChange(cal)){
			cout << time << "ʱ��: ����" << running->data.name << "����" << endl;
			record(p, running->data.name, time); 
			running = queue->head;
			linkListPush(finish, running->data);
			linkListPop(queue, running);
			cal = 0;
		}
		if(isChange(cal)){
			if(running->data.restTime == 0){
				cout << time << "ʱ��: ����" << running->data.name << "����" << endl;
				record(p, running->data.name, time); 
				running = queue->head;
				linkListPop(queue, running);
				cal = 0;
			}else{
				linkListPush(queue, running->data);
				running = queue->head;
				linkListPop(queue, running);
				cal = 0;
			}
			
		}
		
		
		//��� 
		cout << endl;
		if(running != NULL)
	    	cout << "\t\t" << time <<"ʱ��" << endl;
	    select(1, queue);
	    cout << "\t�������еĽ��̣�" << running->data.name << endl; 
	    select(0, finish);
        //��ǰ����ʱ�� ���� r 
        if (running != NULL) {
            --(running->data.restTime);
        }
		time++;
		cal++;
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
	int random = 0, randomT = 0;
	cout << "****************************" << endl;
	cout << "������ ����ʱ�� Ҫ�����ʱ��" << endl; 
	srand((unsigned)time(NULL));
	random = 0;  //ʮ���ڵ������ 
	randomT = rand()%10+1;
	pcbs[0].name = '1';pcbs[0].arriveTime = 0;pcbs[0].runTime = 0;pcbs[0].startTime = 0;
	pcbs[0].finishTime = 0;pcbs[0].serverTime = randomT; pcbs[0].restTime = randomT;pcbs[0].priority = 0;
	cout << "  " << 1 << "\t " << 0 << " \t\t" << randomT << endl;
	
	random = rand()%10;  //ʮ���ڵ������ 
	randomT = rand()%10+1;
	pcbs[1].name = '2';pcbs[1].arriveTime = random;pcbs[1].runTime = 0;pcbs[1].startTime = 0;
	pcbs[1].finishTime = 0;pcbs[1].serverTime = randomT; pcbs[1].restTime = randomT;pcbs[1].priority = 0;
	cout << "  " << 2 << "\t " << random << " \t\t" << randomT << endl;
	
	random = rand()%10;  //ʮ���ڵ������ 
	randomT = rand()%10+1;
	pcbs[2].name = '3';pcbs[2].arriveTime = random;pcbs[2].runTime = 0;pcbs[2].startTime = 0;
	pcbs[2].finishTime = 0;pcbs[2].serverTime = randomT; pcbs[2].restTime = randomT;pcbs[2].priority = 0;
	cout << "  " << 3 << "\t " << random << " \t\t" << randomT << endl;
	
	random = rand()%10;  //ʮ���ڵ������ 
	randomT = rand()%10+1;
	pcbs[3].name = '4';pcbs[3].arriveTime = random;pcbs[3].runTime = 0;pcbs[3].startTime = 0;
	pcbs[3].finishTime = 0;pcbs[3].serverTime = randomT; pcbs[3].restTime = randomT;pcbs[3].priority = 0;
	cout << "  " << 4 << "\t " << random << " \t\t" << randomT << endl;
	
	random = rand()%10;  //ʮ���ڵ������ 
	randomT = rand()%10+1;
	pcbs[4].name = '5';pcbs[4].arriveTime = random;pcbs[4].runTime = 0;pcbs[4].startTime = 0;
	pcbs[4].finishTime = 0;pcbs[4].serverTime = randomT; pcbs[4].restTime = randomT;pcbs[4].priority = 0;
	cout << "  " << 5 << "\t " << random << " \t\t" << randomT << endl;
	cout << "****************************" << endl;
	
	cout << endl;
	cout << "�̽������ȵ���:" << endl << endl; 
	SJF(pcbs, n);
	
	cout << endl;
	cout << "����Ӧ�����ȵ��ȣ�" << endl << endl;
	HRRN(pcbs, n);
	
	cout << endl;
	cout << "ʱ��Ƭ��ת����:" << endl << endl;
	RR(pcbs, n); 
	return 0;
} 
