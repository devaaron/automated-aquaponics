"Author: Aaron Snowden"
"report.py: reads in the data from the arduino, sends values to log files"
"report.py runs on one or two arguments"
"if it is given one: the sensor data is recorded and a json object file is given"
"if it is given two: the sensor data is recorded and a python dictionary is given"
    #the former case is for direct data into a chart, the latter is for keeping average data
	
#!DEMULTIPLEX WHERE TO, JUST WRITE TO BOTH!
	
import __future__
from serial import Serial
import string
import json
import sys
import os
import shutil

args = 0
reportList = [{},{},{},{},{},{}]
nameList = ["tempPlant", "depth", "humidityLamp", "tempLamp", "light", "moisture"]
argRefDict = {"tP":nameList[0], "d":nameList[1], "hL":nameList[2], "tL":nameList[3],"l":nameList[4], "m":nameList[5]}
xyLabel = ["unit", "value"]
destDirect = "/aqp/"
agList = ["max", "min", "avg", "-"]

def dictWriteAll(list): #write list inList into a dictionary
    report_dict = {nameList[0]: list[0], nameList[1]: list[1], nameList[2]: list[2], nameList[3]: list[3], nameList[4]: list[4], nameList[5]: list[5]}
    print "dictWriteAll: dictionary entry:"
    print report_dict
    file = "dailyReport.txt"
    #writeToJSON(report_dict, file)
def writeGivenToList(givenList):
    for i in range(0, len(givenList)):
        file = givenList[i] + "_list.txt"
		newValue = 0
		for j in range (0, len(nameList)):
			if givenList[i] == nameList[j]:
				newValue = reportList[i]
				writeToList(file, newValue)
def writeGivenToJSON(givenList):
	for i in range(0, len(givenList)):
		sourceFile = givenList[i] + "_list.txt"
		prevFile = givenList[i] + ".txt"
		nextFile = destDirect + givenList[i] + ".json"
		valueFromSource = 0
		for j in range (0, len(nameList)):
			if givenList[i] == nameList[j]:
			valueFromSource = listToValue(sourceFile)
			dict = valueToDict(str(sys.args[2]), valueFromSource)
			writeJSON_chart(dict, prevFile, nextFile)
#LIST TO AG -> AG -> ENTRYTODICT -> WRITETOJSONCHART
#File TO ENTRY -> ENTRYTODICT -> WRITETOJSONCHART
def valueToDict(xValue, yValue):
	dict = {xyLabel[0]: xValue, xyLabel[1]: yValue}
	return dict
def getDict(sourceList)
	yValue = listToValue(sourceList)
	dict = valueToDict(str(sys.args[2])), yValue)
	return dict
def listToValue(file): #removes the given file, returns the aggregate if there is one, else returns last entry of file
	result = 0
	if args == 2:
		result = lastInListToValue(file)
	elif args == 4:
		if (str(sys.args[3]) == "max"):
			result = getMax(file)
		elif (str(sys.args[3]) == "min"):
			result = getMin(file)
		elif (str(sys.args[3]) == "avg"):
			result = getAvg(file)
		elif (str(sys.args[3]) == "-"):
			result = lastInListToValue(file)
		else:
			print "input 3 error, aggregate option not recognized"
	os.remove(file)
	return result
def lastInListToValue(fileName):
	file = open(fileName, 'r')
	list = file.readLines();
	value = list[len(list)-1]
	return value
def getAvg(fileName):
    file = open(fileName, 'r')
    values = file.readlines()
    sum = 0
    avg = 0
    for i in range(0, len(values)):
        sum += int(values[i])
    avg = sum/(len(values))
    file.close
    return avg
def getMax(fileName):
    file = open(fileName, 'r')
    values = file.readlines()
    max = int(values[0])
    for i in range (0, len(values)):
        if int(values[i]) > max:
            max = int(values[i])
    file.close
    return max
def getMin(fileName):
    file = open(fileName, 'r')
    values = file.readlines()
    min = int(values[0])
    for i in range (0, len(values)):
        if int(values[i]) < min:
            min = int(values[i])
    file.close
    return min
	#if list gets too large, erase first 10 values
def writeToList(fileName, addedValue):
	file = open(fileName, 'a')
	file.write(addedValue + "\n")
	file.close
