/***************************************************************
 * Name:      ERYAPIXEdialogDetectorSetup.cpp
 * Purpose:   ERYA Detector Setup implementation functions
 * Author:    Vasco Manteigas (vm.manteigas@campus.fct.utl.pt)
 * Created:   2015-12-01
 * Copyright: Grupo de Reacoes Nucleares - LIBPHYS
 * License:
 **************************************************************/

#include "ERYAPIXEdialogDetectorSetup.h"
#include "ERYAPIXEMainFrame.h"
#include "ERYAPIXEdialogRemark.h"


ERYAPIXEdialogDetectorSetup::ERYAPIXEdialogDetectorSetup( wxWindow* parent )
:
dialogDetectorSetup( parent )
{
  // Call the parent frame
 ERYAPIXEMainFrame *Parent = (ERYAPIXEMainFrame *) GetParent();
 // Start the local detector variable
 DetectorParameters LocalDetector;
 // Sync the local data from parent frame to child frame
 Parent->GetDetector(LocalDetector);
 // And display the content
 LocalDetector.GetDisplay(textFunctionEficiency, tableConstantEficiency);
}

// Event trigger by user selection of a certain range of grid cells
void ERYAPIXEdialogDetectorSetup::OnEficiencySelect( wxGridRangeSelectEvent& event )
{
  dataClipboard->Clear();
  for (int i=0; i<tableConstantEficiency->GetNumberRows();i++)
  {
   if (tableConstantEficiency->IsInSelection(i,0))
   {
    for (int j=0; j<tableConstantEficiency->GetNumberCols(); j++) // It will copy the lines that was fully selected
    {
     if (tableConstantEficiency->IsInSelection(i,j))
     {
        dataClipboard->Append(tableConstantEficiency->GetCellValue(i,j)); // Copy the cell content and the tab marks
        dataClipboard->Append(wxT("\t"));
     }
     else
     {
        dataClipboard->Append(wxT("\t"));
     }
    }
     dataClipboard->Append(wxT("\n")); // A new line after processing a full line of columns
   }
  }
}

// Handles copy data from spreadsheet to the operating system clipboard
void ERYAPIXEdialogDetectorSetup::OnEficiencyCopy( wxCommandEvent& event )
{
  if (wxTheClipboard->Open())
  {
    wxTheClipboard->Clear();
    wxTextDataObject* textClipboard = new wxTextDataObject();
    textClipboard->SetText(*dataClipboard);
    wxTheClipboard->SetData(textClipboard);
    wxTheClipboard->Close();
  }
}

// Handles paste data from the operating system clipboard to the spreadsheet
void ERYAPIXEdialogDetectorSetup::OnEficiencyPaste( wxCommandEvent& event )
{
   if (wxTheClipboard->Open())
   {
    if (wxTheClipboard->IsSupported( wxDF_TEXT ))
    {
        wxTextDataObject* textClipboard = new wxTextDataObject();
        wxTheClipboard->GetData( *textClipboard );
        *dataClipboard = textClipboard->GetText();
    }
    wxTheClipboard->Close();
    // Initial cell selection
    int InitialRow = tableConstantEficiency->GetGridCursorRow();
    int InitialColumn = tableConstantEficiency->GetGridCursorCol();
    // Parsing the string
    wxString error,InputList;
    wxArrayString OutputList;
    InputList.Clear();
    InputList.Append(*dataClipboard);
    TextLineParser textblock(InputList,wxEmptyString);
    int ListSize = textblock.GetUnexcluded().GetCount();
    int CellCount = 0;
    // If anything goes correctly, the program will copy the output list along the table until the list finishes, or the table ends
    if(error == wxT("OK"))
    {
      for(int i=InitialRow; i<tableConstantEficiency->GetNumberRows(); i++)
      {
        for(int j=InitialColumn; j<tableConstantEficiency->GetNumberCols(); j++)
        {
          if (CellCount < ListSize)
          {
            tableConstantEficiency->SetCellValue(i,j,textblock.GetUnexcluded().Item(CellCount));
            CellCount = CellCount + 1;
          }
        }
      }
    }
   }
}


void ERYAPIXEdialogDetectorSetup::OnEficiencyLoad( wxCommandEvent& event )
{
ERYAPIXEMainFrame *Parent = (ERYAPIXEMainFrame *) GetParent();
wxString DefaultDatabaseDirectory;
Parent->GetConfig(DefaultDatabaseDirectory);
wxFileDialog *OpenDialog = new wxFileDialog(this, wxT("Select the desired Detector Profile file"), DefaultDatabaseDirectory, wxEmptyString, wxT("ERYA Detector Profile(*.epsd)|*.epsd|ASCII Table Detector Profile(*.txt)|*.txt|Microsoft Excel Xlsx File(*.xlsx)|*.xlsx"), wxFD_OPEN, wxDefaultPosition);
if (OpenDialog->ShowModal() == wxID_OK)
{
wxString ERYAPIXESetupPath = OpenDialog->GetPath();
wxFileName file(ERYAPIXESetupPath);
DetectorParameters LocalDetector(textFunctionEficiency, tableConstantEficiency, infoRemark);
DetectorFile localfile(ERYAPIXESetupPath,file.GetExt(),LocalDetector);
if (localfile.DetectorFileLoad())
 {
  LocalDetector = localfile.GetParameters();
  if(!(LocalDetector.GetDisplay(textFunctionEficiency,tableConstantEficiency)))
   {
    wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Fatal Error: Invalid format file!"), wxT("Import Error"), wxOK | wxICON_ERROR);
    dial->ShowModal();
   }
 }
 else
 {
   wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Invalid or Unsupported Database File Format!"), wxT("Import Error"), wxOK | wxICON_ERROR);
    dial->ShowModal();
 }
}
OpenDialog->Close();
}

