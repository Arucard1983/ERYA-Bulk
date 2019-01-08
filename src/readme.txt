ERYA-Bulk Version 4.10 ReadMe

----------------------------------------------------

1. Minimum Requirements

ERYA-Bulk requires a computer with an supported operating system or architecture.
- Current Version supports Windows 7 or greater (Windows XP may work), Mac OS X 10.11 or greater (Mac OS X 10.10 may work), Linux with wxWidgets GTK+ 3.0 toolkit (libwxgtk3 library) or greater.
- 64-bit x86-64 is the default suported architecture for all operating systems listed before. Windows and Linux 32-bit versions are provided for legacy systems.
- ARM binary versions are also provided for Linux systems.
- The minimum usefull screen resolution is 1280*720, but it is highly recommended to use a FullHD display (1920*1080) for better view.
- If the text size in labels and menus are to small or too big, even using the recomended screen resolutions, try to set the default pixel scale to normal (72 or 90 dpi), using the native operating system tools.
- About 100 Mb of free hard disk space is highly recommended.

2. Supported Features
# Ziegler's Parameters database supports the SRIM tables for all elements, along the Ziegler's Parameters of this stopping-power model itself. An optional algebraic expression are also supported.
# Detector's Efficiency suports algebraic functions as standard mode.
# Isolation of legacy LabView code in order to support as a strict import feature.
# All native ERYA databases are XML files.
# All previous LabView ERYA features was ported to current ERYA.

3. Installation from Binary Package.

# Linux Instructions
- Download the "Linux" package to a temporary folder, and then open a Terminal emulator:
$ cd <temp folder>
# If you use Debian, or Ubuntu, then install the package using the following commands:
$ sudo dpkg -i eryabulk_4.00-5_amd64.deb
$ sudo apt-get -f install
# Or using the GUI GNOME Software installer, Synaptic or any package manager avaliable.
- Click on package file, and the pacjage manager should install any additional libraries.
# Open the ERYA-Bulk from your favorite Desktop Environment (Menu>Science>ERYA-Bulk), or simply type on Terminal:
$ /opt/ERYA-Bulk/ERYA-Bulk
# The ERYA-Bulk will create a profile directory as ~/.ERYA-Bulk, where the local configuration settings will be stored.
# To uninstall, simply use this command on Terminal:
$ sudo apt-get remove eryabulk
- If necessary, delete the user profile folder:
$ rm -rf ~/.ERYA-Bulk
- If the program complains about missing libraries, check your Linux Distro support website to search the package name of libwxgtk3. On Debian systems, just type:
$ sudo apt-get install libwxgtk3.0-0v5


# Windows Instructions
- Open the Self-Extractor archive, and select the desired target folder. You can extract to the standard Program Directory, or to a user Documents folder.
- To uninstall, just delete the program folder or run the uninstaller program.
- Ince removed, if necessary delete the C:\users\<name>\Local Settings\Application Data\ERYA-Bulk\ folder.

# Mac OS X Instructions
- Unpack the zipped file, and then copy the application package (ERYA-Bulk.app) to the Applications folder.
- To uninstall, delete the application from the Application folder, and then the ~/Library/Application Support/ERYA-Bulk folder.

4. First Run

# At start-up, once the program don't found any configuration file, it will display a wizard, asking the user to follow the instructions.
Once completed, ERYA-Profiling will create a ERYA-Profiling.conf on the local user settings directory, which location are defaulted by wxWidgets (see remarks above), where should store the target location and the name of three core database files (Element Database, Detector Efficiency and Ziegler Parameters).
# You can use the original Databases from LabView ERYA, including the text format, and the binary format. However ERYA-Bulk will export, check and convert to their native format as their default databases.
# The original bundled databases are located in the same directory of the program, and during this step the program will default this folder to ease the user selection. 
# The program will check the selected files, and if everything goes correctly, the selected databases and the configuration file will be stored sucefully, otherwise you will get an error and ERYA will start in reset mode (No database loaded at all).
- Once done selecting (it's better to choose the bundled databases first), ERYA-Bulk will start at full functionality.
- In next start-up, ERYA-Bulk will load the config file automatically and load the database files without user interaction.
# Note: If any default database is damaged, an error will be issued, and probably require to repeat the first time procedure to reset.
# Note: The ERYA-Bulk.conf file will be stored on the following directory.
- Linux : ~/.ERYA-Bulk/
- Windows : C:\users\<name>\Local Settings\Application Data\ERYA-Bulk\
- Mac OS X: ~/Library/Application Support/ERYA-Bulk
- Portable (All Operating Systems): The same program directory.


5. About files compability

This software can export the Ziegler, Detector and Element database from an ERYA software built with the LabView Runtime.
- Binary database format is partial supported, and this software only import them. You cannot export the current database on LabView format, since the LabView DataLog format is not documented!
- Text-form databases can be exported (very good support), and imported(some issues). Please, check the on-line help about some particular supported files.
- The user can import IBANDL files to edit Element's characteristics, but saving is limited. However, you can use third-party tools to edit the exported IBANDL file, if necessary.
- Experimental support of Microsoft Office Excel Xlsx files (2007,2010,2013, and beyond) at very basic level was implemented.
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

7. Troubleshooting and Issues

- If you use a debug release, click "Cancel" on start-up to disable annoying debugging warnings unless you want to contribute the ERYA development.
- If the program fails to open the main window and opens error mensages, this means the essentials databases are corrupted, or the configuration file described on section #4 are corrupted.
# To fix, remove the application and config files as described by section #3, and make a fresh install of the application.
- On most realistic situations, any fitting process rarely exceeds several seconds, unless you use a slow computer.
# Our tests show that any Fitting procedure takes between 4 to 13 steps, 10 in average.
Intel Core x86-64 computers may evaluate a full fitting analysis in less than 3 seconds.
On other hand, a Tegra 3(ARM) may take up to 30 seconds to do the same task.
- The application may crash in some unpredictable situations, but is it now a much rare issue.
# The user can, finally, import binary LabView files which structure are compatible with ERYA-Bulk. The new import feature was possible due to some third-party research, and much reverse engineering.
- Since the LabView import libraries cannot be 100% safe to work, please backup the original file before import.
# The ERYA-Bulk can also save some results as a Microsoft Office Excel spreadsheet files, making the clipboard feature less relevant.

Copyrights

Group: 2017 LibPhys
Programmer e-mail: vm.manteigas@campus.fct.unl.pt
wxMathPlot/wxWidgets licence:  https://github.com/wxWidgets/wxWidgets/blob/master/docs/licence.txt