def diminListSize(fileName): #diminish the number of values separated by ", "
	list = file.readlines();
	if len(list) == 1 or list[0] == "":
		print fileName + " at length 0"
		file.write(str(addedValue)
	elif len(list) < 80:
		file.write("\n"+str(addedValue))
	elif len(list) > 80:
		file.close
		os.remove(fileName)
		file = open(fileName, 'w')
		for i in range (10, len(list))
			#DEBUGGING TO MAKE SURE i STARTS @ 10
			print i 
			if i > 10:
				file.write("\n"+list[i])
			elif i == 10:
				file.write(list[i])
def listInputError(n):#when can't be broken into list, give this error
    print ("listInputError: Irregular number of sensor reports")
    print ("listInputError: n =")
    print (n)
    print (inList[n-1])
    exit(3)
def receiveData():
    if not (arduinoCheck()):
       exit(1)
    ser = Serial('/dev/ttyACM0', 9600, timeout = 1)
    ack = False
    r=0
    input = ""
    while(ack == False):
        ser.write("REPORT\n")
        input = ser.readline()
        if (input.find('REPORTACK') != -1):
            print (input.find('REPORTACK'))
            ack = True
    input = ser.readline()
    print ("receiveData: "+input)
    inList = input.split(",");
    n = len(inList)
    if(n<6) or (n>6):
        listInputError(n)
    else:
        return inList
def arduinoCheck():
    try :
        open('/dev/ttyACM0', "r")
        print "arduinoCheck: Arduino found"
        return True
    except IOError:
        print "arduinoCheck: Arduino not found"
        return False
def concatExisting(sourceFile, destFile, delim):
	source = open(sourceFile, 'r')
	dest = open(destFile, 'a')
	breakCondition = False
	full_content = ""
	start = True
	while not breakCondition:
		if start == False:
			content = source.read(1)
			if content == '':
				breakCondition = False
		else:
			content = source.read(1)
			if content != '':
				start = False
				dest.write(delim)
		dest.write(content)
	dest.close()
def writeJSON_chart(dict, prevUsedFileName, nextUsedFileName):
	#if there is no prev, then add the necessary head; else just take it's on prev
	if ( not os.path.isfile(prevUsedFileName)): 
		writeJSON_head(prevUsedFileName)
	else:
		shutil.copyFile(prevUsedFileName, nextUsedFileName)
	concatJSON(dict, nextUsedFileName)
	concatExisting("chartSettingsTail.txt", nextUsedFileName, "")
	os.remove(prevUsedFileName)
	#copy from next to prev. setup for next round
	shutil.copyFile(nextUsedFileName, prevUsedFileName)
	#give prev used a delim to setup for next round
	concatDelim(", ", prevUsedFileName)
def concatDelim(delim, fileName):
	#add string (delim) to file
	file = open(fileName, 'a')
	file.write(delim)
def concatJSON(dict, fileName):
    #put the dictionary into JSON
    with open (fileName, 'a') as jFile:
            json.dump(dict, jFile, sort_keys = True, indent = 4, ensure_ascii=False)
    with open (fileName) as tempfile:
            json.load(tempfile)
#write head
def writeJSON_head(newFile):
	#unfortunately, it's not clear that overwriting will be sufficient
	#a remove must be done to allocate different physical memory
	#especially a read of all bytes will be done in the next function

	os.remove(newFile)
	file = open(newFile, 'w')
	file.write
#append tail to doc
def concatJSON_tail(fileName):
	
def arg_one():
	writable = str(sys.args[1])
	write_list = writable.split(".")
	fullName_writeList = []
	for j in range (0, len(nameList))
		for i in range (0, len(write_list))
			if write_list[i] in  nameList[j]:
				fullName = argRefDict[write_list[i]]
				fullName_writeList.append(fullName)
			else:
				print "args: improper input"
	writeGivenToList(fullName_writeList)
def arg_two():
	writable = str(sys.args[1])
	write_list = writable.split(".")
	fullName_writeList = []
	for i in range (0, len(write_list))
		for j in range (0, len(nameList))
			if write_list[i] in  nameList[j]:
				fullName = argRefDict[write_list[i]]
				fullName_writeList.append(fullName)
			else:
				print "args: improper input"
	writeGivenToJSON(fullName_writeList)
def arg_four():
	writable = str(sys.args[1])
	write_list = writable.split(".")
	jsonable = str(sys.args[4])
	json_list = jsonable.split(".")
	fullName_writeList = []
	fullName_JSONList = []
	fullName = ""
	fullJSON = ""
	for j in range (0, len(nameList))
		for i in range (0, len(write_list))
			if write_list[i] in  nameList[j]:
				fullName = argRefDict[write_list[i]]
				fullName_writeList.append(fullName)
			else:
				print "args: improper input"
				
		for i in range (0, len(json_list))
			if json_list[i] in nameList[j]:
				fullJSON = argRefDict[json_list[i]]
				fullName_JSONList.append(fullJSON)
			else:
				print "args: improper input"
	writeGivenToList(fullName_writeList)
	writeGivenToJSON(fullName_JSONList)
def main():
    args = len(sys.args)-1
    print "main: number of arguments received: "
    print args
    #ask arduino for report &  #receive arduino report
    print("main: REPORT REQUESTED")
    theList = receiveData()
    
    print "nameList length:"
    print len(nameList)-1
    
    dictWriteAll(theList)
    
	if args == 1:
		one_args()
	elif args == 2:
		two_args()
    elif args == 4:
		four_args()
    else:
		print "main: invalid argument count"
		exit(2)
    print ("main: REPORT COMPLETE")
main()
