///Header///
/////////////////////////////////////////////////////////
/*
Title:		Host Checking Header
Descr:		Program to confirm a machine is up to company standards. This includes particular software installed, certain services running, a domain check, and files updated within a specified amount of time.
			This cpp file contains function definitions for the functions declared in HostFunctions.h.
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

///Library Imports///
/////////////////////////////////////////////////////////
#include "HostFunctions.h" //Include yourself which will include any standard libraries necessary.
/////////////////////////////////////////////////////////
///End Library Imports


///Function Definitions///
/////////////////////////////////////////////////////////

HostFunctions::HostFunctions() //Constructor. Nothing needs to be initialized.
{}

HostFunctions::~HostFunctions() //Destructor. Nothing needs to be explicitly destoryed upon termination.
{}

///Registry Value Get///
/*
- This function will take a registry key and return the value of the key value that was specified (ex: DisplayName, DisplayVerison).
- This is used to match a registry key with a desired value (ex: DisplayName = Notepad ++)
- This function is used in conjunction with the regKeyFind function to loop through the entire uninstall key value(s) in the registry.
- Possibly update this to be private... the main function shouldn't ever explicitly call this function.
*/

bool HostFunctions::regValueGet(const HKEY& hKey, const LPCWSTR& subKey, const LPCWSTR& regValue, const std::wstring& valueTarget)
{
	//Declarations
	DWORD dataSize{};//container for the size of the string value returned
	DWORD actualSize; //the size of the string returned by RegGetValue will differ after the string is written to a variable
	std::wstring data;//variable for the value returned from the registry key
	LONG retCode = RegGetValueW(hKey, subKey, regValue, RRF_RT_REG_SZ, nullptr, nullptr, &dataSize); //get the size of the string value
	if (retCode != ERROR_SUCCESS) //check for a successful call
	{
		//MessageBox(NULL, "Sizing of value failed", "Put return code here", NULL); //simple logging. Enhance at a later date
		return false;
	}

	data.resize(dataSize / sizeof(wchar_t)); //resizing the data variable

	retCode = RegGetValueW(hKey, subKey, regValue, RRF_RT_REG_SZ, nullptr, &data[0], &dataSize); //Used for error checking.

	actualSize = dataSize / sizeof(wchar_t);
	actualSize--;
	data.resize(actualSize);

	RegCloseKey(hKey);
	if (data == valueTarget) //If the key value equals our desired value.
		return true;
	else
		return false;
}
///End Registry Value Get///

///Registry Key Find///
/*
- This function is used to loop through a desired registry key.
- In conjunction with the function regValueGet, this function can loop thorugh all subkeys of a key and return the location of a key if a value is matched (Ex: DisplayName = Google Chrome")
*/
bool HostFunctions::regKeyFind(const HKEY& hKey, const LPCWSTR& subKey, const LPCWSTR& regValue, const std::wstring& valueTarget)
{
	const int MAX_LENGTH = 255; //Setting the size to contain buffer overflow.
	WCHAR bufferContainer[MAX_LENGTH];
	bool exists = false; //Key does not exist unless found.
	LPCWSTR finalSubkey;
	std::wstring catHolder;
	HKEY returnedHandle;
	LONG keyOpenCode = RegOpenKeyW(hKey, subKey, &returnedHandle); //Open the parent key
	if (keyOpenCode != ERROR_SUCCESS)
	{
		//MessageBox(NULL, "Error Opening Key", "Put return code here", NULL); //simple logging. Enhance at a later date
		return false;
	}

	DWORD bufferSize = sizeof(bufferContainer);
	for (int i = 0;; i++) //Loop through the parent key to get all subkeys.
	{
		LONG enumStatus = RegEnumKeyW(returnedHandle, i, (LPWSTR)bufferContainer, bufferSize); //Status of enumeration
		if (enumStatus == ERROR_SUCCESS) //If successfully found a key
		{
			catHolder = subKey + (std::wstring)bufferContainer;
			finalSubkey = catHolder.c_str();
			//MessageBoxW(NULL, finalSubkey, L"Put return code here", NULL);
			exists = regValueGet(hKey, finalSubkey, regValue, valueTarget); //Check to see if key value matches desired value.
			if (exists == true)
			{
				break; //If true, no more checking is needed. The key exists.
			}
			
		}
		
			if(enumStatus == ERROR_NO_MORE_ITEMS) //If end of all subkeys, exit the loop
				break;
		
	}
	//Cleanup
	RegCloseKey(hKey);
	RegCloseKey(returnedHandle);
	if (exists == true)
	{
		//finalSubkey will contain the hex key to the software if applicable
		return true;
	}
	else
		return false;

}
///End Registry Key Find///

