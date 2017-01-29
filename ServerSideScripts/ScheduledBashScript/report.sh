#!/bin/sh
#script for:
#calling python report program with encryption
#sending report to uxb3 to html page
#sending report to local http folder

report.sh

python report.py `date "+%m%d.%H"`

if [ $? -eq 0 ]
then

	#accumLog needs a [ in front
	#accumLog does not get erased daily
	#throw the newly gotten dailyReport at the end of accumLog
	head ./aqp/dailyReport.txt >> ./aqp_sh_files/accumLog.txt

	cp ./aqp_sh_files/accumLog.txt ./aqp/reportLog.txt

	#a comma is needed between each JSON array 
	echo "," >> ./aqp_sh_files/accumLog.txt

	#a ] is needed at the current days JSON script end
	echo "]" >> ./aqp/reportLog.txt


	#send the data to the web server
	scp -r ../aqp/. ams231@uxb3.wiu.edu:~/http/aquaponics/aqp/.

	echo "aqp.sh: report sent" >> scriptUsage.txt

	else
		echo "aqp.sh: report not sent" >> scriptUsage.txt

fi

#record the use of the script
echo "system: aqp.sh complete" >> scriptUsage.txt
date >> ./aqp/scriptUsage.txt

exit
