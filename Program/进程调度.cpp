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
#define Round 2   //时间片 
using namespace std;

//记录函数
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

//计算平均周转时间 
float calculate(PCB p[]){
	int num = 0;
	for(int i = 0; i < n; i++){
		num += p[i].finishTime-p[i].arriveTime;
	}	
	float result;
	result = (float)num/(float)n;
} 

//计算响应比
float Pb(int time, int start, int rest){
	int up = time - start+rest;
	return (float)up/(float)rest;
}

//判断时间片到达中断
int isChange(int num){
	return num == Round?1:0;
} 



//短进程优先调度 
void SJF(PCB p[], int num){
	int time = 0;
	int newPcb = 0; 
	int processNum = num;
	Node *running= NULL;  // 代表正在运行的进程 
	Node *mid = NULL; 
	linkList queue, finish;       //就绪队列 
	linkListInit(queue);
	linkListInit(finish);
	while(1){
		for(int i = 0;i < num; i++){
			if(p[i].arriveTime == time){  //模拟每一秒的时间，检查是否有进程到达， 到达的进程加入就绪队列 
				linkListPush(queue, p[i]); 
				newPcb = 1;
				cout << time << "时刻：进程" << p[i].name << "到达" << endl; 
				processNum--; 
			}
		}
		//print(queue);
		 
		if(islinkListEmpty(queue) && processNum == 0){
			cout << "进程调度完毕" << endl;
			cout << "平均周转时间：" << calculate(p) <<endl; 
			break; 
		}
		
		//正在运行的进程运行结束 
		if (running != NULL && running->data.restTime == 0) {
            cout << time << "时刻: 进程" << running->data.name << "结束" << endl;
			record(p, running->data.name, time); 
		//	cout << running->data.name << endl;
            //linkListPop(queue, running);
            linkListPush(finish, running->data); //进入完成队列 
            running = NULL;
        }
        
		//首次运行或者某进程运行已结束 
		if(!islinkListEmpty(queue) && running == NULL){
			mid = queue->head;
			
			while(mid != NULL){
				//处理机空闲 , 从队列中选出一个服务时间最短的 
				if(running == NULL && queue->size > 1 && mid->data.restTime <= mid->next->data.restTime) 
					running = mid;
				else if(running == NULL && queue->size > 1 && mid->data.restTime > mid->next->data.restTime)
					running = mid->next;
				//首次运行 
				else if(running == NULL && queue->size == 1)
					running = mid;
				//找出队列中的最短进程，获得处理机
				//当队列中存在两个相同时间的进程时，排在前面的那一个获得处理机
				//当正在运行的进程剩余服务时间跟队列中最小服务时长进程相等时，不作进程切换 
				else if(running != NULL && mid->data.restTime < running->data.restTime){
					running = mid; 
				}
				mid = mid->next;
			}
			linkListPop(queue, running);  //该进程移除就绪队列 
			newPcb = 0;
		}
		//cout << queue->head->data.arriveTime << endl;
		if(newPcb && running != NULL){
			mid = queue->tail;
			//新进程到达判断是否切换进程, 即—抢占 
			if(mid->data.arriveTime == time && mid->data.restTime < running->data.restTime){
				cout << "发生抢占" << endl; 
				linkListPush(queue, running->data);
				linkListPop(queue, mid); 
				running = mid;
			}
			newPcb = 0;
		}
		cout << endl;
		if(running != NULL)
	    	cout << "\t\t" << time <<"时刻" << endl;
	    select(1, queue);
	    cout << "\t正在运行的进程：" << running->data.name << endl; 
	    select(0, finish);
        //当前进程时间 —— 
        if (running != NULL) {
            --(running->data.restTime);
        }
		time++;
	} 
} 

