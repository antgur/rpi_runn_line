Project:

RPI based four 8x8 LED segments running line

Structure:

Two software items both running as system services having common variables in 
share memory section.
1. cl_serv
2. cl_cli

cl_serv driving 4x8x8 LED indicator. It shifting picture of strings which codes
are located in share memory shifting strings array. Shifting speed depends
on var constants also located in share memory section. Detailed vars 
describtion is located in the file utils/ipcr.h.

cl_cli managing clients connecting via TCP protocol. It also running as system 
service. Waiting from the client codes for shifting strings and after each 
string shifted aut from the line returns for the client remaining strings in
the array queueing for the shifting.

