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
# Description: 检测工具
#
=============================================================================*/ 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

#define SIZE 80
#define sig 20
#define SenMAX 8000     //句子数量
#define LineMAX 4000    //每行句子字符数量

int flag_matrix_len =0;
char flag_matrix[SIZE+1][8];
char flag[8];
int seq[SenMAX+1][LineMAX+1];   //存储每行句子序列值

double flag_sum = 0;   //单词总数 
int sen_sum=0;      //句子数量   
int cur_sen=0;      //当前句子所在行
int cur_word=0;      //当前句子所在行
double right=0;        //

void init(char *argv[]);
void findex(FILE *fp);

int main(int argc,char *argv[]) //1:tst.crt  2:out.crt  3:flag_dict_file  4:res.crt 
{
    
    FILE *res_file=NULL;    //结果文件
    FILE *tst_file=NULL;    //词性标注

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
    while((idx=fgets(str,LineMAX,tst_file))!=NULL) //行遍历
    {
       // sen_len++;          //出现的句子数量
        cur_sen++;          //当前句子所在行
        cur_word = 0;       //单词位置归零
        while((idx=strchr(idx,'/'))!=NULL)      //以"/"作为分隔符，进行搜索
        {
            memset(flag,'\0',sig-1);
            //向后，存储词性
            int i=0;
            start = idx;
            start++;
            while(*start!=' ') //再次遇到空格时停止
            {
                //拷贝英文
                *(flag+i)=*(start++);
                i++;
            }
            *(flag+i) = '\0'; //结束标记
            //将中文和词性存入相应数组中，对应相应检索函数
            flag_sum++;
            cur_word++;
            findex(res_file);    
            idx++;
        }
    }
    printf("单词数量:%f,匹配单词数量:%f\n",flag_sum,right);
    fprintf(res_file,"\n==================================\n");
    fprintf(res_file,"单词数量:%f,匹配单词数量:%f\n",flag_sum,right);
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
    FILE *out_file=NULL;    //序列文件载入
    FILE *flag_file=NULL;   //序列字典查找
    
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
    //读取词性序列信息
    printf("\n------------词性字典载入------------\n");
    fscanf(flag_file,"cnt:%d\n",&flag_matrix_len); 
    printf("本训练集词性种类:%d\n",flag_matrix_len);
    for(int i=1;i<=flag_matrix_len;i++)
    {
        fscanf(flag_file,"%d::%s\n",&i,flag_matrix[i]);
    }

    printf("\n------------viterbi算法验证------------\n");
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

    printf("句子数量:%d\n",(sen_sum));

    fclose(flag_file);
    fclose(out_file);

}
void findex(FILE *fp)
{
    int out = 0;
    int cur_flag = 0;
    //词性登记
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
