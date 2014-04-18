Creating the MaxCoin Windows Installer
======================================

The installer is created using the NullSoft Installer System, or NSIS. This compiles the maxcoin.nsi file to create a single, executable installer. The installer takes the required DLL files, the command-line and the graphical versions of the MaxCoin wallet, a README, a license and an icon as inputs, some of which have been provided. The DLLs are dependant upon the build environment used and should be modified as required.

The installer is simple and easy to understand by looking at the NSI script. It acts as follows:

* Ensure admin rights are present
* Check that the Windows version is Vista or later
* Present the user with the MIT license
* Ask the user for the install location, providing a default
* Install the compressed files to that location
* Create an uninstaller and add that to the location
* Add application information to the registry, so that Add/Remove programs can be used correctly

When modifying the installer, it is important that the INSTALLSIZE define is updated. This is the size of the contained files in kB.