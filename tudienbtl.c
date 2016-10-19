#include <gtk/gtk.h>
#include "tudien.h"
#define ICON_LOCATION "/home/fk/gtk/"

/*--------------KHAI BAO NGUYEN MAU HAM------------------
---------------------------------------------------------*/
static void thongtin(GtkButton *button,GtkWindow *parent);
static void thongbao2(GtkButton *button,GtkWindow *parent);
static void thongbao3(GtkButton *button,GtkWindow *parent);
static void trogiup(GtkWidget *button,gpointer data);
char *get_from_entry(GtkWidget *button,gpointer data);
char *get_from_text_view(GtkWidget *button,gpointer data);
void question(GtkWidget *widget,GtkWidget *window);
void ghifile(GtkWidget *button,gpointer data);
void Delete(GtkWidget *button,gpointer data);
info search(GtkWidget *button,gpointer data);
void Link_search(GtkWidget *button,gpointer data);
void hiennghia(GtkWidget *button,gpointer data);
void showSoundex(GtkWidget *button, gpointer data);
gboolean autocomplete(GtkEntry *entry,GdkEventKey *event);
GdkPixbuf *create_pixbuf(const gchar * filename);
GtkWidget *SetupWindow(gchar *data,gboolean Transient);
/*-------------------------------------------------------------
--------------------------------------------------------------*/
const gchar *const_text=NULL;
gchar *word,*mean,word2[50];
info p,p2;
char *stack_word[500];
int kt,kt2,dem_vong=0,dem_max=0,num_element;
GtkWidget *textview;

void quit(GtkWidget *window,gpointer data)
{
	gtk_main_quit();
}

static void thongtin(GtkButton *button,GtkWindow *parent)
{
  	GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file("./image/help.jpg", NULL);
	GtkWidget *dialog = gtk_about_dialog_new();

  	gtk_about_dialog_set_name(GTK_ABOUT_DIALOG(dialog), "EV DICTIONARY");
 	gtk_about_dialog_set_logo(GTK_ABOUT_DIALOG(dialog), pixbuf);
  	g_object_unref(pixbuf), pixbuf = NULL;
  	gtk_dialog_run(GTK_DIALOG (dialog));
  	gtk_widget_destroy(dialog);
}

static void show (GtkWidget *widget,GtkWidget *xx)
{
	gtk_widget_show(xx);
}

static void show2 (GtkWidget *widget,GtkWidget *xx)
{
	gtk_widget_hide(xx);
	gtk_widget_show(xx);
}

static void hide (GtkWidget *widget,GtkWidget *xx)
{
	gtk_widget_hide(xx);
}

static void reset (GtkWidget *widget,GtkWidget *xx)
{
	xx=gtk_text_view_new ();
}

static void editableMean (GtkWidget *widget,GtkWidget *xx)
{
	gtk_text_view_set_editable (GTK_TEXT_VIEW (xx), TRUE);
}

static void uneditableMean (GtkWidget *widget,GtkWidget *xx)
{
	gtk_text_view_set_editable (GTK_TEXT_VIEW (xx), FALSE);
}

static void changeFontOfTextview(GtkWidget *widget,GtkWidget *xx)
{
	PangoFontDescription *font;
	font = pango_font_description_from_string ("Ubuntu Monospace Italic 10");
	gtk_widget_modify_font (xx, font);
}

static void defaultFontOfTextview(GtkWidget *widget,GtkWidget *xx)
{
	PangoFontDescription *font;
	font = pango_font_description_from_string ("Ubuntu Monospace 10");
	gtk_widget_modify_font (xx, font);
}

static gboolean delete_event (GtkWidget *window, GdkEvent *event, gpointer data)
{
		return FALSE;
}

void question(GtkWidget *widget,GtkWidget *window)
{
  GtkWidget *dialog;
  gint answer;
  dialog = gtk_message_dialog_new(GTK_WINDOW(window),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_QUESTION,GTK_BUTTONS_YES_NO,"Close without saving?");
  gtk_window_set_title(GTK_WINDOW(dialog), "Quit");
  answer = gtk_dialog_run(GTK_DIALOG(dialog));
  if (answer == GTK_RESPONSE_YES) 
    {
      	gtk_widget_destroy (window);
        gtk_widget_destroy (dialog);
        
    } 
  else 
    {
        gtk_widget_destroy (dialog);
	
    }
}

