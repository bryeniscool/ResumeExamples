###Header###
########################################################
'''
Title:		Dot to Space
Descr:		Simple function that takes in a file, and replaces all dots with spaces.
Author:		Bryen Wittman
Date:		5-14-2018
Modified:	Bryen Wittman, 5-14-2018
Version:	1.0
Language:	Python
Comments:
		- Must be run with python 3.x
		- Need to add error checking
'''
########################################################
###End Header###

###Function Declarations###
########################################################

###Dot to Space###
def dotToSpace():

	#Getting user input for file names of in and out
	inStr = input("Please enter the file name to read from: ")
	outStr = input("Please enter the file name to write to: ")

	#Opening files for reading and writing
	iFile = open(inStr, "r")
	oFile = open(outStr, "w")

	#Loop through in file and replace a '.' with a ' '
	for line in iFile:
		if "." in line:
			oFile.write(line.replace(".", " "))
		else:
			oFile.write(line)

	#Cleanup
	iFile.close()
	oFile.close()
	return
###End Dot to Space###

########################################################
###End Function Declarations###

###PROGRAM EXECUTION###
dotToSpace()