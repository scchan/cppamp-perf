#!/bin/bash


export PATH=$PATH:/opt/amd/bin
#export PATH=$PATH:/opt/amd.D2/bin/

#export PATH=$PATH:/home/xkerox/Code/github/HSAFoundation/HSAIL-HLC-Development/bin/

clc2 -cl-std=CL2.0 -o $1.bc $1 
llvm-dis -o $1.ll $1.bc


#Info:  Link(llvm-link) bc --> lnkd.bc ...
llvm-link  -prelink-opt  -l /opt/amd/bin//builtins-hsail.bc -o $1.lnkd.bc $1.bc


#Info:  Optimize(opt)   lnkd.bc --> opt.bc ...
#opt -O3 -gpu -whole -o ./tmp/vector_copy.opt.bc ./tmp/vector_copy.lnkd.bc


#Info:  llc arch=hsail  opt.bc --> brig ...
#llc -O2 -march=hsail-64 -o ./tmp/vector_copy.brig ./tmp/vector_copy.opt.bc

# disable optimization
llc -march=hsail-64 -o $1.lnkd.brig $1.lnkd.bc


#Info:  HSAILasm        brig --> vector_copy.hsail ...
hsailasm -disassemble -o $1.lnkd.hsail $1.lnkd.brig




