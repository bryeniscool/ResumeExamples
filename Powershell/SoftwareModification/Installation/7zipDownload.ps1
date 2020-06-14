###Header###
########################################################
<#
Title:		Software Uninstaller with Version Check
Descr:		Tool to download a piece of software and install it. The downaload url will need to be provided.
Author:		Bryen Wittman
Date:		05-27-2015
Modified:	Bryen Wittman, 05-27-2015
Version:	1.0
Language:	Powershell
Comments:
		- Must be run with administrator privileges (for the install section)
		- The script can be adjusted to run for any software. To show functionality, it is setup for 7-zip.
		- The script is currently setup to download and install version 1806 of 7-zip.
		- The script will exit with a 0 (for success) or a 1 (for error).
		- A log file is currently set to be written to "C:\Windows\Temp\7zipInstall.log"
		- A .bat file has been supplied with this Powershell script. As long as both files are in the same directory, executing the .bat file will launch the script.
#>
########################################################
###End Header###

###Function Declarations###
########################################################

###Logging###
<#
- This function is designed to help ease logging.
- The message and log file are user defined later in the script
#>
function logging
{
	Param($message, $file)
	$dateTime = get-date -format "MM/dd/yyy HH:mm:ss"
	add-content $file "[$dateTime]---$message"
}
###END Logging###

###Software Retrieval###
<#
- This function downloads the software to a user defined location on the machine.
- Please edit the source of the installtion file and the destination of the download later in the script.
#>
function softwareGrab
{
	#Importing parameters
	Param($source, $destination, $logFile)
	
	#Creating the object for download
	$z = New-Object System.Net.WebClient
	
	#Downloading the file
	$z.DownloadFile($source, $destination)
	#Check for success. Exit on failure.
	If($?)
	{
		$result="The setup file for 7-zip has been downloaded!!!"
	}
	else
	{
		$result = "The setup files were unable to be downloaded."
		logging $result $logFile
		exit 1
	}
	
	#Log result
	logging $result $logFile
	
	#Cleanup
	Remove-Variable z
	return
}
###End Software Retrieval###

###Software Installation###
<#
- This function installs the software that was downloaded.
- The destination field will be the same used in previous functions (Software Retrieval).
- The arguments variable should be defined by the user, and is structured to allow for silent installtions. Please define it later in the scirpt.
#>
function installSoftware
{
	#Importing parameters
	Param($destination, $arguments, $logFile)
	
	
	#Initiate Install
	Start-Process $destination -argumentList $arguments -Wait
	#Checking for success. Exit on failure.
	If($?)
	{
		$result="The installation for 7-zip succeeded!!!"
	}
	else
	{
		$result = "Whaa-whaa. The setup failed"
		logging $result $logFile
		exit 1
	}
	
	#Logging
	logging $result $logFile

	#Cleanup Setup Files
	Remove-Item $destination -recurse
}
###End Software Installation###

#################################################################
###End Function Declarations###

###PROGRAM EXECUTION###
#################################################################
#Set Variables
$source = "https://www.7-zip.org/a/7z1806-x64.msi"
$destination = "C:\Users\Public\Desktop\7z1806-x64.msi"
$arguments = "/qn /norestart"
$logFile = "C:\Windows\Temp\7zipInstall.log"

#Execute functions
softwareGrab $source $destination $logFile
installSoftware $destination $arguments $logFile
exit 0
