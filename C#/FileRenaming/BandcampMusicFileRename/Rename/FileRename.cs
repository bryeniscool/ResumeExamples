///Header///
/////////////////////////////////////////////////////////
/*
Title:          Bandcamp Music File Rename
Descr:          This file is used to navigate through child directories within the bandcamp root directory, discover metadata information for each album, move files to the proper location, and delete files and direcotries no longer needed. It contains two method:
                    FileRenameAction(string musicSource)
                    SongRenameSub(DirectoryInfo albumDir, string rootFolder)
Author:         Bryen Wittman
Data:           06/10/2020
Modified:       Bryen Wittman, 06/10/2020
Version:        1.0
Language:       C#, .NET CORE ver. 3.1
Comments:
                The FileRenameAction method is called by main.
                The SongRenameSub method is used only internally by the FileRename class.
*/
/////////////////////////////////////////////////////////
///End Header///

///Library Imports///
/////////////////////////////////////////////////////////
using System;
using System.IO;
using TagLib;
using BandcampMusicFileRename.StringManipulation;
using BandcampMusicFileRename.DataGrab;
///End Library Imports///
/////////////////////////////////////////////////////////

///NAMESPACE BandcampMusicFileRename.Rename///
/////////////////////////////////////////////////////////
namespace BandcampMusicFileRename.Rename
{

///CLASS FileRename///
/////////////////////////////////////////////////////////
    public class FileRename
    {

///METHOD FileRenameAction///
/////////////////////////////////////////////////////////
/*
- The FileRenameAction method is used to get a DirectoryInfo object pointing to the root directory of the Bandcamp files.
- It then traverses the created object, calling the SongRenameSub method on each child directory.
*/
        public void FileRenameAction(string musicSource)
        {
            //Create a DirectoryInfo object referencing the Bandcamp root directory.
            DirectoryInfo sourceDir = new DirectoryInfo(musicSource);

            //Loop through all child directories in the Bandcamp root directory.
            foreach(DirectoryInfo artistsDir in sourceDir.GetDirectories("*"))
                SongRenameSub(artistsDir, musicSource);
        }
/////////////////////////////////////////////////////////
///End METHOD FileRenameAction///

///METHOD SongRenameSub///
/////////////////////////////////////////////////////////
/*
- The SongRenameSub method is used to collect all songs in an album, rename them and move them. In order to accomplish this, the follwing actions need to be exectued:
    -Get the album year.
    -Get the album artist.
    -Get the album title.
    -Create the artist directory if it doesn't exist.
    -Create the album directory under the artist directory if it doesn't exist.
    -Rename all songs to the appropriate format.
    -Move any remaining files that aren't a song.
    -Delete the original Bandcamp album folder.
*/

        private void SongRenameSub(DirectoryInfo albumDir, string rootFolder)
        {
            //Create an array of FileInfo to hold each song found.
            FileInfo[] songs;

            //Strings to hold titles, paths and any additional files in the albums.
            string finalAlbumTitle = null;
            string newAlbumPath = null;
            string extraFile = null;

            //Create an SongRename object.
            SongRename songRenameObj = new SongRename();

            //Fille the FileInfo array with all songs located in the directory. Once complete, find the year, artist and album from the first song in the array, then build the directory name for the album and the path for the newly created directory to go.
            songs = SongList.SongListDiscovery(albumDir);
            UInt32 aYear = GetYear.GetYearDiscovery(songs[0].ToString());
            string aArtist = GetArtist.GetArtistDiscovery(songs[0].ToString());
            string aAlbum = GetAlbum.GetAlbumDiscovery(songs[0].ToString());
            finalAlbumTitle = "(" + aYear + ") " + aAlbum + "/";
            newAlbumPath = rootFolder + aArtist + "/" + finalAlbumTitle;

            //If the artist doesn't exist, create it.
            if(!Directory.Exists(rootFolder + aArtist))
                Directory.CreateDirectory(rootFolder + aArtist);
            //If the album doesn't exist, create it.
            if(!Directory.Exists(newAlbumPath))
                Directory.CreateDirectory(newAlbumPath);

            //Loop through each song and rename them using the SongRenameAction method from the SongRename object.
            foreach(FileInfo song in songs)
                songRenameObj.SongRenameAction(song, aArtist, newAlbumPath);

            //Loop through any additional files in the bandcamp direcotry and move them to the newly created album directory.
            foreach(FileInfo leftOverFiles in albumDir.GetFiles("*"))
            {
                extraFile = AlbumName.AlbumNameSearch(leftOverFiles);
                System.IO.File.Move((albumDir + "/" + extraFile), (newAlbumPath + extraFile));
            }

            //Delete the Bandcamp directory as it's no longer needed.
            Directory.Delete(albumDir.ToString());
        }
/////////////////////////////////////////////////////////
///End METHOD SongRenameSub///
    }
/////////////////////////////////////////////////////////
///End CLASS FileRename///
}
/////////////////////////////////////////////////////////
///End NAMESPACE BandcampMusicFileRename.Rename///