/*-----------------CHUC NANG THONG BAO -----------------
---------------------TRONG CUA SO TOOL------------------
-------------------------------------------------------*/
static void thongbao2(GtkButton *button,GtkWindow *parent)
{
	GtkWidget *dialog;
	if(kt2==0)
		dialog = gtk_message_dialog_new (parent, GTK_DIALOG_MODAL,GTK_MESSAGE_INFO, GTK_BUTTONS_OK,"\t\tSuccess!\t\t");
	else 
		dialog = gtk_message_dialog_new (parent, GTK_DIALOG_MODAL,GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,"\tFail!\t");

	gtk_window_set_title (GTK_WINDOW (dialog), "Notify");
	gtk_dialog_run (GTK_DIALOG (dialog));
	gtk_widget_destroy (dialog);
}

static void thongbao3(GtkButton *button,GtkWindow *parent)
{
	GtkWidget *dialog;
	if(kt==0)
	{
		dialog = gtk_message_dialog_new (parent, GTK_DIALOG_MODAL,GTK_MESSAGE_WARNING, GTK_BUTTONS_OK,"This word already exits, Add an other word!");
		gtk_window_set_title (GTK_WINDOW (dialog), "Warning");
		gtk_dialog_run (GTK_DIALOG (dialog));
		gtk_widget_destroy (dialog);
	}
	else
	{
		dialog = gtk_message_dialog_new (parent, GTK_DIALOG_MODAL,GTK_MESSAGE_OTHER, GTK_BUTTONS_OK,"This word doesn't exit, can be added!");
		gtk_window_set_title (GTK_WINDOW (dialog), "Notify");
		gtk_dialog_run (GTK_DIALOG (dialog));
		gtk_widget_destroy (dialog);
	}
}


/*---------------LAY TEXT TU ENTRY VA TEXT_VIEW TRONG CUA SO TOOL-------------
------------------------------------------------------------------------------*/
char *get_from_entry(GtkWidget *button,gpointer data)
{
	const char* word2=gtk_entry_get_text(GTK_ENTRY(data));
	word=(char *)word2;
	return word;
}

char *get_from_text_view(GtkWidget *button,gpointer data)
{
	GtkTextBuffer *buffer;
	GtkTextIter start, end;
	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW(data));
	gtk_text_buffer_get_start_iter(buffer,&start);
    gtk_text_buffer_get_end_iter (buffer,&end);
	mean = gtk_text_buffer_get_text (buffer, &start, &end, FALSE);
	return mean;
}
/*-------------------CHUC NANG THEM TU MOI------------
-----------------------VA CHINH SUA TU----------------
------------------------------------------------------*/
void ghifile(GtkWidget *button,gpointer data)
{
	info p0;
	int kt,rsize;
	BTA *bt;
	bt=btopn("dict",0,0);
	kt=btsel(bt,str_lwr(word),(char*)&p0,sizeof(info),&rsize);
	info p;
	long pos,len;
	FILE *f1=fopen("anhviet.dict","a+");
	FILE *f2;
	fprintf(f1,"\n");
	fclose(f1);
	f1=fopen("anhviet.dict","a+");
	f2=fopen("anhviet.dict","rb");
	fseek(f2,0,SEEK_END);
	pos=ftell(f2);
	fprintf(f1,"%s",mean);
	fclose(f1);
	fclose(f2);
	f2=fopen("anhviet.dict","rb");
	fseek(f2,0,SEEK_END);
	len=ftell(f2)-pos;
	p.pos=pos;
	p.len=len;
	fclose(f2);
	
	if(kt!=0)
	{
		kt2=btins(bt,str_lwr(word),(char *)&p,sizeof(info));
	}
	else 
	{
		kt2=btupd(bt,word,(char*)&p,sizeof(info));
	}
	btcls(bt);

}

void Delete(GtkWidget *button,gpointer data)
{
	BTA *bt;
	bt=btopn("dict",0,0);
	kt2=btdel(bt,word);
	btcls(bt);
}
/*--------------------CHUC NANG TIM KIEM TRONG CUA SO MAIN--------
------------------------------VA CUA SO TOOL--------------------*/		

info search(GtkWidget *button,gpointer data)
{
        int i;
        char const *word=gtk_entry_get_text((GTK_ENTRY(data)));
        strcpy(word2,(char *)word);
	for (i=0;i<strlen(word);i++)
	  {
	    if (word2[i] >= 'A' && word2[i] <= 'Z') 
	      word2[i] = word2[i] - 'A' + 'a';
	  }
	BTA *bt;
	int rsize;
	bt=btopn("dict",0,0);
	kt=btsel(bt,word2,(char*)&p,sizeof(info),&rsize);
	btcls(bt);
	return p;
}

