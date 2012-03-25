 /*=============================================================================
#
# Author: tianweidut - liutianweidlut@gmail.com
#
# QQ : 416774905
#
# Last modified:	2010-12-05 22:37
#
# Filename:		moveMatrix.c
#
# Description:ת�ƾ��������� 
#
=============================================================================*/ 
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
//#include "moveMatrix.h"

#define DEBUG 1     //���Կ���
#define SIZE  80   //���ʴ�������
#define LineMAX  8000   //ÿ�仰�洢���ַ����� 
#define WordMAX  30000  //�ʻ��ܵ����� 
#define sig      40     //���ʳ���

struct chin
{
    //int ch_idx;     //��������
    int ch_cnt;     //���ֳ���Ƶ�ʣ���Ӧÿ�����ԣ�
};

struct map_str
{ 
    struct chin ch[WordMAX+1];        //ÿ�����Զ�Ӧ�ĵ���  
    int  flag_cnt;             //���Գ�������
};

double move_matrix[SIZE+1][SIZE+1];     //ת�ƾ�������M^2�ľ���
double pi_matrix[SIZE+1];               //��ʼ����
char flag_matrix[SIZE+1][8];                 //���Դ洢���飬��ÿ�����Ա�ǳ�һ�����֣���flag[0]�洢�ǼǴ���
char ch_matrix[WordMAX+1][50];         //���ִʻ�洢����,��ÿһ���ʱ�ǳ�һ������!!!ע��Խ������
struct map_str map_matrix[SIZE+1];                //�����뵥�ʵ�ӳ��,����-����-��������

int flag_matrix_len =0;
int word_matrix_len =0;
int map_matrix_len =0;
int sen_len=0;          //��������
int sen_new=0;          //�¾��ӱ�ʾ
double word_all = 0.0;      //���е�������
char flag[sig];             //����
char HANZI[sig];            //����

#define  ch_dict_file       "./key/ch_dict_file.crt"      //���Ĵʵ�
#define  flag_dict_file     "./key/flag_dict_file.crt"    //���Դʵ�
#define  move_dict_file     "./key/move_dict_file.crt"    //ת�Ƹ���
#define  map_dict_file      "./key/map_dict_file.crt"     //�������
#define  pi_dict_file       "./key/pi.crt"                //��ʼ����

//int copy_to_matrix(char *HANZI,char *flag);
void init(void);            //����״̬��ʼ��

int copy_to_matrix(void);
int write_flag(int old,int new);
void write_arr_to_file(void);

int main(int argc,char *argv[])
{
    FILE *source_file=NULL;
    char str[LineMAX];
    
    char *idx=NULL;
    char *start=NULL;
    int flag_len=0;         //���Գ���
    int word_len=0;         //���ĵ��ʳ���

    init();
    if((source_file=fopen(argv[1],"a+"))==NULL)
   // if((source_file=fopen("BAIKE002.crt","a+"))==NULL)
    {
        perror("source file open failed");
        exit(EXIT_FAILURE);
    } 
 
    while((idx=fgets(str,LineMAX,source_file))!=NULL) //�б���
    {
        sen_len++;               //���ֵľ�������
        sen_new = 0;        //�³���һ������
        //�ʵ�ת������Ϊ��λ
        int old_flag=0;         //ǰһ����������
        int new_flag=0;         //��һ����������
        while((idx=strchr(idx,'/'))!=NULL)      //��"/"��Ϊ�ָ�������������
        {
            memset(flag,'\0',sig-1);
            memset(HANZI,'\0',sig-1);
 
            //idxָ��ָ��λ��
            start = idx;
            //��ǰ���洢���Ĵʻ�
            start-=2;           //��������+�����ո���зָ�����
            word_len =0;
            while((int)(*start)&0x80)       //Ϊ���ģ�ѭ������                  
            {
                word_len+=2;    //��������
                //��������
                start-=2;       
            }
            start+=2;
            strncpy(HANZI,start,word_len);
            start = idx;
            //��󣬴洢����
            int i=0;
            start++;
            while(*start!=' ') //�ٴ������ո�ʱֹͣ
            {
                //����Ӣ��
                *(flag+i)=*(start++);
                i++;
            }
            *(flag+i) = '\0'; //�������
           // printf("%s,%s\n",HANZI,flag);
            //�����ĺʹ��Դ�����Ӧ�����У���Ӧ��Ӧ��������
            //new_flag=copy_to_matrix(HANZI,flag);    //���ش���
            new_flag=copy_to_matrix();    //���ش���
            idx++;
            //д��ת�ƾ���:new_flag
            old_flag=write_flag(old_flag,new_flag);  //����old
        }
    }
    
    //��������д洢
   // printf("--����--\n");
    pi_matrix[0]=sen_len;
    printf("��������: %d\n",sen_len);
    write_arr_to_file();   
    
    fclose(source_file);
    exit(EXIT_SUCCESS);
}

