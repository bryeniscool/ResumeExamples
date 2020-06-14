###Header###
########################################################
'''
Title:		Replacement
Descr:		Simple collection of functions with replacing a '.' with a space, a space with a '.', and a dynamic function for different user inputs.
Author:		Bryen Wittman
Date:		2-16-2019
Modified:	Bryen Wittman, 2-16-2019
Version:	1.0
Language:	Python
Comments:
		- Must be run with python 3.x
		- Need to add error checking
		- When using the 'userRep' function, a file will be generate with the name '<user input>-LinesAdjusted.txt. This file will report what lines were changed.
		- Output files are being open with the 'w' parameter. This WILL DELETE any data if an existing file is used. Please use caution.
		- The functions 'dotToSpace' and 'spaceToDot' can be found in alternate files. They are included here to show how a library could be built out regarding string manipulation for an entire file.
'''
########################################################
###End Header###

###Function Declarations###
########################################################

###Dot to Space###
def dotToSpace(origFile, modFile):

	#Opening files for reading and writing
	iFile = open(origFile, "r")
	oFile = open(modFile, "w")

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

###Space to Dot###
def spaceToDot(origFile, modFile):
	

	#Opening files for reading and writing
	iFile = open(origFile, "r")
	oFile = open(modFile, "w")

	#Loop through in file and replace a ' ' with a '.'
	for line in iFile:
		if " " in line:
			oFile.write(line.replace(" ", "."))
		else:
			oFile.write(line)

	#Cleanup
	iFile.close()
	oFile.close()
	return
###End Space to Dot###

###Dynamic Call###
def userRep(origFile, modFile, origStr, repStr):
	
	#Iterator to track what line has been modified.
	i = 1
	
	#Array to keep track of modified lines for writing at the end.
	modified = []
	
	#Opening files for reading and writing
	iFile = open(origFile, "r")
	oFile = open(modFile, "w")

	#Loop through in file and replace the old user string with the new user string.
	for line in iFile:
		if origStr in line:
			oFile.write(line.replace(origStr, repStr))
			modified.append(str(i))
		else:
			oFile.write(line)
		i += 1
		
	#Cleanup
	iFile.close()
	oFile.close()
	
	#Openeing a new file to display what lines have changed
	dFile = open(modFile + "-LinesAdjusted.txt", "w")
	
	#Check to see if the list is empty. If not, loop through the tracking list to identify what files have been changed, and output to the file. If the list is empty, display that in output.
	if(len(modified) > 0):	
		dFile.write("The following lines have been modified:\n")
		for adj in modified:
			dFile.write(adj + "\n")
	else:
		dFile.write("No lines were modified:\n")

	#Cleanup
	dFile.close()
		
	return

########################################################
###End Function Declarations###

###PROGRAM EXECUTION###
#Demonstrating the dynamic function call, as the other two functions have examples in their own files.

#Getting user input for file names of in and out
inStr = input("Please enter the file name to read from: ")
outStr = input("Please enter the file name to write to: ")

userRep(inStr, outStr, "data", "info")