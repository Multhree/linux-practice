/* 选择文件对话框 filesel.c */
#include <gtk/gtk.h>
void on_file_select_ok (GtkWidget *button,GtkFileSelection *fs) {
	GtkWidget *dialog;
	gchar message[1024];
	const gchar *filename;
	filename = gtk_file_selection_get_filename(fs);
	if(g_file_test(filename,G_FILE_TEST_IS_DIR))
		sprintf(message,"你选择的目录是： %s",filename);
	else
		sprintf(message,"you choice is: %s",filename);
	dialog = gtk_message_dialog_new(NULL,
	                                GTK_DIALOG_DESTROY_WITH_PARENT,
	                                GTK_MESSAGE_INFO,
	                                GTK_BUTTONS_OK,
	                                message,
	                                NULL);
	gtk_widget_destroy(GTK_WIDGET(fs));
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}
void on_button_clicked (GtkWidget *button,gpointer userdata) {
	GtkWidget* dialog ;
	dialog = gtk_file_selection_new("choice a file: ");
	gtk_window_set_position(GTK_WINDOW(dialog),GTK_WIN_POS_CENTER);
	gtk_signal_connect(GTK_OBJECT(dialog),"destroy",
	                   GTK_SIGNAL_FUNC(gtk_widget_destroy),&dialog);
	gtk_signal_connect(GTK_OBJECT(GTK_FILE_SELECTION(dialog)->ok_button),
	                   "clicked",
	                   GTK_SIGNAL_FUNC(on_file_select_ok),
	                   GTK_FILE_SELECTION(dialog));
	gtk_signal_connect_object(GTK_OBJECT(GTK_FILE_SELECTION(dialog)->cancel_button),
	    "clicked",
	    GTK_SIGNAL_FUNC(gtk_widget_destroy),
	    GTK_OBJECT(dialog));
	gtk_widget_show(dialog);
}
int main ( int argc , char* argv[]) {
	GtkWidget *window;
	GtkWidget *button;
	gtk_init(&argc,&argv);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(G_OBJECT(window),"delete_event",
	                 G_CALLBACK(gtk_main_quit),NULL);
	gtk_window_set_title(GTK_WINDOW(window),"hello");
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	gtk_container_set_border_width(GTK_CONTAINER(window),20);
	button = gtk_button_new_with_label("choice a file ");
	g_signal_connect(G_OBJECT(button),"clicked",
	                 G_CALLBACK(on_button_clicked),NULL);
	gtk_container_add(GTK_CONTAINER(window),button);
	gtk_widget_show(button);
	gtk_widget_show(window);
	gtk_main();
	return FALSE;
}

