///Header///
/////////////////////////////////////////////////////////
/*
Title:		Host Checking Header
Descr:		Program to confirm a machine is up to company standards. This includes particular software installed, certain services running, a domain check, and files updated within a specified amount of time.
			This header file contains function and variable declarations for the HostFunctions.cpp file.
Author:		Bryen Wittman
Data:		1/27/2019
Modified:	Bryen Wittman, 3/05/2019
Version:	1.1
Language:	C++, Built for Windows operating system
Comments:
		- The program will load an icon in the system tray. The icon will be either a green checkmark or a red x depending on the machine compliance. Left-clicking on the icon will load a dialog box
			that informs the user of what passed and what failed. Right-clicking on the icon will display two options:
				1) An option to exit the host checker, killing the process.
				2) An option to run a scan of the machine for compliance.
			The program will auto run a check every five minutes. Note a window must be built in order to display the icon in the system tray, but the window is always hidden from the user.
		- The main.cpp file is used to build the window, handle input, and format output.
		- The HostFunctions.cpp file contains all of the functions that create the compliance check.
		- The checkmark and x icons are located in a local folder titled "icons", and added to the resource.h file in order to import them to the window.
		- Depending on the service that is being examined, admin rights may be necessary when launching the program. Some A/V services will not release information to a standard user.
*/
/////////////////////////////////////////////////////////
///End Header///

///Library Imports and Definitions///
#ifndef HOSTFUNCTIONS_H
#define HOSTFUNCTION_H

#include <string>
#include <Windows.h>
/////////////////////////////////////////////////////////
///End Library Imports and Definitions


///Class Definition///
/////////////////////////////////////////////////////////
/*
- This class will contain all of the checks that are used to check for machine compliance. This inlcudes registry dives to find installed software, file time checks and service checks.
*/
class HostFunctions
{
public:
	HostFunctions(); //Constructor
	~HostFunctions(); //Destructor
	bool regKeyFind(const HKEY& hKey, const LPCWSTR& subKey, const LPCWSTR& regValue, const std::wstring& valueTarget); //Used to find the key containing the DisplayName of whatever is passed to it.
	bool fileModTime(const LPCWSTR& fileName, const uint64_t& timeLimit); //Used to convert times and commpare specified time against a file time.
	bool serviceRunning(const std::wstring& serviceName); //Used to check for a service running.

private:
	bool regValueGet(const HKEY& hKey, const LPCWSTR& subKey, const LPCWSTR& regValue, const std::wstring& valueTarget); //Used to retrieve the data in a REG_SZ value of a key.
	std::string disVersion;
};
/////////////////////////////////////////////////////////
///End Class Definition///
#endif //HostFunctions