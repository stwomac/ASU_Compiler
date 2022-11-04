#/bin/bash
make stage0

num=1;
rm -r outputs
mkdir outputs
for i in {1..9}
do
    echo ${i}
	touch m${i}.lst m${i}.asm
	./stage0 00${i}.dat m${i}.lst m${i}.asm
	mv m${i}.lst m${i}.asm outputs
	cp 00${i}.dat outputs
	cd outputs
	mkdir 0${i}
	mv 00${i}.dat m${i}.lst m${i}.asm 0${i}
	cd ..
	(( num++ ))
    sleep 0.2
done

for i in {10..52}
do
	touch m${i}.lst m${i}.asm
	./stage0 0${i}.dat m${i}.lst m${i}.asm
	mv m${i}.lst m${i}.asm outputs
	cp 0${i}.dat outputs
	cd outputs
	mkdir ${i}
	mv 0${i}.dat m${i}.lst m${i}.asm ${i}
	cd ..
	(( num++ ))
done


