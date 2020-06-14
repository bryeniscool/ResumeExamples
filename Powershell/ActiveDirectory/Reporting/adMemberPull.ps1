<#
- This mini-file is a simple four commands that:
	1) Imports the Active Directory module.
	2) Prompt the user to enter a distribution or security group name and assign it to variable.
	3) Prompt the user to enter a path to a file where the information will be stored and assign a variable to it.
	4) Retrieve all members of the supplied group and write the name and account name to a file.
	
- The purpose of this is to pull all members from an AD group. This is mainly used for reporting, but can be expanded to deleting or adding members.
- This script must be run with privileges to read from Active Directory.
- A .bat file has been supplied with this Powershell script. As long as both files are in the same directory, executing the .bat file will launch the script.
#>	

#Import the AD module
Import-Module ActiveDirectory

#Prompt user for group name
$group = read-host "Enter the distribution group you want to pull from"

#Prompt user for output location
$location = read-host "Enter the full location where you want to store the text file INCLUDING the name of the file (Ex. C:\Users\test.txt)"

#Gather all members of the supplied group, and output the name and account name to the supplied location.
Get-ADGroupMember "$group" | Select Name, SamAccountName | FORMAT-Table >> "$location"