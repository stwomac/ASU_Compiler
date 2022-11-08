#/bin/bash
rm -r data
mkdir data
cd data
mkdir outputs

num=1

cp /usr/local/4301/src/Makefile .
cp /usr/local/4301/include/stage1.h .
cp /usr/local/4301/src/stage1main.C .

for i in {101..178}
do

   cp /usr/local/4301/data/stage1/${i}.dat .
   cp /usr/local/4301/data/stage1/${i}.lst .
   cp /usr/local/4301/data/stage1/${i}.asm .
   
done


clear
ls