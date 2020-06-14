<#
- This mini-file is a simple three commands that:
	1) Imports the Active Directory module.
	2) Prompts the user to specify an OU in AD to clear.
	3) Searches all groups within the supplied directory, and deletes said group if no members are inhabiting the group.
	
- The purpose of this is to cleanup stale security and distribution groups. Many times groups are made for a purpose, then a better solution arrises, or the group is no longer needed.
- This script must be run with an account that has access to modify the Active Directory OU supplied.
- The $adOU variable can be modified to be static. If there is an OU that always needs to be cleaned out, it makes sense to hard code it in.
- If multiple OU's are in need of cleaning, simply turn $adOU into an array, supply your OU values, then loop through the array with a foreach statement.
- A .bat file has been supplied with this Powershell script. As long as both files are in the same directory, executing the .bat file will launch the script.
#>	
#Import the module
import-module ActiveDirectory

#Prompt user for path to OU
$adOU = Read-Host("Please insert the the full OU path (ex: ou=Denver,ou=Colorado,ou=US,ou=NA,dc=fake,dc=net")

#Run cleanup
Get-ADGroup -Filter * -SearchBase "$adOU" -Properties Members | where {-not$_.members} | remove-adgroup -Confirm:$false