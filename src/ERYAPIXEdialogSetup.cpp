/***************************************************************
 * Name:      ERYAPIXEdialogSetup.cpp
 * Purpose:   ERYA Config Editor implementation
 * Author:    Vasco Manteigas (vm.manteigas@campus.fct.utl.pt)
 * Created:   2015-12-01
 * Copyright: Grupo de Reacoes Nucleares - LIBPHYS
 * License:
 **************************************************************/

#include "ERYAPIXEdialogSetup.h"
#include "ERYAPIXEMainFrame.h"
#include "FileLibrary.h"


ERYAPIXEdialogSetup::ERYAPIXEdialogSetup( wxWindow* parent )
:
dialogSetup( parent )
{
 // Get the current setup information
 wxString SetupPath, SetupDetector, SetupDatabase, SetupZiegler;
 // Call the parent frame
 ERYAPIXEMainFrame *Parent = (ERYAPIXEMainFrame *) GetParent();
 // Sync the local data from parent frame to child frame
 if(Parent->LoadSetupFile(SetupPath,SetupDetector,SetupDatabase,SetupZiegler))
 {
 // Set the default folder
 fileDefaultDetector->SetInitialDirectory(SetupPath);
 fileDefaultDatabase->SetInitialDirectory(SetupPath);
 fileDefaultZiegler->SetInitialDirectory(SetupPath);
 if (SetupPath == ::wxStandardPaths::Get().GetUserLocalDataDir())
     radioDefaultConfig->SetSelection(0);
 else
     radioDefaultConfig->SetSelection(1);
 // Set the current default databases
 wxFileName rootDetector(SetupPath + wxFileName::GetPathSeparator() + SetupDetector);
 wxFileName rootDatabase(SetupPath + wxFileName::GetPathSeparator() + SetupDatabase);
 wxFileName rootZiegler(SetupPath + wxFileName::GetPathSeparator() + SetupZiegler);
 fileDefaultDetector->SetFileName(rootDetector);
 fileDefaultDatabase->SetFileName(rootDatabase);
 fileDefaultZiegler->SetFileName(rootZiegler);
 }
}

