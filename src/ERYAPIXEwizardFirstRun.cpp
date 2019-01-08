#include "ERYAPIXEwizardFirstRun.h"
#include "ERYAPIXEMainFrame.h"

ERYAPIXEwizardFirstRun::ERYAPIXEwizardFirstRun( wxWindow* parent )
:
wizardFirstRun( parent )
{
 // Set some local parameters
 wxString fileconfig = ::wxStandardPaths::Get().GetExecutablePath();
  wxFileName mainconfig(fileconfig);
  wxString rootconfig = mainconfig.GetPath();
  wxString pathconfig = rootconfig;
 fileDetectorFirstRun->SetPath(pathconfig);
 fileElementsFirstRun->SetPath(pathconfig);
 fileZieglerFirstRun->SetPath(pathconfig);
}

void ERYAPIXEwizardFirstRun::OnFirstRunCancel( wxWizardEvent& event )
{
 wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Are you sure you want to quit?\nERYA Bulk will not working correctly\nwithout a configuration file."), wxT("Abort Configuration Wizard ?"), wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
 if (dial->ShowModal() == wxID_YES)
 {
  // Call the parent frame
 ERYAPIXEMainFrame *Parent = (ERYAPIXEMainFrame *) GetParent();
 // Sync the values
 Parent->SetSync(false);
 return;
 }
}

void ERYAPIXEwizardFirstRun::OnFirstRunFinish( wxWizardEvent& event )
{
  // Get the two compatible name paths
  wxString fileconfig = ::wxStandardPaths::Get().GetExecutablePath();
  wxFileName mainconfig(fileconfig);
  wxString rootconfig = mainconfig.GetPath();
  wxString pathconfig = rootconfig;
  wxString standardconfig = ::wxStandardPaths::Get().GetUserLocalDataDir();
 // Get the collected variables
 wxString StandardDirectory, StandardDetector, StandardDatabase, StandardZiegler;
 wxFileName DetectorName,ElementName,ZieglerName;
 int DefaultSetupSetting = radioSetupDefault->GetSelection();
 if (DefaultSetupSetting == 0)
  StandardDirectory = standardconfig;
 else
  StandardDirectory = pathconfig;
 // Get the Detector file
 DetectorName = fileDetectorFirstRun->GetFileName();
 StandardDetector = DetectorName.GetFullName();
 // Get the Detector file
 ElementName = fileElementsFirstRun->GetFileName();
 StandardDatabase = ElementName.GetFullName();
 // Get the Ziegler file
 ZieglerName = fileZieglerFirstRun->GetFileName();
 StandardZiegler = ZieglerName.GetFullName();
 // Call the parent frame
 ERYAPIXEMainFrame *Parent = (ERYAPIXEMainFrame *) GetParent();
 // Sync the values
 Parent->SetSync(true);
 if (DefaultSetupSetting == 0)
  Parent->SetDefault(true);
 else
  Parent->SetDefault(false);
 Parent->SetConfig(StandardDirectory);
 Parent->SetDetectorFileName(StandardDetector);
 Parent->SetDatabaseFileName(StandardDatabase);
 Parent->SetZieglerFileName(StandardZiegler);
 return;
}