//int copy_to_matrix(char *HANZI,char *flag)     //������Ӧ���ַ���
int copy_to_matrix(void)
{
    int out = 0;
    int cur_flag=0;
    int cur_ch =0;
    //���ԵǼ�
    for(int i=1;i<=flag_matrix_len;i++)
    {
        if(0==strcmp(flag,flag_matrix[i]))
        {
            //��ʱƥ��һ��,�޸�
            out = 1;
            cur_flag = i;    //��ǰ��������λ��
            break;
        }
    }
    if(0==out)
    {
        strcpy(flag_matrix[++flag_matrix_len],flag);  //
        cur_flag = flag_matrix_len;
    }
    //�Ծ��׳��ֵ��ʽ���ͳ��
    if(0==sen_new)
    {
        pi_matrix[cur_flag]++;
        sen_new=1; //�Ǿ���
    }
    //���ֵ��ʵǼ�
    out=0;
    for(int i=1;i<=word_matrix_len;i++)
    {
        if(0==strcmp(HANZI,ch_matrix[i]))
        {
            out=1;
            cur_ch = i;     //��ǰ��������λ��
            break;
        }
    }
    if(0==out)
    {
        strcpy(ch_matrix[++word_matrix_len],HANZI);
        cur_ch = word_matrix_len;
    }
    //���������ӳ��Ǽ�,��֮ǰ�ĺ��ֺʹ��ԵǼǽ��д���
    map_matrix[cur_flag].ch[cur_ch].ch_cnt++;   //ӳ�����飺����λ��-->�������飺����λ��-->���ĳ���Ƶ��
    map_matrix[cur_flag].flag_cnt++; 
    move_matrix[cur_flag][0] = map_matrix[cur_flag].flag_cnt;   //���ָô�����
    
    return cur_flag;    //���ص�ǰ��������
}

