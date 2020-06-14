///Header///
/////////////////////////////////////////////////////////
/*
Title:          Bandcamp Music File Rename
Descr:          This file is used to slice the album directory name out of the supplied path.
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
///End Library Imports///
/////////////////////////////////////////////////////////

///NAMESPACE BandcampMusicFileRename.StringManipulation///
/////////////////////////////////////////////////////////
namespace BandcampMusicFileRename.StringManipulation
{

///CLASS AlbumName///
/////////////////////////////////////////////////////////
    public static class AlbumName
    {

///METHOD AlbumNameSearch///
/////////////////////////////////////////////////////////
        public static string AlbumNameSearch(FileInfo albumPath)
        {
            //Get the index of the last "/" (or "\") in the path.
            //Slice the string from the final slash to the end of the path, then return.
            int slash = SongBeginning.SongBeginningSearch(albumPath.ToString());
            return albumPath.ToString().Substring(slash, (albumPath.ToString().Length - slash));
        }
/////////////////////////////////////////////////////////
///End METHOD AlbumNameSearch///
    }
/////////////////////////////////////////////////////////
///End CLASS AlbumName///
}
/////////////////////////////////////////////////////////
///End NAMESPACE BandcampMusicFileRename.StringManipulation///
