void TOOL();

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "btree.h"
#include <math.h>

#define MAXLENG 3000
typedef struct 
{
	int triso;
	char word[100];
}list;
typedef struct 
{
  long pos;
  long len;
}info;
//----CHUYEN CHU HOA THANH CHU THUONG-------

char *str_lwr(char *word)
{
  int i;
  for (i=0;i<strlen(word);i++)
    {
      if (word[i] >= 'A' && word[i] <= 'Z') 
        word[i] = word[i] - 'A' + 'a';
    }
    return strdup(word);
}
//HAM DUNG DE CHUYEN TU CO SO 64 VE CO SO 10
//
//
int GetDemicalValue (char*str)
{

  int i,j;
   char base64[]= "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    long decValue = 0;
    int len=strlen(str);
    for (i = 0; i<len; i++)
    {
      char c=str[i];
      for(j=0;j<64;j++)
      {
        if(c==base64[j])break;
      }
          decValue += (long)pow(64,len-i-1)*j;
 }
return decValue;
}
//HAM DEM SO DONG TRONG FILE INDEX
//
//
long demdong()
{
  long number=0;
  char s[100];
  FILE *f=fopen("anhviet.index","r");
  while(!feof(f))
  {
    fgets(s,200,f);
    number++;
  }
  fclose(f);
  return number;
}
void SaveToBtree()
{
  char s[30];
  BTA *bt;
  int i;
  long N;
    FILE *f=fopen("anhviet.index","r");
  bt=btcrt("dict",0,0);
  if(f==NULL)
  {
    perror("\nLoi mo file de doc:");
    return;
  }
  N=demdong();
  for(i=0;i<N;i++)
  {
    char word[100];
    info p;
    fscanf(f,"%[^\t]",word);
    fscanf(f,"%s\t",s);
    p.pos=GetDemicalValue(s);
    fscanf(f,"%s%*c",s);
    p.len=GetDemicalValue(s);
    btins(bt,word,(char *)&p,sizeof(info));
  }
  btcls(bt);
  fclose(f);
}
/*---------------HAM TINH SOUNDEX CUA TU--------------------
------------------------------------------------------------*/
         
 int SoundEx(char *SoundEx,
              char *WordString,
              int   LengthOption,
              int   CensusOption)
  {
      int  InSz = 31;
      char WordStr[32];     /* one bigger than InSz */
      int  SoundExLen, WSLen, i;
      char FirstLetter, *p, *p2;

      SoundExLen = WSLen = 0;
      SoundEx[0] = 0;

      if(CensusOption) {
        LengthOption = 4;
      }

      if(LengthOption) {
          SoundExLen = LengthOption;
      }
      if(SoundExLen > 10) {
          SoundExLen = 10;
      }
      if(SoundExLen < 4) {
          SoundExLen = 4;
      }

      if(!WordString) {
          return(0);
      }


      for(p = WordString,p2 = WordStr,i = 0;(*p);p++,p2++,i++) {
        if(i >= InSz) break;
        (*p2) = (*p);
      }
      (*p2) = 0;

      for(p = WordStr;(*p);p++) {
        if( (*p) >= 'a' && (*p) <= 'z' ) {
            (*p) -= 0x20;
        }
      }


      for(p = WordStr;(*p);p++) {
        if( (*p) < 'A' || (*p) > 'Z' ) {
            (*p) = ' ';
        }
      }

      for(i = 0, p = p2 = WordStr;(*p);p++) {
          if(!i) {
              if( (*p) != ' ' ) {
                  (*p2) = (*p);
                  p2++;
                  i++;
              }
          }
          else {
              (*p2) = (*p);
              p2++;
          }
      }
      (*p2) = 0;

      /* Get length of WordStr
      */
      for(i = 0,p = WordStr;(*p);p++) i++;


      /* Remove trailing spaces
      */
      for(;i;i--) {
          if(WordStr[i] == ' ') {
            WordStr[i] = 0;
          }
          else {
              break;
          }
      }

      /* Get length of WordStr
      */
      for(WSLen = 0,p = WordStr;(*p);p++) WSLen++;

      if(!WSLen) {
        return(0);
      }

      if(!CensusOption) {
          if(WordStr[0] == 'P' && WordStr[1] == 'S') {
              WordStr[0] = '_';
          }

          if(WordStr[0] == 'P' && WordStr[1] == 'F') {
              WordStr[0] = '_';
          }


          if(WordStr[0] == 'G' && WordStr[1] == 'H') {
              WordStr[1] = '_';
          }

          for(i = 0;i < WSLen;i++) {
              if(WordStr[i] == 'D' && WordStr[i+1] == 'G') {
                  WordStr[i] = '_';
                  i++;
                  continue;
              }
              if( WordStr[i] == 'G' && WordStr[i+1] == 'H') {
                  WordStr[i] = '_';
                  i++;
                  continue;
              }
              if(WordStr[i] == 'K' && WordStr[i+1] == 'N') {
                  WordStr[i] = '_';
                  i++;
                  continue;
              }
              if(WordStr[i] == 'G' && WordStr[i+1] == 'N') {
                  WordStr[i] = '_';
                  i++;
                  continue;
              }
              if(WordStr[i] == 'M' && WordStr[i+1] == 'B') {
                  WordStr[i+1] = '_';
                  i++;
                  continue;
              }

              if(WordStr[i] == 'P' && WordStr[i+1] == 'H') {
                  WordStr[i] = 'F';
                  WordStr[i+1] = '_';
                  i++;
                  continue;
              }
              if(WordStr[i]  ==  'T'  &&
                 WordStr[i+1] == 'C' &&
                 WordStr[i+2] == 'H'
                ) {

                  WordStr[i] = '_';
                  i++; i++;
                  continue;
              }
              if(WordStr[i] == 'M' && WordStr[i+1] == 'P'
                 && (WordStr[i+2] == 'S' ||
                     WordStr[i+2] == 'T' ||
                     WordStr[i+2] == 'Z')
                ) {
                  WordStr[i+1] = '_';
                  i++;
              }
          }
      } /* end if(!CensusOption) */

      for(p = p2 = WordStr;(*p);p++) {
        (*p2) = (*p);
        if( (*p2) != '_' ) {
            p2++;
        }
      }
      (*p2) = 0;

      FirstLetter = WordStr[0];

      if(FirstLetter == 'H' || FirstLetter == 'W') {
          WordStr[0] = '-';
      }

      if(CensusOption == 1) {
          for(p = &(WordStr[1]);(*p);p++) {
             if((*p) == 'H' || (*p) == 'W') {
                 (*p) = '.';
             }
          }
      }


      for(p = WordStr;(*p);p++) {
          if( (*p) == 'A'   ||
              (*p) == 'E'   ||
              (*p) == 'I'   ||
              (*p) == 'O'   ||
              (*p) == 'U'   ||
              (*p) == 'Y'   ||
              (*p) == 'H'   ||
              (*p) == 'W'
            ){
              (*p) = '0';   /* zero */
          }
          if( (*p) == 'B'   ||
              (*p) == 'P'   ||
              (*p) == 'F'   ||
              (*p) == 'V'
            ){
              (*p) = '1';
          }
          if( (*p) == 'C'   ||
              (*p) == 'S'   ||
              (*p) == 'G'   ||
              (*p) == 'J'   ||
              (*p) == 'K'   ||
              (*p) == 'Q'   ||
              (*p) == 'X'   ||
              (*p) == 'Z'
            ){
              (*p) = '2';
          }
          if( (*p) == 'D'   ||
              (*p) == 'T'
            ){
              (*p) = '3';
          }
          if( (*p) == 'L' ) {
              (*p) = '4';
          }

          if( (*p) == 'M'   ||
              (*p) == 'N'
            ){
              (*p) = '5';
          }
          if( (*p) == 'R' ) {
              (*p) = '6';
          }
      }

      if(CensusOption == 1) {

          for(p = p2 = &WordStr[1];(*p);p++) {
            (*p2) = (*p);
            if( (*p2) != '.' ) {
                p2++;
            }
          }
          (*p2) = 0;
      }

      for(p = p2 = &(WordStr[0]);(*p);p++) {
        (*p2) = (*p);
        if( (*p2) != p[1] ) {
            p2++;
        }
      }
      (*p2) = 0;
      for(p = p2 = &WordStr[1];(*p);p++) {
        (*p2) = (*p);
        if( (*p2) != ' ' && (*p2) != '0' ) {
            p2++;
        }
      }
      (*p2) = 0;

      for(WSLen = 0,p = WordStr;(*p);p++) WSLen++;

      for(i = WSLen;i < SoundExLen;i++ ) {
          WordStr[i] = '0';
      }

      WordStr[SoundExLen] = 0;
      WordStr[0] = FirstLetter;
      for(p2 = SoundEx,p = WordStr;(*p);p++,p2++) {
          (*p2) = (*p);
      }
      (*p2) = 0;
      
      return(SoundExLen);
  }
  /*-----DEM DAU CACH TRONG MOT TU-------------*/
