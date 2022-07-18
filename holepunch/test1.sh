#!/bin/bash

printf "\n Remember to lower the refresh interval to 10 for this test ...\n\n"
sleep 2

insmod /holepunch/kernel_module/dm-holepunch.ko
printf "ooo\no\no" | holepunch create /dev/sdb1 5
printf "ooo\no" | holepunch open /dev/sdb1 test
mkfs.ext4 /dev/mapper/holepunch
mount /dev/mapper/holepunch /mnt/home
cd /mnt/home

fname=""
contents=""

for ((i=0; i<500; i++))
do
    printf -v fname "hello%03d" "$i"
    printf -v contents "hello from %03d" "$i"
    echo "$contents" > "$fname"
done

cd ..
umount home

mount /dev/mapper/holepunch /mnt/home
cd /mnt/home
ls

for ((i=0; i<498; i++))
do
    sleep 0.05
    printf -v fname "hello%03d" "$i"
    printf "Deleting $fname...\n"
    rm "$fname"
done

# cd ..
# umount home
# holepunch close test

# printf "ooo\no" | holepunch open /dev/sdb1 test
# mount /dev/mapper/holepunch /mnt/home
# cd /mnt/home

sleep 0.1
ls
cat "hello498"
cat "hello499"
# cat hi
# cat bar
# cat baz