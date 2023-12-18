#/bin/bash
rm -r data
rm -r source
rm -r executables
mkdir data
mkdir source
mkdir executables
rm Makefile
rm stage2.h
rm stage2main.C


cp /usr/local/4301/src/Makefile .
cp /usr/local/4301/include/stage2.h .
cp /usr/local/4301/src/stage2main.C .

file=Makefile
cp Makefile data

sed -i 's/targets2srcfiles =.*/targets2srcfiles = stage2/' $file


cd data

for i in {201..249}
do

   cp /usr/local/4301/data/stage2/${i}.dat .
   cp /usr/local/4301/data/stage2/${i}.lst .
   cp /usr/local/4301/data/stage2/${i}.asm .
   
done

cp /usr/local/4301/src/Makefile .

clear

cd ..
ls 