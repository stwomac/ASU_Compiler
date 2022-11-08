#/bin/bash
make stage0

rm -r outputs
mkdir outputs
for i in {101..178}
do
   echo ${i} ha ha ha
	touch m${i}.lst m${i}.asm
	./stage0 ${i}.dat m${i}.lst m${i}.asm
   
   
   if test -f ${i}.lst
   then
      diff m${i}.lst ${i}.lst > ld${i}.out
      mv ld${i}.out outputs
      cp ${i}.lst outputs
   fi
   
   if test -f ${i}.asm
   then
      diff m${i}.asm ${i}.asm > ad${i}.out
      mv ad${i}.out outputs
      cp ${i}.asm outputs
   fi
   
	mv m${i}.lst m${i}.asm outputs
	cp ${i}.dat outputs
	cd outputs
	mkdir ${i}
	mv ${i}.dat m${i}.lst m${i}.asm ${i} 
   
   if test -f ${i}.lst
   then
      mv ld${i}.out ${i}.lst ${i} 
   fi
   
   if test -f ${i}.asm
   then
      mv ad${i}.out ${i}.asm ${i} 
   fi
   
	cd ..
    
done



