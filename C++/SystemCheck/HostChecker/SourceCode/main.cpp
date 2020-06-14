///Header///
/////////////////////////////////////////////////////////
/*
Title:		Host Checking
Descr:		Program to confirm a machine is up to company standards. This includes particular software installed, certain services running, a domain check, and files updated within a specified amount of time.
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
#include "HostFunctions.h" //Custom built class
#include "resource.h" //Icon resources
/////////////////////////////////////////////////////////
///End Library Imports///

///Preamble declarations for callback functions, the machine compliance check, and a function to change the icon. Please see the end of the code for the definitions///
/////////////////////////////////////////////////////////
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam); //Message handler
void CALLBACK timerFunc(HWND hwnd, UINT msg, UINT id, DWORD dwTime); //Used for the timer function call
void hostChecker(); //Performs the analysis to check for host compliance
void statusFlip(); //Changes icon if applicable
/////////////////////////////////////////////////////////
///End Preamble Declarations///

///Global Variable Declarations///
/////////////////////////////////////////////////////////
//Constant Global Variables. I hate doing this, but it's the easiest way to pass the flag for pass or fail

const HKEY ROOT_KEY = HKEY_LOCAL_MACHINE; //Root of the registry path
const LPCWSTR UNINSTALL_PATH64 = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\"; //The sub path for the system installations references the x64 path 
const LPCWSTR FILE_VERPATH = L"C:\\Windows\\setupact.log"; //Path to a file that is to be checked for modified time. Please make this a file on your system.
const LPCWSTR REG_DN = L"DisplayName"; //Specifying "DisplayName" as the value to search for
const LPCWSTR NOTE_DN = L"Notepad++ (64-bit x64)"; //The display name value to search for Notepadd++ in the registry
const LPCWSTR CHROME_DN = L"Google Chrome"; //The display name value to search for Google Chrome in the regsitry
const std::wstring WU_SRV = L"wuauserv"; //The service name for the Windows Update Service
const std::wstring WF_SRV = L"MpsSvc"; //The service name for the Windows Firewall Service
const std::wstring PRNT_SPOOL_SRV = L"Spooler"; //The service name for the Print Spooler
const uint64_t SEVEN_DAYS = 6048000000000; //Seven days in 100 blocks of milliseconds
//The next variables are used to check what domain the machine is joined to
//const LPCWSTR DOMAIN_VAL = L"Domain"; //The value to reference when checking if the machine is joined to a companies domain
//const std::wstring DOM_OPT = L"<YOUR DOMAIN HERE>"; //The domain name to look for
//const LPCWSTR DOMAIN_SUBKEY = L"SYSTEM\\CurrentControlSet\\services\\Tcpip\\Parameters"; //The sub path to check for the system's current domain joined to
///End Constant Global Variables///

//Global variables with volatile data
HostFunctions HF; //Object var to access our functions found in the header file
bool hostCheckerCurValue = false; //The flag to determine what icon and message to post
bool hostCheckerPrevValue; //The flag to determine what icon and message to post
NOTIFYICONDATA hcTrayIcon; //System tray icon initialization
HICON icon0; //The "failed" icon
HICON icon1; //The "passed" icon
HMENU appMenu; // The context menu handler for the icon
///End Volatile Global Variables///

///End Global Variable Declarations///
/////////////////////////////////////////////////////////

//MAIN ENTRY POINT.
//Below is the main function which will:
//	-Create the window and handle / Activate the window *Note: A window must be created in order to show the notification icon. The window will be hidden from view at all times
//	-Create the instance of the window
//	-Attach the notification icon to the window
//	-Create the timer for the regular interval check

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevHInstance, LPSTR lpCmdLind, int nCmdShow)
{
	//Variable declarations for WinMain
	/////////////////////////////////////////////////////////
	HWND hWnd; //Main window handle
	MSG message; //Message var to receive incoming messages from the window processor function
	const char* OPT_CLASS = "Host Checker"; //Window class name
	TCHAR hoverTip[128] = TEXT("Host Checker v 1.0\n--------------------------\nLeft-Click for details.\nRight-Click to run a scan.");
	/////////////////////////////////////////////////////////

	//Load the success and failure icons into memory
	/////////////////////////////////////////////////////////
	HICON logoIcon = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON3), IMAGE_ICON, 400, 400, LR_DEFAULTCOLOR);
	icon0 = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	icon1 = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON2), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	if (!icon0)
	{
		//MessageBox(NULL, "Failed to Load Icon0; 'Failure' icon", "Host Checker", NULL); //Used for testing and reporting.
		return 1;
	}

	if (!icon1)
	{
		//MessageBox(NULL, "Failed to Load Icon1; 'Success' icon", "Host Checker", NULL); //Used for testing and reporting.
		return 1;
	}
	/////////////////////////////////////////////////////////


	//Defining and registering of the window class
	/////////////////////////////////////////////////////////
	WNDCLASSEX winClass;

	winClass.cbSize = sizeof(WNDCLASSEX);
	winClass.lpfnWndProc = WndProc;
	winClass.hInstance = hInstance;
	winClass.style = CS_DBLCLKS;
	winClass.lpszClassName = OPT_CLASS;
	winClass.hIcon = logoIcon;
	winClass.hIconSm = 0;
	winClass.hCursor = 0;
	winClass.lpszMenuName = NULL;
	winClass.cbClsExtra = 0;
	winClass.cbWndExtra = 0;
	winClass.hbrBackground = (HBRUSH)(CreateSolidBrush(RGB(0, 0, 0)));
	/////////////////////////////////////////////////////////

	//Now register the class and check for an error
	if (!RegisterClassEx(&winClass))
	{
		//Below message box is used for error checking.
		//MessageBox(NULL, "Failed to register class", "Host Checker", NULL);
		return 1;
	}
	/////////////////////////////////////////////////////////

	//Creating the window
	/////////////////////////////////////////////////////////
	hWnd = CreateWindowEx(0, OPT_CLASS, "Blank Window", WS_OVERLAPPED, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, HWND_DESKTOP, NULL, hInstance, NULL);

	//Error checking
	if (!hWnd)
	{
		//Below message box is used for error checking.
		//MessageBox(NULL, "Failed to create window", "Host Checker", NULL);
		return 1;
	}
	/////////////////////////////////////////////////////////

	//Run the initial checks for the host checker and set the previous checker holder
	/////////////////////////////////////////////////////////
	hostChecker();
	hostCheckerPrevValue = hostCheckerCurValue;
	/////////////////////////////////////////////////////////

	//Set the parameters of the system tray icon
	/////////////////////////////////////////////////////////
	hcTrayIcon.hWnd = hWnd;
	hcTrayIcon.cbSize = sizeof(NOTIFYICONDATA);
	hcTrayIcon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	hcTrayIcon.uID = ID_TRAY_APP_ICON;
	hcTrayIcon.uCallbackMessage = WMU_SYSICON;
	strncpy_s(hcTrayIcon.szTip, hoverTip, sizeof(hoverTip));
	if(hostCheckerCurValue)
	{
		hcTrayIcon.hIcon = icon1;
	}
	else
		hcTrayIcon.hIcon = icon0;
	/////////////////////////////////////////////////////////

	//Show Window (hidden) and attach the icon to it
	/////////////////////////////////////////////////////////
	ShowWindow(hWnd, SW_HIDE);
	Shell_NotifyIcon(NIM_ADD, &hcTrayIcon);
	/////////////////////////////////////////////////////////

	//Set Timer. The check is set to run every 5 minutes
	/////////////////////////////////////////////////////////
	SetTimer(NULL, 0, 300000, (TIMERPROC)&timerFunc); //Set the timer for a host checker scan to every five minutes
	/////////////////////////////////////////////////////////
	
	//Run Loop
	/////////////////////////////////////////////////////////
	while (GetMessage(&message, NULL, 0, 0))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
	/////////////////////////////////////////////////////////

	//Cleanup
	Shell_NotifyIcon(NIM_DELETE, &hcTrayIcon);
	KillTimer(NULL, 0);
	
	//End of progrem execution
	return 0;
}
/////////////////////////////////////////////////////////
///END OF MAIN///

///Function Declarations///
/////////////////////////////////////////////////////////

///User Input Handler///
/*
- This function is designed to handle user input. There is a menu creation for a right click, actions mapped to the actions within that menu and display option for the left click on the icon.
*/

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE: //This is the definition for the right-click pop up menu. It gives two options, and has actions mapped to both of those options.
		appMenu = CreatePopupMenu();
		AppendMenu(appMenu, MF_STRING, ID_TRAY_RUN_CHECK, TEXT("Run a Host Checker Scan"));
		AppendMenu(appMenu, MF_SEPARATOR, NULL, NULL);
		AppendMenu(appMenu, MF_STRING, ID_TRAY_EXIT, TEXT("Exit the Host Checker"));
		break;
	case WMU_SYSICON:
		
		switch (wParam)
		{
		case ID_TRAY_APP_ICON: //Sets the app icon.
			SetForegroundWindow(hwnd);
			break;
		}
		if (lParam == WM_RBUTTONDOWN)//On right mouse click show the menu with two options; Run scan and exit. Track the mouse position and take action if those options are selected.
		{
			POINT curPoint;
			GetCursorPos(&curPoint);
			SetForegroundWindow(hwnd);
			UINT clicked = TrackPopupMenu(appMenu, TPM_RETURNCMD | TPM_NONOTIFY, curPoint.x, curPoint.y, 0, hwnd, NULL);
			SendMessage(hwnd, WM_NULL, 0, 0);
			switch (clicked)
			{
			case ID_TRAY_RUN_CHECK: //If run check is selected.
			{
				hostChecker();
				/*
				if (hostCheckerCurValue == hostCheckerPrevValue)
				{
					MessageBox(NULL, "Scan Complete.", "Host Checker", MB_TOPMOST | MB_SETFOREGROUND);
					break;
				}
				else
					statusFlip();
				*/
				if(hostCheckerCurValue != hostCheckerPrevValue)
					statusFlip();
				MessageBox(NULL, "Scan Complete.", "Host Checker", MB_TOPMOST | MB_SETFOREGROUND);
				break;
			}
			case ID_TRAY_EXIT: //If exit is selected.
			{	
				Shell_NotifyIcon(NIM_DELETE, &hcTrayIcon);
				KillTimer(NULL, 0);
				PostQuitMessage(0);
				break;
			}
			}
		}

		if (lParam == WM_LBUTTONUP) //For a left mouse click, display a message showing the status of the scans. This could be structured better, possibly in an upgrade. The output format adjusts dependent upon resolutions.
									//That will also need to be addressed in a future update.
		{
			std::string pass = "Pass\n";
			std::string fail = "FAIL\n";
			std::string message;
			//Building the final message
			//Notepad ++ Install
			message.append("Notepad ++ Installed.......................... ");
			if (HF.regKeyFind(ROOT_KEY, UNINSTALL_PATH64, REG_DN, NOTE_DN))
				message.append(pass);
			else
				message.append(fail);
			//Google Chrome Install
			message.append("Chrome Installed.......................... ");
			if (HF.regKeyFind(ROOT_KEY, UNINSTALL_PATH64, REG_DN, CHROME_DN))
				message.append(pass);
			else
				message.append(fail);
			//Windows File Check
			message.append("Windows File Date Check.................. ");
			if (HF.fileModTime(FILE_VERPATH, SEVEN_DAYS))
				message.append(pass);
			else
				message.append(fail);
			//Windows Update Service
			message.append("Win Update Service Running............ ");
			if (HF.serviceRunning(WU_SRV))
				message.append(pass);
			else
				message.append(fail);
			//Windows Firewall Service
			message.append("Win Firewall Service Running..... ");
			if (HF.serviceRunning(WF_SRV))
				message.append(pass);
			else
				message.append(fail);
			//Print Spooler Service
			message.append("Print Spooler Service Running......... ");
			if (HF.serviceRunning(PRNT_SPOOL_SRV))
				message.append(pass);
			else
				message.append(fail);
			//Domain Check. Commented out due to unknown domains.
			/*
			message.append("Joined to a Domain....... ");
			if (HF.regValueGet(ROOT_KEY, DOMAIN_SUBKEY, DOMAIN_VAL, DOM_OPT))
				message.append(pass);
			else
				message.append(fail);
			*/
			//Final Message Display
			MessageBox(NULL, message.c_str(), "Host Checker Status", MB_TOPMOST | MB_SETFOREGROUND);
		}
		break;
	case WM_CLOSE: //Close and destroy messages.
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
///End User Input Handler///

