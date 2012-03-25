#!/bin/bash

#step1:将文件进行分组
rm -R train* set*                   #预处理：删除不必要文件夹
mkdir Training
cp ../Training/* ./Training     #预处理：拷贝初始测试文件

train_file=(train1 train2 train3 train4 train5) #数组只能用空格进行区分
set_file=(set1 set2 set3 set4 set5)
key_file=(key1 key2 key3 key4 key5)

echo "=====================创建功能文件夹========================="
for cnt in {1..5}
do
    echo -n "创建功能文件夹   " 
    echo -n "${train_file[cnt-1]}  "
    echo -n "${set_file[cnt-1]}  "
    echo  "${key_file[cnt-1]}"
    mkdir ${train_file[cnt-1]}  #构造5个不同的数集
    mkdir ${set_file[cnt-1]}    #构造5个不同的测试+训练集
    mkdir ${key_file[cnt-1]}    #构造训练结果文件
done

echo "========================完成CRT文件归类========================="
cnt=0 
#对文件进行随意分割
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

#对剩余文件处理
if [ -f ./Training/*.crt ]
then
    mv ./Training/*.crt ${set_file[0]}
fi

echo "========================移动文件========================"
echo 拷贝基本数据到不同train中

for cnt in {1..5}
do
    #echo ${set_file[*]} "-->" ${train_file[cnt-1]}
    cp -R ${set_file[*]} ${train_file[cnt-1]}
    echo  完成set和${key_file[cnt-1]} "-->" ${train_file[cnt-1]}
    cp -R ${key_file[cnt-1]} ${train_file[cnt-1]}
done

rm -Rf ${set_file[*]}
rm -Rf ${key_file[*]}

echo "====================合并测试文件======================"
for cnt in {1..5}
do 
    echo -n "处理${train_file[cnt-1]}"
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
        #此处将测试文件也作为训练文件进行训练
        cat ./"${train_file[cnt-1]}/"tst.crt >> ./"${train_file[cnt-1]}/"train.crt
done
echo "========================选出测试集和训练集，并对测试集进行备份========================"
#处理每个train文件夹,选出测试集和训练集，并对测试集进行备份
#用于正确性判断
for cnt in {1..5}
do 
    echo ${train_file[cnt-1]}/${set_file[cnt-1]} "-->" ${train_file[cnt-1]} 
    mkdir ${train_file[cnt-1]}/right/
    cp  ${train_file[cnt-1]}/tst.crt ${train_file[cnt-1]}/right/
done


echo "========================清除训练集的词性标注========================"
#使用sed进行过滤
#sed -e 's/^[0-9]\{1,\}//' -->过滤开始标号
#-e 's/\/[a-zA-Z]\{1,\}//g' -->过滤词性部分
#-e 's/[[:space:]]\{2,\}/ /g' -->过滤多余空格
#-e 's/^[[:space:]]//'  -->去除开始空格
#BAIKE002.crt>BAIKE002  -->文件令存为其他文件
for cnt in {1..5}
do
    #echo $(ls ${train_file[cnt-1]}/right/*) 
    for f in $(ls ${train_file[cnt-1]}/right/*) 
    do
        #echo "$f"   
        sed -e 's/^[0-9]\{1,\}//' -e 's/\/[a-zA-Z]\{1,\}//g' -e 's/[[:space:]]\{2,\}/ /g' -e 's/^[[:space:]]//' $f >> "$f+copy" 
        rm -f $f  
    done
    echo "ok:完成 ${train_file[cnt-1]} 处理"
done

echo "====================运行分析程序======================="
rm -R ./key/*

for cnt in {1..5}
do
    echo "处理${train_file[cnt-1]}"
    ./moveMatrix ${train_file[cnt-1]}/train.crt
    mv ./key/* "${train_file[cnt-1]}/key$cnt/"
done    
    
exit 0

