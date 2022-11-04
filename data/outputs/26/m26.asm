; JohnPaul Flores & Steven Womack    Thu Nov  3 19:20:01 2022
%INCLUDE "Along32.inc"
%INCLUDE "Macros_Along.inc"

 SECTION   .text                        
  global  _start                        ; program stage0no026

 _start:                                
        Exit    {0}                     

SECTION .data                           

SECTION .bss                            
I2      resd    1                       ; v
I0      resd    1                       ; var000
I1      resd    1                       ; var001
