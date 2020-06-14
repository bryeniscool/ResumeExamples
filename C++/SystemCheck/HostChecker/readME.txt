Hello!
This program will run various checks to see if particular software is installed, services are running, the current domain the machine is attached to and the last time a specific file was modified. Below is how the .exe file in "Executable" is currently setup.
  1) A check to see if Notepad ++ (x64) is installed.
  2) A check to see if Google Chrome (x64) is installed.
  3) A check to see if the Windows Update service is running.
  4) A check to see if the Windows Firewall service is running.
  5) A check to see if the Print Spooler service is running.
  6) A check to see if the file "C:\Windows\setupact.log" has been modified within 7 days.
There is no domain check, as if you will be downloading and testing this executable, a test for an unknown domain is useless.

The file "C:\Windows\setupact.log" may not be present on your system. Please adjust the file location for something that is confirmed to exist (Line 48).

If all checks pass, a green check mark will appear in the system tray. If they fail, a red X will be displayed.

By left-clicking on the icon, a breakdown of what passed and what failed will be available. Please note, this display needs to be adjusted. Depending on the resolution, the results could be out of alignment.

By right-clicking an option to exit the host checker will be displayed, and an option to run a scan on demand will be displayed.

By default, the scan will occur every five minutes after launch. 

Please use admin rights if necessary. Some services require admin rights for reads on them. If you continually see a sevice failing the check, yet you visibly see it running, try restarting the application with administrator rights.

This was setup in our environment to make sure machines were compliant with our AV and Endpoint Management software. It was also used to make sure end users were not pulling machines off of our corporate domain.

Line 46 of main.cpp will show a path to the  uninstall registry location. If checking for x86 application installs, uncomment and use line 47.

The icons folder contains all of the icons for the application, the system bar, and the system tray. I used Windows icons for all of them. These can be adjusted in the HostChecker.rc file. If adding more icons, please adjust the resources.h file.

For more information on the functions created, please look at the source code for extensive notes. 
