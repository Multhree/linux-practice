#ifndef sem_h
#define sem_h

#include<stdio.h>
#include<sys/sem.h>

int getNewSem(key_t key)//��ȡ���ź��� 
{
	return semget(key,1,0666|IPC_CREAT);
}

union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *arry;
};

int setSemValue(int semid,int n)//�����ź��� 
{
	union semun semTemp;
        semTemp.val = n;
        if(semctl(semid,0,SETVAL,semTemp)==-1)
        {
		return 0;
	}
        return 1;
}

int delSem(int semid)//ɾ���ź��� 
{
	union semun semTemp;
        semctl(semid,0,IPC_RMID,semTemp);
}

int P(int semid)//P���� 
{
	struct sembuf semTemp;
        semTemp.sem_num = 0;
	semTemp.sem_op = -1;
	semTemp.sem_flg = SEM_UNDO;
        if(semop(semid,&semTemp,1) == -1)
        {
            printf("P() failed\n");
            return 0;
        }
        return 1;
}

int V(int semid)//V����
{
	struct sembuf semTemp;
        semTemp.sem_num = 0;
	semTemp.sem_op = 1;
	semTemp.sem_flg = SEM_UNDO;
        if(semop(semid,&semTemp,1) == -1)
        {
            printf("V() failed\n");
            return 0;
        }
        return 1;
}

#endif
