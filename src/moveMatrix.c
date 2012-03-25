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
# Description:转移矩阵概率求解 
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

#define DEBUG 1     //调试开关
#define SIZE  80   //单词词性数量
#define LineMAX  8000   //每句话存储的字符数量 
#define WordMAX  30000  //词汇总的数量 
#define sig      40     //单词长度

struct chin
{
    //int ch_idx;     //汉字索引
    int ch_cnt;     //汉字出现频率（对应每个词性）
};

struct map_str
{ 
    struct chin ch[WordMAX+1];        //每个词性对应的单词  
    int  flag_cnt;             //词性出现数量
};

double move_matrix[SIZE+1][SIZE+1];     //转移矩阵，生成M^2的矩阵
double pi_matrix[SIZE+1];               //初始概率
char flag_matrix[SIZE+1][8];                 //词性存储数组，对每个词性标记成一个数字，在flag[0]存储登记词性
char ch_matrix[WordMAX+1][50];         //汉字词汇存储数组,将每一单词标记成一个数组!!!注意越界问题
struct map_str map_matrix[SIZE+1];                //词性与单词的映射,中文-词性-出现数量

int flag_matrix_len =0;
int word_matrix_len =0;
int map_matrix_len =0;
int sen_len=0;          //句子数量
int sen_new=0;          //新句子标示
double word_all = 0.0;      //所有单词数量
char flag[sig];             //词性
char HANZI[sig];            //汉字

#define  ch_dict_file       "./key/ch_dict_file.crt"      //中文词典
#define  flag_dict_file     "./key/flag_dict_file.crt"    //词性词典
#define  move_dict_file     "./key/move_dict_file.crt"    //转移概率
#define  map_dict_file      "./key/map_dict_file.crt"     //发射概率
#define  pi_dict_file       "./key/pi.crt"                //初始概率

//int copy_to_matrix(char *HANZI,char *flag);
void init(void);            //参数状态初始化

int copy_to_matrix(void);
int write_flag(int old,int new);
void write_arr_to_file(void);

int main(int argc,char *argv[])
{
    FILE *source_file=NULL;
    char str[LineMAX];
    
    char *idx=NULL;
    char *start=NULL;
    int flag_len=0;         //词性长度
    int word_len=0;         //中文单词长度

    init();
    if((source_file=fopen(argv[1],"a+"))==NULL)
   // if((source_file=fopen("BAIKE002.crt","a+"))==NULL)
    {
        perror("source file open failed");
        exit(EXIT_FAILURE);
    } 
 
    while((idx=fgets(str,LineMAX,source_file))!=NULL) //行遍历
    {
        sen_len++;               //出现的句子数量
        sen_new = 0;        //新出现一个句子
        //词的转移以行为单位
        int old_flag=0;         //前一个词性索引
        int new_flag=0;         //新一个词性索引
        while((idx=strchr(idx,'/'))!=NULL)      //以"/"作为分隔符，进行搜索
        {
            memset(flag,'\0',sig-1);
            memset(HANZI,'\0',sig-1);
 
            //idx指向分割符位置
            start = idx;
            //向前，存储中文词汇
            start-=2;           //根据中文+两个空格进行分割特性
            word_len =0;
            while((int)(*start)&0x80)       //为中文，循环继续                  
            {
                word_len+=2;    //汉字数量
                //拷贝中文
                start-=2;       
            }
            start+=2;
            strncpy(HANZI,start,word_len);
            start = idx;
            //向后，存储词性
            int i=0;
            start++;
            while(*start!=' ') //再次遇到空格时停止
            {
                //拷贝英文
                *(flag+i)=*(start++);
                i++;
            }
            *(flag+i) = '\0'; //结束标记
           // printf("%s,%s\n",HANZI,flag);
            //将中文和词性存入相应数组中，对应相应检索函数
            //new_flag=copy_to_matrix(HANZI,flag);    //返回词性
            new_flag=copy_to_matrix();    //返回词性
            idx++;
            //写入转移矩阵:new_flag
            old_flag=write_flag(old_flag,new_flag);  //更新old
        }
    }
    
    //将结果进行存储
   // printf("--测试--\n");
    pi_matrix[0]=sen_len;
    printf("句子数量: %d\n",sen_len);
    write_arr_to_file();   
    
    fclose(source_file);
    exit(EXIT_SUCCESS);
}