void ERYAPIXEdialogDetectorSetup::OnEficiencySave( wxCommandEvent& event )
{
ERYAPIXEMainFrame *Parent = (ERYAPIXEMainFrame *) GetParent();
wxString DefaultDatabaseDirectory;
Parent->GetConfig(DefaultDatabaseDirectory);
wxFileDialog *SaveDialog = new wxFileDialog(this, wxT("Save the current Detector Profile as..."), DefaultDatabaseDirectory, wxEmptyString,wxT("ERYA Detector Profile(*.epsd)|*.epsd|ASCII Table Detector Profile(*.txt)|*.txt|Microsoft Excel Xlsx File(*.xlsx)|*.xlsx"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT, wxDefaultPosition);
if (SaveDialog->ShowModal() == wxID_OK)
{
wxString ERYAPIXESetupPath = SaveDialog->GetPath();
// If the user choose the legacy version, the custom function are ignored.
wxFileName file(ERYAPIXESetupPath);
DetectorParameters LocalDetector(textFunctionEficiency, tableConstantEficiency, infoRemark);
DetectorFile localfile(ERYAPIXESetupPath,file.GetExt(),LocalDetector);
if (!(localfile.DetectorFileSave()))
 {
   wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Invalid or Unsupported Detector File Format!"), wxT("Export Error"), wxOK | wxICON_ERROR);
    dial->ShowModal();
 }
 else
 {
   LocalDetector = localfile.GetParameters();
 }
}
SaveDialog->Close();
}

void ERYAPIXEdialogDetectorSetup::OnEficiencyClear( wxCommandEvent& event )
{
tableConstantEficiency->ClearGrid();
textFunctionEficiency->Clear();
}

void ERYAPIXEdialogDetectorSetup::OnEficiencyOK( wxCommandEvent& event )
{
 // Check the custom function, before validation
 wxString CustomFunctionEfficiency = textFunctionEficiency->GetValue();
 if(CustomFunctionEfficiency.Trim() == wxEmptyString) // Empty function are fine
 {
  // Call the parent frame
 ERYAPIXEMainFrame *Parent = (ERYAPIXEMainFrame *) GetParent();
 // Get the current local frame data
 DetectorParameters LocalDetector(textFunctionEficiency, tableConstantEficiency, infoRemark);
 // Store the local data from child frame to parent frame
 Parent->SaveDetector(LocalDetector);
 // Return to parent frame
 Close();
 }
 else
 {
  AlgebraicFunction LocalDetectorCustomFunction = AlgebraicFunction(CustomFunctionEfficiency);
   if (LocalDetectorCustomFunction.GetErrorString().Len() == 0)
    {
     if(LocalDetectorCustomFunction.IsFunctionDefined()) //Correct function means OK
     {
      // Call the parent frame
      ERYAPIXEMainFrame *Parent = (ERYAPIXEMainFrame *) GetParent();
      // Get the current local frame data
      DetectorParameters LocalDetector(textFunctionEficiency, tableConstantEficiency, infoRemark);
      // Store the local data from child frame to parent frame
      Parent->SaveDetector(LocalDetector);
      // Return to parent frame
      Close();
     }
     else // The function is invalid, then the user should fix it.
     {
      wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Declaration Error: A function should have:\nA independent variable declared by \"fxvar\"\n,A dependent variable declared by \"fyvar\"\n") + LocalDetectorCustomFunction.GetErrorString() , wxT("Invalid Detector Efficiency Function!"), wxOK | wxICON_ERROR);
      dial->ShowModal();
     }
    }
    else // Algebraic or syntax errors will halt the fit procedure
    {
     wxMessageDialog *dial = new wxMessageDialog(NULL, LocalDetectorCustomFunction.GetErrorString() , wxT("Invalid Detector Efficiency Function!"), wxOK | wxICON_ERROR);
     dial->ShowModal();
    }
 }
}

void ERYAPIXEdialogDetectorSetup::OnEficiencyCancel( wxCommandEvent& event )
{
 Close();
}

void ERYAPIXEdialogDetectorSetup::OnEficiencyHelp( wxCommandEvent& event )
{
 ERYAPIXEdialogRemark *remark = new ERYAPIXEdialogRemark(this,infoRemark,2);
 remark->ShowModal();
}