void Link_search(GtkWidget *button,gpointer data)
{
	int i;
	const char *tmp;
	char *word;
	tmp=gtk_button_get_label(GTK_BUTTON(button));
	word=(char *)tmp;
	for (i=0;i<strlen(word);i++)
	  {
	    if (word[i] >= 'A' && word[i] <= 'Z') 
	      word[i] = word[i] - 'A' + 'a';
	  }
	BTA *bt;
	int rsize;
	bt=btopn("dict",0,0);
	kt=btsel(bt,word,(char*)&p,sizeof(info),&rsize);
	btcls(bt);
	strcpy(word2,word);
}

void hiennghia(GtkWidget *button,gpointer data)
{

	GtkWidget *table1;
	GtkTextIter iter;
	GtkTextBuffer *buffer;
	GtkTextChildAnchor *anchor;
    GtkWidget *buttons1[50];
    int i,j;
	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (data));
	if(kt==0)
	{
	gchar text[100000];
	char c;
	FILE *f=fopen("anhviet.dict","rb");
	if(f==NULL)
	{
		perror("\nLoi mo file dict de doc:");
		return;
	}
	i=0;
	fseek(f,p.pos,SEEK_SET);
	for(j=0;j<p.len;j++)
	{
		fread(&c,sizeof(char),1,f);
		text[i++]=c;
	}
	text[i]='\0';
		gtk_text_buffer_set_text (buffer, text,-1);
	fclose(f);


	}
	else
	{
		GdkPixbuf *notfound;
		gchar text[50];
		//strcpy(text,"Not found ! :)\n");
		gtk_text_buffer_set_text (buffer, "",-1);
		gtk_text_buffer_get_end_iter(buffer,&iter);
		notfound = gdk_pixbuf_new_from_file ("./image/notfound.jpg", NULL);
		gtk_text_buffer_insert_pixbuf (buffer, &iter, notfound);
	}
	gtk_text_view_set_editable (GTK_TEXT_VIEW (data), FALSE);
}

