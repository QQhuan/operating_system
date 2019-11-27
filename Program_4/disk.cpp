#include <malloc.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <stdio.h>
#include <algorithm>

#define TOTAL_MEMORY 200
using namespace std;

//请求信息块 
typedef struct PCB{
	int Num;		 //被访问的磁道号 
	int distance;    //调度时移动的距离，方便输出显示 
} PCB;

//记录磁盘扫描的方向，移动总距离
typedef struct record{
	int distance;  //用来计算平均寻道长度 
	int now;       //当前所在磁道数
	int direction; //磁头移动方向, 1表示向外，0表示向里 
}record; 

//进程链表 
typedef struct Node{
	PCB data;
	struct Node *next;
}Node;

//队列 
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
	Node *ret = new Node;
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


//先来先服务调度 
void FCFS(int disk[]){
	cout << "先来先服务调度：" << endl; 
	cout << "磁头刚从80 >>> 100" << endl;
	cout << "******************" << endl;
	int x = 100, num = 0;
	for(int i = 0; i < 10; i++){
		int n = x > disk[i]?x-disk[i]:disk[i]-x;
		num += n;
		x = disk[i];
		cout << "    " << disk[i] << "      " << n << endl; 
	}
	cout << "******************" << endl;
	cout << "先来先服务平均寻道时间：" << num/10 << endl << endl; 
}

bool cmp(int i, int j){
	return i < j;
} 
//最短寻道时间优先
void SSTF(int disk[]){
	cout << "最短寻道时间优先：" << endl; 
	cout << "磁头刚从80 >>> 100" << endl;
	cout << "******************" << endl;
	int flag[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 
	int now = 100, num = 0;
	for(int i = 0; i < 10; i++){
		int mid = 10000, k = 0;
		for(int j = 0; j < 10; j++){
			if(0 == flag[j]){
				int t = now>disk[j]?now-disk[j]:disk[j]-now;
				if(mid > t){
					mid = t;
					k = j;
				}
			} 
		}
		now = disk[k];
		num += mid; //加上总的寻道数
		flag[k] = 1; 
		cout << "    " << disk[k] << "      " << mid << endl;
	}
	cout << "******************" << endl;
	cout << "最短寻道时间优先平均寻道时间：" << num/10 << endl << endl; 
} 

//扫描算法
void SCAN(int disk[]){
	cout << "扫描算法：" << endl; 
	cout << "磁头刚从80 >>> 100" << endl;
	cout << "******************" << endl;
	int direction = 1, num = 0, now = 100; //方向
	int flag[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 
	for(int i = 0; i < 10; i++){
		int mid = 10000, k = -1;
		if(1 == direction){
			for(int j = 0; j < 10; j++){
				if(0 == flag[j] && disk[j] >= now){
					int t = disk[j]-now;
					if(mid > t){
						mid = t;
						k = j;
					}
				}
			}
			if(-1 == k){
				direction = 0;
				i--;
				cout << "(磁头调转)" << endl; 
			}else{
				now = disk[k];
				num += mid;
				flag[k] = 1;
				cout << "    " << disk[k] << "      " << mid << endl;
			}
		}else if(0 == direction){
			for(int j = 0; j < 10; j++){
				if(0 == flag[j] && disk[j] <= now){
					int t = now-disk[j];
					if(mid > t){
						mid = t;
						k = j;
					}
				}
			}
			if(-1 == k){
				direction = 1;
				i--;
				cout << "(磁头调转)" << endl; 
			}else{
				now = disk[k];
				num += mid;
				flag[k] = 1;
				cout << "    " << disk[k] << "      " << mid << endl;
			}
		}
	}
	cout << "******************" << endl;
	cout << "最短寻道时间优先平均寻道时间：" << num/10 << endl << endl;	  
} 

//循环扫描
void CSCAN(int disk[]){
	cout << "循环扫描算法：" << endl; 
	cout << "磁头刚从80 >>> 100" << endl;
	cout << "******************" << endl;
	int num = 0, now = 100, f = 0; //方向
	int flag[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 
	for(int i = 0; i < 10; i++){
			int mid = 10000, k = -1;
			for(int j = 0; j < 10; j++){
				if(0 == flag[j] && disk[j] >= now){
					int t = disk[j]-now;
					if(mid > t){
						mid = t;
						k = j;
					}
				}
			}
			if(-1 == k){
				if(10 == f){
					break;
				}else{
					int l = 10000, m = 0;
					for(int j = 0; j < 10; j++){
						if(0 == flag[j]){
							if(l > disk[j]){
								l = disk[j];
								m = j;
							}
								
						}
					}
					int h = now-disk[m];
					num += now-disk[m];
					now = disk[m];
					flag[m] = 1;
					f++;  //记录完成请求的数量 
					cout << "    " << disk[m] << "      " << h << endl;
				}
			}else{
				now = disk[k];
				num += mid;
				flag[k] = 1;
				f++;  //记录完成请求的数量 
				cout << "    " << disk[k] << "      " << mid << endl;
			}
			
	}
	cout << "******************" << endl;
	cout << "最短寻道时间优先平均寻道时间：" << num/10 << endl << endl;
} 

int main(){
	int disk[10] = {55, 58, 39, 18, 90, 160, 150, 38, 184, 66};
	
	FCFS(disk); 
	
	SSTF(disk);
	
	SCAN(disk);
	
	CSCAN(disk); 
	return 0;
}

