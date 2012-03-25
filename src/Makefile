#=============================================================================
#
# Author: tianweidut - liutianweidlut@gmail.com
#
# QQ : 416774905
#
# Last modified: 2010-12-09 23:19
#
# Filename: Makefile
#
# Description: Makefile
#
#=============================================================================
CFLAGS= -g -std=gnu99 
INCS= 
CC= gcc
SRCS= viterbi.h hmm.c moveMatrix.c seq.c showViterbi.c viterbi.c eva.c

all:	moveMatrix hmm seq showViterbi eva

moveMatrix:	moveMatrix.o
	$(CC) moveMatrix.c -o moveMatrix $(CFLAGS)  

hmm:	hmm.o
	$(CC) hmm.c -o hmm $(CFLAGS)  

seq:	seq.o
	$(CC) seq.c -o seq $(CFLAGS)  

showViterbi:	showViterbi.o viterbi.o viterbi.h
	$(CC) viterbi.c showViterbi.c -o showViterbi $(CFLAGS)  

eva:	eva.o
	$(CC) eva.c -o eva $(CFLAGS)

clean:
	-rm *.o a.out
