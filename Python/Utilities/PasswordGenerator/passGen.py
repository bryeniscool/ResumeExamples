###Header###
########################################################
'''
Title:		Console Password Generator
Descr:		Simple Tool to generate a random password based off of options set by the user
Author:		Bryen Wittman
Date:		4-20-2018
Modified:	Bryen Wittman, 4-20-2018
Version:	1.0
Language:	Python
Comments:
		- Must be run with python 3.x
		- Need to incorporate a GUI library for this
		- Need to add error checking
'''
########################################################
###End Header###

###Library Imports###
########################################################
import random 			#Used to generate randomness for the password
import time				#Used to seed the random generator
import string			#Used for the character sets (digits, punctuation, lower and upper case)
import sys				#Used for exiting the program after an error is encountered
########################################################
###End Library Imports###

###Preamble###
########################################################
random.seed(time.time())	#Seeding the random generator
########################################################
###End Preamble###

###Function Declarations###
########################################################

###Character Set Shuffle###
'''
This function is used to shuffle all of the character sets, and also determine what character sets to include in the end password.
#Parameters#
critSelection - This is a user supplied parameter that is a string of characters. The options are "l, u, d, p". This function will check for any of those characters in the critSelection paramter, and add the associated character set (see below) to the final array that is passed out.

l = Lowercase Alpha
u = Uppercase Alpha
d = Numeric Digit
P = Punctuation Mark

#Return#
The final item returned is an array of arrays that contains all of the characters possible for selection
'''

def charSetShuffle(critSelection):
	lowerHold = []
	upperHold = []
	digitHold = []
	puncHold = []
	charGroup = []
	i = 0
	x = 0

	for item in string.ascii_lowercase:
		lowerHold.append(item)

	for item in string.ascii_uppercase:
		upperHold.append(item)

	for item in string.digits:
		digitHold.append(item)

	for item in string.punctuation:
		puncHold.append(item)

	#Space is not included in string.punctuation. Added here.
	puncHold.append(" ")

	while i < 10000:
		random.shuffle(lowerHold)
		random.shuffle(upperHold)
		random.shuffle(digitHold)
		random.shuffle(puncHold)
		i += 1

	
	if "l" in critSelection:
		charGroup.append(lowerHold)
	if "u" in critSelection:
		charGroup.append(upperHold)
	if "d" in critSelection:
		charGroup.append(digitHold)
	if "p" in critSelection:
		charGroup.append(puncHold)

	if len(charGroup) == 0:
		print("\nNo options specified for password creation.\nNow exiting...")
		sys.exit()

	return charGroup
###End Character Set and Shuffle###

###Character Set Selection###
'''
Helper function to determine what character set (of the sets that were made available from the charSetShuffle(critSelection) function) to pull from for the final password.
#Parameters#
limiter - This is passed so the selection is never out of bounds. As long as the choice remains out of bounds, a new set will be selected (see "while choice >= limiter")

#Return#
The final item returned is the character set array to choose the individual character from.
'''

def charSet(limiter):
	####Determine what character set to use####
	choice = 5

	while choice >= limiter: 
		point = random.randint(0, 999999999999999999999999)
		if point % 4 == 0:
			choice = 3
		elif point % 3 == 0:
			choice = 2
		elif point % 2 == 0:
			choice = 1
		else:
			choice = 0
	return choice
###End Character Set Selection###

###Character Selection###
'''
This function selects the actual character to return to the final password.
#Parameters#
setArray - The character array that was selected from the charSet(limiter) function.

#Return#
The final item returned is the character for the final password
'''
def charSpec(setArray):
	####Picking the actual character from the set of choice####
	choice = charSet(len(setArray))
	selection = random.choice(setArray[choice])
	return selection
	
###End Character Selection###

###Main###
'''
The main function for program execution. This contains two user defined variables, and the final password array.
'''
def main():
	
	options = input("Lowercase Alpha = l\nUppercase Alpha = u\nDigits = d\nPunctuation = p\nDetermine the characters you want included: ")
	length = input("Enter the length of the password: ")

	if int(length) <= 0:
		print("\nPassword length can not be 0 or a negative number.\nNow exiting...")
		sys.exit()

	charArray = charSetShuffle(options)
	passArray= []
	i = 0
	while i != int(length):
		passArray.append(charSpec(charArray))
		i += 1


	print("\n")
	for data in passArray:
		print(data, sep='', end='', flush=True)

	print("\n")
###End Main###
#################################################################
###End Function Declarations###




###PROGRAM EXECUTION###
if __name__ == "__main__":
	main()