///File Modification Time///
/*
- This function is used to check the modified or acccessed file time against a specified time.
- The times must be converted from file times to system times in order to do the checking.
- The times low and high parts are concatenated to get the accurate time, then the comparison takes place.
- The times are only compariable in 100 blocks of milliseconds. Please adjust accordingly.
- This is currently setup for last modified time.
*/
bool HostFunctions:: fileModTime(const LPCWSTR& fileName, const uint64_t& timeLimit)
{
	SYSTEMTIME st, cSt; //System time declarations.
	FILETIME creation, la, lw, cFt; //File time declarations. la = Last accesss, lw = Last written.
	bool retCode; //Check if the file exists
	HANDLE fileH; //Handle to the file to check.
	fileH = CreateFileW(fileName, GENERIC_READ, 2, NULL, 3, FILE_ATTRIBUTE_NORMAL, NULL); //Pull the file.
	retCode = GetFileTime(fileH, &creation, &la, &lw); //Get the mod and access time of the file. Use whichever one suites the proper needs.
	if (!retCode) //If the file doesn't exists, exit.
		return false;
	CloseHandle(fileH); //Close the file.
	GetSystemTime(&cSt); //Get the current system time.
	FileTimeToSystemTime(&lw, &st); //Convert times.
	SystemTimeToFileTime(&cSt, &cFt); //Convert times.

	ULARGE_INTEGER system, file, combined; //Variables to hold the final time.
	file.HighPart = lw.dwHighDateTime; //File low time.
	file.LowPart = lw.dwLowDateTime; //File high time.
	system.HighPart = cFt.dwHighDateTime; //System low time.
	system.LowPart = cFt.dwLowDateTime; //System high time.

	combined.QuadPart = system.QuadPart - file.QuadPart; //Get the difference of the system time and the file time resulting in the last time modified.
	 if (combined.QuadPart <= timeLimit) //If the file last time written is less than the time limit specified (specified as seven days in main.cpp)
		return true;
	else
		return false;
}
///End File Modification Time///

///Service Running Check///
/*
- This function will poll a service status, mainly checking to see if it's currently running.
- The fucntion uses the lowest possible rights (GENERIC_READ) to access the services.
- If the service to be checked has rights beyond what a standard user can see, you must run the application with administrator rights (some A/V services do this)
*/
bool HostFunctions::serviceRunning(const std::wstring& serviceName)
{
	SC_HANDLE servicesManagerH; //create services manager handle
	servicesManagerH = OpenSCManager(NULL, NULL, GENERIC_READ); //assign the handle to the services db
	if (servicesManagerH == NULL) //Check for a valid assignment. If none, return false
	{
		//MessageBox(NULL, "Service Manager Failed", "Put return code here", NULL); //simple logging. Enhance at a later date
		return false;
	}

	LPCWSTR lpServiceName = (LPCWSTR)serviceName.data(); //convert service wstring to lpwstring

	SC_HANDLE serviceH; //create service handle
	serviceH = OpenServiceW(servicesManagerH, lpServiceName, GENERIC_READ); //assignt the handle to the defines service
	if (serviceH == NULL)//Check for a valid assignment. If none, return false
	{
		//MessageBox(NULL, "Service Handle Failed", "Put return code here", NULL); //simple logging. Enhance at a later date
		CloseServiceHandle(servicesManagerH); //close the manager handle
		return false;
	}

	SERVICE_STATUS servStatus; //create the variable for the service status
	LPSERVICE_STATUS servStatusRef = &servStatus; //create a reference to the service status as an LP, so casting does not need to be performed

	if (QueryServiceStatus(serviceH, servStatusRef) == NULL) //check to see if the service is running
	{
		CloseServiceHandle(servicesManagerH); //close the manager handle
		CloseServiceHandle(serviceH); //close the service handle
		return false;
	}

	if (servStatus.dwCurrentState == SERVICE_RUNNING) //final return for function. If service is running return true, if service is anything else, retrun false
	{
		CloseServiceHandle(servicesManagerH); //close the manager handle
		CloseServiceHandle(serviceH); //close the service handle
		return true;
	}
	else
	{
		CloseServiceHandle(servicesManagerH); //close the manager handle
		CloseServiceHandle(serviceH); //close the service handle
		return false;
	}
}
///End Service Running Check///
/////////////////////////////////////////////////////////
///End Function Definitions///