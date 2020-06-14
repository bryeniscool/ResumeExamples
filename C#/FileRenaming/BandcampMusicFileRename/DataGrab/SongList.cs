///Header///
/////////////////////////////////////////////////////////
/*
Title:          Bandcamp Music File Rename
Descr:          This file is used to build an array of FileInfo which relates to all songs located in the supplied directory.
                A song is defined as any file with the extension '.flac' or '.mp3'.
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
/////////////////////////////////////////////////////////
///End Library Imports///

///NAMESPACE BandcampMusicFileRename.DataGrab///
/////////////////////////////////////////////////////////
namespace BandcampMusicFileRename.DataGrab
{

///CLASS SongList///
/////////////////////////////////////////////////////////
    public static class SongList
    {

///METHOD SongListDiscovery///
/////////////////////////////////////////////////////////
        public static FileInfo[] SongListDiscovery(DirectoryInfo albumDir)
        {
            //Chceck every file in the directory for a .flac extension. If the array is not null return the array, else check every file in the directory for a .mp3 extension.
            FileInfo[] songs = null;
            songs = albumDir.GetFiles("*.flac");

            if(songs == null || songs.Length == 0)
                songs = albumDir.GetFiles("*.mp3");

            return songs;
        }
/////////////////////////////////////////////////////////
///End METHOD SongListDiscovery///
    }
/////////////////////////////////////////////////////////
///End CLASS SongList///
}
/////////////////////////////////////////////////////////
///End NAMESPACE BandcampMusicFileRename.DataGrab///
