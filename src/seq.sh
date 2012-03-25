#!/bin/bash

echo "-----------处理测试文件，根据字典生成测试序列---------------"

train_file=(train1 train2 train3 train4 train5)
key_file=(key1 key2 key3 key4 key5)
i=(ch_dict_file.crt seq.seq tst.crt+copy)

for cnt in {1..5}
do
    echo "./${train_file[cnt-1]}/${key_file[cnt-1]}/${i[0]} ./${train_file[cnt-1]}/${i[1]} ./${train_file[cnt-1]}/right/${i[2]}"
    ./seq ./${train_file[cnt-1]}/${key_file[cnt-1]}/${i[0]} ./${train_file[cnt-1]}/${i[1]} ./${train_file[cnt-1]}/right/${i[2]}
done

exit 0
