; JohnPaul Flores & Steven Womack    Fri Nov  4 17:47:09 2022
%INCLUDE "Along32.inc"
%INCLUDE "Macros_Along.inc"

SECTION .text                           
global  _start                          ; program stage0no022

_start:                                 
        Exit    {0}                     

SECTION .data                           
B1      dd      0                       ; no
I0      dd      0                       ; small
I1      dd      -1                      ; smalleryet
B0      dd      -1                      ; yes

SECTION .bss                            
I3      resd    1                       ; aaaaaaaaaaaaaaa
B2      resd    1                       ; right
I2      resd    1                       ; some
