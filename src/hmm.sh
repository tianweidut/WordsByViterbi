#!/bin/bash

echo "HMM概率文件生成"

train_file=(train1 train2 train3 train4 train5)
key_file=(key1 key2 key3 key4 key5)
i=(ch_dict_file.crt flag_dict_file.crt move_dict_file.crt map_dict_file.crt data.hmm pi.crt)

for cnt in {1..5}
do
    echo "./${train_file[cnt-1]}/${key_file[cnt-1]}/"
    cp hmm ./${train_file[cnt-1]}/${key_file[cnt-1]}/
    cd  ./${train_file[cnt-1]}/${key_file[cnt-1]}/
    pwd
    echo ${i[0]} ${i[1]} ${i[2]} ${i[3]} ${i[4]} ${i[5]}
    ./hmm ${i[0]} ${i[1]} ${i[2]} ${i[3]} ../${i[4]} ${i[5]}
    cd ../../
    #pwd
    #加入程序执行部分
done

