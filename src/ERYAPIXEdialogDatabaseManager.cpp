/***************************************************************
 * Name:      ERYAPIXEdialogDatabaseManager.cpp
 * Purpose:   ERYA Database Manager implementation
 * Author:    Vasco Manteigas (vm.manteigas@campus.fct.utl.pt)
 * Created:   2015-12-01
 * Copyright: Grupo de Reacoes Nucleares - LIBPHYS
 * License:
 **************************************************************/

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif //WX_PRECOMP

#include "ERYAPIXEdialogAddDatabase.h"
#include "ERYAPIXEdialogDatabaseManager.h"
#include "ERYAPIXEMainFrame.h"
#include "ERYAPIXEdialogRemark.h"
#include "PlotLibrary.h"
#include "FileLibrary.h"

ERYAPIXEdialogDatabaseManager::ERYAPIXEdialogDatabaseManager( wxWindow* parent )
:
dialogDatabaseManager( parent )
{
 // Call the parent frame
 ERYAPIXEMainFrame *Parent = (ERYAPIXEMainFrame *) GetParent();
 // Start the local database variable
 MainDatabase.Clear();
 // Sync the local data from parent frame to child frame
 Parent->GetDatabase(MainDatabase);
 // If the database is not empty, load the table.
 if (MainDatabase.GetCount()>0)
 {
  MainDatabase.ResetElementGammaMenu(choiceElementName,choiceGammaPeak);
  choiceElementName->SetSelection(0);
  choiceGammaPeak->SetSelection(0);
  if(!(MainDatabase.FindGetAllElementInfo(choiceElementName->GetString(0),choiceGammaPeak->GetString(0), choiceElementName, choiceGammaPeak, textAtomicNumber, textAbundance, textAtomicMass, textIsotopicMass, tableElementViewer, renderElementViewer)))
           {
           wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("The Element Database consistency are damaged. Please, reload with a new one."), wxT("Database is Corrupted!"), wxOK | wxICON_ERROR);
           dial->ShowModal();
           }
 }
}

void ERYAPIXEdialogDatabaseManager::OnElementName( wxCommandEvent& event )
{
 int i = choiceElementName->GetSelection();
 choiceGammaPeak->SetSelection(0);
 if(!(MainDatabase.FindGetAllElementInfo(choiceElementName->GetString(i),choiceGammaPeak->GetString(0), choiceElementName, choiceGammaPeak, textAtomicNumber, textAbundance, textAtomicMass, textIsotopicMass, tableElementViewer, renderElementViewer)))
           {
           wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("The Element Database consistency are damaged. Please, reload with a new one."), wxT("Database is Corrupted!"), wxOK | wxICON_ERROR);
           dial->ShowModal();
           }
}

void ERYAPIXEdialogDatabaseManager::OnGammaPeak( wxCommandEvent& event )
{
 int i = choiceElementName->GetSelection();
 int j = choiceGammaPeak->GetSelection();
 if(!(MainDatabase.FindGetAllElementInfo(choiceElementName->GetString(i),choiceGammaPeak->GetString(j), choiceElementName, choiceGammaPeak, textAtomicNumber, textAbundance, textAtomicMass, textIsotopicMass, tableElementViewer, renderElementViewer)))
           {
           wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("The Element Database consistency are damaged. Please, reload with a new one."), wxT("Database is Corrupted!"), wxOK | wxICON_ERROR);
           dial->ShowModal();
           }
}

void ERYAPIXEdialogDatabaseManager::OnDatabaseAdd( wxCommandEvent& event )
{
   ChangeElement = false;
   ERYAPIXEdialogAddDatabase* editor = new ERYAPIXEdialogAddDatabase(this);
   editor->ShowModal();
   if(WriteElement)
   {
    if(MainDatabase.NewElement(EditElement,false)) // By default it adds a new element
         {
          if(!(MainDatabase.FindGetAllElementInfo(EditElement.GetElement(),EditElement.GetGamma(), choiceElementName, choiceGammaPeak, textAtomicNumber, textAbundance, textAtomicMass, textIsotopicMass, tableElementViewer, renderElementViewer)))
           {
           wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("The Element Database consistency are damaged. Please, reload with a new one."), wxT("Database is Corrupted!"), wxOK | wxICON_ERROR);
           dial->ShowModal();
           }
         }
         else
         {
          wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Cannot add new Element \"") + EditElement.GetElement() + wxT("\", \"") + EditElement.GetGamma() + wxT("\" keV\nIt already exists on Database!"), wxT("Database Error!"), wxOK | wxICON_ERROR);
          dial->ShowModal();
         }
   }
}

