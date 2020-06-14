///Header///
/////////////////////////////////////////////////////////
/*
Titl
Descr:          This file uses the TagLib library to retrieve the artist name from the metadata of a song.
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

///CLASS GetArtist///
/////////////////////////////////////////////////////////
    public static class GetArtist
    {

///METHOD GetArtistDiscovery///
/////////////////////////////////////////////////////////
        public static string GetArtistDiscovery(string musicFilePath)
        {
            //Create the TagLib file object and return the Artist property.
            TagLib.File file = TagLib.File.Create(musicFilePath);
            return file.Tag.FirstAlbumArtist;
        }
/////////////////////////////////////////////////////////
///End METHOD GetArtistDiscovery///
    }
/////////////////////////////////////////////////////////
///End CLASS GetArtist///
}
/////////////////////////////////////////////////////////
///End NAMESPACE BandcampMusicFileRename.DataGrab///
