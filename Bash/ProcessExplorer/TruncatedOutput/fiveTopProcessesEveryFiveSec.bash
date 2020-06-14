#!/bin/bash
#This simple script will display the top five processes running based on cpu consumption percentage. The command field will be truncated to supply a much cleaner look.
#I understand there is a bevy of cli utilities that already accomplish this, but this was much, much lighter and gave me exactly what I needed.
#If the "sort" flag is edited this can be adjusted to what ever value desired.
clear
while [ true ]
do
	ps -eo user,stat,cmd,pmem,start,time,pid,pcpu --sort=-%cpu | head -6
	echo
	echo
	sleep 5
done