void ERYAPIXEdialogDatabaseManager::OnDatabaseEdit( wxCommandEvent& event )
{
  // Get the selected element, and pass the Element to the Element Editor
  int i = choiceElementName->GetSelection();
  int j = choiceGammaPeak->GetSelection();
  int adress = MainDatabase.FindElement(choiceElementName->GetString(i), choiceGammaPeak->GetString(j));
  EditElement = MainDatabase.Item(adress);
   ChangeElement = true;
   ERYAPIXEdialogAddDatabase* editor = new ERYAPIXEdialogAddDatabase(this);
   editor->ShowModal();
   // If the Element are updated, then store the new element.
  if(WriteElement)
   {
     if(MainDatabase.NewElement(EditElement,true)) // By default it renames an actual element
         {
          if(!(MainDatabase.FindGetAllElementInfo(EditElement.GetElement(),EditElement.GetGamma(), choiceElementName, choiceGammaPeak, textAtomicNumber, textAbundance, textAtomicMass, textIsotopicMass, tableElementViewer, renderElementViewer)))
           {
           wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("The Element Database consistency are damaged. Please, reload with a new one."), wxT("Database is Corrupted!"), wxOK | wxICON_ERROR);
           dial->ShowModal();
           }
         }
         else
         {
          wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Cannot replace the Element \"") + EditElement.GetElement() + wxT("\", \"") + EditElement.GetGamma() + wxT("\" keV\nIt already exists on Database!"), wxT("Database Error!"), wxOK | wxICON_ERROR);
          dial->ShowModal();
         }
   }
}

void ERYAPIXEdialogDatabaseManager::OnDatabaseRemove( wxCommandEvent& event )
{
 wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Delete the displayed element of the database?"), wxT("Confirm delectation."), wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
 if (dial->ShowModal() == wxID_YES)
 {
  int i = choiceElementName->GetSelection();
  int j = choiceGammaPeak->GetSelection();
  if(MainDatabase.RemoveElement(choiceElementName->GetString(i),choiceGammaPeak->GetString(j), choiceElementName, choiceGammaPeak, textAtomicNumber, textAbundance, textAtomicMass, textIsotopicMass, tableElementViewer, renderElementViewer))
   {
     if(!(MainDatabase.FindGetAllElementInfo(choiceElementName->GetString(0),choiceGammaPeak->GetString(0), choiceElementName, choiceGammaPeak, textAtomicNumber, textAbundance, textAtomicMass, textIsotopicMass, tableElementViewer, renderElementViewer)))
           {
           wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("The Element Database consistency are damaged. Please, reload with a new one."), wxT("Database is Corrupted!"), wxOK | wxICON_ERROR);
           dial->ShowModal();
           }
   }
  else
  {
    wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("The Element Database consistency are damaged. Please, reload with a new one."), wxT("Database is Corrupted!"), wxOK | wxICON_ERROR);
    dial->ShowModal();
  }
 }
}

void ERYAPIXEdialogDatabaseManager::OnDatabaseClear( wxCommandEvent& event )
{
  wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Delete all the contents of the current database?"), wxT("Confirm database erase."), wxYES_NO | wxNO_DEFAULT | wxICON_WARNING);
  if (dial->ShowModal() == wxID_YES) // It will erase all contents from the active database.
  {
   MainDatabase.Clear();
   choiceElementName->Clear();
   choiceGammaPeak->Clear();
   textAbundance->Clear();
   textAtomicMass->Clear();
   textAtomicNumber->Clear();
   textIsotopicMass->Clear();
   tableElementViewer->ClearGrid();
   renderElementViewer->DelAllLayers(false,true);
   infoRemark.Clear();
  }
}

