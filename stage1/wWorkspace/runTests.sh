#/bin/bash/

make clean
rm -r testing
mkdir testing
make stage1
file=Makefile


echo to use type in the data files name without extension e.g 001 or 102
echo they will be placed in the testing folder

if [ $# -lt 1 ]
then
   echo Enter more arguements
   
else

   sed -i 's/targets2srcfiles =.*/targets2srcfiles = /' $file
   for i in $@
   do
      echo $i ha ha ha
      
      #cd data
      if test -f data/$i.dat
      then
         
         mkdir testing/$i
         touch m$i.lst m$i.asm
         ./stage1 data/$i.dat m$i.lst m$i.asm
         
         
         if test -f data/$i.lst
         then
            diff m$i.lst data/$i.lst > ld$i.out
            mv ld$i.out testing/$i
            cp data/$i.lst testing/$i
         fi
         
         if test -f data/$i.asm
         then
            diff m$i.asm data/$i.asm > ad$i.out
            mv ad$i.out testing/$i
            cp data/$i.asm testing/$i
         fi
         
         sed -i 's/targetsAsmLanguage =.*/targetsAsmLanguage = m'$i'/' $file
         make m$i
         
         mv m$i m$i.lst m$i.asm testing/$i
         cp data/$i.dat testing/$i
      else
         echo $i.dat does not exist
      fi
      
   done
   
   rm *.o
   sed -i 's/targetsAsmLanguage =.*/targetsAsmLanguage =  /' $file
   sed -i 's/targets2srcfiles =.*/targets2srcfiles = stage1/' $file
fi


