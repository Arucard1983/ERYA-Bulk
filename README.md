# ERYA-Bulk
A scientific program to evaluate nuclear reaction yields from bulk samples.

# Project Webpage
https://sites.fct.unl.pt/nuclear/pages/home

https://sites.fct.unl.pt/nuclear/software/erya-bulk

# Main Features
Since this readme are under construction, the user can either fetch the user manual on "docs" directory, or visit the project webpage, for further information.

# Installation from Source
As explained on Tutorial Manual, official packages avaliable on this repository or under project webpage was built and tested using Linux computers running Debian 11. Windows versions are built using MinGW under Debian 11. An experimental Mac OS X port was made using osx-cross under Debian 11.

To built ERYA-Bulk straight from Github it require the user to have:
1. A compatible C++ compiler for your operating system (GCC, MinGW, Clang, etc)
2. To build wxWidgets 3.0.4 or 3.0.5 (the last used on Debian 11) prior to build this software.
3. A git client software.

For Linux host machines, the user recomend to use the wxWidgets libraries provided by your distribution.
The author strongly recomended to use MSYS2 or a similar POSIX shell for Windows hosts, as long it builts a native Windows executable, since it simplifies the compilation of wxWidgets libraries from the source code. Do not be confused with WSL that is a compatibility layer designed to run native Linux binaries on Windows hosts.

To test if your machine had wxWidgets headers and libraries, open the Terminal (Linux) or the MSYS2 shell (Windows) and type the following commands:

$ g++ -v

$ wx-config

$ wx-config --libs

It should print the linker variables on the last sentence, otherwise the wxWidgets installation are broken.

Once everything settled, open a Terminal and type the following sequence of commands:

$ git clone https://github.com/Arucard1983/ERYA-Bulk.git

$ cd ERYA-Bulk

Use the correct makefile respective to your native environment, or edit straight one of them to your machine constraints:

$ make -f makefile.Linux

$ make -f makefile.MSYS2

The makefile will create a "/build" folder and all necessary assets to make ERYA-Bulk work. Read the manual to operate the software correctly.
