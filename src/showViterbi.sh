#!/bin/bash

echo "-----------Éú³ÉÒş²ØĞòÁĞ---------------"

train_file=(train1 train2 train3 train4 train5)
key_file=(key1 key2 key3 key4 key5)
i=(data.hmm seq.seq out.crt flag_dict_file.crt ch.crt)

for cnt in {1..5}
do
    echo  ./${train_file[cnt-1]}/${i[0]} ./${train_file[cnt-1]}/${i[1]} ./${train_file[cnt-1]}/${i[2]} ./${train_file[cnt-1]}/${key_file[cnt-1]}/flag_dict_file.crt ch.crt
    ./showViterbi ./${train_file[cnt-1]}/${i[0]} ./${train_file[cnt-1]}/${i[1]} ./${train_file[cnt-1]}/${i[2]} ./${train_file[cnt-1]}/${key_file[cnt-1]}/${i[3]} ./${train_file[cnt-1]}/${i[4]}


done

exit 0
