/*2018年11月01日 星期四 21时02分19秒*/
#include <gtk/gtk.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

/*******************************/
int sockfd = -1;
struct sockaddr_in address;
gchar message[1024] = "";
/*******************************/
void on_file_select_ok (GtkWidget *button,GtkFileSelection *fs) 
{
	GtkWidget *dialog;
	const gchar *filename;
	filename = gtk_file_selection_get_filename(fs);
	int len = strlen(filename);
	if(!g_file_test(filename,G_FILE_TEST_IS_DIR) && filename[len-1] == 'g' && filename[len-2] == 'p' && filename[len-3] == 'j')
	{
		sprintf(message,"the pic you selected is: %s",filename);
		int i = 0;
		for(i = 0;i < strlen(filename);i++)
		{
			write(sockfd, &filename[i], 1);
		}
		char end = ':';
		write(sockfd, &end, 1);	
	}
	else if (!g_file_test(filename,G_FILE_TEST_IS_DIR) && filename[len-1] == 'f' && filename[len-2] == 'd' && filename[len-3] == 'p')
	{
		sprintf(message,"the pdf you selected is: %s",filename);
		int i = 0;
		for(i = 0;i < strlen(filename);i++)
		{
			write(sockfd, &filename[i], 1);
		}
		char end = ':';
		write(sockfd, &end, 1);	
	}
	else
	{
		sprintf(message,"bad choice: %s",filename);
	}
	dialog = gtk_message_dialog_new(NULL,GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO,GTK_BUTTONS_OK,message,NULL);
	gtk_widget_destroy(GTK_WIDGET(fs));
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}
void on_button_clicked (GtkWidget *button,gpointer userdata)
{
	GtkWidget* dialog ;
	dialog = gtk_file_selection_new("choice a file: ");
	gtk_window_set_position(GTK_WINDOW(dialog),GTK_WIN_POS_CENTER);
	gtk_signal_connect(GTK_OBJECT(dialog),"destroy", GTK_SIGNAL_FUNC(gtk_widget_destroy),&dialog);
	gtk_signal_connect(GTK_OBJECT(GTK_FILE_SELECTION(dialog)->ok_button), "clicked",GTK_SIGNAL_FUNC(on_file_select_ok),GTK_FILE_SELECTION(dialog));
	gtk_signal_connect_object(GTK_OBJECT(GTK_FILE_SELECTION(dialog)->cancel_button),"clicked",GTK_SIGNAL_FUNC(gtk_widget_destroy),GTK_OBJECT(dialog));
	gtk_widget_show(dialog);
}
int main (int argc , char* argv[]) 
{
    /*********************************************/
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	address.sin_family = AF_INET;                      
	address.sin_addr.s_addr = inet_addr(argv[1]);//server_ip
	address.sin_port = htons(6666);//port                         
	if (connect(sockfd, (struct sockaddr *) &address, (socklen_t) sizeof(address)) == -1)
	{
		perror("ops: client\n");
	  	exit(1);
	}
    /*********************************************/
	GtkWidget *window;
	GtkWidget *button_pic;
	GtkWidget *button_pdf;
	GtkWidget *box;

	gtk_init(&argc,&argv);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(G_OBJECT(window),"delete_event",G_CALLBACK(gtk_main_quit),NULL);
	gtk_window_set_default_size(GTK_WINDOW(window),400,100);
	gtk_window_set_title(GTK_WINDOW(window),"Useful Tool");
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	gtk_container_set_border_width(GTK_CONTAINER(window),20);
    /*********************************************/
	box = gtk_vbox_new(0,0);
    	gtk_container_add(GTK_CONTAINER(window),box);
    
	button_pic = gtk_button_new_with_label("choice a pic ");
	g_signal_connect(G_OBJECT(button_pic),"clicked",G_CALLBACK(on_button_clicked),NULL);
	gtk_box_pack_start(GTK_BOX(box),button_pic,1,1,3);
	
	button_pdf = gtk_button_new_with_label("choice a pdf ");
	g_signal_connect(G_OBJECT(button_pdf),"clicked",G_CALLBACK(on_button_clicked),NULL);
	gtk_box_pack_start(GTK_BOX(box),button_pdf,1,1,3);
	
	gtk_widget_show_all(window);
	gtk_main();
	return 0;
}


