###Header###
########################################################
<#
Title:		Password Rotater
Descr:		Powershell toll to change the password of a specified local account.
Author:		Bryen Wittman
Date:		09-20-2016
Modified:	Bryen Wittman, 09-20-2016
Version:	1.0
Language:	Powershell
Comments:
		- Must be run with administrator privileges
		- The current tool only has functionality to change the password for a local account. There are plans to imporve this to have the ability to change AD account passwords.
		- The storing of the password is not secure and should be updated. The original idea was to output the password to a file, transfer that password to a secure location, then delete the file. Even
			that methedology isn't great.
		- A .bat file has been supplied with this Powershell script. As long as both files are in the same directory, executing the .bat file will launch the script.
#>
########################################################
###End Header###

###Function Declarations###
########################################################

###Generate Password###
<#
- This function will randomize all of the characters in the $set array, and build the passowrd accordingly.
- The length of the password is user determined by passing the paramter $length.
#>
Function pass()
{
	param([int]$length)
	$set = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()-_{}][<>".ToCharArray() #Build the character from which the password characters will be selected from.
	$result = "" #Placeholder for the string variable that will contain the password.
	#Enter a loop and add a character to the password string ($result) until the desired password length is reached.
	for($x = 0;$x -lt $length; $x++){
		$result += $set | Get-Random
	}
	
	#Return the passowrd string
	return $result
}

###End Generate Password###
#################################################################
###End Function Declarations###

###PROGRAM EXECUTION###
$localAdmin = "administrator" #username to edit
$compName = $env:computername #Setting the computer name to a variable
$passLength = 16 #The desired password length.

#Function Call
$adminPass = pass -length $passLength

#This is the wonky part. The computer name and password is sent to a file called "powershell.txt". Please delete that file after you have secured and stored the password somewhere properly.
$compName | Add-Content 'C:\Windows\Temp\powershellDELETE.txt'
$adminPass | Add-Content 'C:\Windows\Temp\powershellDELETE.txt'
Add-Content 'C:\Windows\Temp\powershellDELETE.txt' "`n"

#Create the user object to edit.
$user = [adsi]"WinNT://localhost/$($localAdmin), user"

#Change the password.
$user.SetPassword($adminPass)
