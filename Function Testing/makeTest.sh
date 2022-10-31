#! /bin/sh
echo "To use cp the baseTemplate and rename it testTemplate, make changes to is as you will"
g++ -g -Wall -std=c++11 -c testTemplate.cpp -I/usr/local/4301/include/ -I.
g++ -g -Wall -std=c++11 -c stage0main.C -I/usr/local/4301/include/ 
g++ -o testTemplate stage0main.o testTemplate.o -L/usr/local/4301/lib -lm
