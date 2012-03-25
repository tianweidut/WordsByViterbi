 /*=============================================================================
#
# Author: tianweidut - liutianweidlut@gmail.com
#
# QQ : 416774905
#
# Last modified:	2010-12-09 08:25
#
# Filename:		showViterbi.c
#
# Description: Viterbi.c 算法演示程序
#
=============================================================================*/ 
#include "viterbi.h"

#define data_hmm "./key/data.hmm"
#define seq_seq  "./key/seq.seq"
#define out_crt "./key/out.crt"

char flag_matrix[80][20];
int flag_matrix_len = 0;

void Print(FILE*fp,FILE*ch,int T,int *seq);

int main(int argc ,char *argv[]) //1:data.hmm  2:seq.seq 3.out.crt 4:flag_dict_file 5:ch.crt
{
    int t,T;
    HMM hmm; 
    int *seq;   //观察序列（中文单词）
    int *res;   //结果：隐藏输出结果
    double **A;     //转移矩阵
    int    **B;     //对应状态
    double max=0.0;     //最大概率
    FILE *fp=NULL;
    FILE *out=NULL;     //结果文件
    FILE *flag=NULL;
    FILE *ch=NULL;
    int cnt=0;
    
    if(argc != 6)
    {
		printf("Usage error \n");
		printf("Usage: testvit <model.hmm> <obs.seq> <out.crt>\n");
		exit (1);
    }
    if((fp=fopen(argv[1],"r"))==NULL)
    /*if((fp=fopen(data_hmm,"r"))==NULL)*/
    {
        perror("fp1 error");
        exit(EXIT_FAILURE);
    }
    printf("----------获得HMM模型数据-----------\n");
    ReadHMM(fp,&hmm);

    fclose(fp);

    if((fp=fopen(argv[2],"r"))==NULL)
    /*if((fp=fopen(seq_seq,"r"))==NULL)*/
    {
        perror("fp2 error");
        exit(EXIT_FAILURE);
    }
    if((out=fopen(argv[3],"w+"))==NULL)
    /*if((out=fopen(out_crt,"w+"))==NULL)*/
    {
        perror("out.crt fail");
        exit(EXIT_FAILURE);
    }
    if((flag=fopen(argv[4],"r+"))==NULL)
    //if((flag=fopen("./key/flag_dict_file.crt","r"))==NULL)
    {
        perror("key flag\n");
        exit(EXIT_FAILURE);
    }
    if((ch=fopen(argv[5],"w+"))==NULL)
    {
        perror("ch flag\n");
        exit(EXIT_FAILURE);
    }
    //read_flag
    printf("\n------------词性拷贝至内存------------\n");
    fscanf(flag,"cnt:%d\n",&flag_matrix_len); 
    printf("本训练集词性种类:%d\n",flag_matrix_len);
    for(int i=1;i<=flag_matrix_len;i++)
    {
        fscanf(flag,"%d::%s\n",&i,flag_matrix[i]);
    }
    

    while(OK==ReadSequence(fp,&T,&seq))   //每次仅读取一句话，需要进行修改
    {
        cnt++;
        res = icreateArray(1,T);    //创建结果序列
        A = dcreateMatrix(1,T,1,hmm.N);
        B = icreateMatrix(1,T,1,hmm.N);
    
        /*printf("----------------------------------\n");*/
        /*printf("Viterbi算法计算过程\n");*/
        Viterbi(&hmm,T,seq,A,B,res,&max);
        /*printf("----------------------------------\n");*/
        /*printf("打印隐藏状态结果:第%d句\n",cnt);*/
        Print(out,ch,T,res);

        //进行参数复位
        free_array(res,1,T);
        free_array(seq,1,T);
        free_imatrix(B,1,T,1,hmm.N);
        free_dmatrix(A,1,T,1,hmm.N);
        T = 0;
        max = 0;
    }

    FreeHMM(&hmm);
    fclose(fp);
    fclose(out);
    fclose(ch);
    exit(EXIT_SUCCESS);
}
void Print(FILE*fp,FILE*ch,int T,int *seq)
{


    fprintf(fp,"T= %d\n",T);
    //fprintf(ch,"T= %d\n",T);
    /*printf("T= %d\n",T);*/
    
    for(int i=1;i<=T;i++)
    {
        fprintf(fp,"%d ",seq[i]);
        fprintf(ch,"/%s ",flag_matrix[seq[i]]);
       // printf("%s ",flag_matrix[seq[i]]);
       // printf("%d ",seq[i]); 
    }
    fprintf(fp,"\n");
    fprintf(ch,"\n");
    /*printf("\n");*/
}
