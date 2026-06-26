#!/bin/bash
#SBATCH --job-name=mandeljob
#SBATCH --account=project_462001452
#SBATCH --reservation=SummerSchoolCPU
#SBATCH --partition=small
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=1
#SBATCH --mem-per-cpu=1G
#SBATCH --time=00:05:00
#SBATCH --out=out/%x.%j.out

# Set the number of threads based on cpus-per-task
#export OMP_NUM_THREADS=2
export OMP_NUM_THREADS=${SLURM_CPUS_PER_TASK:-1}
#export OMP_PROC_BIND=close
#export OMP_PLACES=threads

# Run the program
echo "adaptive:"
srun --exclusive ./adaptive 5
srun --exclusive ./adaptive 6
srun --exclusive ./adaptive 7
srun --exclusive ./adaptive 8
srun --exclusive ./adaptive 9
srun --exclusive ./adaptive 10

echo "blocked:"
srun --exclusive ./blocked 5
srun --exclusive ./blocked 6
srun --exclusive ./blocked 7
srun --exclusive ./blocked 8
srun --exclusive ./blocked 9

echo "direct:"
srun --exclusive ./direct 5
srun --exclusive ./direct 6
srun --exclusive ./direct 7
srun --exclusive ./direct 8
srun --exclusive ./direct 9


