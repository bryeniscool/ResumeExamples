///Header///
/////////////////////////////////////////////////////////
/*
Title:		Largest File Search - Main
Descr:		Program to search through all files in a directory/subdirectories and output the ten largest to a text file.
Author:		Bryen Wittman
Data:		3/14/2018
Modified:	Bryen Wittman, 2/28/2019
Version:	1.2
Language:	C++. Currently adjusted for Windows.
Comments:
		- The main.cpp file is used to show functionality. Please adjust this file to your liking. To see a description of the internal methods being used, please see Tools.cpp and Tools.h.
		- A root file path is supplied, then the program will search it, and all subdirectories for the largest files present.
		- Please be aware: The program will crash if it encounters a file that is outside of the Microsoft file naming conventions. This applies particularly to name length.
		- 
		- See Tools.H for all library imports.
Updates:
		- Included logging to an external file. (2/28, 1.2)
		
Future:
		- Provide gui for end user. The structure will be a dialog box offering a "browse" feature to select the root directory. After that is selected, a button to "run scan" will be pressed
			and the program will execute. The top ten largest files, and their decimal size in megabytes, will be displayed as a list in the gui. A button to export results to a text file will 
			be included. Also, an option to load file explorer to the exact location of any of the listed files.
		- Create logic to convert large megabyte files to gigabytes.
		
*/
/////////////////////////////////////////////////////////
///End Header///

///Library Imports///
/////////////////////////////////////////////////////////
#include "Tools.h" //Conatins internally built functions for accessing, sorting, and displaying file information.
/////////////////////////////////////////////////////////
///End Library Imports///


///Global Declarations; Namespace Declarations; Main Function Preamble Declarations///
/////////////////////////////////////////////////////////
namespace fs = std::experimental::filesystem; //Although std::experimental::filesystem is used in the tools source code, the namespace is created here to keep the scope focused only in main.cpp.
/////////////////////////////////////////////////////////
///End Global Declarations; Namespace Declarations; Main Function Declarations///

///Function Declarations///
/////////////////////////////////////////////////////////
///Main///
/*
- This is the only function in main.cpp. It is used to demonstrate the functionality of the program. Please adjust accordingly.
*/

int main()
{
	//Variable Declarations
	Tools t; 															//Object creation to access the functions built in Tools.cpp.
	std::string dataFile; 												//String var for the file path to save the results in.
	std::string filePath; 												//String var for the root of the file path to search. Note, all sub directories will be included in the search.
	std::string errorFile = "C:\\Windows\\Temp\\FileSizeErrors.log"; 		//String var for the location of the error log. This can be changed to be dynamic.
	std::ofstream outFile; 												// Ofstream var to open the data file.
	//End Variable Declarations
	
	//Gathering Dynamic Data
	std::cout << "Please enter the file and path to save info to (ex: C:\\Users\\Me\\Documents\\BigFiles.txt):\n";		//Message to user to input the file path to save the date. 
	std::getline(std::cin, dataFile);																				//Retrieving input and assinging it to variable.
	std::cout << "Please enter the root directory to search (ex: C:\\):\n";											//Message to user to input the file path to the root of the search.
	std::getline(std::cin, filePath);																				//Retrieving input and assinging it to variable.
	outFile.open(dataFile);																							//Opening dataFile steam for writing.
	//End Gathering Dynamic Data
	
	//Program Loop
	for (auto& it : fs::recursive_directory_iterator(filePath)) 		//Create the iterator to loop through all directories and files.
	{
		if(it.status().type() == fs::file_type::directory)				//Check to see if iterator is currently on a directory.
			outFile << it.path() << std::endl;							//If true, write the path to the supplied data file.
		if (it.status().type() == fs::file_type::regular)				//Check to see if iterator is currently on a file.
		{
			outFile << it.path() << std::endl;							//If true, write the path to the supplied data file.
			try {
				t.listCheck(fs::file_size(it), it.path());				//Try to gather the size of the file, and add it to the top ten list if applicable.
			}
			catch (fs::filesystem_error& err) {
				t.logging(err, errorFile, it);							//Catch any errors, and write them to the log file supplied earlier.
			}
		}
	}
	
	outFile.close(); 										//Close the dataFile stream.
	t.listWriteOut("BIN", "C:\\Windows\\Temp\\BinarySize.txt");			//Write a summary of the top ten list as Binary MegaByte size.
	t.listWriteOut("DEC", "C:\\Windows\\Temp\\DecimalSize.txt");			//Write a summary of the top ten list as Decimal MegaByte size.
	t.listWriteOut("BYT", "C:\\Windows\\Temp\\ByteSize.txt");				//Write a summary of the top ten list as Byte size.
	return 0;												//Main return.
}			
///End Main///
/////////////////////////////////////////////////////////
///End Function Declarations///
