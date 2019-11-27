#include <malloc.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <stdio.h>
#include <algorithm>

#define TOTAL_MEMORY 200
using namespace std;

//������Ϣ�� 
typedef struct PCB{
	int Num;		 //�����ʵĴŵ��� 
	int distance;    //����ʱ�ƶ��ľ��룬���������ʾ 
} PCB;

//��¼����ɨ��ķ����ƶ��ܾ���
typedef struct record{
	int distance;  //��������ƽ��Ѱ������ 
	int now;       //��ǰ���ڴŵ���
	int direction; //��ͷ�ƶ�����, 1��ʾ���⣬0��ʾ���� 
}record; 

//�������� 
typedef struct Node{
	PCB data;
	struct Node *next;
}Node;

//���� 
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
	Node *ret = new Node;
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


//�����ȷ������ 
void FCFS(int disk[]){
	cout << "�����ȷ�����ȣ�" << endl; 
	cout << "��ͷ�մ�80 >>> 100" << endl;
	cout << "******************" << endl;
	int x = 100, num = 0;
	for(int i = 0; i < 10; i++){
		int n = x > disk[i]?x-disk[i]:disk[i]-x;
		num += n;
		x = disk[i];
		cout << "    " << disk[i] << "      " << n << endl; 
	}
	cout << "******************" << endl;
	cout << "�����ȷ���ƽ��Ѱ��ʱ�䣺" << num/10 << endl << endl; 
}

bool cmp(int i, int j){
	return i < j;
} 
//���Ѱ��ʱ������
void SSTF(int disk[]){
	cout << "���Ѱ��ʱ�����ȣ�" << endl; 
	cout << "��ͷ�մ�80 >>> 100" << endl;
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
		num += mid; //�����ܵ�Ѱ����
		flag[k] = 1; 
		cout << "    " << disk[k] << "      " << mid << endl;
	}
	cout << "******************" << endl;
	cout << "���Ѱ��ʱ������ƽ��Ѱ��ʱ�䣺" << num/10 << endl << endl; 
} 

//ɨ���㷨
void SCAN(int disk[]){
	cout << "ɨ���㷨��" << endl; 
	cout << "��ͷ�մ�80 >>> 100" << endl;
	cout << "******************" << endl;
	int direction = 1, num = 0, now = 100; //����
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
				cout << "(��ͷ��ת)" << endl; 
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
				cout << "(��ͷ��ת)" << endl; 
			}else{
				now = disk[k];
				num += mid;
				flag[k] = 1;
				cout << "    " << disk[k] << "      " << mid << endl;
			}
		}
	}
	cout << "******************" << endl;
	cout << "���Ѱ��ʱ������ƽ��Ѱ��ʱ�䣺" << num/10 << endl << endl;	  
} 

//ѭ��ɨ��
void CSCAN(int disk[]){
	cout << "ѭ��ɨ���㷨��" << endl; 
	cout << "��ͷ�մ�80 >>> 100" << endl;
	cout << "******************" << endl;
	int num = 0, now = 100, f = 0; //����
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
					f++;  //��¼������������ 
					cout << "    " << disk[m] << "      " << h << endl;
				}
			}else{
				now = disk[k];
				num += mid;
				flag[k] = 1;
				f++;  //��¼������������ 
				cout << "    " << disk[k] << "      " << mid << endl;
			}
			
	}
	cout << "******************" << endl;
	cout << "���Ѱ��ʱ������ƽ��Ѱ��ʱ�䣺" << num/10 << endl << endl;
} 

int main(){
	int disk[10] = {55, 58, 39, 18, 90, 160, 150, 38, 184, 66};
	
	FCFS(disk); 
	
	SSTF(disk);
	
	SCAN(disk);
	
	CSCAN(disk); 
	return 0;
}

