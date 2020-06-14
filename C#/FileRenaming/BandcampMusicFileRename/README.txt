Hello!
This program was designed to rename any music albums purchased from the online store 'Bandcamp' to a personal file naming standard.

There are four namespaces created in this program based off of the file structure:
    - BandcampMusicFileReanme
    - BandcampMusicFileReanme.DataGrab
    - BandcampMusicFileReanme.Rename
    - BandcampMusicFileReanme.StringManipulation

All .cs files contain descriptions within the files.

The entry point of execution is the 'MainEntry.cs' file located in the root directory.


The only item that needs to be changed in order to execute properly is line 41 in 'MainEntry.cs'. This line contains the path to the root of the bandcamp directory.

The target naming structure is as follows:
    <Directory>(Artist Name)/<Directory>(Album Year) (Album Title)/<Files>(Song Number) (Song Title) [(Artist Name)].(Extension)

There isn't a sample Bandcamp file to test with, because of copyright laws. For reference, a typical Bandcamp album resembles the structure outlined below:
    (Artist) - (Album)
    |
     > (Artist) - (Album) - (Song Number) (Song Name).(Extension) 

For Example:
    Cynic - Traced in Air
    |
     > Cynic - Traced in Air - 01 Nunc Fluens.flac

Once the script runs, the directory will look as such:
    Cynic
    |
     > (2008) Traced in Air
      |
       > 01 Nunc Fluens [Cynic].flac
