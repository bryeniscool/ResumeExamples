###Header###
#########################################################
<#
Title:		User Profile Scrubber
Descr:		Script to delete all unwanted user profiles from a machine.
Author:		Bryen Wittman
Data:		2/20/2019
Modified:	Bryen Wittman, 2/20/2019
Version:	1.1
Language:	Powershell
Comments:
		- Rather than pull the currently logged in user value, a list of user names will be provided "$unwantedUsers". This is to insure there is nothing wonky with who is logged in
			at the time, and to also keep local account profiles "LocalAdmin, Temp, etc..."
		- This script must be executed with administrator privileges.
		- A .bat file has been supplied with this Powershell script. As long as both files are in the same directory, executing the .bat file will launch the script. 
Updates:
		- Include logging to an external file.
		
#>
########################################################
###End Header###

###Global Variables###
#A list is used to gather the user names to check. A list was chosen so the .Remove method could be used. If an array is not of a fixed state, .Remove is not available.
#The below users are simply to provide an example. Place any and all user profiles to remove in the list below.
$unwantedUsers = [System.Collections.Generic.List[System.Object]]("beth.sanchez", "summer.smith", "squanchy", "pencilvester", "bacon.samurai")
$direcFolders = @() #Array placeholder for the list of folders in the direcotry "C:\Users"
$direcLoc = "C:\\Users\\" #The parent folder to find child directories from
########################################################
###End Global Variables###

###Function Declarations###
########################################################

###Logging##
function logging
{
	Param($message, $file)
	$dateTime = get-date -format "MM/dd/yyy HH:mm:ss"
	add-content $file "[$dateTime]---$message"
}
###End Logging###

###User/Folder Check###
<#
- This is the only code of the script. Parameters can be put in place to accept the list, array and directory, but because the script is so small, global variables were used.
- Please note, this will delete the profiles of any user placed in the $unwantedUsers list. Use with caution.
#>
function usrFldrCheck
{
	#Fill the $direcFolders array with the child objects of $direcLoc.
	Get-ChildItem $direcLoc | foreach-object {$direcFolders += "$direcLoc$_"}

	#Loop through all folders in the $direcFolders array, checking for a match with the $unwantedUsers list. If a match is found, use WMI to remove the user profile, then remove that
		#user from the $unwantedUsers array. This is to keep things optimal, no matter how small the list of child folders is. By removing already found users, we are not checking dead
		#values against the rest of the child folders.
	foreach($folder in $direcFolders)
	{
		foreach($user in $unwantedUsers)
		{
			if($folder -like "*$user*")
			{
				#Get the user profile if there's a match.
				$aProfile = Get-WmiObject -query "select * from win32_UserProfile where LocalPath='$folder'"
				#Remove the user profile.
				$aProfile | Remove-WmiObject
				#Logging if statement. Probably should have used variables for the message and log location, but again, the script is so small there's no need.
				if($?)
				{
					logging "The user profile $user was removed from the machine." "C:\Windows\Temp\sdScrubber.log"
				}
				else
				{
					logging "The user profile $user was found, but this script was unable to remove it." "C:\Windows\Temp\sdScrubber.log"
				}
				#Remove the user from the list.
				$unwantedUsers.Remove($user) 2>&1 | out-null
				break
			}
		}
	}
}
###End User/Folder Check###

#################################################################
###End Function Declarations###

###PROGRAM EXECUTION###
usrFldrCheck