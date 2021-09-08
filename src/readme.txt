ERYA-Bulk Version 5.00 ReadMe

----------------------------------------------------

1. Minimum Requirements

ERYA-Bulk requires a computer with an supported operating system or architecture.
- Windows 7, 10 or greater (Windows XP may work), Mac OS X 10.11 or greater (Mac OS X 10.10 may work), Linux Debian 11 or Ubuntu 21.04 or greater.
- 64-bit x86-64 is the default suported architecture for all operating systems listed before. Windows and Linux 32-bit versions are provided for legacy systems.
- ARM binary versions are also provided for Linux systems.
- The minimum usefull screen resolution is 1280*720, but it is highly recommended to use a FullHD display (1920*1080) for better view.
- About 10 to 100 Mb of free hard disk space is highly recommended.

2. Supported Features
# Ziegler's Parameters database supports the SRIM tables for all elements, along the Ziegler's Parameters of this stopping-power model itself. An optional algebraic expression are also supported.
# Detector's Efficiency suports algebraic functions as standard mode.
# Isolation of legacy LabView code in order to support as a strict import feature.
# All native ERYA databases are XML files.
# All previous LabView ERYA features was ported to current ERYA.

3. Installation from Binary Package.

# Linux Instructions
3.1. Check first if your Linux distribution are compatible with Debian 11 or Ubuntu 21.04 codebase.

3.2. Download the correct arch version to your computer, then open a Terminal window on the same folder of the downloaded package.
- For Ubuntu systems, it's not necessary to open a root shell before install programs:
$ sudo apt update
$ sudo apt dist-upgrade
$ sudo dpkg -i *.deb
$ sudo apt -f install
- On Debian 11, due to higher security restrictions, it is necessary to open a root shell first:
$ su [Ask for your root password...]
$ apt update
$ apt dist-upgrade
$ dpkg -i *.deb
$ dpkg -t stretch-backports -f install

3.3. Open the ERYA-Bulk from your favorite Desktop Environment (Menu>Education>ERYA-Bulk), or simply type on Terminal:
$ LC_ALL=C /opt/ERYA-Bulk/ERYA-Bulk

3.4. The ERYA-Bulk will create a profile directory as ~/.ERYA-Bulk, where the local configuration settings will be stored.

3.5. To uninstall, simply use this command on Terminal:
$ sudo apt remove eryabulk

3.6. If necessary, delete the user profile folder:
$ rm -rf ~/.ERYA-Bulk

# Windows Instructions
- Open the Package File that will start the installation wizard program. Follow the instructions.
- To uninstall, just run the uninstaller program.
- Once removed, if necessary delete the C:\users\<name>\Local Settings\Application Data\ERYA-Bulk-Win64\ folder.

# Mac OS X Instructions
- Unpack the zipped file, and then copy the application package (ERYA-Bulk.app) to the Applications folder.
- To uninstall, delete the application from the Application folder, and then the ~/Library/Application Support/ERYA-Bulk-OSX folder.

4. First Run

# When ERYA don't find any configuration file, it will start a setup wizard, asking for the default Database, Detector and Ziegler files to be the default ones.
- Once completed the setup, ERYA will start automatically straight to GUI, loading the default databases into memory.
- A failure on this stage will make ERYA starts on reset mode, warning the user about the faulty file.
# You can import the original Databases from LabView ERYA, including the ones using plain text format, and the binary format, using the Database Tools on the program context.
- However ERYA-Bulk will import, check and convert to their native format when saves their content.
# To fix setup errors, open the tool from Database > Setup, and make the necessary changes.
- Once fixed, click on "Save" to create a new configuration file. The new settings will be tested, and ERYA will reload the databases to check if everything are fine.
- To reset any setup files, and forces ERYA to start the Setup Wizard, you can click on "Delete" button to make it.
# Note: The ERYA-Bulk.conf file will be stored on the following directory.
- Linux : ~/.ERYA-Bulk/
- Windows : C:\users\<name>\Local Settings\Application Data\ERYA-Bulk\
- Mac OS X: ~/Library/Application Support/ERYA-Bulk
- Portable (All Operating Systems): The same program directory.

5. About files compability

# This software can export the Ziegler, Detector and Element database from an ERYA software built with the LabView Runtime:
- Binary database format importation from LabView as supported for Ziegler's and Elements databases. The user should select "Legacy txt" format, and ERYA will convert automatically to the native ERYA Database format. 
# Text-form databases can be imported (very good support), and exported (some issues). Further details can be obtained from the user manual.
# The user can import IBANDL files to edit Element's characteristics, and also export them. However some edition of IBANDL files created by ERTA may be needed.
# Experimental support of Microsoft Office Excel Xlsx files (2007,2010,2013, and beyond) at very basic level was implemented.
# Note: ERYA-Bulk will use internally the native file format, and makes automatically the conversions if it loads any supported third-party file format.

6. Bugfixes
# The crash issues was finally fixed on fitting procedures due to some bugs on code. Now crash issues are more rare, since the better exception handling avoids the majority of crash issues.
# The Levenberg-Marquardt algorithm was also modified to handle more convergence methods, whre it can solve many false non-convergence results.
# Added Support for Calibration Parameters on Cross-Section and Detector Settings
# The packages was now updated to supported the new Debian 9 Stretch.
# Fix a serious bug on built-in Xlsx library that make abnormal saving times with sparse cells.
# A new option to generate partial Yields, once fitted, for several energies between minimum and maximum input Energy, intended to generate a profile, and narrows the bridge with the new ERYA-Profiling software.
# A few missing options from the LabView ERYA was now implemented: A calibration coeficient for each Element/Isotope.
# The Setup widget from "Databases">"Setup Default Databases" now work correctly, generating the new default files on the correct places.
# The ERYA-Bulk don't need to be restarted once the new default databases are set.
# Backports several compatibility libraries from the new wxWidgets based ERYA-Profiling.
# Support to SRIM tables and custom function to the Stopping-Power evaluation, along an updated Ziegler's Parameters widget.
# The fitting and basic profiling process now had a gauge dialog that informs some basic progress information, along the possibility to abort the whole process.
# Some minor bugfixes on main interface events.
# The widget dialogs can now be resized by mouse.
# SRIM tables are the default setting for the Stopping-Power model.
# References about Stoichiometry are replaced to Composition.
# Help files was removed from code.
# ERYA Databases had a Comment field that can be edited by a built-in text editor. This editor are avaliable when the user click "Get Info" on the Databases Tools.

7. Troubleshooting and Issues

# If the program fails to open the main window and opens error mensages, this means the essentials databases are corrupted, or the configuration file described on section #4 are corrupted.
- To fix, remove the application and config files as described by section #3, and make a fresh install of the application.
# On most common situations, any fitting process rarely exceeds one minute.
- Samples with 10 or 20 elements (beyond any realistic sample) may take several minutes to fit, but it's expected from the numerical implementation.
# The application may crash in some unpredictable situations, but is it now a much rare issue.
# The user can import binary LabView files which structure are compatible with ERYA-Bulk. The new import feature was possible due to some third-party research, and much reverse engineering.
- Since the LabView import libraries cannot be 100% safe to work, please backup the original file before import.
# The ERYA-Bulk can also save some results as a Microsoft Office Excel spreadsheet files, making the clipboard feature less relevant.

Copyrights

Group: 2021 LibPhys
Programmer e-mail: vm.manteigas@campus.fct.unl.pt
wxMathPlot/wxWidgets licence:  https://github.com/wxWidgets/wxWidgets/blob/master/docs/licence.txt
License: LGPLv3
