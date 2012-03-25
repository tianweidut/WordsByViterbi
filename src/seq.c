 /*=============================================================================
#
# Author: tianweidut - liutianweidlut@gmail.com
#
# QQ : 416774905
#
# Last modified:	2010-12-08 14:27
#
# Filename:		seq.c
#
# Description: ������ĵ������л�����:0(��ʾδ��¼�ʻ�) 
#
=============================================================================*/ 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

#define  ch_dict_file       "./key/ch_dict_file.crt"      //���Ĵʵ�
#define  seq_dict_file      "./key/seq.seq"              //���дʵ�
#define  src_dict_file      "./key/tst.crt+copy"              //���дʵ�

#define LineMAX     8000        //ÿ�仰�ַ�����
#define sig         40
#define WordMAX     30000       //���ĵ������ֵ
#define SenMAX      4000        //��������

int sen_len = 0;            //������
int word_len=0;             //������
char HANZI[sig];            //���ִ洢
int  ch_matrix_len =0;      //���ĵ�������
char ch_matrix[WordMAX+1][80];    //���ĵ���
int  hero[SenMAX+1][LineMAX+1];     //���ӵ��ʱ�

FILE *src_file=NULL;        //����Դ�ļ�

void init(char *argv1,char *argv3);
void write_into_arr(void);
void write_into_file(char *argv2);

int main(int argc,char *argv[])  //1:ch_dict 2:seq_dict 3:src_file
{

    char *idx=NULL;
    char *start=NULL;
    char *end=NULL;
    char *line=NULL;
    char str[LineMAX];      

    
    init(argv[1],argv[3]);

    while((line=fgets(str,LineMAX,src_file))!=NULL)   //�б���
    {
        sen_len ++;
        printf(".");
        start = line;
        word_len = 0;       //ÿ�仰�е�������

        while((end=strchr(start,' '))!=NULL)    //���ӱ���
        {
            memset(HANZI,'\0',sig-1);   //���ʹ�λ
            strncpy(HANZI,start,(int)(end-start));  //��ʱΪ�ӻ����Ĵʻ�
            word_len++;
            start = ++end;      //�˴����׳���
            write_into_arr(); 
        } 
        hero[sen_len][0] = word_len;
    }
    printf("\n--����--\n"); 
    write_into_file(argv[2]);

    exit(EXIT_SUCCESS);
}
void write_into_arr(void)
{
    //��HANZI����
    int cur_idx=0;
    for(int i=1;i<=ch_matrix_len;i++)
    {
        if(0 == strcmp(HANZI,ch_matrix[i]))
        {
            cur_idx = i;    //��������
        }
    }
    hero[sen_len][word_len]=cur_idx;
    
} 
void init(char *argv1,char *argv3)
{
    //��ȡ�ļ���Ϣ
    FILE *ch_dict=NULL;         //���ĵ����ļ�

    /*if((ch_dict=fopen(ch_dict_file,"a+"))==NULL)    //W+����ļ�����*/
    if((ch_dict=fopen(argv1,"a+"))==NULL)    //W+����ļ�����
    {
        perror("ch_dict open failed");
        exit(EXIT_FAILURE);
    }
    /*if((src_file=fopen(src_dict_file,"a+"))==NULL)*/
    if((src_file=fopen(argv3,"a+"))==NULL)
    {
        perror("src_file open failed");
        exit(EXIT_FAILURE);
    }
    
    printf("---------------��ʼ��------------\n");
    fscanf(ch_dict,"cnt:%d\n",&ch_matrix_len);
    printf("���ĵ�������:%d\n",ch_matrix_len);
    for(int i=1;i<=ch_matrix_len;i++)
    {
        fscanf(ch_dict,"%d::%s\n",&i,ch_matrix[i]);
    }
    
    fclose(ch_dict);
}

void write_into_file(char *argv2)
{
    FILE *seq_dict=NULL;        //�����ļ�
    /*if((seq_dict=fopen(seq_dict_file,"w+"))==NULL)*/
    if((seq_dict=fopen(argv2,"w+"))==NULL)
    {
        perror("seq_dict open failed");
        exit(EXIT_FAILURE);
    }
    //��seq�ļ�д������
    printf("ͳ�ƽ��:��������:%d\n",sen_len);
    for(int i=1;i<=sen_len;i++)
    {
        fprintf(seq_dict,"T= %d\n",hero[i][0]);
        for(int j=1;j<=hero[i][0];j++)
        {
            fprintf(seq_dict,"%d ",hero[i][j]);
        }
        fprintf(seq_dict,"\n");
    }

    fclose(seq_dict);
}
