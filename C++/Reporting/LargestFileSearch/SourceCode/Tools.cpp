///Header///
/////////////////////////////////////////////////////////
/*
Title:		Largest File Search - Tools Source
Descr:		Program to search through all files in a directory/subdirectories and output the ten largest to a text file. This is the source file specifying functions inside of the Tools class.
Author:		Bryen Wittman
Data:		3/14/2018
Modified:	Bryen Wittman, 2/28/2019
Version:	1.2
Language:	C++. Currently adjusted for Windows.
Comments:
		- The Tools.cpp file defines public and private functions for use with main.cpp.
Updates:
		- Logging function added. (2/28, 1.2)	
*/
/////////////////////////////////////////////////////////
///End Header///


//Include Self
#include "Tools.h"

///Function Declarations///
/////////////////////////////////////////////////////////
///Constructor///
/*
- The constructor has two tasks.
	1) Map the string values to a the proper enum values. These will be used in a function defined later in this file (listWriteOut).
	2) Zero out and size the size and name vectors. Vector.size() is used to sort the file size and file name vectors. By sizing to ten, there is no need to worry about going over the ten member limit.
		This also allows the traversal of small directories. If a directory with 9 files or less is selected as root, there is no need to worry about overflowing the vector with garbage data.
*/
Tools::Tools()
{
	sizeMap["DEC"] = DEC;
	sizeMap["BIN"] = BIN;
	sizeMap["BYT"] = BYT;

	for (int i = 0; i < resultSize; i++)
	{
		sizeSort.insert(sizeSort.begin(), 0);
		nameSort.insert(nameSort.begin(), "End of Results");
	}

}
///End Constructor///

///Destructor///
/*
- Nothing is explicity destoryed in our class. Build in desturctors for every class/function/method that we use will take care of any used memory.
*/
Tools::~Tools()
{}
///End Destructor///

///List Check for Larger File///
/*
- This is a public accessor function for the sizeList. Truly the sizeList is not needed, and it's definition could be put here.
*/
void Tools::listCheck(uintmax_t byteSize, std::experimental::filesystem::v1::path fileName)
{
	sizeList(byteSize, fileName);
	return;
}
///End List Check for Larger File///

///Write Results to File///
/*
- This function will write the results of the top ten list to a file.
	option - This paramater specifies the size type that is to be written to the file.
	fileName - The location of the file to write
*/
void Tools::listWriteOut(std::string option, std::string fileName)
{
	//Open output file for writing
	std::ofstream writeOut;
	writeOut.open(fileName);
	
	///Switch to control size option. Convert list based on option. Default is to convert to decimal megabytes.
	switch (sizeMap[option])
	{
	case DEC:
		sizeListToDecMB();
		for (std::vector<uintmax_t>::size_type vecLoc = 0; vecLoc < sizeSort.size(); vecLoc++)
		{
			writeOut << nameSort[vecLoc] << std::endl;
			writeOut << sizeDecMB[vecLoc] << " MB" << std::endl << std::endl;
		}
		break;

	case BIN:
		sizeListToBinMB();
		for (std::vector<uintmax_t>::size_type vecLoc = 0; vecLoc < sizeSort.size(); vecLoc++)
		{
			writeOut << nameSort[vecLoc] << std::endl;
			writeOut << sizeBinMB[vecLoc] << " MB" << std::endl << std::endl;
		}
		break;

	case BYT:
		for (std::vector<uintmax_t>::size_type vecLoc = 0; vecLoc < sizeSort.size(); vecLoc++)
		{
			writeOut << nameSort[vecLoc] << std::endl;
			writeOut << sizeSort[vecLoc] << " B" << std::endl << std::endl;
		}
		break;

	default:
		sizeListToDecMB();
		for (std::vector<uintmax_t>::size_type vecLoc = 0; vecLoc < sizeSort.size(); vecLoc++)
		{
			writeOut << nameSort[vecLoc] << std::endl;
			writeOut << sizeDecMB[vecLoc] << " MB" << std::endl << std::endl;
		}
		break;
	}

	writeOut.close();
	return;
	
}
///End Write Results to File///

///Logging///
/*
- Function to take in error, take in path of file, get current time and date, format the string, output to a file
*/
void Tools::logging(std::experimental::filesystem::filesystem_error& err, std::string& fileName, const std::experimental::filesystem::v1::directory_entry& path)
{
	std::ofstream errOut;
	errOut.open(fileName, std::ios_base::app);
	time_t now = time(0);
	char timeOut[26];
	ctime_s(timeOut, sizeof timeOut, &now);
	for (int i = 0; i < 26; i++)
	{
		if (timeOut[i] == '\0')
			timeOut[i - 1] = '\0';
	}
	errOut << "[" << timeOut << "]---" << err.what() << " on file:" << std::endl;
	errOut << path.path() <<std::endl;
	errOut.close();
	return;
}
///End Logging///

///Convert Bytes to Binary MegaBytes///
/*
- Simple function that returns a value of binary megabytes
*/
double Tools::binaryByteToMB(uintmax_t byteSize)
{
	return byteSize * binConv;
}
///End Convert Bytes to Binary MegaBytes///

///End Convert Bytes to Decimal MegaBytes///
/*
- Simple function that returns a value of decimal megabytes
*/
double Tools::decimalByteToMB(uintmax_t byteSize)
{
	return byteSize * decConv;
}
///End Convert Bytes to Decimal MegaBytes///

///Convert Entire List to Decimal MegaBytes///
/*
- Simply function that loops through a vector and converts all values of bytes to decimal megabytes.
*/
void Tools::sizeListToDecMB()
{
	for (std::vector<uintmax_t>::iterator it = sizeSort.begin(); it < sizeSort.end(); it++)
	{
		sizeDecMB.push_back(decimalByteToMB(*it));
	}
	return;
}
///End Convert Entire List to Decimal MegaBytes///

///Convert Entire List to Binary MegaBytes///
/*
- Simply function that loops through a vector and converts all values of bytes to binary megabytes.
*/
void Tools::sizeListToBinMB()
{
	for (std::vector<uintmax_t>::iterator it = sizeSort.begin(); it < sizeSort.end(); it++)
	{
		sizeBinMB.push_back(binaryByteToMB(*it));
	}
	return;
}
///End Convert Entire List to Binary MegaBytes///

///Top Ten List According To Size///
/*
- This function will take in a file path and file size.
- It compares the incoming size to every size in the existing size array. If the incoming file is larger, it is inserted at that point in the vector, and the value at the end is popped off.
- If a new item is added to the list, the nameList function is called. This is to match the file path to the file size. Seeing as the vectors are created with different types, two vectors need to 
	be tracked for the top ten sizes.
- nameList is called within this function to ensure accuracy among sizes and file names.
*/
void Tools::sizeList(uintmax_t byteSize, std::experimental::filesystem::v1::path fileName)
{
	for (std::vector<uintmax_t>::size_type ssIT = 0; ssIT < sizeSort.size(); ssIT++)
	{
		if (byteSize > sizeSort[ssIT])
		{
			vectorTrack = (ssIT);
			sizeSort.insert(sizeSort.begin() + ssIT, byteSize);
			sizeSort.pop_back();
			nameList(fileName);
			break;
		}
	}
	return;
}
///End Top Ten List According to Size///

//Top Ten List of File Names///
/*
- This function simply keeps track of the file names that associate with sizeSort file sizes.
- It is only called within sizeList
*/
void Tools::nameList(std::experimental::filesystem::v1::path fileName)
{
	nameSort.insert(nameSort.begin() + vectorTrack, fileName);
	nameSort.pop_back();

	return;
}
///End Top Ten List of File Names///