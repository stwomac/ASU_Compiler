; JohnPaul Flores & Steven Womack    Fri Nov  4 17:47:10 2022
%INCLUDE "Along32.inc"
%INCLUDE "Macros_Along.inc"

SECTION .text                           
global  _start                          ; program stage0no052

_start:                                 
        Exit    {0}                     

SECTION .data                           
B1      dd      0                       ; no
B3      dd      0                       ; should_be_false
B2      dd      -1                      ; should_be_true
B0      dd      -1                      ; yes

SECTION .bss                            
