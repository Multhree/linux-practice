#include<stdio.h>
#include"sem.h"
#include"box.h"
//����ǰ�涨��õ��������ź�����صĺ��� 

int main()
{
	
	while(1)
	{
		int op;
		printf("input op:0 send, 1 receive, 2 recall, 3 exit:");//����ָ��ִ�в��� 
		scanf("%d",&op);
		if(op == 0)
		{
			printf("input msg:");
			int msg;
			scanf("%d",&msg);
			send(boxA,msg);//������Ϣ������A 
		}
		else if(op == 1)
		{
			printf("msg:%d\n",receive(boxB));//������B������Ϣ  
		}
		else if(op == 2)
		{
			recall(boxA);
		}
		else if(op == 3)
		{
			deleteBox(boxB);
			break;
		}
		else
		{
			printf("invalid op! please input again!\n");
		}
	}
	return 0;
}
