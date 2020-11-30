#!/bin/sh

# sed -i "s/ԭ�ַ���/���ַ���/g" `grep ԭ�ַ��� -rl ����Ŀ¼`
# [ 0 = 0 ] && echo "yes"|| echo "not"

# ��־�ļ� ���Ƹ�ʽ��  ����_����.log
fn_log_expire()
{
    log_max_num=$1
    log_dir=$2
    for file in `ls *.log |sort -g -r`
    do
        file_name=$(basename "$file")
        extension=${file_name##*.}
        file_name=${file_name%.*}
        numb_part=${file_name##*_}
        base_name="${file_name%%_*}_" 
        # echo "$file_name.$extension $base_name $numb_part"
        [ $numb_part -le $log_max_num ] && mv "$file" "$base_name$(($numb_part +1))".$extension
    done
}