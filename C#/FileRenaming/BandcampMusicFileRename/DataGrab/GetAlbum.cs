///Header///
/////////////////////////////////////////////////////////
/*
Title:          Bandcamp Music File Rename
Descr:          This file uses the TagLib library to retrieve the album name from the metadata of a song.
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
using TagLib;
/////////////////////////////////////////////////////////
///End Library Imports///

///NAMESPACE BandcampMusicFileRename.DataGrab///
/////////////////////////////////////////////////////////
namespace BandcampMusicFileRename.DataGrab
{

///CLASS GetAlbum///
/////////////////////////////////////////////////////////
    public static class GetAlbum
    {

///METHOD GetAlbumDiscovery///
/////////////////////////////////////////////////////////
        public static string GetAlbumDiscovery(string musicFilePath)
        {
            //Create the TagLib file object and return the Album property.
            TagLib.File file = TagLib.File.Create(musicFilePath);
            return file.Tag.Album;
        }
/////////////////////////////////////////////////////////
///End METHOD GetAlbumDiscovery///
    }
/////////////////////////////////////////////////////////
///End CLASS GetAlbum///
}
/////////////////////////////////////////////////////////
///End NAMESPACE BandcampMusicFileRename.DataGrab///
