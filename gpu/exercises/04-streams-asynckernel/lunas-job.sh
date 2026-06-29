#!/bin/bash -x
#SBATCH --account=project_462001452
#SBATCH --reservation=SummerSchoolGPU
#SBATCH --partition=small-g
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --time=00:05:00
#SBATCH --output=out/%x.%j
#SBATCH --error=err/%x.%j
#SBATCH --cpus-per-task=4
#SBATCH --gpus-per-node=1



# Run the program
# srun ./$@
srun  rocprofv3 --runtime-trace --output-format pftrace -- ./$@