void showSoundex(GtkWidget *button, gpointer data)
{
	int rsize;
	char *mau,soundex[5],listword[MAXLENG],xxx[50];
	list l[500];
	int kt,N,k,i,j;
	GtkTextIter iter;
	GtkTextChildAnchor *anchor;
	GdkPixbuf *undo;
	GtkTextBuffer *buffer;
	GtkWidget *table1;
	GtkWidget *buttons1[400][2];
		BTA *stree;
		buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (data));
		gtk_text_buffer_set_text (buffer, "",-1);
		stree=btopn("soundex_dict",0,0);
		if(stree==NULL)
		{
			Save_to_soundex_dict();
			stree=btopn("soundex_dict",0,0);
		}
	    mau=word2;
		SoundEx(soundex,mau,0,0);
		kt=btsel(stree,soundex,listword,MAXLENG*sizeof(char),&rsize);
		if(kt==0)
		{
			N=giai_ma(listword,mau,l);
			table1 = gtk_table_new (2, (N+1)/2, TRUE);
			gtk_table_set_row_spacings (GTK_TABLE (table1),5);
			gtk_table_set_col_spacings (GTK_TABLE (table1),5);
			gtk_text_buffer_get_end_iter(buffer,&iter);
			undo = gdk_pixbuf_new_from_file ("./image/icon_searchout.png", NULL);
			gtk_text_buffer_insert_pixbuf (buffer, &iter, undo);
			gtk_text_buffer_insert (buffer, &iter,"Có thể bạn muốn tìm :\n",-1);
			gtk_text_buffer_get_end_iter(buffer,&iter);
			gtk_text_buffer_insert (buffer, &iter,"",-1);
			gtk_text_buffer_get_end_iter(buffer,&iter);
			anchor = gtk_text_buffer_create_child_anchor (buffer, &iter);
			k=0;
			for (i = 0; i < (N+1)/2; i++)
			{
			for (j = 0; j <2; j++)
			{
				
				buttons1[i][j] = gtk_button_new_with_mnemonic (l[k++].word);
				g_signal_connect(G_OBJECT (buttons1[i][j]), "clicked",G_CALLBACK (Link_search),textview);
				g_signal_connect(G_OBJECT (buttons1[i][j]), "clicked",G_CALLBACK (hiennghia),textview);
				gtk_button_set_relief (GTK_BUTTON (buttons1[i][j]), GTK_RELIEF_NONE);
				gtk_table_attach_defaults (GTK_TABLE(table1), buttons1[i][j],i, i + 1,j, j + 1);
				if(k>=N)break;
			}
            }
			gtk_text_view_add_child_at_anchor (GTK_TEXT_VIEW (data), table1, anchor);
			gtk_widget_show_all(data);
		}

	btcls(stree);
	gtk_text_view_set_editable (GTK_TEXT_VIEW (data), FALSE);
}
/*------------AUTOCOMPLETE-------------------------
---------------------------------------------------*/
gboolean autocomplete(GtkEntry *entry,GdkEventKey *event)
{
	guint k = event->keyval;
	
	 if(k==gdk_keyval_from_name ("Insert")||k==gdk_keyval_from_name ("Tab"))
	{
		BTA *list_word;
		char word[20];
		long N_index;
		int i,rsize;
		FILE *f=fopen("anhviet.index","r");
		strcpy(word,gtk_entry_get_text(entry));
		list_word=btopn("list_word",0,0);
		if(list_word==NULL)
		{
			list_word=btcrt("list_word",0,0);
			char word[20],line[200],*wordtmp=NULL;
			N_index=demdong();
			for(i=0;i<N_index;i++)
			{
					fgets(line,200,f);
				
					if(dem_dau_tab(line)==2)
					{
						if(dem_dau_cach(line)>0)wordtmp=strtok(line," ");
							else wordtmp=strtok(line,"\t");
						if(dem_dau_gach(wordtmp)>0)
						wordtmp=strtok(word,"-");
						strcpy(word,wordtmp);
						btins(list_word,strdup(word),strdup(word),sizeof(word));
					}

			}
			fclose(f);
		}	
		 	char out[20],tmp[20];
			int kt,flag=0;
			
			if(dem_vong==0&&const_text==NULL)
				{
					kt=btins(list_word,strdup(word),strdup(word),sizeof(word));
					const_text=strdup(word);
				}
				else
			kt=btins(list_word,strdup(const_text),strdup(const_text),sizeof(word));
			strcpy(tmp,const_text);
			kt=btseln(list_word,tmp,out,sizeof(out),&rsize);
			if(kt==0)
			{
				for(i=0;i<=dem_vong;i++)
				{
					strcpy(tmp,out);
					kt=btseln(list_word,tmp,out,sizeof(out),&rsize);
					if(strncmp(const_text,out,strlen(const_text))!=0){flag=1;dem_vong=0;break;};                               
				}
				dem_vong++;
					if(flag==0)gtk_entry_set_text(entry,out); 

			}
				strcpy(tmp,const_text);
				btdel(list_word,tmp);
				btcls(list_word);
	} 
	else {const_text=NULL;dem_vong=0;return FALSE;}
	return TRUE; 
}

/*---------TAO ICON CUA SO MAIN VA CUA SO TOOL---------
-----------------------------------------------------*/
GdkPixbuf *create_pixbuf(const gchar * filename)
{
   GdkPixbuf *pixbuf;
   GError *error = NULL;
   pixbuf = gdk_pixbuf_new_from_file(filename, &error);
   if(!pixbuf) {
      fprintf(stderr, "%s\n", error->message);
      g_error_free(error);
   }

   return pixbuf;
}

GtkWidget *SetupWindow(gchar *data,gboolean Transient)
{
    GdkPixmap *background;
    GdkPixbuf *pixbuf;
    GdkScreen *ourscreen;
    GdkColormap *colormap;
    GtkStyle *style;
    GdkColor fg;
    GdkColor bg;
    GError *error = NULL;
    GdkRectangle *rect;
    GtkWidget *window;

    pixbuf = gdk_pixbuf_new_from_file ("./image/background.jpg",&error);
    gdk_pixbuf_render_pixmap_and_mask (pixbuf, &background, NULL, 0);
    style = gtk_style_new ();
    style->bg_pixmap[0] = background;
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (window), data);
    gtk_window_set_default_size(GTK_WINDOW(window),700,550);
    gtk_widget_set_style (GTK_WIDGET(window), GTK_STYLE(style));
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    return(window);
}

