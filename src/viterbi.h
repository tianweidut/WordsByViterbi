 /*=============================================================================
#
# Author: tianweidut - liutianweidlut@gmail.com
#
# QQ : 416774905
#
# Last modified: 2010-12-08 16:18
#
# Filename: viterbi.h
#
# Description: 维特比算法
#
=============================================================================*/ 
#ifndef __viterbi__
#define __viterbi__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>

#define OK 1
#define NO 0

typedef struct      //定义HMM模型      
{
    int N;          //词性数量
    int M;          //中文单词
    double **A;     //转移概率矩阵
    double **B;     //发射概率矩阵
    double *pi;     //初始概率
}HMM;

void ReadHMM(FILE *fp,HMM *phmm);   //从文件中获取HMM模型
void FreeHMM(HMM *phmm);            //进行空间释放
int ReadSequence(FILE *fp,int *pT,int **pO);   //获取每句话：pT 数量 pO观察序列
void PrintSequence(FILE*fp,int T,int *seq);
void Viterbi(HMM *phmm,int T,int *seq,double **A,int **B,int *res,double *max);
double **dcreateMatrix(int rs,int re,int cs,int ce);
int **icreateMatrix(int rs,int re,int cs,int ce);
double *dcreateArray(int start,int end);
int *icreateArray(int start,int end);
void free_array(int *arr,int s,int e);
void free_darray(double *arr,int s,int e);
void free_dmatrix(double **m,int s1,int e1,int s2,int e2);
void free_imatrix(int **m,int s1,int e1,int s2,int e2);


#endif
