#!/bin/bash

function RunSimulation {
    TAM=( 0 1 2 )
    #TAM=( 0 )
    L=( 10000 20000 30000 30000 20000 10000 )
    #echo ${L[0]}	
    #TAM=( 1048576 2097152 )
    echo "Number of processes $1"
    echo "---------------------------------------------------------"
    
    for T in ${TAM[@]}; do
	echo "---------------------------------------------------------"    	
	echo "[!] Running with [n = $T ] ..."
     	for i in $(seq 1 $3); do
        	echo "Run $i ..."
        	./$2 $1 ${L[$T]} ${L[5-$T]} >> ./Outputs/times
		done
    	./average.sh ./Outputs/times >> ./Outputs/output$T
    	rm ./Outputs/times
    	echo "---------------------------------------------------------"
    done
}


# ================== MAIN SCRIPT ===================================
EXECUTABLE="pth_code"
#THREADS=( 1 2 4 8 16 )
NUMRUNS=5
THREADS=( 1 2 4 8 16 )

# Run the simulations with given the executable
echo "========================================================================="
echo "[!] Running speedup simulation for the executable [$EXECUTABLE]"
if [ ! -f $EXECUTABLE ]; then
    make
else
    make clean; make
fi

for THREAD in ${THREADS[@]}; do
    echo "=========================================================="
    echo "$THREAD" >> ./Outputs/output
    RunSimulation $THREAD $EXECUTABLE $NUMRUNS
    echo "=========================================================="
done

CMD=""
FOLDERS=$(ls ./Outputs | sort)
for FOLDER in ${FOLDERS[@]}; do
    CMD=$CMD$FOLDER" "
done
cd ./Outputs
paste $CMD > ../Plot/times.dat
cd ..

#make plot
