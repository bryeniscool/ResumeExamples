///Header///
/////////////////////////////////////////////////////////
/*
Title:          Bandcamp Music File Rename
Descr:          This file is used to find the beginning of the song in a file path. This is based off of the final 'slash' delimiter in the file path.
Author:         Bryen Wittman
Data:           06/10/2020
Modified:       Bryen Wittman, 06/10/2020
Version:        1.0
Language:       C#, .NET CORE ver. 3.1
Comments:
*/
/////////////////////////////////////////////////////////
///End Header///

///Library Imports///
/////////////////////////////////////////////////////////
using System;
///End Library Imports///
/////////////////////////////////////////////////////////

///NAMESPACE BandcampMusicFileRename.StringManipulation///
/////////////////////////////////////////////////////////
namespace BandcampMusicFileRename.StringManipulation
{

///CLASS SongBeginning///
/////////////////////////////////////////////////////////
    public static class SongBeginning
    {

///METHOD SongBeginningSearch///
/////////////////////////////////////////////////////////
        public static int SongBeginningSearch(string song)
        {
            //String variale holder/
            string slash = null;

            //Change the expected file path slash based upon the underlying OS.
            //Windows = '\'
            //Linux = '/'
            if(System.Runtime.InteropServices.RuntimeInformation.OSDescription.Contains("Windows"))
                slash = @"\";
            else
                slash = @"/";

            //Return the index of the final 'slash' in the file path.
            return (song.LastIndexOf(slash) + 1);
        }
/////////////////////////////////////////////////////////
///End METHOD SongBeginningSearch///
    }
/////////////////////////////////////////////////////////
///End CLASS SongBeginning///
}
/////////////////////////////////////////////////////////
///End NAMESPACE BandcampMusicFileRename.StringManipulation///