int write_flag(int old,int new)
{
    if(0==old)
    {
        //��һ�ν��У�ֱ�Ӹ���
        return new;
    }
    //����ת�Ƹ��ʾ���
    move_matrix[old][new]++;     //״̬old-->״̬new��������
    return new;
}
void  write_arr_to_file(void)
{
    FILE *ch_dict=NULL;      //���Ĵʵ�
    FILE *flag_dict=NULL;    //���Դʵ�
    FILE *move_dict=NULL;    //ת�Ƹ���
    FILE *map_dict=NULL;     //�������
    FILE *pi_dict=NULL;      //��ʼ����
    
    if((ch_dict=fopen(ch_dict_file,"w+"))==NULL)    //W+����ļ�����
    {
        perror("ch_dict open failed");
        exit(EXIT_FAILURE);
    }
    if((flag_dict=fopen(flag_dict_file,"w+"))==NULL)
    {
        perror("flag_dict open failed");
        exit(EXIT_FAILURE);
    }
    if((move_dict=fopen(move_dict_file,"w+"))==NULL)
    {
        perror("move_dict open failed");
        exit(EXIT_FAILURE);
    }
    if((map_dict=fopen(map_dict_file,"w+"))==NULL)
    {
        perror("map_dict open failed");
        exit(EXIT_FAILURE);
    }
    if((pi_dict=fopen(pi_dict_file,"w+"))==NULL)
    {
        perror("pi_dict open failed");
        exit(EXIT_FAILURE);
    }

    //���������ת�浽�ļ���
    printf("\n------------���Դʵ�洢------------\n");
    printf("��ѵ������������:%d\n",flag_matrix_len);
    fprintf(flag_dict,"cnt:%d\n",flag_matrix_len); 
    for(int i=1;i<=flag_matrix_len;i++)
    {
       // printf("%d::%s\t",i,flag_matrix[i]);
        fprintf(flag_dict,"%d::%s\n",i,flag_matrix[i]);
    }
    
    printf("\n------------���ĵ��ʴʵ�洢------------\n");
    printf("��ѵ�������ĵ�������:%d\n",word_matrix_len);
    fprintf(ch_dict,"cnt:%d\n",word_matrix_len); 
    for(int i=1;i<=word_matrix_len;i++)
    {
        //printf("%d::%s\t",i,ch_matrix[i]);
        fprintf(ch_dict,"%d::%s\n",i,ch_matrix[i]);
    }
   
    printf("\n------------map�洢------------\n");
    for(int i=1;i<=flag_matrix_len;i++)
    {
        for(int j=1;j<=word_matrix_len;j++)
        {
            fprintf(map_dict,"%d-->\t%d-->\t%d\n",i,j,map_matrix[i].ch[j].ch_cnt);
        }
    }

    printf("\n------------״̬ת�Ƹ��ʴ洢------------\n");
    fprintf(move_dict,"cnt:%d\n",flag_matrix_len); 
    for(int i=1;i<=flag_matrix_len;i++)
    {
        word_all += move_matrix[i][0];
        fprintf(move_dict,"%f\t",move_matrix[i][0]);
        for(int j=1;j<=flag_matrix_len;j++)
        {
            fprintf(move_dict,"%f\t",move_matrix[i][j]);
        }
        fprintf(move_dict,"\n");
    }

    printf("\n------------��ʼ���ʴ洢------------\n");
    //ע���ʼ����=���ʴ��Գ��ִ���/���е���������
    double tmp = 0.0;
    printf("���е��ʸ���%f\n",word_all);
    for(int i=1;i<=flag_matrix_len;i++)
    {
        /*tmp = (double)pi_matrix[i]/sen_len;*/
        tmp = (double)move_matrix[i][0]/word_all;
        fprintf(pi_dict,"%f\n",tmp);
    }

    fclose(ch_dict);
    fclose(flag_dict);
    fclose(move_dict);
    fclose(map_dict);
    fclose(pi_dict);
}   
void init(void)
{
    FILE *ch_dict=NULL;      //���Ĵʵ�
    FILE *flag_dict=NULL;    //���Դʵ�
    FILE *move_dict=NULL;    //ת�Ƹ���
    FILE *map_dict=NULL;     //�������
    int change =0;
    
    if((ch_dict=fopen(ch_dict_file,"r"))==NULL)
    {
        //û���ļ�
        printf("*************����ѵ��***************\n");
        //����ѡ��Ĭ��
        //fclose(ch_dict);
        return;
    }
   
    printf("################��ɲ���ѵ��###################\n");
    //�Ѿ�ѵ�������������룬Ҳ����ͨ���ļ��ض��򣬽�ѵ�����ϳ�һ���ļ� 
    if((flag_dict=fopen(flag_dict_file,"a+"))==NULL)    //a+ ���޸�Դ�ļ�,׷������β
    {
        perror("flag_dict open failed");
        exit(EXIT_FAILURE);
    }
    if((move_dict=fopen(move_dict_file,"a+"))==NULL)
    {
        perror("move_dict open failed");
        exit(EXIT_FAILURE);
    }
    if((map_dict=fopen(map_dict_file,"a+"))==NULL)
    {
        perror("map_dict open failed");
        exit(EXIT_FAILURE);
    }
    //���ļ���ȡ�������
    //���ļ�����ת�浽�����У���Ӳ��ת���ڴ���
    printf("\n------------���Կ������ڴ�------------\n");
    fscanf(flag_dict,"cnt:%d\n",&flag_matrix_len); 
    printf("��ѵ������������:%d\n",flag_matrix_len);
    for(int i=1;i<=flag_matrix_len;i++)
    {
        fscanf(flag_dict,"%d::%s\n",&i,flag_matrix[i]);
    }
    
    printf("\n------------���ĵ��ʿ������ڴ�------------\n");
    fscanf(ch_dict,"cnt:%d\n",&word_matrix_len); 
    printf("��ѵ�������ĵ�������:%d\n",word_matrix_len);
    for(int i=1;i<=word_matrix_len;i++)
    {
        fscanf(ch_dict,"%d::%s\n",&i,ch_matrix[i]);
    }
   
    printf("\n------------mapת���ڴ�------------\n");
    for(int i=1;i<=flag_matrix_len;i++)
    {
        for(int j=1;j<=word_matrix_len+1;j++)
        {
            fscanf(map_dict,"%d-->\t%d-->\t%d\n",&i,&j,&map_matrix[i].ch[j].ch_cnt);
        }
    }

    printf("\n------------״̬ת�Ƹ��ʿ������ڴ�------------\n");
    fscanf(move_dict,"cnt:%d\n",&flag_matrix_len); 
    for(int i=1;i<=flag_matrix_len;i++)
    {
        fscanf(move_dict,"%lf\t",&move_matrix[i][0]);
        for(int j=1;j<=flag_matrix_len;j++)
        {
            fscanf(move_dict,"%lf\t",&move_matrix[i][j]);
        }
        fscanf(move_dict,"\n");
    }

    fclose(ch_dict);
    fclose(flag_dict);
    fclose(move_dict);
    fclose(map_dict);

}
