#!/bin/sh

export PROJECT="/scratch/project_462001452/meyerl"
alias qu="squeue --me"
alias fCC="CC -O3 -fopenmp"
alias myhipCC="CC -xhip -O3"

if [[ "$part" == "GPU" ]]; then
	module load LUMI partition/G rocm 
elif [[ "$part" == "CPU" ]]; then
	module load LUMI partition/C 
else
	echo hiiii 
fi


