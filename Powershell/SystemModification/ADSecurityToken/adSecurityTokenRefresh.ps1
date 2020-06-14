<#
- This mini-file is a simple three commands that:
	1) Assigns the currently logged in username to a variable
	2) Kills the explorer.ext process
	3) Starts the explorer.exe process specifying the username variable as the start context.
	
- The purpose of this is to refresh the AD token for the logged in user. Many times a user is added to an AD group, but the replication is so slow that it will take hours until the user's machine
	actually recognizes that the user account is in the group. These commands will excelerate the process by forcing the user AD token to be refreshed.
- This script must be run under the logged in user context. Do not elevate the execution of this script.
- A .bat file has been supplied with this Powershell script. As long as both files are in the same directory, executing the .bat file will launch the script.
#>	


$name = $env:username #Finding the logged in username.

taskkill /f /IM explorer.exe #Kill the explorer process. This could be any process that is currently running.
RunAs /user:optiv\$name explorer.exe #Start the killed process, and force the user to type their password in.