/***************************************************************
 * Name:      ERYAPIXEdialogZieglerParameters.cpp
 * Purpose:   ERYA Ziegler Bragg parameters implementation
 * Author:    Vasco Manteigas (vm.manteigas@campus.fct.utl.pt)
 * Created:   2015-12-01
 * Copyright: Grupo de Reacoes Nucleares - LIBPHYS
 * License:
 **************************************************************/

#include "ERYAPIXEdialogZieglerParameters.h"
#include "ERYAPIXEMainFrame.h"
#include "ERYAPIXEdialogRemark.h"

ERYAPIXEdialogZieglerParameters::ERYAPIXEdialogZieglerParameters( wxWindow* parent )
:
dialogZieglerParameters( parent )
{
   // Call the parent frame
 ERYAPIXEMainFrame *Parent = (ERYAPIXEMainFrame *) GetParent();
 // Start the local detector variable
 ZieglerParameters LocalZiegler;
 // Sync the local data from parent frame to child frame
 Parent->GetZiegler(LocalZiegler);
 Parent->GetSRIM(LocalSRIMTable);
 // And display the content
 LocalZiegler.GetDisplay(textZieglerFunction, choiceZieglerVersion, tableZieglerParameters);
 spinZieglerSRIMCurrentElement->SetValue(1);
 tabZieglerTables->SetSelection(0);
 if(!(LocalSRIMTable.GetDisplay(1,tableZieglerSRIM)))
    tableZieglerSRIM->ClearGrid();
}


void ERYAPIXEdialogZieglerParameters::OnZieglerSelectElement( wxSpinEvent& event )
{
 int AtomicNumber = spinZieglerSRIMCurrentElement->GetValue();
 if(!(LocalSRIMTable.GetDisplay(AtomicNumber,tableZieglerSRIM)))
    tableZieglerSRIM->ClearGrid();
}

void ERYAPIXEdialogZieglerParameters::OnZieglerSelectElementText( wxCommandEvent& event )
{
 int AtomicNumber = spinZieglerSRIMCurrentElement->GetValue();
 if(!(LocalSRIMTable.GetDisplay(AtomicNumber,tableZieglerSRIM)))
    tableZieglerSRIM->ClearGrid();
}

void ERYAPIXEdialogZieglerParameters::OnZieglerSelectElementEnter( wxCommandEvent& event )
{
 int AtomicNumber = spinZieglerSRIMCurrentElement->GetValue();
 if(!(LocalSRIMTable.GetDisplay(AtomicNumber,tableZieglerSRIM)))
    tableZieglerSRIM->ClearGrid();
}

void ERYAPIXEdialogZieglerParameters::OnUpdateZieglerSRIM( wxCommandEvent& event )
{
 int AtomicNumber = spinZieglerSRIMCurrentElement->GetValue();
 if(!(LocalSRIMTable.Append(AtomicNumber,tableZieglerSRIM)))
 {
  wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("The SRIM Database of Element with Atomic Number ") + wxString::Format("%d",AtomicNumber) + wxT("\nalready exists. Replace with the current spreadsheet displayed here ?"), wxT("Replace the Selected SRIM table ?"), wxYES_NO | wxNO_DEFAULT | wxICON_WARNING);
  if (dial->ShowModal() == wxID_YES) // It will erase all contents from the active database.
    {
      if(!(LocalSRIMTable.Replace(AtomicNumber,tableZieglerSRIM)))
      {
        tableZieglerSRIM->ClearGrid();
         wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("General Error: Cannot update SRIM Table Database on memory.") , wxT("ERYA General Error"), wxOK | wxICON_ERROR);
         dial->ShowModal();
      }
    }
 }
}

void ERYAPIXEdialogZieglerParameters::OnEraseZieglerSRIM( wxCommandEvent& event )
{
 wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Delete all SRIM tables Database ?"), wxT("Confirm SRIM erase."), wxYES_NO | wxNO_DEFAULT | wxICON_WARNING);
  if (dial->ShowModal() == wxID_YES) // It will erase all contents from the active database.
    {
      LocalSRIMTable.Clear();
      spinZieglerSRIMCurrentElement->SetValue(1);
      if(!(LocalSRIMTable.GetDisplay(1,tableZieglerSRIM)))
       tableZieglerSRIM->ClearGrid();
    }
}


