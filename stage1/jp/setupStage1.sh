#/bin/bash
rm -r data
mkdir data
mkdir source
mkdir executables
rm Makefile
rm stage1.h
rm stage1main.C


cp /usr/local/4301/src/Makefile .
cp /usr/local/4301/include/stage1.h .
cp /usr/local/4301/src/stage1main.C .

file=Makefile
cp Makefile data

sed -i 's/targets2srcfiles =.*/targets2srcfiles = stage1/' $file


cd data

for i in {101..178}
do

   cp /usr/local/4301/data/stage1/${i}.dat .
   cp /usr/local/4301/data/stage1/${i}.lst .
   cp /usr/local/4301/data/stage1/${i}.asm .
   
done

cp /usr/local/4301/src/Makefile .

clear

cd ..
ls 