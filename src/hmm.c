 /*=============================================================================
#
# Author: tianweidut - liutianweidlut@gmail.com
#
# QQ : 416774905
#
# Last modified:	2010-12-09 23:02
#
# Filename:		hmm.c
#
# Description：hmm模型文件生成
#
=============================================================================*/ 
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int word_matrix_len = 0;    //中文单词数量 -->M 观察序列数量
int flag_matrix_len = 0;    //词性数量     -->N 隐藏序列数量

#define  ch_dict_file       "./key/ch_dict_file.crt"      //中文词典
#define  flag_dict_file     "./key/flag_dict_file.crt"    //词性词典
#define  move_dict_file     "./key/move_dict_file.crt"    //转移概率
#define  map_dict_file      "./key/map_dict_file.crt"     //发射概率
#define  hmm_dict_file      "./key/data.hmm"              //HMM模型
#define  pi_dict_file       "./key/pi.crt"                //初始概率
    
#define  WordNUM    30000   //中文单词最大数量
#define  SIZE       80      //词性最大数量

double flag[SIZE+1];            //该单词出现的总的次数

int main(int argc,char *argv[])  //1：ch_dict  2:flag_dict  3:move_dict 4:map_dict 5:hmm_dict 6:pi_dict
{
    FILE *ch_dict=NULL;      //中文词典
    FILE *flag_dict=NULL;    //词性词典
    FILE *move_dict=NULL;    //转移概率
    FILE *map_dict=NULL;     //发射概率
    FILE *pi_dict=NULL;      //初始概率
    FILE *hmm_dict=NULL;     //HMM模型训练文件
    int tmp_tmp=0;
    double tmp=0.0;
    double sum=0.0;
    double sig=0.0;
    int intsig=0;

    /*if((ch_dict=fopen(ch_dict_file,"a+"))==NULL)    //W+清除文件内容*/
    if((ch_dict=fopen(argv[1],"a+"))==NULL)    //W+清除文件内容
    {
        perror("ch_dict open failed");
        exit(EXIT_FAILURE);
    }
    /*if((flag_dict=fopen(flag_dict_file,"a+"))==NULL)*/
    if((flag_dict=fopen(argv[2],"a+"))==NULL)
    {
        perror("flag_dict open failed");
        exit(EXIT_FAILURE);
    }
    /*if((hmm_dict=fopen(hmm_dict_file,"w+"))==NULL)*/
    if((hmm_dict=fopen(argv[5],"w+"))==NULL)
    {
        perror("hmm_dict open failed");
        exit(EXIT_FAILURE);
    }

    printf("\n-------------获取中文单词数量-----------------\n");    
    fscanf(ch_dict,"cnt:%d\n",&word_matrix_len); 
    printf("本训练集中文单词种类:%d\n",word_matrix_len);
    fprintf(hmm_dict,"M= %d\n",word_matrix_len);
    
    printf("\n-------------获取词性数量---------------------\n");    
    fscanf(flag_dict,"cnt:%d\n",&flag_matrix_len); 
    printf("本训练集词性种类:%d\n",flag_matrix_len);
    fprintf(hmm_dict,"N= %d\n",flag_matrix_len);

    fclose(ch_dict);
    fclose(flag_dict);

    /*if((move_dict=fopen(move_dict_file,"a+"))==NULL)*/
    if((move_dict=fopen(argv[3],"a+"))==NULL)
    {
        perror("move_dict open failed");
        exit(EXIT_FAILURE);
    }
    
    fscanf(move_dict,"cnt:%d\n",&tmp_tmp);
    tmp =(double)tmp_tmp;
    printf("\n-------------转移矩阵概率计算-----------------\n");
    fprintf(hmm_dict,"A:\n");
    for(int i=1;i<=flag_matrix_len;i++)
    {
        fscanf(move_dict,"%lf\t",&sum);
        /*printf("%lf\t",sum);*/
        flag[i]=sum;
        for(int j=1;j<=flag_matrix_len;j++)
        {
            fscanf(move_dict,"%lf\t",&sig);
            tmp = (double)sig/sum;
            fprintf(hmm_dict,"%f ",tmp);
            /*printf("%f ",tmp);*/
        }
        fscanf(move_dict,"\n");
        fprintf(hmm_dict,"\n");
        /*printf("\n");*/
    }
    
    /*int in=getchar();*/
    printf("\n-------------发射概率计算-----------------\n");
    fprintf(hmm_dict,"B:\n");
    tmp = 0.0;
    sum = 0.0;
    intsig = 0;
    int m,n;
    /*if((map_dict=fopen(map_dict_file,"a+"))==NULL)*/
    if((map_dict=fopen(argv[4],"a+"))==NULL)
    {
        perror("map_dict open failed");
        exit(EXIT_FAILURE);
    }
    for(int i=1;i<=flag_matrix_len;i++)
    {
        for(int j=1;j<=word_matrix_len;j++)
        {
            fscanf(map_dict,"%d-->\t%d-->\t%d\n",&m,&n,&intsig);
            tmp = (double)intsig/(double)flag[i];
            //printf("%d-->\t%d-->\t%d--------->flag[%d]:%f\ttmp:%f\n",m,n,intsig,i,flag[i],tmp);
            if(tmp!=0)
            {
                /*printf("%f ",tmp);*/
            }
            fprintf(hmm_dict,"%f ",tmp);
        }
        fprintf(hmm_dict,"\n");
        /*printf("\n");*/
    }

    printf("\n-------------初始概率计算-----------------\n");
    //注意初始概率计算：应该是该词性出现的概率在总体词性中出现的改良版

    //此处写入pi 初始概率
    /*if((pi_dict=fopen(pi_dict_file,"a+"))==NULL)*/
    if((pi_dict=fopen(argv[6],"a+"))==NULL)
    {
        perror("pi_dict open failed");
        exit(EXIT_FAILURE);
    }
    fprintf(hmm_dict,"pi:\n");
    for(int i=1;i<=flag_matrix_len;i++)
    {
        fscanf(pi_dict,"%lf\n",&tmp);
        fprintf(hmm_dict,"%f ",tmp);
    }
    fprintf(hmm_dict,"\n\n");

    fclose(hmm_dict);
    fclose(map_dict);
    fclose(move_dict);
    fclose(pi_dict);

    exit(EXIT_SUCCESS);
}