///Timer Handler///
/*
- This function is designed to force a host scan on a specified interval. Currently, the timer is setup to run every five minutes. Possibly update this to reset the timer if a manually scan is initiated.
*/
void CALLBACK timerFunc(HWND hwnd, UINT msg, UINT id, DWORD dwTime)
{
	hostChecker();
	if (hostCheckerCurValue == hostCheckerPrevValue)
		return;
	else
		statusFlip();
	return;
}
///End Timer Hanlder///

///Host Scan///
/*
- This function is designed to do the actual check for compliance. It calls the variouus functions found in HostFunctions.cpp, and determines either true or false for machine compliance.
*/

void hostChecker()
{
	if (HF.regKeyFind(ROOT_KEY, UNINSTALL_PATH64, REG_DN, NOTE_DN) &&	//Notepad ++ Installed
		HF.regKeyFind(ROOT_KEY, UNINSTALL_PATH64, REG_DN, CHROME_DN) &&	//Google Chrome Installed
		HF.fileModTime(FILE_VERPATH, SEVEN_DAYS) &&						//Windows file within seven days
		HF.serviceRunning(WU_SRV) &&									//Windows Update service running
		HF.serviceRunning(WF_SRV) &&									//Windows Firewall service running
		HF.serviceRunning(PRNT_SPOOL_SRV))							//Print Spooler service running
		///HF.regValueGet(ROOT_KEY, DOMAIN_SUBKEY, DOMAIN_VAL, DOM_OPT))	//Machine joined to a specific domain
		hostCheckerCurValue = true; //Possible change of a global variable. All systems passed.
	else
		hostCheckerCurValue = false; //Possible change of a global variable. One or more systems failed.

	return;
}
///End Host Scan///

///Icon Set///
/*
- This function is designed to flip the icon from pass to fail or fail to pass depending upon the latest scan results.
*/
void statusFlip()
{
	Shell_NotifyIcon(NIM_DELETE, &hcTrayIcon);
	if (hostCheckerCurValue) //Value set from hostChecker() function.
		hcTrayIcon.hIcon = icon1;
	else
		hcTrayIcon.hIcon = icon0;

	hostCheckerPrevValue = hostCheckerCurValue;
	Shell_NotifyIcon(NIM_ADD, &hcTrayIcon); //Add the desired icon to the system tray.
	return;
}
///End Icon Set///
/////////////////////////////////////////////////////////
///End Function Declarations///