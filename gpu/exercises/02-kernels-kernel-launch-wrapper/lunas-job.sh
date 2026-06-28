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
echo "some different combos"
srun ./wrapped 1 1
srun ./wrapped 1 2
srun ./wrapped 2 1
srun ./wrapped 1 10
srun ./wrapped 10 1
srun ./wrapped 10 10

echo "with 1025 threads"
srun ./wrapped 10 1025

