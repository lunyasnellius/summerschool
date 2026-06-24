#!/bin/bash -x
#SBATCH --account=project_462001452
#SBATCH --reservation=SummerSchoolCPU
#SBATCH --partition=small
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=2
#SBATCH --time=00:05:00
#SBATCH --output=out/%x.%j.out
#SBATCH --error=out/%x.%j.err

# Run the program
srun $@
srun $@
srun $@
srun $@
#srun $SLURM_JOB_NAME
