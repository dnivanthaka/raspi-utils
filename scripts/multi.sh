#!/bin/bash
# by Paul Colby (http://colby.id.au), no rights reserved ;)
# Modified for multi-processor display by D.N. Amerasinge <nivanthaka@gmail.com>

NUM_CPUS=`sed -n 's/^cpu[0-9]//p' /proc/stat | wc -l`

for num in $(seq 0 $((${NUM_CPUS} - 1))); do
PREV_TOTAL[$num]=0
PREV_IDLE[$num]=0
done

while true; do
  # Get the total CPU statistics, discarding the 'cpu ' prefix.
  for i in $(seq 0 $((${NUM_CPUS} - 1))); do
    CPU=(`sed -n "s/^cpu${i}//p" /proc/stat`)
    IDLE=${CPU[3]} # Just the idle CPU time.

    # Calculate the total CPU time.
    TOTAL=0
    for VALUE in "${CPU[@]}"; do
        let "TOTAL=$TOTAL+$VALUE"
    done

    # Calculate the CPU usage since we last checked.
    DIFF_IDLE=$((IDLE-${PREV_IDLE[i]}))
    DIFF_TOTAL=$((TOTAL-${PREV_TOTAL[i]}))
    let "DIFF_USAGE=(1000*($DIFF_TOTAL-$DIFF_IDLE)/$DIFF_TOTAL+5)/10"
    echo -en "CPU ${i}: $DIFF_USAGE% "

    # Remember the total and idle CPU times for the next check.
    PREV_TOTAL[$i]="$TOTAL"
    PREV_IDLE[$i]="$IDLE"
  done
    echo -en "\r\b\b"
    # Wait before checking again.
    sleep 1
done
