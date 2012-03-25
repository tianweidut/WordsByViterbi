#!/bin/sh
echo "==================================================="
echo "              HMM���Ա�ע���V1.0"
echo "              ���ߣ�tianweidu"
echo "              QQ:416774905"
echo "==================================================="

while [ 1 = 1 ]
do
    echo "������ִ������"
    echo "\t\t0:ִ��Makefile���ɿ�ִ���ļ�"
    echo "\t\t1:��Training�ļ��н��д�������ģ��ѵ��������ԭʼ����"
    echo "\t\t2:����HMMģ���ļ�:data.hmm"
    echo "\t\t3:�Ա�ע�ļ�(���ĵ���)�������л�seq.seq"
    echo "\t\t4:����Viterbi�㷨�����д��Ա�ע"
    echo "\t\t5:��׼��⹤�ߣ����ɼ�����ļ�.crt"
    echo "\t\t6:Ĭ�ϲ���"
    echo "\t\t7:�˳�����"
    echo -n "��������Ӧ����:"
    read choice
    case "$choice" in
        0) 
            echo "make makefile"
            make all
            ;;
        1) 
            echo "ִ��ģ��ѵ���ű����ļ�����ű�"
            ./ttrain.sh
            ;;
        2)
            echo "ִ��HMMģ���ļ����ɽű�"
            ./hmm.sh
            ;;
        3)
            echo "ִ�����ĵ��ʣ��۲����У����л����ɽű�"
            ./seq.sh
            ;;
        4)
            echo "ִ��Viterbi�㷨�ű�"
            ./showViterbi.sh
            ;;
        5)
            echo "ִ��׼ȷ�ʼ��ű�"
            ./rightTest.sh
            ;;

        6)
            echo "Ĭ�ϲ���"
            make clean
            make all
            ./ttrain.sh
            ./hmm.sh
            ./seq.sh
            ./showViterbi.sh
            ./rightTest.sh
            ;;
        7)
            echo "-----------------��ӭ�ٴ�ʹ��^-^---------------"
            exit 0
            ;;
        *)
            echo "�Բ���������ָ������"
            ;;
    esac
done
exit 0
