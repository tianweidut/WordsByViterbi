#!/bin/bash


echo "========================���ѵ�����Ĵ��Ա�ע========================"
#ʹ��sed���й���
#sed -e 's/^[0-9]\{1,\}//' -->���˿�ʼ���
#-e 's/\/[a-zA-Z]\{1,\}//g' -->���˴��Բ���
#-e 's/[[:space:]]\{2,\}/ /g' -->���˶���ո�
#-e 's/^[[:space:]]//'  -->ȥ����ʼ�ո�
#BAIKE002.crt>BAIKE002  -->�ļ����Ϊ�����ļ�
sed -e 's/^[0-9]\{1,\}//' -e 's/\/[a-zA-Z]\{1,\}//g' -e 's/[[:space:]]\{2,\}/ /g' -e 's/^[[:space:]]//' "./right.crt" >> "right2.crt" 

exit 0
