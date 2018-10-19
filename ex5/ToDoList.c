#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>

#define STRING_LEN 64
#define FILE_NAME "ToDoList.txt"

GtkWidget *uiList;
GtkWidget *textToDo;
GtkWidget *textDeadLine;
char selectText[STRING_LEN*2] = {0};
const gchar *list_item_data_key="list_item_data";

typedef struct _Node
{
	char name[STRING_LEN];
	char num[STRING_LEN];
	struct _Node *next;
}Node;

Node *head = NULL;	//链表头
Node *tail = NULL;	//链表尾

void DelList()		//清空链表
{
	if(head==NULL)
		return;
	Node *p = head;
	while(p)
	{
		Node *tmp = p->next;
		free(p);
		p = tmp;
	}
	head = NULL;
	tail = NULL;
}

void InitList()		//初始化
{
	DelList();
	head = (Node *)malloc(sizeof(Node));
	head->next = NULL;
	tail = head;
}

void InsertNode(Node *node)//添加结点 
{
	if(head==NULL)
		return;
	tail->next = node;
	tail = node;
	tail->next = NULL;
}

void DelNode(const char *str)//删除结点
{
	if(head==NULL)
		return;
	Node *pl = head, *pr = head->next;
	while(pr)
	{
		char buf[STRING_LEN*2]={0};
		sprintf(buf,"ToDo:%s \t DeadLine:%s",pr->name,pr->num);
		if(strcmp(buf,str)==0)
		{
			if(pr->next==NULL)
			{
				tail = pl;
			}
			pl->next = pr->next;
			free(pr);
			pr = pl->next;
			break;
		}
		else
		{
			pl = pr;
			pr = pr->next;
		}
	}
}

void ShowTip(const char *info)
{
	GtkWidget *dialog = gtk_message_dialog_new(NULL,GTK_DIALOG_MODAL|GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,"%s",info); 
	gtk_dialog_run(GTK_DIALOG(dialog)); 
	gtk_widget_destroy(dialog);
}

void ReadFile()//从文件读取ToDoList
{
	FILE *fp = fopen(FILE_NAME,"r");
	if(fp==NULL)
		return;
	DelList();
	InitList();
	char ToDoStr[STRING_LEN],DDLStr[STRING_LEN];
	while(fscanf(fp,"%s %s",ToDoStr,DDLStr)==2)
	{
		Node *pnew = (Node *)malloc(sizeof(Node));
		strcpy(pnew->name,ToDoStr);
		strcpy(pnew->num,DDLStr);
		InsertNode(pnew);
	}
	fclose(fp);
}

void WriteFile()//保存数据到ToDoList文件
{
	FILE *fp = fopen(FILE_NAME,"w");
	if(fp==NULL)
		return;
	Node *p = head->next;
	while(p)
	{
		fprintf(fp,"%s %s\n",p->name,p->num);
		p = p->next;
	}
	fclose(fp);
}


void AddNodeToList(const char *ToDoStr, const char *DDLStr)//向uiList中加入元素
{
	char str[STRING_LEN*2] = {0};
	sprintf(str, "ToDo:%s \t DeadLine:%s",ToDoStr,DDLStr);

	gchar *string;
	GtkWidget *label=gtk_label_new (str);
	GtkWidget *list_item = gtk_list_item_new ();
	gtk_container_add (GTK_CONTAINER (list_item), label);
	gtk_widget_show (label);
	gtk_container_add (GTK_CONTAINER (uiList), list_item);
	gtk_widget_show (list_item);
	gtk_label_get (GTK_LABEL (label), &string);
	g_object_set_data (G_OBJECT (list_item), list_item_data_key, string);
}

void onBtnInsert(GtkWidget *widget, gpointer data)//点击ADD按钮
{
	const char *ToDoStr = gtk_entry_get_text((GtkEntry *)textToDo);
	const char *DDLStr  = gtk_entry_get_text((GtkEntry *)textDeadLine);
	if(strlen(ToDoStr)==0 || strlen(DDLStr)==0)
		return;
	
	Node *pnew = (Node *)malloc(sizeof(Node));
	sprintf(pnew->name,"%s",ToDoStr);
	sprintf(pnew->num,"%s",DDLStr);
	InsertNode(pnew);
	
	AddNodeToList(ToDoStr,DDLStr);
	
	gtk_entry_set_text((GtkEntry *)textToDo,"");
	gtk_entry_set_text((GtkEntry *)textDeadLine,"");
	ShowTip("ADD SUCCESSED");
}
void onBtnDelete(GtkWidget *widget, gpointer data)//点击Finished按钮
{
	gtk_list_clear_items(GTK_LIST(uiList),0,-1);
	DelNode(selectText);
	Node *p = head->next;
	while(p)
	{
		AddNodeToList(p->name,p->num);
		p = p->next;
	}
	ShowTip("Congratulations!");
}
void onBtnSave(GtkWidget *widget, gpointer data)//点击SAVE按钮
{
	WriteFile();
	if(widget!=0 || data!=0)
		ShowTip("SAVE SUCCESSED");
}
void onBtnRead(GtkWidget *widget, gpointer data)//READ按钮
{
	gtk_list_clear_items(GTK_LIST(uiList),0,-1);
	ReadFile();
	Node *p = head->next;
	while(p)
	{
		AddNodeToList(p->name,p->num);
		p = p->next;
	}
	if(widget!=0 || data!=0)
		ShowTip("READ SUCCESSED");
}
void onBtnExit(GtkWidget *widget, gpointer data)//点击EXIT按钮
{
	gtk_main_quit();
	onBtnSave(0,0);
}

