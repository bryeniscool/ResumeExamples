###Header###
########################################################
<#
Title:		Software Uninstaller (original)
Descr:		Powershell tool to uninstall any instance of installed softwareif it DisplayNames match.
Author:		Bryen Wittman
Date:		06-25-2018
Modified:	Bryen Wittman, 12-03-2018
Version:	1.3
Language:	Powershell
Comments:
		- Must be run with administrator privileges
		- The registry key being searched is is "HKLM:\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall".
		- The tool will run twice; checking in the x64 path, and then again in the x86 path.
		- The userUninstallArgs value must be adjusted according to the software manufacturers uninstall params. QuietUninstallString, and the msi uninstalls should not need any adjusting.
		- Testing is important. There are display commands to show what the command is pulling located right under the delete commands (highlited with comments). To view the output before deleting, switch the
			comment field.
		- A .bat file has been supplied with this Powershell script. As long as both files are in the same directory, executing the .bat file will launch the script.
#>
########################################################
###End Header###

###Global Variables###
########################################################
$userString = "*autoit*" #Variable to compare against the DisplayName value in the registry. Wildcards are accepted. Example in place. 
$userUninstallArgs = "/silent" #Variable to pass if a exe is being used. Typically this is used if the UninstallString value in the registry is pointing to a .exe, and there is no QuietUninstallString value.
########################################################
###End Global Variables###

###Function Declarations###
########################################################

###Registry Find###
<#
This function will search both "HKLM:\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall" and "HKLM:\SOFTWARE\Wow6432Node\Microsoft\Windows\CurrentVersion\Uninstall" for any keys whose DisplayValue matches
the variable "userString". If a match is found, this function will try to find a "QuietUninstallString" value. If present, the uninstall executes. If there is not a "QuietUninstallString",
the functions will determine if the "UninstallString" value contains the keyword "msi". If it does, an uninstall will proceed with the standard "/qn /norestart" flags for the uninstall. If it doesn't,
the "UninstallString" is executed with the "userUninstallArgs" value appended to the end of the the "UninstallString" value.
- Parameters
	- $secondSearchString: This is the supplied DisplayName value ($userString). It's named as such to differentiate between the function calls.
	- $secondUninstallArgs: This is the supplied uninstall flags ($userUninstallArgs). It's named as such to differentiate between the function calls.
#>
Function global:registryFind
{
	param([string]$secondSearchString = "Nothing", [string]$secondUninstallArgs)
	#Comparing "DisplayName" values to the user supplied string.
	if ((get-itemproperty -path $_.PsPath).DisplayName -like $secondSearchString) 
	{
		$value = (get-itemproperty -path $_.PsPath.Substring(26) -Name "QuietUninstallString")."QuietUninstallString" 2>$null
		if (-not $value)
		#If QuietUninstallString does not exist
		{
			$value = (get-itemproperty -path $_.PsPath.Substring(26) -Name "UninstallString")."UninstallString"
			
			#Check for "msiexec" in the "UninstallString" value
			if($value.ToLower().startswith("msiexec"))
			{
				$file = $value.split(" ")[0]+" "
				$args = $value.split(" ")[1]+" /qn"
				
				#At times, a "/i" will be in the string value rather than the "/X" that we want to uninstall. This replaces the "i" with an "X"
				if($args[1] -eq "i")
				{
					$args = $args.remove(1,1).insert(1,"X")
				}
				Start-Process -filePath $file -ArgumentList $args
				#*Test command to see the value pulled for UninstallString when it begins with msiexec:
				#$file+$args
				timeout 15
			}
			
			else
			#msi value and QuietUninstallString, not found. Executing normal uninstall from a supplied .exe location. Please modify $userUninstallArgs to make the uninstall silent.
			{
				if($value[0] -ne '"')
				{
					$value = $value.insert(0,'"')
					$value = $value.insert($value.length, '"')
				}
				$value = $value+" "+$secondUninstallArgs
				cmd.exe /c $value | Out-Null
				#*Test command to see the value pulled for UninstallString when msiexec is not present: 
				#$value
				timeout 15
			}
			
			
		}
		
		else
		#Found QuietUninstallString, and executes the command
		{
			cmd.exe /c $value | Out-Null
			#*Test command to see the value pulled for QuietUninstallString:
			#$value
			timeout 15
		}
	}

  
}
###End Registry Find###

###Registry Calls###
<#
This function is simply used to check the supplied values against both the x64 and x86 registry.
- Parameters
	- $firstPassSearchString: This is the supplied DisplayName value ($userString). It's named as such to differentiate between the function calls.
	- $firstUninstallArgs: This is the supplied uninstall flags ($userUninstallArgs). It's named as such to differentiate between the function calls.
#>
Function global:registrySifting
{
	param([string]$firstPassSearchString = "Nothing", [string]$firstUninstallArgs)
	
		get-childitem -rec -path HKLM:\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall | % {registryFind -secondSearchString $firstPassSearchString -secondUninstallArgs $firstUninstallArgs}
		get-childitem -rec -path HKLM:\SOFTWARE\Wow6432Node\Microsoft\Windows\CurrentVersion\Uninstall | % {registryFind -secondSearchString $firstPassSearchString -secondUninstallArgs $firstUninstallArgs}
}
###End Registry Calls###
#################################################################
###End Function Declarations###

###PROGRAM EXECUTION###
registrySifting -firstPassSearchString $userString -firstUninstallArgs $userUninstallArgs
