#ifndef box_h
#define box_h

#include"sem.h"
#include<stdio.h>
#include<stdlib.h>
#include<sys/shm.h>

#define boxId  1234
#define boxLen 5

struct box
{
//����ͷ 
	int bid;//�����ʶ�� 
	int bsize;//�Ÿ����� 
	/*ͬ���ź���*/
	int mailnum;//���������ż�������ص��ź��� 
	int freenum;//�������пո�������ص��ź��� 
	/*�����ź���*/
	int rmutex;//�����½�ʱ�Ļ����ź��� 
	int wmutex;//�����ż�ʱ�Ļ����ź��� 
	int out;//��ǰ�ɶ�ȡ�ż����Ÿ��ַ 
	int in;//��ǰ�ɴ����ż����Ÿ��ַ 
//������ 
	int  *buf;
	void *shm;
};

struct box* getNewBox(int len,int n)//��ȡ������ 
{
	int shmid;
        struct box* msgbox = (struct box*)malloc(sizeof(struct box));
        shmid = shmget((key_t)(boxId+n),sizeof(int)*len,0666|IPC_CREAT);
	msgbox->shm = shmat(shmid,0,0);
        msgbox->buf = (int *)(msgbox->shm);
	//��ʼ���Ÿ����� 
        msgbox->bsize = len;
        //��ʼ�������ţ�Ϊ�����źŵı�� 
        msgbox->bid = shmid;
        //��ʼ���ź��� 
        msgbox->mailnum = getNewSem((key_t)(shmid+1));
        setSemValue(msgbox->mailnum,0);

        msgbox->freenum = getNewSem((key_t)(shmid+2));
        setSemValue(msgbox->freenum,len);

        msgbox->rmutex = getNewSem((key_t)(shmid+3));
        setSemValue(msgbox->rmutex,1);

        msgbox->wmutex = getNewSem((key_t)(shmid+4));
        setSemValue(msgbox->wmutex,1);

        msgbox->out = 0;
        msgbox->in = 0;


	return msgbox;
}

void send(struct box* dest,int msg)//����
{
	P(dest->freenum);
	P(dest->wmutex);
	dest->buf[dest->in] = msg;
	dest->in = (dest->in+1)%dest->bsize;
	V(dest->wmutex);
	V(dest->mailnum);
};


int receive(struct box* addr)//����
{
	int msg;
	P(addr->mailnum);
	P(addr->rmutex);
	msg = addr->buf[addr->out];
	addr->out = (addr->out+1)%addr->bsize;
	V(addr->rmutex);
	V(addr->freenum);
	
	return msg;
};

void recall(struct box* addr)//���� 
{
	P(addr->mailnum);
	P(addr->wmutex);
	P(addr->rmutex);
	addr->in = (addr->in-1)%addr->bsize;
	V(addr->rmutex);
	V(addr->wmutex);
	V(addr->freenum);
};
void deleteBox(struct box* msgbox)//ɾ������ 
{
	delSem(msgbox->freenum);
	delSem(msgbox->mailnum);
	delSem(msgbox->rmutex);
	delSem(msgbox->wmutex);
	shmdt(msgbox->shm);
	shmctl(msgbox->bid,IPC_RMID,0);
};

struct box* boxA = getNewBox(boxLen,1);
struct box* boxB = getNewBox(boxLen,2);
#endif
