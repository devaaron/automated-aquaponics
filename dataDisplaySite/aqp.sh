#!/bin/sh
#script for:
#calling python report program with encryption
#sending report to uxb3 to html page
#accumLog or reportLog needs a [ in front and at end

#create dailyReport.txt with today's sensor values

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
	#check -r syntax!!

#tempPlant append statements
cat ./aqp/tempPlant.txt >> ./aqp/accumtempPlant.txt
cp ./aqp/chartSettingsHead.txt ./aqp/temp.txt
cat ./aqp/accumtempPlant.txt >> ./aqp/temp.txt
cat ./aqp/chartSettingsTail.txt >> ./aqp/temp.txt
cp ./aqp/temp.txt ./aqp/tempPlant.json
echo "," >> ./aqp/accumtempPlant.txt

#depth append statements
cat ./aqp/depth.txt >> ./aqp/accumdepth.txt
cp ./aqp/chartSettingsHead.txt ./aqp/temp.txt
cat ./aqp/accumdepth.txt >> ./aqp/temp.txt
cat ./aqp/chartSettingsTail.txt >> ./aqp/temp.txt
cp ./aqp/temp.txt ./aqp/depth.json
echo "," >> ./aqp/accumdepth.txt

#humidityLamp append statements
cat ./aqp/humidityLamp.txt >> ./aqp/accumhumidityLamp.txt
cp ./aqp/chartSettingsHead.txt ./aqp/temp.txt
cat ./aqp/accumhumidityLamp.txt >> ./aqp/temp.txt
cat ./aqp/chartSettingsTail.txt >> ./aqp/temp.txt
cp ./aqp/temp.txt ./aqp/humidityLamp.json
echo "," >> ./aqp/accumhumidityLamp.txt

#tempLamp append statements
cat ./aqp/tempLamp.txt >> ./aqp/accumtempLamp.txt
cp ./aqp/chartSettingsHead.txt ./aqp/temp.txt
cat ./aqp/accumtempLamp.txt >> ./aqp/temp.txt
cat ./aqp/chartSettingsTail.txt >> ./aqp/temp.txt
cp ./aqp/temp.txt ./aqp/tempLamp.json
echo "," >> ./aqp/accumtempLamp.txt

#light append statements
cat ./aqp/light.txt >> ./aqp/accumlight.txt
cp ./aqp/chartSettingsHead.txt temp.txt
cat ./aqp/accumlight.txt >> temp.txt
cat ./aqp/chartSettingsTail.txt >> temp.txt
cp temp.txt ./aqp/light.json
echo "," >> ./aqp/accumlight.txt

#pH append statements
cat ./aqp/moisture.txt >> ./aqp/accummoisture.txt
cp ./aqp/chartSettingsHead.txt temp.txt
cat ./aqp/accummoisture.txt >> temp.txt
cat ./aqp/chartSettingsTail.txt >> temp.txt
cp temp.txt ./aqp/moisture.json
echo "," >> ./aqp/accummoisture.txt


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
