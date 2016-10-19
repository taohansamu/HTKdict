#include <stdio.h>
#include <string.h>
#include "tudien.h"
#include "btree.h"
int main()
{
BTA *list_word;
		char word[20];
		long N_index;
		int i,rsize;
		FILE *f=fopen("anhviet.index","r");
		printf("Nhap vao tu:");scanf("%s",word);
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
						printf("%s\t",wordtmp);	
						strcpy(word,wordtmp);
						btins(list_word,strdup(word),strdup(word),sizeof(word));
					}

			}
			fclose(f);
		}	
		 	char out[20],tmp[20];
			int kt;
			kt=btins(list_word,strdup(word),strdup(word),sizeof(word));
			printf("kt chen vao %d\n",kt);
			//do
			//{
				kt=btseln(list_word,strdup(word),out,sizeof(out),&rsize);
			//	printf("%s\n",out);
				if(kt==0)
				while(1)
				{
					strcpy(tmp,out);
					btseln(list_word,tmp,out,sizeof(out),&rsize);
					if(strncmp(out,word,strlen(word))!=0)break;
					printf("%s\n",out);

				}
				btdel(list_word,word);
				//printf("Cac tu de hoan thanh:%s\t",out);
				printf("kt tu next:%d\n",kt);
				printf("Cac tu de hoan thanh:%s\t",out);
				//if(strncmp(word,out,strlen(word))!=0)break;
			//}//while(1);
				btcls(list_word);


}
