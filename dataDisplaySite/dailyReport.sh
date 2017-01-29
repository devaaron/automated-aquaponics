#!/bin/sh
#script for:
#calling python report program with encryption
#sending report to uxb3 to html page
#sending report to local http folder
#NOTE: script should be run with directory placement in mind, home/pi/data

#get max of temp @ lamps and light
python report.py - `date "+%m.%d"` max tL.l

#get avg of light
python report.py - `date "+%m.%d"` avg l

#get avg of moisture (don't write)
python report.py - `date "+%m.%d"` avg m

if [ $? -eq 0 ]
then
	
	#send to this http folder
	cp -r ./aqp/. ./http/aquaponics/.
	
	#send the data to the web server
	scp -r ./aqp/. ams231@uxb3.wiu.edu:~/http/aquaponics/aqp/.

	echo "dailyReport.sh: report sent" >> scriptUsage.txt

	else
		echo "dailyReport.sh: report not sent" >> scriptUsage.txt

fi

#record the use of the script
echo "system: dailyReport.sh complete" >> scriptUsage.txt
date >> scriptUsage.txt

exit