int dem_dau_cach(char *s)
{
	int i,sum=0;
	for(i=0;i<strlen(s);i++)
	{
		if(s[i]==' ')
			sum++;
	}
	return sum;
}
  /*-----DEM DAU TAB TRONG MOT TU-------------*/
int dem_dau_tab(char *s)
{int i,sum=0;
	for(i=0;i<strlen(s);i++)
	{
		if(s[i]=='\t')
			sum++;
	}
	return sum;
}	
  /*-----DEM DAU '-' TRONG MOT TU-------------*/
int dem_dau_gach(char *s)
{int i,sum=0;
	for(i=0;i<strlen(s);i++)
	{
		if(s[i]=='-')
			sum++;
	}
	return sum;
}	
  /*-----LUU CAC SOUNDEX-------------*/
void Save_to_soundex_dict()
{
BTA *stree,*da_doc;
	int i,kt;
	int N;
	char line[200];
	stree=btopn("soundex_dict",0,0);
	FILE *f=fopen("anhviet.index","r");
	stree=btcrt("soundex_dict",0,0);
	da_doc=btcrt("readed",0,0);
	if(f==NULL)
	{
		perror("\nLoi mo file de doc:");
		return;
	}
	N=demdong();
		char *word;
		for(i=0;i<N;i++)
		{
			int rsize;
			char listword[MAXLENG],wordtmp[MAXLENG];
			fgets(line,200,f);
			if(dem_dau_tab(line)==2)
			{
				if(dem_dau_cach(line)>0)word=strtok(line," ");
				else word=strtok(line,"\t");
				if(dem_dau_gach(word)>0);
				word=strtok(word,"-");
				strcpy(wordtmp,word);
				if(strlen(word)>=3)
				{
					char soundex[5];
					kt=-1;
					kt=binsky(da_doc,word,1);
					if(kt==0)
					{
						kt=-1;
						SoundEx(soundex,word,0,0);
						kt=btsel(stree,soundex,listword,sizeof(listword),&rsize);
						if(kt==0)
						{
							strcat(listword,"\t");
							strcat(listword,word);
							btupd(stree,soundex,listword,sizeof(listword));
						}
						else
							btins(stree,soundex,wordtmp,sizeof(listword));
					}
				}
			}

		}
	fclose(f);
	btcls(da_doc);
	btcls(stree);	
}

int giai_ma(char *in,char *mau,list *l)
{
	int i,j,N;
	char *tmp,out[MAXLENG];
	strcpy(out,in);
	tmp=out;
	i=0;	
  while(tmp)
	{
		if(i==0)
		tmp=strtok(out,"\t");
		else tmp=strtok(NULL,"\t");
		if(tmp!=NULL)
		{
			l[i].triso=abs(strncmp(tmp,mau,strlen(mau)));
			strcpy(l[i].word,tmp);
		}
		i++;
	}
	N=i-1;
	for(i=0;i<N-1;i++)
		for(j=i+1;j<N;j++)
		{
			if(l[j].triso<l[i].triso)
			{
				list tmp;
				tmp=l[i];
				l[i]=l[j];
				l[j]=tmp;
			}
		}
    return N;
}