// Event trigger by user selection of a certain range of grid cells.
void ERYAPIXEdialogZieglerParameters::OnZieglerSelect( wxGridRangeSelectEvent& event )
{
   dataClipboard->Clear();
   wxGrid* CurrentTable;
   if(tabZieglerTables->GetSelection() == 1)
    CurrentTable = tableZieglerParameters;
   else
    CurrentTable = tableZieglerSRIM;
  for (int i=0; i<CurrentTable->GetNumberRows();i++)
  {
   if (CurrentTable->IsInSelection(i,0))
   {
    for (int j=0; j<CurrentTable->GetNumberCols(); j++) // It will copy the lines that was fully selected
    {
     if (CurrentTable->IsInSelection(i,j))
     {
        dataClipboard->Append(CurrentTable->GetCellValue(i,j)); // Copy the cell content and the tab marks
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
void ERYAPIXEdialogZieglerParameters::OnZieglerCopy( wxCommandEvent& event )
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
void ERYAPIXEdialogZieglerParameters::OnZieglerPaste( wxCommandEvent& event )
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
    // Get the correct table
    wxGrid* CurrentTable;
    if(tabZieglerTables->GetSelection() == 1)
     CurrentTable = tableZieglerParameters;
    else
     CurrentTable = tableZieglerSRIM;
    // Initial cell selection
    int InitialRow = CurrentTable->GetGridCursorRow();
    int InitialColumn = CurrentTable->GetGridCursorCol();
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
      for(int i=InitialRow; i<CurrentTable->GetNumberRows(); i++)
      {
        for(int j=InitialColumn; j<CurrentTable->GetNumberCols(); j++)
        {
          if (CellCount < ListSize)
          {
            CurrentTable->SetCellValue(i,j,textblock.GetUnexcluded().Item(CellCount));
            CellCount = CellCount + 1;
          }
        }
      }
    }
 }
}

void ERYAPIXEdialogZieglerParameters::OnZieglerLoad( wxCommandEvent& event )
{
ERYAPIXEMainFrame *Parent = (ERYAPIXEMainFrame *) GetParent();
wxString DefaultDatabaseDirectory;
Parent->GetConfig(DefaultDatabaseDirectory);
wxFileDialog *OpenDialog = new wxFileDialog(this, wxT("Select the desired Ziegler Parameters file"), DefaultDatabaseDirectory, wxEmptyString, wxT("ERYA-Profiling Ziegler(*.epsz)|*.epsz|ASCII file(Legacy ERYA/SRIM)(*.txt)|*.txt|Microsoft Excel Xlsx File(*.xlsx)|*.xlsx"), wxFD_OPEN, wxDefaultPosition);
 if (OpenDialog->ShowModal() == wxID_OK)
 {
 wxString ERYAPIXEZieglerPath = OpenDialog->GetPath();
 wxFileName ZieglerFileName(ERYAPIXEZieglerPath);
 wxString Version = ZieglerFileName.GetExt();
 ZieglerParameters LocalParameters(textZieglerFunction, choiceZieglerVersion, tableZieglerParameters, infoRemark);
 int DefaultMode;
 if (tabZieglerTables->GetSelection()==1)
    DefaultMode = 0;
 else
    DefaultMode = spinZieglerSRIMCurrentElement->GetValue();
 ZieglerFile file(ERYAPIXEZieglerPath,Version,LocalParameters,LocalSRIMTable,DefaultMode);
 if(file.ZieglerFileLoad())
  {
   LocalParameters = file.GetParameters();
   LocalParameters.GetDisplay(textZieglerFunction, choiceZieglerVersion, tableZieglerParameters);
   LocalSRIMTable = file.GetTables();
    if(!(LocalSRIMTable.GetDisplay(spinZieglerSRIMCurrentElement->GetValue(),tableZieglerSRIM)))
        tableZieglerSRIM->ClearGrid();
  }
}
OpenDialog->Close();
}

void ERYAPIXEdialogZieglerParameters::OnZieglerSave( wxCommandEvent& event )
{
ERYAPIXEMainFrame *Parent = (ERYAPIXEMainFrame *) GetParent();
wxString DefaultDatabaseDirectory;
Parent->GetConfig(DefaultDatabaseDirectory);
wxFileDialog *SaveDialog = new wxFileDialog(this, wxT("Save the current Ziegler Parameters as..."), DefaultDatabaseDirectory, wxEmptyString, wxT("ERYA-Profiling Ziegler(*.epsz)|*.epsz|Spreadsheet ASCII File(*.txt)|*.txt|Microsoft Excel Xlsx File(*.xlsx)|*.xlsx"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT, wxDefaultPosition);
 if (SaveDialog->ShowModal() == wxID_OK)
 {
  wxString ERYAPIXEZieglerPath = SaveDialog->GetPath();
  ZieglerParameters LocalZiegler(textZieglerFunction, choiceZieglerVersion,tableZieglerParameters, infoRemark);
  wxFileName ZieglerFileName(ERYAPIXEZieglerPath);
  wxString Version = ZieglerFileName.GetExt();
  int DefaultMode;
  if (tabZieglerTables->GetSelection()==1)
    DefaultMode = 0;
  else
    DefaultMode = spinZieglerSRIMCurrentElement->GetValue();
  ZieglerFile file(ERYAPIXEZieglerPath,Version,LocalZiegler,LocalSRIMTable,DefaultMode);
  if(file.ZieglerFileSave())
  {
   LocalZiegler = file.GetParameters();
   LocalSRIMTable = file.GetTables();
  }
 }
SaveDialog->Close();
}

void ERYAPIXEdialogZieglerParameters::OnZieglerClear( wxCommandEvent& event )
{
 if(tabZieglerTables->GetSelection() == 1)
 {
  tableZieglerParameters->ClearGrid();
  textZieglerFunction->Clear();
  choiceZieglerVersion->SetSelection(0);
 }
 else
 {
  tableZieglerSRIM->ClearGrid();
  textZieglerFunction->Clear();
  choiceZieglerVersion->SetSelection(3);
 }
}

void ERYAPIXEdialogZieglerParameters::OnZieglerOK( wxCommandEvent& event )
{
 // Check the custom function, before validation
 wxString CustomZieglerFunction = textZieglerFunction->GetValue();
 if(choiceZieglerVersion->GetSelection() != 2) //Options that use built-in functions don't need custom macros
 {
  // Call the parent frame
 ERYAPIXEMainFrame *Parent = (ERYAPIXEMainFrame *) GetParent();
 // Get the current local frame data
 ZieglerParameters LocalZiegler(textZieglerFunction,choiceZieglerVersion,tableZieglerParameters,infoRemark);
 // Store the local data from child frame to parent frame
 Parent->SaveZiegler(LocalZiegler);
 Parent->SaveSRIM(LocalSRIMTable);
 // Return to parent frame
 Close();
 }
 else
 {
   AlgebraicFunction LocalZieglerFunction = AlgebraicFunction(CustomZieglerFunction);
   if(LocalZieglerFunction.GetErrorString().Len() == 0)
   {
    if(LocalZieglerFunction.IsFunctionWithVectorDefined()) //Correct function means OK
    {
    // Call the parent frame
    ERYAPIXEMainFrame *Parent = (ERYAPIXEMainFrame *) GetParent();
    // Get the current local frame data
    ZieglerParameters LocalZiegler(textZieglerFunction,choiceZieglerVersion,tableZieglerParameters,infoRemark);
    // Store the local data from child frame to parent frame
    Parent->SaveZiegler(LocalZiegler);
    Parent->SaveSRIM(LocalSRIMTable);
    // Return to parent frame
    Close();
    }
    else // The function is invalid, then the user should fix it.
    {
      wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Declaration Error: A function should have:\nA independent variable declared by \"fxvar\"\n,A dependent variable declared by \"fyvar\"\nAlso requires an array of variables\ndeclared by \"fnvar\"\n") + LocalZieglerFunction.GetErrorString() , wxT("Invalid Ziegler's Stopping-Power Function!"), wxOK | wxICON_ERROR);
      dial->ShowModal();
    }
   }
   else
   {
      wxMessageDialog *dial = new wxMessageDialog(NULL, LocalZieglerFunction.GetErrorString() , wxT("Invalid Ziegler's Stopping-Power Function!"), wxOK | wxICON_ERROR);
      dial->ShowModal();
   }
 }
}


void ERYAPIXEdialogZieglerParameters::OnZieglerCancel( wxCommandEvent& event )
{
Close();
}

void ERYAPIXEdialogZieglerParameters::OnZieglerHelp( wxCommandEvent& event )
{
 ERYAPIXEdialogRemark *remark = new ERYAPIXEdialogRemark(this,infoRemark,3);
 remark->ShowModal();
}
