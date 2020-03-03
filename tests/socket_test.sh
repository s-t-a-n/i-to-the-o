#!/bin/sh


if [ $# -ne 3 ]
then 
	echo "Usage: $0 : rhost port number_of_connections"
	exit 1
else
	RHOST=$1
	PORT=$2
	CONC=$3
fi

for i in $(seq 0 $CONC)
do
	(					\
		nc $RHOST $PORT -w1 -z; 	\
	 	echo "connection $i is spun up" \
	)&
done

wait
