#!/bin/sh
echo "==================================================="
echo "              HMM词性标注软件V1.0"
echo "              作者：tianweidu"
echo "              QQ:416774905"
echo "==================================================="

while [ 1 = 1 ]
do
    echo "请输入执行任务："
    echo "\t\t0:执行Makefile生成可执行文件"
    echo "\t\t1:对Training文件夹进行处理，进行模型训练，生成原始数据"
    echo "\t\t2:生成HMM模型文件:data.hmm"
    echo "\t\t3:对标注文件(中文单词)进行序列化seq.seq"
    echo "\t\t4:利用Viterbi算法，进行词性标注"
    echo "\t\t5:标准检测工具，生成检测结果文件.crt"
    echo "\t\t6:默认操作"
    echo "\t\t7:退出程序"
    echo -n "请输入相应数字:"
    read choice
    case "$choice" in
        0) 
            echo "make makefile"
            make all
            ;;
        1) 
            echo "执行模型训练脚本及文件处理脚本"
            ./ttrain.sh
            ;;
        2)
            echo "执行HMM模型文件生成脚本"
            ./hmm.sh
            ;;
        3)
            echo "执行中文单词（观察序列）序列化生成脚本"
            ./seq.sh
            ;;
        4)
            echo "执行Viterbi算法脚本"
            ./showViterbi.sh
            ;;
        5)
            echo "执行准确率检测脚本"
            ./rightTest.sh
            ;;

        6)
            echo "默认操作"
            make clean
            make all
            ./ttrain.sh
            ./hmm.sh
            ./seq.sh
            ./showViterbi.sh
            ./rightTest.sh
            ;;
        7)
            echo "-----------------欢迎再次使用^-^---------------"
            exit 0
            ;;
        *)
            echo "对不起请输入指定数字"
            ;;
    esac
done
exit 0