void window_add()
{
	GtkWidget *window;
	GtkWidget *label1,*label2,*label3,*label4;
	GtkWidget *entry;
	GtkWidget *toolbar;
	GtkToolItem *buttonsave, *buttoncancel;
	GtkWidget *textview;
	GtkWidget *hpane,*vpane,*vbox,*hpane2;
	GtkWidget *scrolled_win;
	
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(window), 500, 400);
	gtk_container_set_border_width (GTK_CONTAINER (window), 10);
	gtk_window_set_title (GTK_WINDOW (window), "Add");
	
	label1=gtk_label_new(NULL);
	gtk_label_set_markup (GTK_LABEL(label1),"<span font_desc='Bitstream Charter 14'>Add</span>");
	label2=gtk_label_new(NULL);
	gtk_label_set_markup (GTK_LABEL(label2),"<span font_desc='Bitstream Charter Italic 14'>             Word:</span>");
	label4=gtk_label_new(NULL);
	gtk_label_set_markup (GTK_LABEL(label4),"<span font_desc='Bitstream Charter Italic 8'>*press Enter to check the existence</span>");
	label3=gtk_label_new(NULL);
	gtk_label_set_markup (GTK_LABEL(label3),"<span font_desc='Bitstream Charter Italic 14'>Mean:                    </span>");
	
	entry=gtk_entry_new();
	toolbar = gtk_toolbar_new ();
	buttonsave = gtk_tool_button_new_from_stock(GTK_STOCK_APPLY);
	buttoncancel = gtk_tool_button_new_from_stock(GTK_STOCK_CANCEL);
	gtk_toolbar_insert (GTK_TOOLBAR (toolbar), buttonsave, 0);
	gtk_toolbar_insert (GTK_TOOLBAR (toolbar), buttoncancel, 1);
	gtk_toolbar_set_show_arrow (GTK_TOOLBAR (toolbar), FALSE);

	textview=gtk_text_view_new();
	scrolled_win = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_win),GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
	hpane = gtk_hpaned_new ();
	hpane2 = gtk_hpaned_new ();
	vbox=gtk_vbox_new(FALSE,5);
	
	g_signal_connect (G_OBJECT(entry), "activate",G_CALLBACK(search),entry);
	g_signal_connect (G_OBJECT(entry), "activate",G_CALLBACK (thongbao3),(gpointer)window);
	g_signal_connect (G_OBJECT(buttonsave), "clicked",G_CALLBACK (get_from_entry),entry);
	g_signal_connect (G_OBJECT(buttonsave), "clicked",G_CALLBACK (get_from_text_view),textview);
	g_signal_connect (G_OBJECT(buttonsave), "clicked",G_CALLBACK (ghifile),NULL);
	g_signal_connect (G_OBJECT(buttonsave), "clicked",G_CALLBACK (thongbao2),(gpointer)window);
	g_signal_connect (G_OBJECT (buttoncancel), "clicked",G_CALLBACK (question),window);

	gtk_container_add (GTK_CONTAINER (scrolled_win), textview);
	gtk_paned_pack1 (GTK_PANED (hpane), label1, TRUE, TRUE);
	gtk_paned_pack2 (GTK_PANED (hpane), toolbar, FALSE, TRUE);
	gtk_paned_pack1 (GTK_PANED (hpane2), label2, TRUE, TRUE);
	gtk_paned_pack2 (GTK_PANED (hpane2), label4, FALSE, TRUE);
	gtk_box_pack_start(GTK_BOX (vbox), hpane,FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX (vbox), hpane2, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX (vbox), entry, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX (vbox), label3, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX (vbox), scrolled_win, TRUE, TRUE, 0);
	
	gtk_container_add(GTK_CONTAINER(window),vbox);
	gtk_window_set_icon(GTK_WINDOW(window), create_pixbuf("./image/logo.png"));
	gtk_widget_show_all(window);
}

