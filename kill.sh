#!/bin/bash
countZ=$(ps -e | grep -c $1)
for (( i=0; i < countZ; i++ ))
do
    z=$(ps -e | grep -w $1)
    IFS=' ' read -a z <<< $z
    if [ -n "${z[0]}" ] ; then
	if [[ ${z[3]}=$1 ]]; then
	    #echo ${z[0]}
	    kill ${z[0]}
	    echo "process $1 ${z[0]} is killed"
	else
	    echo "Process $1 not found"
	fi
    fi
done
