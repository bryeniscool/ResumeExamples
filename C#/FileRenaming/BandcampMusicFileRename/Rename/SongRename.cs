///Header///
/////////////////////////////////////////////////////////
/*
Title:          Bandcamp Music File Rename
Descr:          This file is used to get the indexes of certain delimiters within the song file name, slice out pertinent info and create new strings based off of that data.
                Then, the song files are renamed using the System.IO library.
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
using System.IO;
using BandcampMusicFileRename.StringManipulation;
/////////////////////////////////////////////////////////
///End Library Imports///

///NAMESPACE BandcampMusicFileRename.Rename
/////////////////////////////////////////////////////////
namespace BandcampMusicFileRename.Rename
{

///CLASS SongRename///
/////////////////////////////////////////////////////////
    public class SongRename
    {
        //Class variables to hold indexes and sliced strings.
        int firstNumber = 0;
        int lastNumber = 0;
        int songBeginning = 0;
        int discIndex = 0;
        
        string ext = null;
        string path = null;
        string finalFileName = null;
        string songTitle = null;


///METHOD SongRenameAction///
/////////////////////////////////////////////////////////
        public void SongRenameAction(FileInfo song, string artistName, string newAlbumPath)
        {
            //Convert the FileInfo type to a string, then get indexes for the beginning of the song in the path and the first number in the file.
            songTitle = song.ToString();
            songBeginning = SongBeginning.SongBeginningSearch(songTitle);
            firstNumber = BeginningSearch.SongBeginningSearch(songTitle, discIndex);
            
            //If songBeginning and firstNumber are the same index, exit the routine as the file has already been renamed.
            if(songBeginning == firstNumber)
                return;

            //Find the index of the end of the song, the path to the directory holding the songs, the song extension and the completed, restructured file name.
            lastNumber = EndSearch.SongEndSearch(songTitle);
            path = songTitle.Substring(0, songBeginning);
            ext = song.ToString().Substring(lastNumber, (songTitle.Length - lastNumber));
            finalFileName = songTitle.Substring(firstNumber, (lastNumber - firstNumber)) + " [" + artistName + "]" + ext;

            //Rename the file to the new path.
            System.IO.File.Move(songTitle, (newAlbumPath + finalFileName));
        }
/////////////////////////////////////////////////////////
///End METHOD SongRenameAction///
    }
/////////////////////////////////////////////////////////
///End CLASS SongRename///
}
/////////////////////////////////////////////////////////
///End NAMESPACE BandcampMusicFileRename.Rename
