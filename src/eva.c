 /*=============================================================================
#
# Author: tianweidut - liutianweidlut@gmail.com
#
# QQ : 416774905
#
# Last modified: 2010-12-10 10:38
#
# Filename: eva.c
#
# Description: ��⹤��
#
=============================================================================*/ 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

#define SIZE 80
#define sig 20
#define SenMAX 8000     //��������
#define LineMAX 4000    //ÿ�о����ַ�����

int flag_matrix_len =0;
char flag_matrix[SIZE+1][8];
char flag[8];
int seq[SenMAX+1][LineMAX+1];   //�洢ÿ�о�������ֵ

double flag_sum = 0;   //�������� 
int sen_sum=0;      //��������   
int cur_sen=0;      //��ǰ����������
int cur_word=0;      //��ǰ����������
double right=0;        //

void init(char *argv[]);
void findex(FILE *fp);

int main(int argc,char *argv[]) //1:tst.crt  2:out.crt  3:flag_dict_file  4:res.crt 
{
    
    FILE *res_file=NULL;    //����ļ�
    FILE *tst_file=NULL;    //���Ա�ע

    char *idx = NULL;
    char str[LineMAX];
    char *start = NULL;

    if(5!=argc)
    {
        printf("the argc is short!\n");
        exit(1);
    }
    if((res_file=fopen(argv[4],"w+"))==NULL)
    {
        perror("res fail");
        exit(EXIT_FAILURE);
    }
    if((tst_file=fopen(argv[1],"a+"))==NULL)
    {
        perror("src fail");
        exit(EXIT_FAILURE);
    }

    init(argv);
    cur_sen = 0; 
    while((idx=fgets(str,LineMAX,tst_file))!=NULL) //�б���
    {
       // sen_len++;          //���ֵľ�������
        cur_sen++;          //��ǰ����������
        cur_word = 0;       //����λ�ù���
        while((idx=strchr(idx,'/'))!=NULL)      //��"/"��Ϊ�ָ�������������
        {
            memset(flag,'\0',sig-1);
            //��󣬴洢����
            int i=0;
            start = idx;
            start++;
            while(*start!=' ') //�ٴ������ո�ʱֹͣ
            {
                //����Ӣ��
                *(flag+i)=*(start++);
                i++;
            }
            *(flag+i) = '\0'; //�������
            //�����ĺʹ��Դ�����Ӧ�����У���Ӧ��Ӧ��������
            flag_sum++;
            cur_word++;
            findex(res_file);    
            idx++;
        }
    }
    printf("��������:%f,ƥ�䵥������:%f\n",flag_sum,right);
    fprintf(res_file,"\n==================================\n");
    fprintf(res_file,"��������:%f,ƥ�䵥������:%f\n",flag_sum,right);
    printf("right: %f\n",(double)right/flag_sum);
    printf("right: %f\%\n",(double)(100*right/flag_sum));
    fprintf(res_file,"right: %lf\n",(double)right/flag_sum);
    fprintf(res_file,"\n==================================\n");

    fclose(tst_file);
    fclose(res_file);
    exit(EXIT_SUCCESS);
}
void init(char *argv[])
{   
    FILE *out_file=NULL;    //�����ļ�����
    FILE *flag_file=NULL;   //�����ֵ����
    
    if((out_file=fopen(argv[2],"a+"))==NULL)
    {
        perror("seq fail");
        exit(EXIT_FAILURE);
    }
    if((flag_file=fopen(argv[3],"a+"))==NULL)
    {
        perror("flag fail");
        exit(EXIT_FAILURE);
    }
    //��ȡ����������Ϣ
    printf("\n------------�����ֵ�����------------\n");
    fscanf(flag_file,"cnt:%d\n",&flag_matrix_len); 
    printf("��ѵ������������:%d\n",flag_matrix_len);
    for(int i=1;i<=flag_matrix_len;i++)
    {
        fscanf(flag_file,"%d::%s\n",&i,flag_matrix[i]);
    }

    printf("\n------------viterbi�㷨��֤------------\n");
    int row = 0;
    while(!feof(out_file))
    {
        sen_sum++;
        fscanf(out_file,"T= %d\n",&seq[++row][0]);
        for(int i=1;i<=seq[row][0];i++)
        {
            fscanf(out_file,"%d ",&seq[row][i]);
        }
        fscanf(out_file,"\n");
    }

    printf("��������:%d\n",(sen_sum));

    fclose(flag_file);
    fclose(out_file);

}
void findex(FILE *fp)
{
    int out = 0;
    int cur_flag = 0;
    //���ԵǼ�
    for(int i=1;i<=flag_matrix_len;i++)
    {
        if(0==strcmp(flag,flag_matrix[i]))
        {
            out = 1;
            cur_flag = i;
            break;
        }
    }  
    if(0==out)
    {
        perror("flag num  faile");
        exit(EXIT_FAILURE);
    }

    if(cur_flag == seq[cur_sen][cur_word])
    {
        right++;
    }
    else
    {
        fprintf(fp,"%d::%d\n",cur_sen,cur_word);
    }
}