//int copy_to_matrix(char *HANZI,char *flag)     //检索相应的字符串
int copy_to_matrix(void)
{
    int out = 0;
    int cur_flag=0;
    int cur_ch =0;
    //词性登记
    for(int i=1;i<=flag_matrix_len;i++)
    {
        if(0==strcmp(flag,flag_matrix[i]))
        {
            //此时匹配一个,修改
            out = 1;
            cur_flag = i;    //当前词性所在位置
            break;
        }
    }
    if(0==out)
    {
        strcpy(flag_matrix[++flag_matrix_len],flag);  //
        cur_flag = flag_matrix_len;
    }
    //对句首出现单词进行统计
    if(0==sen_new)
    {
        pi_matrix[cur_flag]++;
        sen_new=1; //非句首
    }
    //汉字单词登记
    out=0;
    for(int i=1;i<=word_matrix_len;i++)
    {
        if(0==strcmp(HANZI,ch_matrix[i]))
        {
            out=1;
            cur_ch = i;     //当前词性所在位置
            break;
        }
    }
    if(0==out)
    {
        strcpy(ch_matrix[++word_matrix_len],HANZI);
        cur_ch = word_matrix_len;
    }
    //汉字与词性映射登记,对之前的汉字和词性登记进行处理
    map_matrix[cur_flag].ch[cur_ch].ch_cnt++;   //映射数组：词性位置-->中文数组：单词位置-->中文出现频率
    map_matrix[cur_flag].flag_cnt++; 
    move_matrix[cur_flag][0] = map_matrix[cur_flag].flag_cnt;   //出现该词数量
    
    return cur_flag;    //返回当前词性索引
}

int write_flag(int old,int new)
{
    if(0==old)
    {
        //第一次进行，直接更新
        return new;
    }
    //更新转移概率矩阵
    move_matrix[old][new]++;     //状态old-->状态new数量更新
    return new;
}
void  write_arr_to_file(void)
{
    FILE *ch_dict=NULL;      //中文词典
    FILE *flag_dict=NULL;    //词性词典
    FILE *move_dict=NULL;    //转移概率
    FILE *map_dict=NULL;     //发射概率
    FILE *pi_dict=NULL;      //初始概率
    
    if((ch_dict=fopen(ch_dict_file,"w+"))==NULL)    //W+清除文件内容
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

    //将数组进行转存到文件中
    printf("\n------------词性词典存储------------\n");
    printf("本训练集词性种类:%d\n",flag_matrix_len);
    fprintf(flag_dict,"cnt:%d\n",flag_matrix_len); 
    for(int i=1;i<=flag_matrix_len;i++)
    {
       // printf("%d::%s\t",i,flag_matrix[i]);
        fprintf(flag_dict,"%d::%s\n",i,flag_matrix[i]);
    }
    
    printf("\n------------中文单词词典存储------------\n");
    printf("本训练集中文单词种类:%d\n",word_matrix_len);
    fprintf(ch_dict,"cnt:%d\n",word_matrix_len); 
    for(int i=1;i<=word_matrix_len;i++)
    {
        //printf("%d::%s\t",i,ch_matrix[i]);
        fprintf(ch_dict,"%d::%s\n",i,ch_matrix[i]);
    }
   
    printf("\n------------map存储------------\n");
    for(int i=1;i<=flag_matrix_len;i++)
    {
        for(int j=1;j<=word_matrix_len;j++)
        {
            fprintf(map_dict,"%d-->\t%d-->\t%d\n",i,j,map_matrix[i].ch[j].ch_cnt);
        }
    }

    printf("\n------------状态转移概率存储------------\n");
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

    printf("\n------------初始概率存储------------\n");
    //注意初始概率=单词词性出现次数/所有单词数量；
    double tmp = 0.0;
    printf("所有单词个数%f\n",word_all);
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
    FILE *ch_dict=NULL;      //中文词典
    FILE *flag_dict=NULL;    //词性词典
    FILE *move_dict=NULL;    //转移概率
    FILE *map_dict=NULL;     //发射概率
    int change =0;
    
    if((ch_dict=fopen(ch_dict_file,"r"))==NULL)
    {
        //没有文件
        printf("*************初次训练***************\n");
        //参数选择默认
        //fclose(ch_dict);
        return;
    }
   
    printf("################完成部分训练###################\n");
    //已经训练过，重新载入，也可以通过文件重定向，将训练集合成一个文件 
    if((flag_dict=fopen(flag_dict_file,"a+"))==NULL)    //a+ 不修改源文件,追加至结尾
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
    //打开文件获取相关数据
    //将文件进行转存到数组中，由硬盘转到内存中
    printf("\n------------词性拷贝至内存------------\n");
    fscanf(flag_dict,"cnt:%d\n",&flag_matrix_len); 
    printf("本训练集词性种类:%d\n",flag_matrix_len);
    for(int i=1;i<=flag_matrix_len;i++)
    {
        fscanf(flag_dict,"%d::%s\n",&i,flag_matrix[i]);
    }
    
    printf("\n------------中文单词拷贝至内存------------\n");
    fscanf(ch_dict,"cnt:%d\n",&word_matrix_len); 
    printf("本训练集中文单词种类:%d\n",word_matrix_len);
    for(int i=1;i<=word_matrix_len;i++)
    {
        fscanf(ch_dict,"%d::%s\n",&i,ch_matrix[i]);
    }
   
    printf("\n------------map转至内存------------\n");
    for(int i=1;i<=flag_matrix_len;i++)
    {
        for(int j=1;j<=word_matrix_len+1;j++)
        {
            fscanf(map_dict,"%d-->\t%d-->\t%d\n",&i,&j,&map_matrix[i].ch[j].ch_cnt);
        }
    }

    printf("\n------------状态转移概率拷贝至内存------------\n");
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