//高相应比调度
void HRRN(PCB p[], int num){
	int time = 0;
	int newPcb = 0;
	int processNum = num;
	Node *running = NULL;  //代表正在运行的进程 
	Node *mid = NULL; 
	linkList queue, finish;       //就绪队列 
	linkListInit(queue);
	linkListInit(finish);
	while(1){
		for(int i = 0;i < num; i++){
			if(p[i].arriveTime == time){  //模拟每一秒的时间，检查是否有进程到达， 到达的进程加入就绪队列 
				linkListPush(queue, p[i]);
				newPcb = 1;
				cout << time << "时刻：进程" << p[i].name << "到达" << endl; 
				processNum--; 
			}
		}
		//print(queue);
		
		if(islinkListEmpty(queue) && processNum == 0){
			cout << "进程调度完毕" << endl;
			cout << "平均周转时间：" << calculate(p) <<endl; 
			break; 
		}
		
		//正在运行的进程运行结束 
		if (running != NULL && running->data.restTime == 0) {
            cout << time << "时刻: 进程" << running->data.name << "结束" << endl;
			record(p, running->data.name, time); 
            //linkListPop(queue, running);
            linkListPush(finish, running->data); 
            running = NULL;
        } 
        
		//首次运行或者某进程运行已结束 
		if(!islinkListEmpty(queue) && running == NULL){
			mid = queue->head;
			while(mid != NULL){
				Node *q = mid->next; 
				//处理机空闲 , 从队列中选出一个x优先级最高的进程 
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
				//首次运行 
				else if(running == NULL && queue->size == 1)
					running = mid;
				//找出队列中的最短进程，获得处理机
				//当队列中存在两个相同时间的进程时，排在前面的那一个获得处理机
				//当正在运行的进程剩余服务时间跟队列中最小服务时长进程相等时，不作进程切换 
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
	    	cout << "\t\t" << time <<"时刻" << endl;
	    select(1, queue);
	    cout << "\t正在运行的进程：" << running->data.name << endl; 
	    select(0, finish);
        //当前进程时间 —— 
        if (running != NULL) {
            --(running->data.restTime);
        }
		time++;
	} 
} 

//时间片轮转调度, 时间片长 2 
void RR(PCB p[], int num){
	int time = 0;
	int cal = 0; 
	int processNum = num;
	Node *running= NULL;  // 代表正在运行的进程 
	Node *mid = NULL; 
	linkList queue, finish;       //就绪队列 
	linkListInit(queue);
	linkListInit(finish);
	while(1){
		for(int i = 0;i < num; i++){
			if(p[i].arriveTime == time){  //模拟每一秒的时间，检查是否有进程到达， 到达的进程加入就绪队列 
				linkListPush(queue, p[i]); 
				cout << time << "时刻：进程" << p[i].name << "到达" << endl; 
				processNum--; 
			}
		}
		
		
		if(islinkListEmpty(queue) && processNum == 0 && running->data.restTime == 0){
			cout << "进程调度完毕" << endl;
			cout << "平均周转时间：" << calculate(p) <<endl; 
			break; 
		}
		
        //首次 
        if(running == NULL){
        	running = queue->head;
			linkListPop(queue, running);
			cal = 0;
		} 
		if(running != NULL && running->data.restTime == 0 && !isChange(cal)){
			cout << time << "时刻: 进程" << running->data.name << "结束" << endl;
			record(p, running->data.name, time); 
			running = queue->head;
			linkListPush(finish, running->data);
			linkListPop(queue, running);
			cal = 0;
		}
		if(isChange(cal)){
			if(running->data.restTime == 0){
				cout << time << "时刻: 进程" << running->data.name << "结束" << endl;
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
		
		
		//输出 
		cout << endl;
		if(running != NULL)
	    	cout << "\t\t" << time <<"时刻" << endl;
	    select(1, queue);
	    cout << "\t正在运行的进程：" << running->data.name << endl; 
	    select(0, finish);
        //当前进程时间 —— r 
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
	
//	char name;		 //进程名 
//	int arriveTime;  //到达时间 
//	int serverTime;  //服务时长 
//	int runTime;	 //已运行时间 
//	int startTime;   //开始时间 
//	int finishTime;  //完成时间
//	int restTime;    //剩余服务时长 
//	int priority;	 //优先级 
	int random = 0, randomT = 0;
	cout << "****************************" << endl;
	cout << "进程名 到达时间 要求服务时间" << endl; 
	srand((unsigned)time(NULL));
	random = 0;  //十以内的随机数 
	randomT = rand()%10+1;
	pcbs[0].name = '1';pcbs[0].arriveTime = 0;pcbs[0].runTime = 0;pcbs[0].startTime = 0;
	pcbs[0].finishTime = 0;pcbs[0].serverTime = randomT; pcbs[0].restTime = randomT;pcbs[0].priority = 0;
	cout << "  " << 1 << "\t " << 0 << " \t\t" << randomT << endl;
	
	random = rand()%10;  //十以内的随机数 
	randomT = rand()%10+1;
	pcbs[1].name = '2';pcbs[1].arriveTime = random;pcbs[1].runTime = 0;pcbs[1].startTime = 0;
	pcbs[1].finishTime = 0;pcbs[1].serverTime = randomT; pcbs[1].restTime = randomT;pcbs[1].priority = 0;
	cout << "  " << 2 << "\t " << random << " \t\t" << randomT << endl;
	
	random = rand()%10;  //十以内的随机数 
	randomT = rand()%10+1;
	pcbs[2].name = '3';pcbs[2].arriveTime = random;pcbs[2].runTime = 0;pcbs[2].startTime = 0;
	pcbs[2].finishTime = 0;pcbs[2].serverTime = randomT; pcbs[2].restTime = randomT;pcbs[2].priority = 0;
	cout << "  " << 3 << "\t " << random << " \t\t" << randomT << endl;
	
	random = rand()%10;  //十以内的随机数 
	randomT = rand()%10+1;
	pcbs[3].name = '4';pcbs[3].arriveTime = random;pcbs[3].runTime = 0;pcbs[3].startTime = 0;
	pcbs[3].finishTime = 0;pcbs[3].serverTime = randomT; pcbs[3].restTime = randomT;pcbs[3].priority = 0;
	cout << "  " << 4 << "\t " << random << " \t\t" << randomT << endl;
	
	random = rand()%10;  //十以内的随机数 
	randomT = rand()%10+1;
	pcbs[4].name = '5';pcbs[4].arriveTime = random;pcbs[4].runTime = 0;pcbs[4].startTime = 0;
	pcbs[4].finishTime = 0;pcbs[4].serverTime = randomT; pcbs[4].restTime = randomT;pcbs[4].priority = 0;
	cout << "  " << 5 << "\t " << random << " \t\t" << randomT << endl;
	cout << "****************************" << endl;
	
	cout << endl;
	cout << "短进程优先调度:" << endl << endl; 
	SJF(pcbs, n);
	
	cout << endl;
	cout << "高相应比优先调度：" << endl << endl;
	HRRN(pcbs, n);
	
	cout << endl;
	cout << "时间片轮转调度:" << endl << endl;
	RR(pcbs, n); 
	return 0;
} 
