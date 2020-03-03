#!/bin/sh

trap "wait" EXIT

if [ $# -ne 4 ]
then 
	echo "Usage: $0 : rhost port connections_per_second time_to_run"
	exit 1
else
	RHOST=$1
	PORT=$2
	CONC=$3
	LIFETIME=$4
fi

spawn()
{
		echo "hi!" | nc $RHOST $PORT -w1 1>/dev/null
}

b=0

for i in $(seq 0 $LIFETIME)
do
	for j in $(seq 0 $CONC)
	do
		spawn &
	done

	sleep 0.1

	b=$(echo $i*$CONC | bc)
	echo "$b: $CONC connections spun up"
done

wait
