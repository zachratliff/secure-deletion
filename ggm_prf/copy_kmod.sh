#!/bin/sh

make 
sudo mount -o loop ~/Desktop/holepunch/jessie.img /mnt/jessie
sudo cp ggm_prf.ko /mnt/jessie
sudo umount /mnt/jessie