void ERYAPIXEdialogDatabaseManager::OnDatabaseLoad( wxCommandEvent& event )
{
ERYAPIXEMainFrame *Parent = (ERYAPIXEMainFrame *) GetParent();
wxString DefaultDatabaseDirectory;
Parent->GetConfig(DefaultDatabaseDirectory);
wxFileDialog *OpenDialog = new wxFileDialog(this, wxT("Select a ERYA Database to open"), DefaultDatabaseDirectory, wxEmptyString, wxT("ERYA database (*.epd)|*.epd|Legacy ERYA database (*.txt)|*.txt"), wxFD_OPEN, wxDefaultPosition);
if (OpenDialog->ShowModal() == wxID_OK)
{
MainDatabaseFile = OpenDialog->GetPath();
wxFileName DatabaseFileName(MainDatabaseFile);
wxString DatabaseVersion = DatabaseFileName.GetExt();
 DatabaseFile file(MainDatabaseFile,DatabaseVersion,MainDatabase);
 if(file.ERYAPIXEDatabaseFileLoad())
  {
   MainDatabase = file.GetDatabase();
   if (MainDatabase.GetCount()>0)
   {
   MainDatabase.ResetElementGammaMenu(choiceElementName,choiceGammaPeak);
   choiceElementName->SetSelection(0);
   choiceGammaPeak->SetSelection(0);
   if(!(MainDatabase.FindGetAllElementInfo(choiceElementName->GetString(0),choiceGammaPeak->GetString(0), choiceElementName, choiceGammaPeak, textAtomicNumber, textAbundance, textAtomicMass, textIsotopicMass, tableElementViewer, renderElementViewer)))
           {
           wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("The Element Database consistency are damaged. Please, reload with a new one."), wxT("Database is Corrupted!"), wxOK | wxICON_ERROR);
           dial->ShowModal();
           }
   }
  }
  else
  {
    wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("This Database File Format is not compatible with ERYA"), wxT("Unsupported Database Format"), wxOK | wxICON_ERROR);
    dial->ShowModal();
  }
}
// Clean up after ourselves
OpenDialog->Close();
}

void ERYAPIXEdialogDatabaseManager::OnDatabaseSave( wxCommandEvent& event )
{
ERYAPIXEMainFrame *Parent = (ERYAPIXEMainFrame *) GetParent();
wxString DefaultDatabaseDirectory;
Parent->GetConfig(DefaultDatabaseDirectory);
wxFileDialog *SaveDialog = new wxFileDialog(this, wxT("Save ERYA Database As..."), DefaultDatabaseDirectory, wxEmptyString,wxT("ERYA database (*.epd)|*.epd|LabView ERYA Source database (*.txt)|*.txt"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT, wxDefaultPosition);
if (SaveDialog->ShowModal() == wxID_OK)
{
MainDatabaseFile = SaveDialog->GetPath();
wxFileName DatabaseFileName(MainDatabaseFile);
wxString DatabaseVersion = DatabaseFileName.GetExt();
 DatabaseFile file(MainDatabaseFile,DatabaseVersion,MainDatabase);
 if(file.ERYAPIXEDatabaseFileSave())
  {
   MainDatabase = file.GetDatabase();
   // Store the saved database to the parent database
 Parent->SaveDatabase(MainDatabase);
 Parent->RefreshDatabase(true);
  }
  else
  {
    wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("This Database File Format is not compatible with ERYA"), wxT("Unsupported Database Format"), wxOK | wxICON_ERROR);
    dial->ShowModal();
    // Send refresh flag to main screen
   ERYAPIXEMainFrame *Parent = (ERYAPIXEMainFrame *) GetParent();
   Parent->RefreshDatabase(false);
  }
}
// Clean up after ourselves
SaveDialog->Close();
 Close();
}

void ERYAPIXEdialogDatabaseManager::OnDatabaseCancel( wxCommandEvent& event )
{
 // Send refresh flag to main screen
 ERYAPIXEMainFrame *Parent = (ERYAPIXEMainFrame *) GetParent();
 Parent->RefreshDatabase(false);
 Close();
}

void ERYAPIXEdialogDatabaseManager::OnDatabaseHelp( wxCommandEvent& event )
{
 infoRemark = MainDatabase.GetInfo();
 ERYAPIXEdialogRemark *remark = new ERYAPIXEdialogRemark(this,infoRemark,1);
 remark->ShowModal();
 MainDatabase.SetInfo(infoRemark);
}
