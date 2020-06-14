///Header///
/////////////////////////////////////////////////////////
/*
Title:          Bandcamp Music File Rename
Descr:          This file is used to return the index of the first number found in the path. This indicates the 'Song Beginning" and that integer is returned.
Author:         Bryen Wittman
Data:           06/10/2020
Modified:       Bryen Wittman, 06/10/2020
Version:        1.0
Language:       C#, .NET CORE ver. 3.1
Comments:
                - The SongBeginningSearch is overloaded with a startIndex parameter. This overloaded function, is used in the instance of multiple discs. The discs will have a number, so the beginning of the search must occur after this disc number.
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
///CLASS BeginningSearch///
/////////////////////////////////////////////////////////
    public static class BeginningSearch
    {

///METHOD SongBeginningSearch (string song)///
/////////////////////////////////////////////////////////
        public static int SongBeginningSearch(string song)
        {
            //Define the search criteria, then return the index of the first occurance.
            char[] digits = {'0','1','2','3','4','5','6','7','8','9'};
            return song.IndexOfAny(digits);
        }
/////////////////////////////////////////////////////////
///End METHOD SongBeginningSearch (string song)///

///METHOD SongBeginningSearch (string song, int startIndex)///
/////////////////////////////////////////////////////////
        public static int SongBeginningSearch(string song, int startIndex)
        {
            //Define the search criteria, then return the index of the first occurance. Begin the search at the supplied index.
            char[] digits = {'0','1','2','3','4','5','6','7','8','9'};
            return song.IndexOfAny(digits, (startIndex + 1));
        }
/////////////////////////////////////////////////////////
///End METHOD SongBeginningSearch (string song, int startIndex)///
    }
/////////////////////////////////////////////////////////
///End CLASS BeginningSearch///
}
/////////////////////////////////////////////////////////
///End NAMESPACE BandcampMusicFileRename.StringManipulation///
