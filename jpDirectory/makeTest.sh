# !bin/bash
echo "To use cp the baseTemplate and rename it testTemplate, make changes to" echo "is as you will"
g++ -g -Wall -std=c++11 -c jp.cpp -I/usr/local/4301/include/ -I.
g++ -g -Wall -std=c++11 -c stage0main.C -I/usr/local/4301/include/ -I.
g++ -o jp stage0main.o jp.o -L/usr/local/4301/lib/ -lm