make clean
make
mprun.sh --job-name FlaviusOpteron --queue ibm-opteron.q --modules libraries/atlas-3.10.1-gcc-4.4.6-opteron --script run_opteron.sh --show-qsub --show-script --batch-job
mprun.sh --job-name FlaviusNehalem --queue ibm-nehalem.q --modules "libraries/atlas-3.10.1-gcc-4.4.6-nehalem" --script run_nehalem.sh --show-qsub --show-script --batch-job
mprun.sh --job-name FlaviusQuad --queue ibm-quad.q --modules "libraries/atlas-3.10.1-gcc-4.4.6-quad" --script run_quad.sh --show-qsub --show-script --batch-job
