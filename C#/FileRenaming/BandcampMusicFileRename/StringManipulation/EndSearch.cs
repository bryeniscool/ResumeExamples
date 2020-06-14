///Header///
/////////////////////////////////////////////////////////
/*
Title:          Bandcamp Music File Rename
Descr:          This file is used to return the index of the final character before the extension of the file. 
Author:         Bryen Wittman
Data:           06/10/2020
Modified:       Bryen Wittman, 06/10/2020
Version:        1.0
Language:       C#, .NET CORE ver. 3.1
Comments:
*/
///End Header///
/////////////////////////////////////////////////////////

///Library Imports///
/////////////////////////////////////////////////////////
using System;
///End Library Imports///
/////////////////////////////////////////////////////////

///NAMESPACE BandcampMusicFileRename.StringManipulation///
/////////////////////////////////////////////////////////
namespace BandcampMusicFileRename.StringManipulation
{

///CLASS EndSearch///
/////////////////////////////////////////////////////////
    public static class EndSearch
    {

///METHOD SongEndSearch///
/////////////////////////////////////////////////////////
        public static int SongEndSearch(string song)
        {
            //Define search criteria.
            string endSearchStringFlac = ".flac";
            string endSearchStringMp3 = ".mp3";

            //If .flac exists in the string, return the index of the character directly before .flac begins. Else, search for .mp3 and return the index of the character directly before .mp3.
            if(song.Contains(endSearchStringFlac))
                return song.LastIndexOf(endSearchStringFlac);
            else if(song.Contains(endSearchStringMp3))
                return song.LastIndexOf(endSearchStringMp3);

            return 0;
        }
/////////////////////////////////////////////////////////
///End METHOD SongEndSearch///
    }
/////////////////////////////////////////////////////////
///End CLASS EndSearch///
}
/////////////////////////////////////////////////////////
///End NAMESPACE BandcampMusicFileRename.StringManipulation///
