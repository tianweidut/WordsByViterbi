 /*=============================================================================
#
# Author: tianweidut - liutianweidlut@gmail.com
#
# QQ : 416774905
#
# Last modified:	2010-12-09 08:25
#
# Filename:		viterbi.c
#
# Description: 维特比算法
#
=============================================================================*/ 
#include "viterbi.h"

void ReadHMM(FILE *fp,HMM *phmm)
{

    fscanf(fp,"M= %d\n",&(phmm->M));
    fscanf(fp,"N= %d\n",&(phmm->N));
    fscanf(fp,"A:\n");

    /*printf("获取转移概率矩阵\n");*/
    phmm->A = (double **)dcreateMatrix(1,phmm->N,1,phmm->N);
    for(int i=1;i<=phmm->N;i++)
    {
        for(int j=1;j<=phmm->N;j++)
        {
            fscanf(fp,"%lf",&(phmm->A[i][j]));
        }
        fscanf(fp,"\n");
    }

    fscanf(fp,"B:\n");
    /*printf("获取发射概率矩阵\n");*/
    phmm->B = (double **)dcreateMatrix(1,phmm->N,1,phmm->M);
    for(int i=1;i<=phmm->N;i++)
    {
        for(int j=1;j<=phmm->M;j++)
        {
            fscanf(fp,"%lf ",&(phmm->B[i][j]));
            if(phmm->B[i][j]!=0)
            {
             //   printf("%f ",(phmm->B[i][j]));
            }
        }
        fscanf(fp,"\n");
       // printf("\n");
    }

    fscanf(fp,"pi:\n");
    /*printf("获取初始概率矩阵\n");*/
    phmm->pi = (double *)dcreateArray(1,phmm->N);
    for(int i=1;i<=phmm->N;i++)
    {
        fscanf(fp,"%lf",&(phmm->pi[i]));
    }
    fscanf(fp,"\n");

}
void FreeHMM(HMM *phmm)
{
    free_dmatrix(phmm->A,1,phmm->N,1,phmm->N);
    free_dmatrix(phmm->B,1,phmm->N,1,phmm->M);
    free_darray(phmm->pi,1,phmm->N);
}
//动态分配矩阵
double **dcreateMatrix(int rs,int re,int cs,int ce)
{
    double **matrix;

    matrix=(double **)calloc((unsigned)(re-rs+1),sizeof(double*));
    if(!matrix)
    {
        perror("matrix failed");
        exit(EXIT_FAILURE);
    }
    matrix -= rs;
    for(int i=rs;i<=re;i++)
    {
        matrix[i]=(double *)calloc((unsigned)(ce-cs+1),sizeof(double));
        if(!matrix[i])
        {
            perror("matrix[i] failure");
            exit(EXIT_FAILURE);
        }
        matrix[i] -= cs;
    }
    return matrix;
}
//动态分配矩阵
int **icreateMatrix(int rs,int re,int cs,int ce)
{
    int **matrix;

    matrix=(int **)calloc((unsigned)(re-rs+1),sizeof(int*));
    if(!matrix)
    {
        perror("matrix failed");
        exit(EXIT_FAILURE);
    }
    matrix -= rs;
    for(int i=rs;i<=re;i++)
    {
        matrix[i]=(int *)calloc((unsigned)(ce-cs+1),sizeof(int));
        if(!matrix[i])
        {
            perror("matrix[i] failure");
            exit(EXIT_FAILURE);
        }
        matrix[i] -= cs;
    }
    return matrix;
}

//动态分配数组
double *dcreateArray(int start,int end)
{
    double *arr;

    arr = (double *)calloc((unsigned)(end-start+1),sizeof(double));
    if(!arr)
    {
        perror("calloc arr fail");
        exit(EXIT_FAILURE);
    }
    return arr-start;
}
int *icreateArray(int start,int end)
{
    int *arr;

    arr = (int *)calloc((unsigned)(end-start+1),sizeof(int));
    if(!arr)
    {
        perror("calloc arr fail");
        exit(EXIT_FAILURE);
    }
    return arr-start;
}

