#include <malloc.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <stdio.h>

#define TOTAL_MEMORY 200
using namespace std;

int endAdr = 660;
typedef struct partition{
	int num;			//������ 
	int startAdress;    //������ʼ��ַ 
	int leisureAdress;  //���е�ַ 
	int size;			//ʣ��ռ� 
	int state;         //״̬ , 0δ���䣬1�ѷ��� 
	struct partition *next;
}partition, *PART;

//������¼�������е���ҵ 
typedef struct PCB{
	string name;		 //��ҵ��
	int startAdress;   //��ʼ��ַ 
	int size;			//��С 
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

linkList queue;       //�ȴ�����, ��������


 
void printPartitionQue(PART partHead){
	PART mPart = partHead;
	printf("������      ��ַ       ������ַ       ���пռ�       ״̬\n");
	while(mPart!=NULL){
		printf("%3d          ", mPart->num);
		printf("%3d          ", mPart->startAdress);
		printf("%3d          ", mPart->leisureAdress);
		printf("%3d           ", mPart->size);
		printf("%3d           ", mPart->state);
		printf("\n");
		mPart = mPart->next;
	}
	printf("\n");
}

/**
*	�������� 
*/
int All = 1;
PART createPartition(){
	PART partHead = NULL, mPart = NULL;
	int size = 640;
//	int sizes[4] = {128, 64, 32, 12};
	int startAdress = 20;  //��ʼ��ַ 
	mPart = partHead = (PART)malloc(sizeof(partition));
	mPart->num = All++;
	mPart->startAdress = startAdress;
	mPart->leisureAdress = startAdress;
	mPart->size = size;
	mPart->state = 0; 
	startAdress += mPart->size;
	mPart->next = NULL;
	return partHead;
}

PART mergeLists(PART a, PART b){  //�ϲ����� 
	if(a == NULL)return b;
	if(b == NULL)return a;
	PART ret = (PART)malloc(sizeof(partition)*1);
	ret->next = NULL;
	PART tail = ret;
	
	while(a && b){
		if(a->size < b->size){
			tail->next = a;
			tail = tail->next;
			a = a->next;
		}else{
			tail->next = b;
			tail = tail->next;
			b = b->next;
		}
	}
	if(a){
		tail->next = a;
	}
	if(b){
		tail->next = b;
	}
	return ret->next;
} 

PART getMid(PART head){  //�õ��м�ڵ� 
	if(!head) return NULL;
	if(!head->next) return head;
	
	PART slow = head;
	PART fast = head->next;
	
	//ֻ��˵ţƤ 
	while(fast && fast->next){
		slow = slow->next;
		fast = fast->next->next;
	}
	return slow;
}

/**
*	�����ÿռ��С������ �鲢���� 
*/
PART sort(PART partHead){
	if(NULL == partHead)
		return NULL;
	if(!partHead->next)
		return partHead;
	PART mid = getMid(partHead);
	PART next = NULL; 
	if(mid) {
		next = mid->next;
		mid->next = NULL;
	}
	return mergeLists(sort(partHead), sort(next));
}


//�ͷ��ڴ� 
void freeP(int size, string name, PART Head){
	int start = 0;
	int end = 0;
	Node *o = queue->head;
	while(o != NULL){
		if(o->data.name == name){
			start = o->data.startAdress;
			linkListPop(queue, o);  //�������г��� 
			cout << "��ҵ��" << name << "�ͷ��ڴ�" << endl; 
			break; 
		}
		o = o->next;
	}
	end = start+size;
	//cout << end; 
	PART h = Head;
	PART q = h;
	while(h != NULL){
		if(h->leisureAdress == end){
			//cout << "ȷʵִ����"; 
			h->leisureAdress = start;
			h->size += size;
			h = Head;
//			while(h != NULL){
//				if(h->startAdress+h->size == start){
//					h->size += size;
//					break;
//				}
//				h = h->next;
//			}
			break;
		}else if(h->leisureAdress+h->size == start){
			h->size += size;
			break;
		} 
		q = h; 
		h = h->next;
	}
	//����һ����� 
	if(NULL == h){
		PART m = (PART)malloc(sizeof(partition));
		m->num = All++;
		m->startAdress = 20;
		m->leisureAdress = endAdr;
		m->size = size;
		m->state = 0; 
		m->next = NULL;
		endAdr += size;
		q->next = m;
	}
}


/**
*	�״���Ӧ�㷨�����ڴ���Ĵ��� 
*/
void assignOfFirstFit(PART partHead, PART mPart, int way, int size, string name){
	mPart = partHead;
	PART p = mPart;
	if(1 == way){  //�����ڴ� 
		while(mPart!=NULL){
			if(mPart->size >= size){
				mPart->size -= size;
				mPart->leisureAdress += size;	
				mPart->state = 0;
				if(mPart->size == 0 && mPart == partHead){
					partHead = mPart->next;
				}else if(mPart->size == 0){
					p->next = mPart->next;
				}
				printf("***********************�ɹ������ķ����б�***********************\n");
				printPartitionQue(partHead);
				PCB p; 
				p.name = name;
				p.size = size;
				p.startAdress = mPart->leisureAdress-size;
				//cout << p.name << ">>" << p.size << ".."<< p.startAdress;
				linkListPush(queue, p);
				cout << "�����е���ҵ:" << endl;
				Node *q = queue->head;
				while(q != NULL){
					cout << "��ҵ����" << q->data.name << "  ��С��" << q->data.size << "  ��ʼ��ַ��" << q->data.startAdress << endl;
					q = q->next; 
				} 
				break;	
			}
			p = mPart;
			mPart = mPart->next;
		}
	}else{
		freeP(size, name, partHead);
		printf("***********************�ɹ��ͷ��ڴ�����ķ����б�***********************\n");
		printPartitionQue(partHead);
		cout << "�����е���ҵ:" << endl;
		Node *q = queue->head;
		while(q != NULL){
			cout << "��ҵ����" << q->data.name << "  ��С��" << q->data.size << "  ��ʼ��ַ��" << q->data.startAdress << endl;
			q = q->next; 
		}
	} 
	
	if(mPart==NULL){
		printf("�ڴ治��!\n");
	}
}


/**
*	�����Ӧ�㷨�����ڴ���Ĵ��� 
*/
PART assignOfBestFit(PART partHead, PART mPart, int way, int size, string name){
	mPart = sort(partHead);
	partHead = mPart;
	if(1 == way){   //������Դ 
		PART p = mPart;
		while(mPart!=NULL){
			if(mPart->size >= size){
				mPart->size -= size;
				mPart->leisureAdress += size;	
				mPart->state = 0;
				if(0 == mPart->size && mPart == partHead){
					partHead = mPart->next;
				}else if(0 == mPart->size){
					p->next = mPart->next;
				}
				printf("***********************�ɹ������ķ����б�***********************\n");
				partHead = sort(partHead);
				printPartitionQue(partHead);
				PCB p;
				p.name = name;
				p.size = size;
				p.startAdress = mPart->leisureAdress-size;
				linkListPush(queue, p);
				cout << "�����е���ҵ:" << endl;
				Node *q = queue->head;
				while(q != NULL){
					cout << "��ҵ����" << q->data.name << "  ��С��" << q->data.size << "  ��ʼ��ַ��" << q->data.startAdress << endl;
					q = q->next; 
				}
				return partHead;
			}
			p = mPart;
			mPart = mPart->next;
		}
	}else{    //�ͷ���Դ 
		freeP(size, name, partHead);
		partHead = sort(partHead);
		printf("***********************�ɹ��ͷ��ڴ�����ķ����б�***********************\n");
		printPartitionQue(partHead);
		cout << "�����е���ҵ:" << endl;
		Node *q = queue->head;
		while(q != NULL){
			cout << "��ҵ����" << q->data.name << "  ��С��" << q->data.size << "  ��ʼ��ַ��" << q->data.startAdress << endl;
			q = q->next; 
		}
		return partHead;
	} 
	
	if(mPart==NULL){
		printf("�ڴ治��!\n");
		return partHead;
	}
}

/**
*  �����㷨���ͷ����ڴ� 
*/
void assignMemory(PART partHead, int assignType){
	int totalPart = 4;
	PART mPart = partHead;
	char c;
	string name;
	int size = 0;
	int way = 0;
	printf("�Ƿ�������ҵ(Y/N)��");
	scanf("%c", &c);
	while(c=='Y'||c =='y'){
		printf("��������ҵ����");
		cin >> name;
		printf("��������ҵ��С��");
		scanf("%d", &size);
		printf("��������䷽ʽ��1������, 2���ͷţ�");
		scanf("%d", &way);
		switch(assignType){
			case 1:
				assignOfFirstFit(partHead, mPart, way, size, name);
				break;	
			case 2:
				partHead = assignOfBestFit(partHead, mPart, way, size, name);
				break;
		}
		
		printf("�Ƿ�������ҵ(Y/N)��");
		getchar();
		scanf("%c", &c);
	}
}

/**
*	�����Ӧ�㷨 
*/
void BestFit(PART partHead){
	printf("***********************����ǰ�ķ����б�***********************\n");
	printPartitionQue(partHead);
	//�����ڴ�
	assignMemory(partHead, 2);
}


/**
*	�״���Ӧ�㷨 
*/
void FirstFit(PART partHead){
	printf("***********************����ǰ�ķ����б�***********************\n");
	printPartitionQue(partHead);
	//�����ڴ� 
	assignMemory(partHead, 1);
}


int main(){
	int k;
	linkListInit(queue);
	PART partHead = createPartition();
	printf("\n1���״���Ӧ�㷨  \n2�������Ӧ�㷨  \n3���˳�\n");
	printf("\n��ѡ���㷨��");
	scanf("%d",&k);
	getchar();
	switch(k){
			case 1:
				FirstFit(partHead);
				break;
			case 2:
				BestFit(partHead);
				break;
			case 3:
				break;
			default:
				printf("ѡ�����������ѡ��");
		} 
	return 0;
}