void OnListSelectionChanged(GtkWidget *gtklist, gpointer func_data)//选择的uiList项目发生变化触发
{
	GList *dlist = GTK_LIST(gtklist)->selection;
	if (!dlist)
	{
		return;
    }
	else
	{
		if(dlist!=NULL)
		{
			const char *buf = g_object_get_data(G_OBJECT(dlist->data),list_item_data_key);
			sprintf(selectText,"%s",buf);
		}
	}
}

void UI_Init(int argc, char *argv[])//初始化界面
{
	gtk_init(&argc,&argv);
	GtkWidget *window=gtk_window_new(GTK_WINDOW_TOPLEVEL);	
	gtk_window_set_default_size(GTK_WINDOW(window),100,250);
	g_signal_connect(G_OBJECT(window),"delete_event",G_CALLBACK(gtk_main_quit),NULL);
	gtk_window_set_title(GTK_WINDOW(window),"ToDoList");	
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	gtk_container_set_border_width(GTK_CONTAINER(window),10);
	//--------------------垂直线性布局----------------------
	GtkWidget *vbox=gtk_vbox_new(0,0);
	gtk_container_add(GTK_CONTAINER(window),vbox);
	//------------------------------------------------------

	
	GtkWidget *scrolled_window = gtk_scrolled_window_new (NULL, NULL);//可拖动窗口
    gtk_widget_set_size_request (scrolled_window, 250, 150);
    gtk_container_add (GTK_CONTAINER (vbox), scrolled_window);

	
	uiList = gtk_list_new();//放于可拖动窗口中的uiList
	gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (scrolled_window),uiList);
	g_signal_connect (uiList, "selection-changed", G_CALLBACK (OnListSelectionChanged), NULL);
	//------------------水平布局---------------------------
	GtkWidget *hbox1 = gtk_hbox_new(0,0);
	gtk_box_pack_start(GTK_BOX(vbox),hbox1,FALSE,FALSE,5);
	
	GtkWidget *lab_name = gtk_label_new("ToDo");
	gtk_box_pack_start(GTK_BOX(hbox1),lab_name,1,0,5);
	
	textToDo = gtk_entry_new();
	gtk_box_pack_start(GTK_BOX(hbox1),textToDo,1,0,5);


	GtkWidget *lab_num = gtk_label_new("DeadLine");
	gtk_box_pack_start(GTK_BOX(hbox1),lab_num,1,0,5);
		
	textDeadLine = gtk_entry_new();
	gtk_box_pack_start(GTK_BOX(hbox1),textDeadLine,1,0,5);
	
	GtkWidget *btn_add = gtk_button_new_with_label("ADD");
	gtk_box_pack_start(GTK_BOX(hbox1),btn_add,1,0,5);
	g_signal_connect(G_OBJECT(btn_add),"clicked",G_CALLBACK(onBtnInsert),NULL);
	
	//-------放入Finished、SAVE、EXIT3个按钮--------
	GtkWidget *hbox2 = gtk_hbox_new(0,0);
	gtk_box_pack_start(GTK_BOX(vbox),hbox2,FALSE,FALSE,5);
	
	GtkWidget *btn_delete = gtk_button_new_with_label("Finished");
	gtk_box_pack_start(GTK_BOX(hbox2),btn_delete,1,0,5);
	g_signal_connect(G_OBJECT(btn_delete),"clicked",G_CALLBACK(onBtnDelete),NULL);
	
	GtkWidget *btn_save = gtk_button_new_with_label("SAVE");
	gtk_box_pack_start(GTK_BOX(hbox2),btn_save,1,0,5);
	g_signal_connect(G_OBJECT(btn_save),"clicked",G_CALLBACK(onBtnSave),NULL);
	
	
	GtkWidget *btn_exit = gtk_button_new_with_label("EXIT");
	gtk_box_pack_start(GTK_BOX(hbox2),btn_exit,1,0,5);
	g_signal_connect(G_OBJECT(btn_exit),"clicked",G_CALLBACK(onBtnExit),NULL);
	//------------------------------------------------------
	onBtnRead(0,0);//从文件读取数据放入链表
	gtk_widget_show_all(window);//显示所有窗体
	gtk_main();
}

int main(int argc, char *argv[])
{
	InitList();			//初始化链表
	UI_Init(argc, argv);//初始化UI
	DelList();			//释放链表
	return 0;
}

