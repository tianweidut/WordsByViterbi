#!/bin/bash

#step1:���ļ����з���
rm -R train* set*                   #Ԥ����ɾ������Ҫ�ļ���
mkdir Training
cp ../Training/* ./Training     #Ԥ����������ʼ�����ļ�

train_file=(train1 train2 train3 train4 train5) #����ֻ���ÿո��������
set_file=(set1 set2 set3 set4 set5)
key_file=(key1 key2 key3 key4 key5)

echo "=====================���������ļ���========================="
for cnt in {1..5}
do
    echo -n "���������ļ���   " 
    echo -n "${train_file[cnt-1]}  "
    echo -n "${set_file[cnt-1]}  "
    echo  "${key_file[cnt-1]}"
    mkdir ${train_file[cnt-1]}  #����5����ͬ������
    mkdir ${set_file[cnt-1]}    #����5����ͬ�Ĳ���+ѵ����
    mkdir ${key_file[cnt-1]}    #����ѵ������ļ�
done

echo "========================���CRT�ļ�����========================="
cnt=0 
#���ļ���������ָ�
for f in $(ls ./Training/*.crt)
do
    #echo  $f $cnt "-->" ${set_file[cnt]}
    mv $f ${set_file[cnt]}
    cnt=$(expr $cnt + 1)
    if [ $cnt = 5 ]
    then
        #echo cnt
        cnt=0
    fi
done

#��ʣ���ļ�����
if [ -f ./Training/*.crt ]
then
    mv ./Training/*.crt ${set_file[0]}
fi

echo "========================�ƶ��ļ�========================"
echo �����������ݵ���ͬtrain��

for cnt in {1..5}
do
    #echo ${set_file[*]} "-->" ${train_file[cnt-1]}
    cp -R ${set_file[*]} ${train_file[cnt-1]}
    echo  ���set��${key_file[cnt-1]} "-->" ${train_file[cnt-1]}
    cp -R ${key_file[cnt-1]} ${train_file[cnt-1]}
done

rm -Rf ${set_file[*]}
rm -Rf ${key_file[*]}

echo "====================�ϲ������ļ�======================"
for cnt in {1..5}
do 
    echo -n "����${train_file[cnt-1]}"
    for num in {1..5}
    do
        echo -n "-->${train_file[cnt-1]}/set$num"
        if [ $cnt != $num ]; then
            for f in $(ls ${train_file[cnt-1]}/set$num/*.crt)
            do
                cat $f >> "${train_file[cnt-1]}/"train.crt
                
            done
            rm -R "${train_file[cnt-1]}/set$num/"
        else
            for f in $(ls ${train_file[cnt-1]}/set$num/*.crt)
            do
                cat $f >> "${train_file[cnt-1]}/"tst.crt
            done
            rm -R "${train_file[cnt-1]}/set$num/"
        fi  
    
    done
        #�˴��������ļ�Ҳ��Ϊѵ���ļ�����ѵ��
        cat ./"${train_file[cnt-1]}/"tst.crt >> ./"${train_file[cnt-1]}/"train.crt
done
echo "========================ѡ�����Լ���ѵ���������Բ��Լ����б���========================"
#����ÿ��train�ļ���,ѡ�����Լ���ѵ���������Բ��Լ����б���
#������ȷ���ж�
for cnt in {1..5}
do 
    echo ${train_file[cnt-1]}/${set_file[cnt-1]} "-->" ${train_file[cnt-1]} 
    mkdir ${train_file[cnt-1]}/right/
    cp  ${train_file[cnt-1]}/tst.crt ${train_file[cnt-1]}/right/
done


echo "========================���ѵ�����Ĵ��Ա�ע========================"
#ʹ��sed���й���
#sed -e 's/^[0-9]\{1,\}//' -->���˿�ʼ���
#-e 's/\/[a-zA-Z]\{1,\}//g' -->���˴��Բ���
#-e 's/[[:space:]]\{2,\}/ /g' -->���˶���ո�
#-e 's/^[[:space:]]//'  -->ȥ����ʼ�ո�
#BAIKE002.crt>BAIKE002  -->�ļ����Ϊ�����ļ�
for cnt in {1..5}
do
    #echo $(ls ${train_file[cnt-1]}/right/*) 
    for f in $(ls ${train_file[cnt-1]}/right/*) 
    do
        #echo "$f"   
        sed -e 's/^[0-9]\{1,\}//' -e 's/\/[a-zA-Z]\{1,\}//g' -e 's/[[:space:]]\{2,\}/ /g' -e 's/^[[:space:]]//' $f >> "$f+copy" 
        rm -f $f  
    done
    echo "ok:��� ${train_file[cnt-1]} ����"
done

echo "====================���з�������======================="
rm -R ./key/*

for cnt in {1..5}
do
    echo "����${train_file[cnt-1]}"
    ./moveMatrix ${train_file[cnt-1]}/train.crt
    mv ./key/* "${train_file[cnt-1]}/key$cnt/"
done    
    
exit 0