// Save the changed settings to a new config file
void ERYAPIXEdialogSetup::OnDefaultSave( wxCommandEvent& event )
{
  wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Do you want to save the changed settings?"), wxT("Save Edited Selection"), wxYES_NO | wxNO_DEFAULT | wxICON_WARNING);
  if (dial->ShowModal() == wxID_YES)
  {
    // Global values
    wxString StandardConfigPath = ::wxStandardPaths::Get().GetUserLocalDataDir();
    wxString ExecutablePath = ::wxStandardPaths::Get().GetExecutablePath();
    wxFileName ExecutableFile(ExecutablePath);
    wxString RootConfigPath = ExecutableFile.GetPath();
    // Get the necessary values
    wxString SetupMainDirectory,SetupFileDetector, SetupFileDatabase, SetupFileZiegler;
    wxFileName DetectorName,ElementName,ZieglerName;
    ElementName = fileDefaultDatabase->GetFileName();
    SetupFileDatabase = ElementName.GetFullName();
    DetectorName = fileDefaultDetector->GetFileName();
    SetupFileDetector = DetectorName.GetFullName();
    ZieglerName = fileDefaultZiegler->GetFileName();
    SetupFileZiegler = ZieglerName.GetFullName();
    // Load and save files
    ElementDatabaseArray TempDatabase;
    ZieglerParameters TempZiegler;
    DetectorParameters TempDetector;
    ElementSRIMArray TempSRIM;
    wxString SourceFileDetector, SourceFileDatabase, SourceFileZiegler;
    SourceFileDetector = DetectorName.GetFullPath();
    SourceFileDatabase = ElementName.GetFullPath();
    SourceFileZiegler = ZieglerName.GetFullPath();
    DetectorFile ThisDetector(SourceFileDetector,wxT("epsd"),TempDetector);
    DatabaseFile ThisDatabase(SourceFileDatabase,wxT("epd"),TempDatabase);
    ZieglerFile ThisZiegler(SourceFileZiegler,wxT("epsz"),TempZiegler,TempSRIM,0);
    // Call the parent frame
   ERYAPIXEMainFrame *Parent = (ERYAPIXEMainFrame *) GetParent();
    if(ThisDatabase.ERYAPIXEDatabaseFileLoad())
    {
     TempDatabase = ThisDatabase.GetDatabase();
    }
    else
    {
     wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Cannot load the original Database file."), wxT("Source File Error!"), wxOK | wxICON_ERROR);
     dial->ShowModal();
     Parent->ProgramQuit(false);
     Close();
    }
    if(ThisDetector.DetectorFileLoad())
    {
     TempDetector = ThisDetector.GetParameters();
    }
    else
    {
     wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Cannot load the original Detector file."), wxT("Source File Error!"), wxOK | wxICON_ERROR);
     dial->ShowModal();
     Parent->ProgramQuit(false);
     Close();
    }
    if(ThisZiegler.ZieglerFileLoad())
    {
     TempZiegler = ThisZiegler.GetParameters();
     TempSRIM = ThisZiegler.GetTables();
    }
    else
    {
     wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Cannot load the original Ziegler file."), wxT("Source File Error!"), wxOK | wxICON_ERROR);
     dial->ShowModal();
     Parent->ProgramQuit(false);
     Close();
    }
    bool SetupDirectory;
    if(radioDefaultConfig->GetSelection() == 0)
    {
         SetupDirectory = true;
         SetupMainDirectory = StandardConfigPath;
    }
    else
    {
        SetupDirectory = false;
        SetupMainDirectory = RootConfigPath;
    }
   if(Parent->ReplaceSetupFile())
   {
     if(Parent->SaveSetupFile(SetupMainDirectory,SetupFileDetector,SetupFileDatabase,SetupFileZiegler,SetupDirectory))
     {
      // Saving the new files
      DetectorFile TargetDetector(SetupMainDirectory + wxFileName::GetPathSeparator() + SetupFileDetector,wxT("epsd"),TempDetector);
      DatabaseFile TargetDatabase(SetupMainDirectory + wxFileName::GetPathSeparator() + SetupFileDatabase,wxT("epd"),TempDatabase);
      ZieglerFile TargetZiegler(SetupMainDirectory + wxFileName::GetPathSeparator() + SetupFileZiegler,wxT("epsz"),TempZiegler,TempSRIM,0);
      if(!(TargetDetector.DetectorFileSave()))
      {
       wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Cannot save a new Detector file!\nRefer to the User Guide for details."), wxT("Target File Error!"), wxOK | wxICON_ERROR);
       dial->ShowModal();
       Parent->ProgramQuit(false);
       Close();
      }
      if(!(TargetDatabase.ERYAPIXEDatabaseFileSave()))
      {
       wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Cannot save a new Database file!\nRefer to the User Guide for details."), wxT("Target File Error!"), wxOK | wxICON_ERROR);
       dial->ShowModal();
       Parent->ProgramQuit(false);
       Close();
      }
      if(!(TargetZiegler.ZieglerFileSave()))
      {
       wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Cannot save a new Ziegler file!\nRefer to the User Guide for details."), wxT("Target File Error!"), wxOK | wxICON_ERROR);
       dial->ShowModal();
       Parent->ProgramQuit(false);
       Close();
      }
      // And saving the current parameters
      Parent->SetConfig(SetupMainDirectory);
      Parent->SetDetectorFileName(SetupFileDetector);
      Parent->SetDatabaseFileName(SetupFileDatabase);
      Parent->SetZieglerFileName(SetupFileZiegler);
      Parent->ProgramQuit(true);
      Close();
     }
     else
     {
     wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Cannot save a new ERYA Bulk configuration file.\nRefer to the User Guide for details."), wxT("Saving Error!"), wxOK | wxICON_ERROR);
     dial->ShowModal();
     Parent->ProgramQuit(false);
     Close();
     }
   }
   else
   {
   wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Cannot create a new ERYA Bulk configuration file.\nRefer to the User Guide for details."), wxT("Reset Error!"), wxOK | wxICON_ERROR);
   dial->ShowModal();
   Parent->ProgramQuit(false);
   Close();
   }
  }
}

// Clears the database names on main widget, but not change the config file!
void ERYAPIXEdialogSetup::OnDefaultReset( wxCommandEvent& event )
{
  wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Do you want to reset the current editions?\nThe actual configuration files content will not be changed."), wxT("Clear Edited Selection"), wxYES_NO | wxNO_DEFAULT | wxICON_WARNING);
  if (dial->ShowModal() == wxID_YES)
  {
   wxString SetupPath = ::wxStandardPaths::Get().GetUserLocalDataDir();
   radioDefaultConfig->SetSelection(0);
   fileDefaultDetector->SetInitialDirectory(SetupPath);
   fileDefaultDatabase->SetInitialDirectory(SetupPath);
   fileDefaultZiegler->SetInitialDirectory(SetupPath);
  }
}

// Delete all custom configuration files.
void ERYAPIXEdialogSetup::OnDefaultDelete( wxCommandEvent& event )
{
  wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Do you really want to erase all Setup Files?\nIt will delete all user modified files,\nand return to the default program settings."), wxT("Confirm ERYA-Bulk Setup Reset"), wxYES_NO | wxNO_DEFAULT | wxICON_WARNING);
  if (dial->ShowModal() == wxID_YES)
  {
   // Call the parent frame
   ERYAPIXEMainFrame *Parent = (ERYAPIXEMainFrame *) GetParent();
   if(Parent->DeleteSetupFile())
   {
    Parent->ProgramQuit(true);
   }
   else
   {
   wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Cannot reset ERYA Bulk.\nRefer to the User Guide about\nhow to perform a manual reset."), wxT("Reset Error!"), wxOK | wxICON_ERROR);
   dial->ShowModal();
   Parent->ProgramQuit(false);
   }
  }
  Close();
}

// Quit any edition, whitout saving it.
void ERYAPIXEdialogSetup::OnDefaultQuit( wxCommandEvent& event )
{
 // Set flag
 ERYAPIXEMainFrame *Parent = (ERYAPIXEMainFrame *) GetParent();
 Parent->ProgramQuit(false);
 Close();
}
