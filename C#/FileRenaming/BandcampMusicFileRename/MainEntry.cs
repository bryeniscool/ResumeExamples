///Header///
/////////////////////////////////////////////////////////
/*
Title:          Bandcamp Music File Rename
Descr:          This program was designed to rename any music albums purchased from the online store 'Bandcamp' to a personal file naming standard.
                This is the main entry point to the program execution.
Author:         Bryen Wittman
Data:           06/10/2020
Modified:       Bryen Wittman, 06/10/2020
Version:        1.0
Language:       C#, .NET CORE ver. 3.1
Comments:
                The external library 'TagLib' must be added via NuGet in order for this program to compile.
                The target naming structure is as follows:
                    <Directory>(Artist Name)/<Directory>(Album Year) (Album Title)/<Files>(Song Number) (Song Title) [(Artist Name)].(Extension)
                The Artist album/year/name are all derived from one of the songs provided in the directories using the external library 'TagLib'.
*/
/////////////////////////////////////////////////////////
///End Header///

///Library Imports///
/////////////////////////////////////////////////////////
using BandcampMusicFileRename.Rename;
/////////////////////////////////////////////////////////
///End Library Imports//

///Namespace BandcampMusicFileRename///
/////////////////////////////////////////////////////////
namespace BandcampMusicFileRename
{

///Class MainEntry///
/////////////////////////////////////////////////////////
    class MainEntry
    {

///Main Entry Point///
        static void Main(string[] args)
        {
            //Change the text in quotes to the location to be modified. Note, the trailing directory deliniation (the "/" or "\" if on a Windows OS) must be included.
            string musicSource = @"/home/USER/BANDCAMPROOT/";

            //Create an object to call the action.
            FileRename actionObj = new FileRename();

            //Execute the changes.
            actionObj.FileRenameAction(musicSource);
        }
/////////////////////////////////////////////////////////
///End Main Entry Point///
    }
/////////////////////////////////////////////////////////
///End Class MainEntry///
}
/////////////////////////////////////////////////////////
///End Namespace BandcampMusicFileRename///
