#include<stdio.h>
#include"sem.h"
#include"box.h"
//引入前面定义好的信箱与信号量相关的函数 

int main()
{
	
	while(1)
	{
		int op;
		printf("input op:0 send, 1 receive, 2 recall, 3 exit:");//根据指令执行操作 
		scanf("%d",&op);
		if(op == 0)
		{
			printf("input msg:");
			int msg;
			scanf("%d",&msg);
			send(boxB,msg);//发送消息给信箱B 
		}
		else if(op == 1)
		{
			printf("msg:%d\n",receive(boxA));//从信箱A接收消息 
		}
		else if(op == 2)
		{
			recall(boxB);
		}
		else if(op == 3)
		{
			deleteBox(boxA);
			break;
		}
		else
		{
			printf("invalid op! please input again!\n");
		}
	}
	return 0;
}
