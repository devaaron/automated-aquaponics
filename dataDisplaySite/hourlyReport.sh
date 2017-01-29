#!/bin/sh
#script for:
#calling python report program with encryption
#sending report to uxb3 to html page
#sending report to local http folder
#NOTE: script should be run with directory placement in mind, home/pi/data

#writes all values to list
python report.py tP.tL.d.hL.l.m

#gets selected values and writes them to JSON
python report.py tP.d.hL `date "+%d.%H"`

if [ $? -eq 0 ]
then
	
	#send to this http folder
	cp -r ./aqp/. ./http/aquaponics/.
	
	#send the data to the web server
	scp -r ./aqp/. ams231@uxb3.wiu.edu:~/http/aquaponics/aqp/.

	echo "hourlyReport.sh: report sent" >> scriptUsage.txt

	else
		echo "hourlyReport.sh: report not sent" >> scriptUsage.txt

fi

#record the use of the script
echo "system: hourlyReport.sh complete" >> scriptUsage.txt
date >> scriptUsage.txt

exit
