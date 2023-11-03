#!/bin/bash 

while true
do

exec 3<> /dev/ttyACM0
sleep 2
echo "input string" >&3

temps=$(cat <&3|head -n1)
atemp=$(echo $temps|awk -F ":" '{print $1}')
wtemp=$(echo $temps|awk -F ":" '{print $2}')

if [ -n $atemp ] || [ -n $wtemp ] ; then
#rrdtool update temp.rrd --template airtemp:watertemp N:$atemp:$wtemp
#	echo "$(date) - updated tempdata.rrd with $ftemp"
	echo "Ambient: $atemp   Water: $wtemp"
else
	echo "$(date) no data"
fi

exec 3>&-

done
