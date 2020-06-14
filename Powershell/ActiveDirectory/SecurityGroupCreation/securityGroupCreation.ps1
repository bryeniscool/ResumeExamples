###Header###
#########################################################
<#
Title:		Creating AD Secuirty Groups and Adding Members
Descr:		Script to confirm creation of AD sec group, and add members.
Author:		Bryen Wittman
Data:		2/14/2019
Modified:	Bryen Wittman, 2/19/2019
Version:	1.2
Language:	Powershell
Comments:
		- This script can be used to create bulk security groups and add an AD member to each.
		- This was used in a particular Local Admin method. Local admin users were tied to a security group in Active Directory, that is pulled to the machine when a scheduled task runs. This eliminates
			the need to set local admin at machine build, and also allows a continual refresh of the local admin group on the machine.
		- This script must be executed with an account that has rights to create and edit Active Directory security groups.
		- A .bat file has been supplied with this Powershell script. As long as both files are in the same directory, executing the .bat file will launch the script.
		- When the '<INSERT YOUR VALUE HERE - TYPE>' text is present, this refers to something specific within your domain. The type is referring to what should be supplied.
Updates:
		- Include logging to an external file.
		- Include logic to add multiple members to a specific group.
		
#>
########################################################
###End Header###

###Module Imports###
########################################################
Import-Module ActiveDirectory #Used to interact with Active Directory objects
########################################################
###End Module Imports###

###Function Declarations###
########################################################

###Create Group and Add User###
<#
- This function will check to see if the security group already exists in Active Directory. If it does, continue on to add the supplied user name. If it doesn't, create the group then continue.
- The 'Get-ADGroup' method is setup to search for the SamAccountName of a user.
- Try/catch sections were setup to suppress error output, and give a more concise definition when a group doesn't exist, or a SamUserName is not present in AD.
- Editing for your environment:
	Line 71 - <INSERT YOUR VALUE HERE - AD Distinguished Name Path> ex: "OU=Admins,OU=Computers,DC=fake,DC=net"
#>

function groupCreAdd
{
	#Importing parameters
	Param($secGroup, $userToAdd)
	
	#Attempting to create group object
	try
	{
		$groupObject = Get-ADGroup -Identity $secGroup
	}
	catch
	{
		echo "The group $secGroup is not present. Attempting to create..."
	}
	
	#Attempting to create the user object
	try
	{
		$userObject = Get-ADUser -Identity $userToAdd
	}
	catch
	{
	}

	#If security group does not already exist, create it. If it does exist, delete any existing members.
	if(!$groupObject)
	{
		New-ADGroup -name $secGroup -groupscope DomainLocal -path <INSERT YOUR VALUE HERE - AD Distinguished Name Path>
		try
		{
			$groupObject = Get-ADGroup -Identity $secGroup
		}
		catch
		{
			echo "`nThe group $secGroup was not created. Did you run this with the proper permissions? Now exiting..."
			exit
		}
			
	}
	else
	{
		Get-ADGroupMember $secGroup | ForEach-Object {Remove-ADGroupMember $secGroup -Member $_ -Confirm:$false}
	}

	#If the SamAccountName supplied is not found in AD, give the option to reenter the name, or continue without adding a user to the group.
	while(!$userObject)
	{
		$userToAdd
		echo "`nUser $userToAdd not found in Active Directory."
		$userToAdd = Read-Host "Enter the SamAccountName of the user (ex: pat.mcgroin) or type continue to skip adding a user to the group $secGroup"
		if($userToAdd.ToLower() -eq "continue")
		{
			$userObject = $null
			break
		}
		try
		{
			$userObject = Get-ADUser -Identity $userToAdd
		}
		catch
		{
		}
	}
	
	#If a confirmed AD user is supplied, add them to the supplied security group
	if($userObject -ne $null)
	{
		ADD-ADGroupMember -Identity $groupObject -Members $userObject
		#If the above command succeded, print a success message, else print a failure message.
		if($?)
		{
			echo "`nThe user $userToAdd was successfully added to the group $secGroup"
		}
		else
		{
			echo "`nFAILED to add the user $userToAdd to the group $secGroup"
		}
	}
}
###End Create Group and Add User###

###Use CSV File###
<#
- This function is used to supply a csv file, for massive additions.
- Please see the "Template.csv" file for the proper way to construct the csv file.
#>

function creationWithCSV
{
	#Request the location of a csv file, and attach it to a variable object.
	$csvFile = Read-Host "Insert the absolute path to the csv file"
	$csvObject = Import-CSV $csvFile
	
	#If the supplied file does not exist, continue asking until proper file is supplied.
	while(!$csvObject)
	{
		"The path {0} does not resolve to a csv file. Please insert the absolute path again" -f $csvFile
		$csvFile = Read-Host "Insert the absolute path to the csv file"
	}
	
	#Loop through every line of the csv file, calling 'groupCreAdd' to create the groups and add the members.
	foreach($cObject in $csvObject)
	{	
		groupCreAdd $cObject.SecGroup $cObject.ADUser
	}
}
###End Use CSV File###

#################################################################
###End Function Declarations###

###PROGRAM EXECUTION###
#################################################################

#Ask the user if they would like to use a csv file or dynamically supply the group and user. Depending on selections, call the proper functions.
$csvCheck = Read-Host "Would you like to use a csv file (enter 'y' for yes or 'n' for no)"
if($csvCheck.ToLower() -eq "y" -or $csvCheck.ToLower() -eq "yes")
{
	creationWithCSV
}
else
{
	$secGroup = Read-Host "Enter the name of the AD sec group"
	$userToAdd = Read-Host "Enter the SamAccountName of the user to add (ex: pat.mcgroin)"
	groupCreAdd $secGroup $userToAdd
}