void window_delete()
{
	GtkWidget *window;
	GtkWidget *label1;
	GtkWidget *entry;
	GtkWidget *toolbar;
	GtkToolItem *buttonsave, *buttoncancel;
	GtkWidget *textview;
	GtkWidget *hpane,*vpane,*vbox;
	GtkWidget *scrolled_win;
	
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
	gtk_container_set_border_width (GTK_CONTAINER (window), 10);
	gtk_window_set_title (GTK_WINDOW (window), "Delete");
	
	label1=gtk_label_new(NULL);
	gtk_label_set_markup (GTK_LABEL(label1),"<span font_desc='Bitstream Charter Italic 14'>Word:</span>");
	
	entry=gtk_entry_new();
	toolbar = gtk_toolbar_new ();
	buttonsave = gtk_tool_button_new_from_stock(GTK_STOCK_APPLY);
	buttoncancel = gtk_tool_button_new_from_stock(GTK_STOCK_CANCEL);
	gtk_toolbar_insert (GTK_TOOLBAR (toolbar), buttonsave, 0);
	gtk_toolbar_insert (GTK_TOOLBAR (toolbar), buttoncancel, 1);
	gtk_toolbar_set_show_arrow (GTK_TOOLBAR (toolbar), FALSE);
	hpane = gtk_hpaned_new ();
	vbox=gtk_vbox_new(FALSE,5);
	g_signal_connect (G_OBJECT(entry), "activate",G_CALLBACK (get_from_entry),entry);
	g_signal_connect (G_OBJECT(entry), "activate",G_CALLBACK (Delete),NULL);
	g_signal_connect (G_OBJECT(entry), "activate",G_CALLBACK (thongbao2),(gpointer)window);
	g_signal_connect_swapped (G_OBJECT (entry), "activate",G_CALLBACK (gtk_widget_destroy),(gpointer) window);

	g_signal_connect (G_OBJECT(buttonsave), "clicked",G_CALLBACK (get_from_entry),entry);
	g_signal_connect (G_OBJECT(buttonsave), "clicked",G_CALLBACK (Delete),NULL);
	g_signal_connect (G_OBJECT(buttonsave), "clicked",G_CALLBACK (thongbao2),(gpointer)window);
	g_signal_connect_swapped (G_OBJECT (buttonsave), "clicked",G_CALLBACK (gtk_widget_destroy),(gpointer) window);
	g_signal_connect (G_OBJECT (buttoncancel), "clicked",G_CALLBACK (question),window);
	
	gtk_paned_pack1 (GTK_PANED (hpane), label1, TRUE, TRUE);
	gtk_paned_pack2 (GTK_PANED (hpane), toolbar, FALSE, TRUE);
	gtk_box_pack_start(GTK_BOX (vbox), hpane,FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX (vbox), entry, FALSE, FALSE, 5);
	gtk_container_add(GTK_CONTAINER(window),vbox);
	gtk_window_set_icon(GTK_WINDOW(window), create_pixbuf("./image/logo.png"));
	gtk_widget_show_all(window);
}

