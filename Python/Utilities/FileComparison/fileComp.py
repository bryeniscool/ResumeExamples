###Header###
########################################################
'''
Title:		File Comparison
Descr:		Simple tool to compare all files in a directory. The fileStatCmp function is based off of the os.stat() signature.
Author:		Bryen Wittman
Date:		4-18-2018
Modified:	Bryen Wittman, 4-18-2018
Version:	1.0
Language:	Python
Comments:
		- Must be run with python 3.x
		- Need to add error checking
		FUTURE ADDITIONS
			- Add separate function to compare strictly on size rather than the entire os.stat() method.
			- Add ability to search an entire drive and report on file comparisons. Note: depending on drive size this could take hours.


'''
########################################################
###End Header###

###Library Imports###
########################################################
import os					#Used to retrieve all files in a directory
import filecmp				#Used to compare stats of different files
import platform				#Used to determine the running OS
########################################################
###End Library Imports###


###Function Declarations###
########################################################

###File Stat Comp###
'''
This function will compare all files in a given directory. There is a check for the OS version, which allows a check to complete the directory path.
The comparison is accomplished by putting all files in a directory into a list, then comparing them.
The function is not recursive. It only compares the top level of files.
Parameters
	- direcLoc: A string of an absolute or relative directory to analyze.
'''

def fileStatCmp(direcLoc):

	#Create the list holder
	dataSet = []

	#Check for the current OS. After determining, make sure the user supplied directory string ends with a slash.
	if platform.system() == "Windows":
		if not direcLoc.endswith("\\"):
			direcLoc = direcLoc + "\\"
	else:
		if not direcLoc.endswith("/"):
			direcLoc = direcLoc + "/"

	#Add only files in the supplied directory to the list holder.
	for item in os.listdir(direcLoc):
		if os.path.isfile(direcLoc + item):
			dataSet.append(direcLoc + item)	

	#Create iterators for comparisons.
	markerOne = 0;
	markerTwo = 1

	#Make the comparisons. To reduce resources at runtime, every item is compared to once, then moved up the list (ex: 1 to 2, 1 to 3, 1 to 4 -- 2 to 3, 2 to 4 -- 3 to 4; end).
	while markerOne < len(dataSet):
		while markerTwo < len(dataSet):
			if filecmp.cmp(dataSet[markerOne], dataSet[markerTwo], shallow=True):
				print(dataSet[markerOne] + " is equal to " + dataSet[markerTwo])
			markerTwo += 1
		markerOne += 1
		markerTwo = markerOne + 1
	return
###End File Stat Comp###


###PROGRAM EXECUTION###

direcLoc = input("Please enter the location of your directory:\n")
fileStatCmp(direcLoc)
	
