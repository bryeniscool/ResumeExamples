Hello!
This program will receive a root directory from user input, then recursively search the directory to find the 10 largest files contained within it. Here are the order of operations:
  1) User is prompted to enter a file path to save the output of the search. This output includes every file (and path) included in the search.
  2) User is prompted to enter the root file path of the directory they want searched.
  3) The search executes. If the file path is incorrect, the search returns immediately.
  4) Once the search finishes, the terminal window closes, the data file is created at the location the user specified in step one. Four files are created (possibly) at C:\Windows\Temp.
	- BinarySize.txt
	- DecimalSize.txt
	- ByteSize.txt
	- ErrorFileSize.log
     The first three files contain the ten largest file lists. The first two have converted the sizes to MB. The third is the original byte sizes. The final file conveys any errors the search encountered while executing.

* Please execute the program with proper permissions. If searching a directory that requires elevated permissions, use elevated permissions to launch the program.

* If Microsoft naming conventions are not filed, the program will crash. I've seen this with filenames that are too long.

* For more information on the functions created, please look at the source code for extensive notes. 
