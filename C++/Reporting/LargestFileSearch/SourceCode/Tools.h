///Header///
/////////////////////////////////////////////////////////
/*
Title:		Largest File Search - Tools Header
Descr:		Program to search through all files in a directory/subdirectories and output the ten largest to a text file. This is the header file specifying classes to be built.
Author:		Bryen Wittman
Data:		3/14/2018
Modified:	Bryen Wittman, 2/28/2019
Version:	1.2
Language:	C++. Currently adjusted for Windows.
Comments:
		- The Tools.h file specifies public and private functions and variables for use with main.cpp.
Updates:
		- Logging function added. (2/28, 1.2)	
*/
/////////////////////////////////////////////////////////
///End Header///

///Library Imports///
/////////////////////////////////////////////////////////
#ifndef Tools_H				//If this file hasn't been defined.
#define Tools_H				//Define it and link it to the program.

#include <vector>			//Used to store the file path0 and file size lists, and sort them accordingly.
#include <string>			//Used for user input, and hard coding file paths.
#include <fstream>			//Used to open files for write.
#include <iostream>			//Used to accept user input in main (getline(cin, var)).
#include <filesystem>		//Used for system directory traversal and retriving file stats.
#include <map>				//Used to enable switch control by mapping strings to enum values.
#include <ctime>			//Used to gather current date and time.
/////////////////////////////////////////////////////////
///End Library Imports///

///Class Definitions///
/////////////////////////////////////////////////////////
///Class Tools///
/*
- This class will hold everything necessary for the traversal, sizing info, and writing of the program.
- Please note the mapping of the enum values to string values. The mapping occurs in the constructor.
*/

class Tools
{
public:
	Tools();																					//Constructor
	~Tools();																					//Destructor
	void listCheck(uintmax_t byteSize, std::experimental::filesystem::v1::path fileName);		//Public accessor function to check a new file against the current top ten files
	void listWriteOut(std::string option, std::string fileName);								//Exports the top ten list in what ever format is specified with option
	void logging(std::experimental::filesystem::filesystem_error& err, std::string& fileName, const std::experimental::filesystem::v1::directory_entry& path);	//Currently used to record errors.
	
private:
	//Private Functions
	double binaryByteToMB(uintmax_t byteSize);													//Convert bytes to binary megabytes
	double decimalByteToMB(uintmax_t byteSize);													//Convert bytes to decimal megabytes
	void sizeListToDecMB();																		//Converts byte list to decimal megabytes
	void sizeListToBinMB();																		//Converts byte list to binary megabytes
	void sizeList(uintmax_t byteSize, std::experimental::filesystem::v1::path fileName);		//Sorts all received file sizes into the top ten. Note, if outside the top ten, file is not kept in list
	void nameList(std::experimental::filesystem::v1::path fileName);							//List of file paths to coincide with size list

	//Private Variables
	double const binConv = .00000095367432;														//Constant variable for binary megabyte conversion
	double const decConv = .000001;																//Constant variable for decimal megabyte conversion
	std::vector<uintmax_t> sizeSort;															//Top ten list of files in bytes
	std::vector<uintmax_t> sizeDecMB;															//Top ten list of files in decimal megabytes
	std::vector<uintmax_t> sizeBinMB;															//Top ten list of files in binary megabytes
	std::vector<std::experimental::filesystem::v1::path> nameSort;								//Top ten list of file paths to coincide with size lists
	__int64 vectorTrack;																		//Used to keep track of vector postions. Normal vector iterators will not work as they will be destoryed on modification of vector
	enum SizeOutput { DEC, BIN, BYT };															//Enum values used for switch statment in listWriteOut
	std::map<std::string, SizeOutput> sizeMap;													//Mapping variable to tie enum values to string values
	int resultSize = 10;																		//Size of the list. Change to include more results.
};
#endif //Tools																					//End of class
///End Class Tools///
/////////////////////////////////////////////////////////
///End Class Definitions///