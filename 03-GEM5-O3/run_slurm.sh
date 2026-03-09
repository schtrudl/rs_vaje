#!/bin/sh
#SBATCH --job-name=gem5_simulation    
#SBATCH --output=gem5_log.txt     
#SBATCH --reservation=fri
#SBATCH --cpus-per-task=2
#SBATCH --ntasks=1
#SBATCH --time=00:15:00

GEM5_WORKSPACE=/d/hpc/projects/FRI/GEM5/gem5_workspace
GEM5_ROOT=$GEM5_WORKSPACE/gem5
GEM5_PATH=$GEM5_ROOT/build/RISCV

srun apptainer exec $GEM5_WORKSPACE/gem5_rv.sif $GEM5_PATH/gem5.opt --outdir=cpu2_stats --debug-flags=O3PipeView  --debug-start=1000 --debug-file=trace.out  cpu_benchmark.py

#srun apptainer exec $GEM5_WORKSPACE/gem5_rv.sif python3 $GEM5_ROOT/util/o3-pipeview.py -o cpu2_stats/pipeview.out  --color cpu2_stats/trace.out

#apptainer exec $GEM5_WORKSPACE/gem5_rv.sif python3 $GEM5_ROOT/util/minorview.py --start-time 0 --end-time 100 trace.out