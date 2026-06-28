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
echo "input 1/1'000'000/512"
srun ./taylor 1 1000000 512

echo "input 1/10'000'000/512"
srun ./taylor 1 10000000 512

echo "input 1/100'000'000/512"
srun ./taylor 1 100000000 512

echo "input 10/1'000'000/512"
srun ./taylor 10 1000000 512

echo "input 10/10'000'000/512"
srun ./taylor 10 10000000 512

echo "input 10/100'000'000/512"
srun ./taylor 10 100000000 512