void free_dmatrix(double **m,int s1,int e1,int s2,int e2)
{
    for(int i=e1;i>=s1;i--)
    {
        free((char*)(m[i]+s2));
    }
    free((char*)(m+s1));    //注意时按照字节释放空间
}
void free_imatrix(int **m,int s1,int e1,int s2,int e2)
{
    for(int i=e1;i>=s1;i--)
    {
        free((char*)(m[i]+s2));
    }
    free((char*)(m+s1));    //注意时按照字节释放空间
}

void free_array(int *arr,int s,int e)
{
   free((char*)(arr+s)); 
}
void free_darray(double *arr,int s,int e)
{
   free((char*)(arr+s)); 
}

/**
 * @brief   一阶Viterbi算法
 *
 * @param   phmm HMM模型
 * @param   T   时间量（每句话包含单词数量）
 * @param   seq 观查序列（中文单词）
 * @param   A   转移概率矩阵
 * @param   B   发射概率矩阵
 * @param   res 路径-->隐藏状态
 * @param   max 最大概率
 */
void Viterbi(HMM *phmm,int T,int *seq,double **A,int **B,int *res,double *max)
{
    int i,j; 
    int time_idx;   //输入序列时间
    int max_valind;
    double max_val,val;

    //step1:序列初始化
    for(i=1;i<=phmm->N;i++)     //词性初始状态检索
    {
        //此处就需要处理未登录词问题
        //seq[1] 此处为单独一句话处理
        A[1][i] = phmm->pi[i] * (phmm->B[i][seq[1]]);
        if(A[1][i]!=0)
        {
          //  printf("Print!-->");
          //  printf("i:%d Val:%f\n",i,A[1][i]);
        }
        B[1][i] = 0;
    }
    //printf("\n");

    //step2:递归:三重循环求解
    for(time_idx=2;time_idx<=T;time_idx++)  //t=2时刻开始
    {
        for(j=1;j<=phmm->N;j++)     //遍历每个隐藏状态(词性)
        {
            max_val = 0.0;
            max_valind = 1;
            for(i=1;i<=phmm->N;i++)     //每个词性对所有其他词性进行扫描
            {
                //对上一个时间，该状态概率 乘 到其他状态转移概率
                val = A[time_idx-1][i]*(phmm->A[i][j]);
                if(val > max_val)
                {
                    max_val = val;  //最大值
                    max_valind = i;     //回溯指针记录
                }
            }
            //t时刻更新概率
            A[time_idx][j] = max_val * (phmm->B[j][seq[time_idx]]);
            //最大值标号记录
            B[time_idx][j] = max_valind;
        }
    }
    
    //step3:最大概率计算
    *max = 0.0;
    res[T] = 1;
    for(i=1;i<=phmm->N;i++)
    {
        if(A[T][i]>*max)
        {
            *max = A[T][i];     //T时刻最大概率
            res[T] = i;         //对应状态值
        }
    } 

    //step4:打印最大概率路径
    for(time_idx=T-1;time_idx>=1;time_idx--)
    {
        res[time_idx] = B[time_idx+1][res[time_idx+1]];
    }
}
int ReadSequence(FILE *fp,int *pT,int **p)   //获取每句话：pT 数量 pO观察序列
{
    int *seq;

    while(!feof(fp))
    {
        fscanf(fp,"T= %d\n",pT);
        //printf("T= %d\n",*pT);
        seq = icreateArray(1,*pT);
        for(int i=1;i<=*pT;i++)
        {
            fscanf(fp,"%d",&seq[i]);
        }
        *p = seq;
        fscanf(fp,"\n");    //此处容易出错
        return OK;
    }
    return NO;
}
void PrintSequence(FILE*fp,int T,int *seq)
{

    fprintf(fp,"T= %d\n",T);
    printf("T= %d\n",T);
    
    for(int i=1;i<=T;i++)
    {
        fprintf(fp,"%d ",seq[i]);
        printf("%d ",seq[i]); 
    }
    fprintf(fp,"\n");
    printf("\n");
}

