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
	int num;			//分区号 
	int startAdress;    //分区开始地址 
	int leisureAdress;  //空闲地址 
	int size;			//剩余空间 
	int state;         //状态 , 0未分配，1已分配 
	struct partition *next;
}partition, *PART;

//用来记录正在运行的作业 
typedef struct PCB{
	string name;		 //作业名
	int startAdress;   //起始地址 
	int size;			//大小 
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

linkList queue;       //等待队列, 就绪队列


 
void printPartitionQue(PART partHead){
	PART mPart = partHead;
	printf("分区号      起址       空闲起址       空闲空间       状态\n");
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
*	创建分区 
*/
int All = 1;
PART createPartition(){
	PART partHead = NULL, mPart = NULL;
	int size = 640;
//	int sizes[4] = {128, 64, 32, 12};
	int startAdress = 20;  //起始地址 
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

PART mergeLists(PART a, PART b){  //合并操作 
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

PART getMid(PART head){  //得到中间节点 
	if(!head) return NULL;
	if(!head->next) return head;
	
	PART slow = head;
	PART fast = head->next;
	
	//只能说牛皮 
	while(fast && fast->next){
		slow = slow->next;
		fast = fast->next->next;
	}
	return slow;
}

/**
*	按可用空间大小按升序 归并排序 
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


//释放内存 
void freeP(int size, string name, PART Head){
	int start = 0;
	int end = 0;
	Node *o = queue->head;
	while(o != NULL){
		if(o->data.name == name){
			start = o->data.startAdress;
			linkListPop(queue, o);  //正在运行出队 
			cout << "作业：" << name << "释放内存" << endl; 
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
			//cout << "确实执行了"; 
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
	//新填一块分区 
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
*	首次适应算法分配内存核心代码 
*/
void assignOfFirstFit(PART partHead, PART mPart, int way, int size, string name){
	mPart = partHead;
	PART p = mPart;
	if(1 == way){  //申请内存 
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
				printf("***********************成功分配后的分区列表***********************\n");
				printPartitionQue(partHead);
				PCB p; 
				p.name = name;
				p.size = size;
				p.startAdress = mPart->leisureAdress-size;
				//cout << p.name << ">>" << p.size << ".."<< p.startAdress;
				linkListPush(queue, p);
				cout << "运行中的作业:" << endl;
				Node *q = queue->head;
				while(q != NULL){
					cout << "作业名：" << q->data.name << "  大小：" << q->data.size << "  起始地址：" << q->data.startAdress << endl;
					q = q->next; 
				} 
				break;	
			}
			p = mPart;
			mPart = mPart->next;
		}
	}else{
		freeP(size, name, partHead);
		printf("***********************成功释放内存分配后的分区列表***********************\n");
		printPartitionQue(partHead);
		cout << "运行中的作业:" << endl;
		Node *q = queue->head;
		while(q != NULL){
			cout << "作业名：" << q->data.name << "  大小：" << q->data.size << "  起始地址：" << q->data.startAdress << endl;
			q = q->next; 
		}
	} 
	
	if(mPart==NULL){
		printf("内存不足!\n");
	}
}


/**
*	最佳适应算法分配内存核心代码 
*/
PART assignOfBestFit(PART partHead, PART mPart, int way, int size, string name){
	mPart = sort(partHead);
	partHead = mPart;
	if(1 == way){   //申请资源 
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
				printf("***********************成功分配后的分区列表***********************\n");
				partHead = sort(partHead);
				printPartitionQue(partHead);
				PCB p;
				p.name = name;
				p.size = size;
				p.startAdress = mPart->leisureAdress-size;
				linkListPush(queue, p);
				cout << "运行中的作业:" << endl;
				Node *q = queue->head;
				while(q != NULL){
					cout << "作业名：" << q->data.name << "  大小：" << q->data.size << "  起始地址：" << q->data.startAdress << endl;
					q = q->next; 
				}
				return partHead;
			}
			p = mPart;
			mPart = mPart->next;
		}
	}else{    //释放资源 
		freeP(size, name, partHead);
		partHead = sort(partHead);
		printf("***********************成功释放内存分配后的分区列表***********************\n");
		printPartitionQue(partHead);
		cout << "运行中的作业:" << endl;
		Node *q = queue->head;
		while(q != NULL){
			cout << "作业名：" << q->data.name << "  大小：" << q->data.size << "  起始地址：" << q->data.startAdress << endl;
			q = q->next; 
		}
		return partHead;
	} 
	
	if(mPart==NULL){
		printf("内存不足!\n");
		return partHead;
	}
}

/**
*  根据算法类型分配内存 
*/
void assignMemory(PART partHead, int assignType){
	int totalPart = 4;
	PART mPart = partHead;
	char c;
	string name;
	int size = 0;
	int way = 0;
	printf("是否输入作业(Y/N)：");
	scanf("%c", &c);
	while(c=='Y'||c =='y'){
		printf("请输入作业名：");
		cin >> name;
		printf("请输入作业大小：");
		scanf("%d", &size);
		printf("请输入分配方式：1、申请, 2、释放：");
		scanf("%d", &way);
		switch(assignType){
			case 1:
				assignOfFirstFit(partHead, mPart, way, size, name);
				break;	
			case 2:
				partHead = assignOfBestFit(partHead, mPart, way, size, name);
				break;
		}
		
		printf("是否输入作业(Y/N)：");
		getchar();
		scanf("%c", &c);
	}
}

/**
*	最佳适应算法 
*/
void BestFit(PART partHead){
	printf("***********************排序前的分区列表***********************\n");
	printPartitionQue(partHead);
	//分配内存
	assignMemory(partHead, 2);
}


/**
*	首次适应算法 
*/
void FirstFit(PART partHead){
	printf("***********************分配前的分区列表***********************\n");
	printPartitionQue(partHead);
	//分配内存 
	assignMemory(partHead, 1);
}


int main(){
	int k;
	linkListInit(queue);
	PART partHead = createPartition();
	printf("\n1、首次适应算法  \n2、最佳适应算法  \n3、退出\n");
	printf("\n请选择算法：");
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
				printf("选择错误，请重新选择。");
		} 
	return 0;
}

