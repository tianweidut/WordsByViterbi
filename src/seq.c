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
# Description: 完成中文单词序列化分析:0(表示未登录词汇) 
#
=============================================================================*/ 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

#define  ch_dict_file       "./key/ch_dict_file.crt"      //中文词典
#define  seq_dict_file      "./key/seq.seq"              //序列词典
#define  src_dict_file      "./key/tst.crt+copy"              //序列词典

#define LineMAX     8000        //每句话字符数量
#define sig         40
#define WordMAX     30000       //中文单词最大值
#define SenMAX      4000        //句子数量

int sen_len = 0;            //句子数
int word_len=0;             //单词数
char HANZI[sig];            //汉字存储
int  ch_matrix_len =0;      //中文单词数量
char ch_matrix[WordMAX+1][80];    //中文单词
int  hero[SenMAX+1][LineMAX+1];     //句子单词表

FILE *src_file=NULL;        //分析源文件

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

    while((line=fgets(str,LineMAX,src_file))!=NULL)   //行遍历
    {
        sen_len ++;
        printf(".");
        start = line;
        word_len = 0;       //每句话中单词数量

        while((end=strchr(start,' '))!=NULL)    //句子遍历
        {
            memset(HANZI,'\0',sig-1);   //单词归位
            strncpy(HANZI,start,(int)(end-start));  //此时为接获中文词汇
            word_len++;
            start = ++end;      //此处容易出错
            write_into_arr(); 
        } 
        hero[sen_len][0] = word_len;
    }
    printf("\n--测试--\n"); 
    write_into_file(argv[2]);

    exit(EXIT_SUCCESS);
}
void write_into_arr(void)
{
    //对HANZI处理
    int cur_idx=0;
    for(int i=1;i<=ch_matrix_len;i++)
    {
        if(0 == strcmp(HANZI,ch_matrix[i]))
        {
            cur_idx = i;    //单词索引
        }
    }
    hero[sen_len][word_len]=cur_idx;
    
} 
void init(char *argv1,char *argv3)
{
    //获取文件信息
    FILE *ch_dict=NULL;         //中文单词文件

    /*if((ch_dict=fopen(ch_dict_file,"a+"))==NULL)    //W+清除文件内容*/
    if((ch_dict=fopen(argv1,"a+"))==NULL)    //W+清除文件内容
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
    
    printf("---------------初始化------------\n");
    fscanf(ch_dict,"cnt:%d\n",&ch_matrix_len);
    printf("中文单词数量:%d\n",ch_matrix_len);
    for(int i=1;i<=ch_matrix_len;i++)
    {
        fscanf(ch_dict,"%d::%s\n",&i,ch_matrix[i]);
    }
    
    fclose(ch_dict);
}

void write_into_file(char *argv2)
{
    FILE *seq_dict=NULL;        //序列文件
    /*if((seq_dict=fopen(seq_dict_file,"w+"))==NULL)*/
    if((seq_dict=fopen(argv2,"w+"))==NULL)
    {
        perror("seq_dict open failed");
        exit(EXIT_FAILURE);
    }
    //向seq文件写入序列
    printf("统计结果:句子数量:%d\n",sen_len);
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