int main(int argc,char *argv[])
{
	BTA *bt;
	GtkWidget *window;
	GtkWidget *label, *label_savecancel;
	GtkWidget *searchbar;
	GtkWidget *icon;
	GtkWidget *button_search,*button_edit,*button_about,*button4;
	GtkWidget *toolbar_search, *toolbar_edit, *toolbar, *toolbar_savecancel;
	GtkWidget *hbox1,*hbox2,*vbox;
	GtkWidget *scrolled_win, *textviewSoundex, *scrolledSoundex;
	GtkWidget *hpanesearch, *hpanelabel, *hpanesearchedit,*hpanesavecancel, *vpane1, *vpane2, *vpane3, *vpaneTextSound;

	GtkToolItem *buttonsave, *buttoncancel;
	GtkToolItem *icon_search, *icon_edit, *icon_clear, *icon_about, *icon_add, *item;
	GtkIconFactory *factory;
	gint i=0;

	gtk_init (&argc, &argv);
	window=SetupWindow("EV Dictionary",TRUE);

	label=gtk_label_new(NULL);
	gtk_label_set_markup (GTK_LABEL(label),"<span font_desc='Ubuntu Mono Bold 20'><span foreground='white'><span background='red'>HKT</span></span></span><span background='white'><span font_desc='Ubuntu Mono Bold 20'><span foreground='red'>dict</span></span></span>");
	label_savecancel=gtk_label_new(NULL);
	gtk_label_set_markup (GTK_LABEL(label_savecancel),"<span font_desc='Nimbus Mono L Bold 14'>Edit Mode</span>");

	factory = gtk_icon_factory_new ();
	toolbar = gtk_toolbar_new ();
	toolbar_search = gtk_toolbar_new ();
	toolbar_edit = gtk_toolbar_new ();
	GtkWidget *custom_icon = gtk_image_new_from_file("./image/icon_search.png");
	icon_search=gtk_tool_button_new (custom_icon,"Search");
	custom_icon = gtk_image_new_from_file("./image/icon_edit.png");
	icon_edit=gtk_tool_button_new (custom_icon,"Edit");
	custom_icon = gtk_image_new_from_file("./image/icon_add.png");
	icon_add=gtk_tool_button_new (custom_icon,"Add");
	custom_icon = gtk_image_new_from_file("./image/icon_delete.png");
	icon_clear=gtk_tool_button_new (custom_icon,"Delete");
	custom_icon = gtk_image_new_from_file("./image/icon_about.png");
	icon_about=gtk_tool_button_new (custom_icon,"About");
	gtk_toolbar_insert (GTK_TOOLBAR (toolbar), icon_add, 0);
	gtk_toolbar_insert (GTK_TOOLBAR (toolbar), icon_clear, 1);
	gtk_toolbar_insert (GTK_TOOLBAR (toolbar), icon_about, 2);
	gtk_toolbar_insert (GTK_TOOLBAR (toolbar_edit), icon_edit, 0);
	gtk_toolbar_insert (GTK_TOOLBAR (toolbar_search), icon_search, 0);
	gtk_toolbar_set_style (GTK_TOOLBAR (toolbar_search), GTK_TOOLBAR_ICONS);
	gtk_toolbar_set_style (GTK_TOOLBAR (toolbar), GTK_TOOLBAR_ICONS);
	gtk_toolbar_set_show_arrow (GTK_TOOLBAR (toolbar_edit), FALSE);
	gtk_toolbar_set_show_arrow (GTK_TOOLBAR (toolbar), FALSE);
	gtk_toolbar_set_show_arrow (GTK_TOOLBAR (toolbar_search), FALSE);
	
	searchbar=gtk_entry_new();

	toolbar_savecancel = gtk_toolbar_new ();
	buttonsave = gtk_tool_button_new_from_stock(GTK_STOCK_APPLY);
	buttoncancel = gtk_tool_button_new_from_stock(GTK_STOCK_CANCEL);
	gtk_toolbar_insert (GTK_TOOLBAR (toolbar_savecancel), buttonsave, 0);
	gtk_toolbar_insert (GTK_TOOLBAR (toolbar_savecancel), buttoncancel, 1);
	gtk_toolbar_set_show_arrow (GTK_TOOLBAR (toolbar_savecancel), FALSE);

	hpanesearch = gtk_hpaned_new ();
	hpanelabel = gtk_hpaned_new ();
	hpanesearchedit = gtk_hpaned_new ();
	hpanesavecancel = gtk_hpaned_new ();
	vpane1 = gtk_vpaned_new ();
	vpane2 = gtk_vpaned_new ();
	vpane3 = gtk_vpaned_new ();
	vpaneTextSound = gtk_vpaned_new ();
	textview = gtk_text_view_new ();
	textviewSoundex = gtk_text_view_new ();
	scrolled_win = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_win),GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
	scrolledSoundex = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledSoundex),GTK_POLICY_AUTOMATIC, GTK_POLICY_NEVER);
	
	bt=btopn("dict",0,0);
	if(bt==NULL)
	{
		SaveToBtree();
	}
	btcls(bt);

	g_signal_connect (G_OBJECT (searchbar), "activate",G_CALLBACK (search),searchbar);
	g_signal_connect (G_OBJECT (searchbar), "activate",G_CALLBACK (hiennghia),textview);
	g_signal_connect (G_OBJECT (searchbar), "activate",G_CALLBACK (show),scrolled_win);
	g_signal_connect (G_OBJECT (searchbar), "activate",G_CALLBACK (show),toolbar_edit);
	g_signal_connect (G_OBJECT (searchbar), "activate",G_CALLBACK (uneditableMean),textview);
	g_signal_connect (G_OBJECT (searchbar), "activate",G_CALLBACK (show),scrolledSoundex);
	g_signal_connect (G_OBJECT (searchbar), "activate",G_CALLBACK (showSoundex),textviewSoundex);
	g_signal_connect (GTK_ENTRY (searchbar),"key-press-event",G_CALLBACK(autocomplete),NULL);
	
	g_signal_connect (G_OBJECT (icon_search), "clicked",G_CALLBACK (search),searchbar);
	g_signal_connect (G_OBJECT (icon_search), "clicked",G_CALLBACK (search),searchbar);
	g_signal_connect (G_OBJECT (icon_search), "clicked",G_CALLBACK (hiennghia),textview);
	
	g_signal_connect (G_OBJECT (icon_edit), "clicked",G_CALLBACK (show), hpanesavecancel);
	g_signal_connect (G_OBJECT (icon_edit), "clicked",G_CALLBACK (hide), scrolledSoundex);
	g_signal_connect (G_OBJECT (icon_edit), "clicked",G_CALLBACK (editableMean), textview);
	g_signal_connect (G_OBJECT (icon_edit), "clicked",G_CALLBACK (changeFontOfTextview), textview);
	
	g_signal_connect (G_OBJECT (buttonsave), "clicked",G_CALLBACK (get_from_entry),searchbar);
	g_signal_connect (G_OBJECT (buttonsave), "clicked",G_CALLBACK (uneditableMean),textview);
	g_signal_connect (G_OBJECT (buttonsave), "clicked",G_CALLBACK (defaultFontOfTextview),textview);
	g_signal_connect (G_OBJECT (buttonsave), "clicked",G_CALLBACK (get_from_text_view),textview);
	g_signal_connect (G_OBJECT (buttonsave), "clicked",G_CALLBACK (ghifile),NULL);
	g_signal_connect (G_OBJECT (buttonsave), "clicked",G_CALLBACK (thongbao2),(gpointer)window);
	g_signal_connect (G_OBJECT (buttonsave), "clicked",G_CALLBACK (hide),hpanesavecancel);
	g_signal_connect (G_OBJECT (buttonsave), "clicked",G_CALLBACK (show),scrolledSoundex);
	
	g_signal_connect (G_OBJECT (buttoncancel), "clicked",G_CALLBACK (defaultFontOfTextview),textview);
	g_signal_connect (G_OBJECT (buttoncancel), "clicked",G_CALLBACK (uneditableMean),textview);
	g_signal_connect (G_OBJECT (buttoncancel), "clicked",G_CALLBACK (hide),hpanesavecancel);
	g_signal_connect (G_OBJECT (buttoncancel), "clicked",G_CALLBACK (show),scrolledSoundex);
	
	g_signal_connect (G_OBJECT (icon_add), "clicked",G_CALLBACK (window_add),NULL);
	g_signal_connect (G_OBJECT (icon_clear), "clicked",G_CALLBACK (window_delete),NULL);
	g_signal_connect (G_OBJECT (icon_about), "clicked",G_CALLBACK (thongtin),(gpointer)window);
	
	g_signal_connect_swapped (G_OBJECT (window), "destroy",G_CALLBACK (quit),NULL);


	gtk_container_add(GTK_CONTAINER(hpanesearch),toolbar_search);
	gtk_container_add(GTK_CONTAINER(hpanesearch),searchbar);
	gtk_paned_pack1 (GTK_PANED (hpanesearchedit), hpanesearch, TRUE, TRUE);
	gtk_paned_pack2 (GTK_PANED (hpanesearchedit), toolbar_edit, FALSE, TRUE);
	gtk_paned_pack1 (GTK_PANED (hpanelabel), label, TRUE, TRUE);
	gtk_paned_pack2 (GTK_PANED (hpanelabel), toolbar, FALSE, TRUE);
	gtk_container_add(GTK_CONTAINER(vpane1),hpanelabel);
	gtk_container_add(GTK_CONTAINER(vpane1),hpanesearchedit);
	gtk_container_add(GTK_CONTAINER(vpane2),vpane1);
	gtk_container_add (GTK_CONTAINER (scrolled_win), textview);
	gtk_container_add (GTK_CONTAINER (scrolledSoundex), textviewSoundex);
	gtk_container_add(GTK_CONTAINER(vpane2),scrolled_win);
	gtk_paned_pack1 (GTK_PANED (vpaneTextSound), vpane2, TRUE, TRUE);
	gtk_paned_pack2 (GTK_PANED (vpaneTextSound), scrolledSoundex, FALSE, TRUE);
	gtk_paned_pack1 (GTK_PANED (hpanesavecancel), label_savecancel, TRUE, TRUE);
	gtk_paned_pack2 (GTK_PANED (hpanesavecancel), toolbar_savecancel, FALSE, TRUE);
	gtk_paned_pack1 (GTK_PANED (vpane3), vpaneTextSound, TRUE, TRUE);
	gtk_paned_pack2 (GTK_PANED (vpane3), hpanesavecancel, FALSE, TRUE);
	gtk_container_add (GTK_CONTAINER (window), vpane3);
	gtk_window_set_icon(GTK_WINDOW(window), create_pixbuf("./image/logo.png"));
	gtk_widget_show_all (window);
	gtk_widget_hide (scrolled_win);
	gtk_widget_hide (toolbar_edit);
	gtk_widget_hide (hpanesavecancel);
	gtk_widget_hide (scrolledSoundex);			
	gtk_main ();	
	return 1;
}
//gcc -Wall -g thinghiem.c -o thinghiem `pkg-config --cflags gtk+-2.0` `pkg-config --libs gtk+-2.0`
//gcc `pkg-config --cflags gtk+-2.0` -o a tudienbtl.c `pkg-config --libs gtk+-2.0` libbt.a -lm