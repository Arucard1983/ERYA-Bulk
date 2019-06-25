/***************************************************************
 * Name:      ERYAPIXEdialogAddDatabase.cpp
 * Purpose:   ERYA Database Editor implementation
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
#include "ERYAPIXEdialogR33DataImport.h"
#include "ERYAPIXEdialogXlsxDataImport.h"
#include "ERYAPIXEdialogRemark.h"
#include "R33Library.h"
#include "DatabaseLibrary.h"
#include "ParserLibrary.h"
#include "FileLibrary.h"
#include "XlsxLibrary.h"


ERYAPIXEdialogAddDatabase::ERYAPIXEdialogAddDatabase( wxWindow* parent)
:
dialogAddDatabase( parent )
{
 ERYAPIXEdialogDatabaseManager *Parent = (ERYAPIXEdialogDatabaseManager *) GetParent();
 bool EditNewElement;
 Parent->ModifyElement(EditNewElement);
 if(EditNewElement)
 {
   ElementDatabase OpenActualElement;
   Parent->GetElement(OpenActualElement);
   OpenActualElement.GetAllElementInfo(textEditElement,textEditGamma,textEditNumber,textEditAbundance,textEditAtomic,textEditIsotopic,infoRemark,tableDataEditor);
 }
}

// Event trigger by user selection of a certain range of grid cells
void ERYAPIXEdialogAddDatabase::OnDatabaseSelect( wxGridRangeSelectEvent& event )
{
  dataClipboard->Clear();
  for (int i=0; i<tableDataEditor->GetNumberRows();i++)
  {
   if (tableDataEditor->IsInSelection(i,0))
   {
    for (int j=0; j<tableDataEditor->GetNumberCols(); j++) // It will copy the lines that was fully selected
    {
     if (tableDataEditor->IsInSelection(i,j))
     {
        dataClipboard->Append(tableDataEditor->GetCellValue(i,j)); // Copy the cell content and the tab marks
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
void ERYAPIXEdialogAddDatabase::OnDatabaseCopy( wxCommandEvent& event )
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
void ERYAPIXEdialogAddDatabase::OnDatabasePaste( wxCommandEvent& event )
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
    int InitialRow = tableDataEditor->GetGridCursorRow();
    int InitialColumn = tableDataEditor->GetGridCursorCol();
    // Parsing the string
    wxString InputList;
    InputList.Append(*dataClipboard);
    TextLineParser textblock(InputList,wxEmptyString);
    int ListSize = textblock.GetUnexcluded().GetCount();
    int CellCount = 0;
    // If anything goes correctly, the program will copy the output list along the table until the list finishes, or the table ends
    if(ListSize > 0)
    {
      for(int i=InitialRow; i<tableDataEditor->GetNumberRows(); i++)
      {
        for(int j=InitialColumn; j<tableDataEditor->GetNumberCols(); j++)
        {
          if (CellCount < ListSize)
          {
            tableDataEditor->SetCellValue(i,j,textblock.GetUnexcluded().Item(CellCount));
            CellCount = CellCount + 1;
          }
        }
      }
    }
 }
}

void ERYAPIXEdialogAddDatabase::OnImportR33( wxCommandEvent& event )
{
wxFileDialog *OpenDialog = new wxFileDialog(this, wxT("Select an IBANDL file to open"), wxEmptyString, wxEmptyString, wxT("IBANDL file (*.r33)|*.r33|ASCII Source Table file(*.txt)|*.txt|ASCII Source Table file (*.dat)|*.dat|Microsoft Excel Xlsx file (*.xlsx)|*.xlsx"), wxFD_OPEN, wxDefaultPosition);
if (OpenDialog->ShowModal() == wxID_OK) // if the user click "Open" instead of "cancel"
{
wxString CurrentIBANDLFilePath = OpenDialog->GetPath(); // Get absolute path of selected file
wxFileName file(CurrentIBANDLFilePath);
if(file.GetExt() == wxT("r33"))
{
 R33File openfile(CurrentIBANDLFilePath);
 if(openfile.IBANDLFileLoad(tableDataEditor,textEditElement,textEditGamma,textEditNumber,textEditAbundance,textEditAtomic,textEditIsotopic,infoRemark))
 {
 wxMessageDialog *info = new wxMessageDialog(NULL, wxT("Cross-Section of selected IBANDL file,\nalong some additional data,\nwas successfully imported.") , wxT("Import Successful!"), wxOK);
 info->ShowModal();
 }
}
else if (file.GetExt() == wxT("xlsx"))
{
 XlsxFile XlsxSigmaFile(CurrentIBANDLFilePath);
 if(XlsxSigmaFile.LoadFile())
 {
   // Opens an additional dialog
   ERYAPIXEdialogXlsxDataImport* datapanel = new ERYAPIXEdialogXlsxDataImport(this);
   datapanel->ShowModal();
   // Set the conversion factor
   double UnitFactor;
   if (Units == 0)
    UnitFactor = 1;
   else
    UnitFactor = 1000;
   if (Angles == 0)
    UnitFactor = UnitFactor * (8 * std::acos(0.0));
   else
    UnitFactor =  UnitFactor * 1.0;
   // Process the imported data
   int NumberSigmaRows,NumberSigmaCols,EnergyColumn,EnergyErrorColumn,SigmaColumn,SigmaErrorColumn,RequiredNumberColumns;
   TableMatrix SigmaMatrix = XlsxSigmaFile.GetTableMatrix();
   SigmaMatrix.GetRealMatrixSize(NumberSigmaRows,NumberSigmaCols);
   tableDataEditor->ClearGrid();
   // Set the default import profiles
   if(Profile == 2) //Four Columns
   {
    EnergyColumn = 0;
    EnergyErrorColumn = 1;
    SigmaColumn = 2;
    SigmaErrorColumn = 3;
    RequiredNumberColumns = 4;
   }
   else if (Profile == 1) //Two Columns
   {
    EnergyColumn = 0;
    EnergyErrorColumn = -1;
    SigmaColumn = 1;
    SigmaErrorColumn = -1;
    RequiredNumberColumns = 2;
   }
   else
   {
    if (NumberSigmaCols == 4 ) //Make a guess by counting the number of columns
    {
     EnergyColumn = 0;
     EnergyErrorColumn = 1;
     SigmaColumn = 2;
     SigmaErrorColumn = 3;
     RequiredNumberColumns = 4;
    }
    else if (NumberSigmaCols == 2 ) //Make a guess by counting the number of columns
    {
     EnergyColumn = 0;
     EnergyErrorColumn = -1;
     SigmaColumn = 1;
     SigmaErrorColumn = -1;
     RequiredNumberColumns = 2;
    }
   }
   if(NumberSigmaCols >= RequiredNumberColumns && NumberSigmaCols > 1) // Check the validity of choice
   {
     int CurrentTableRow = 0;
     for(int i=0; i<NumberSigmaRows; i++) // Read the table, line-by-line
     {
       // Extract the entire row and classify
       wxArrayString CurrentRowData;
       bool IsValidRow = true;
       for(int j=0; j<RequiredNumberColumns; j++)
       {
        int TypeElement;
        wxString TempData = SigmaMatrix.GetTableMatrixValueAt(i,j,TypeElement);
        CurrentRowData.Add(TempData);
        if(TypeElement != 1 && IsValidRow) //Ignore current line
         IsValidRow = false;
       }
       if(IsValidRow) //Non-numerical lines will be discarder
       {
        // Write the current line on the table
         tableDataEditor->SetCellValue(CurrentTableRow,0,wxT("0"));
         tableDataEditor->SetCellValue(CurrentTableRow,1,wxT("0"));
         tableDataEditor->SetCellValue(CurrentTableRow,2,wxT("0"));
         tableDataEditor->SetCellValue(CurrentTableRow,3,wxT("0"));
        // Overwrite Energy value
        wxString c0 = CurrentRowData.Item(EnergyColumn);
        tableDataEditor->SetCellValue(CurrentTableRow,0,c0);
        // Overwrite Sigma Value
        wxString c1 = CurrentRowData.Item(SigmaColumn);
        double temp;
        c1.ToDouble(&temp);
        c1 = wxString::Format("%f",temp * UnitFactor);
        tableDataEditor->SetCellValue(CurrentTableRow,2,c1);
        // If aplicable, overwrite the Energy Error
        if (EnergyErrorColumn >= 0)
        {
         wxString c2 = CurrentRowData.Item(EnergyErrorColumn);
         tableDataEditor->SetCellValue(CurrentTableRow,1,c2);
        }
        // If aplicable, overwrite the Sigma Error
        if (SigmaErrorColumn >= 0)
        {
         wxString c3 = CurrentRowData.Item(SigmaErrorColumn);
         double ntemp;
         c3.ToDouble(&ntemp);
         c3 = wxString::Format("%f",ntemp * UnitFactor);
         tableDataEditor->SetCellValue(CurrentTableRow,3,c3);
        }
         CurrentTableRow = CurrentTableRow + 1;
       }
     }
   }
   else
   {
    wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Cannot process this source Xlsx Excel file!\nExpect a full numerical table with at least two columns!"), wxT("Error: Invalid Excel file"), wxOK | wxICON_ERROR);
    dial->ShowModal();
    tableDataEditor->ClearGrid();
   }
  infoRemark = wxT("Element's original data extracted from an Excel file.");
 }
 else
 {
   wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Corrupted or Incompatible Excel File Format!\nERYA only supports very basic Excel Xlsx files."), wxT("Error: Invalid Excel file"), wxOK | wxICON_ERROR);
   dial->ShowModal();
 }
}
else
{
  // Opens an additional dialog
 ERYAPIXEdialogR33DataImport* datapanel = new ERYAPIXEdialogR33DataImport(this);
 datapanel->ShowModal();
 ITNFile openfile(CurrentIBANDLFilePath,Units,Angles,IgnoreLine);
 if(openfile.ITNFileLoad(tableDataEditor,textEditElement,textEditGamma,textEditNumber,textEditAbundance,textEditAtomic,textEditIsotopic,infoRemark))
 {
 wxMessageDialog *info = new wxMessageDialog(NULL, wxT("Cross-Section of selected ASCII file,\nwas successfully imported.") , wxT("Import Successful!"), wxOK);
 info->ShowModal();
 }
}
}
OpenDialog->Close(); // Clean up after ourselve
}

void ERYAPIXEdialogAddDatabase::OnExportR33( wxCommandEvent& event )
{
wxFileDialog *SaveDialog = new wxFileDialog(this, wxT("Export current element table as IBANDL file"), wxEmptyString, wxEmptyString,wxT("IBANDL file (*.r33)|*.r33|Text Excitation Table file (*.txt)|*.txt|Text Excitation Table file (*.dat)|*.dat|Excel Xlsx file (*.xlsx)|*.xlsx"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT, wxDefaultPosition);
// Saving file
if (SaveDialog->ShowModal() == wxID_OK) // If the user clicked "OK"
{
 // Get the current local frame data
 ElementDatabase TestElement(textEditElement,textEditGamma,textEditNumber,textEditAbundance,textEditAtomic,textEditIsotopic,infoRemark,tableDataEditor);
// Check if their contents are valid
if(!(TestElement.CheckElement()))
{
   wxMessageDialog *info = new wxMessageDialog(NULL, wxT("Element Names should be named exclusively with numbers and letters from \"A\"|\"a\" to \"z\"|\"z\".\nThe underscore \"_\" are acceptable, thought.\nOther fields should be exclusively numerical ones.\nPlease rectify the invalid data, and try again.") , wxT("Invalid Data Record"), wxOK|wxICON_ERROR);
   info->ShowModal();
   return;
}
else
{
wxString CurrentIBANDLFilePath = SaveDialog->GetPath();
// set the path of our current document to the file the user choose to save under
wxFileName file(CurrentIBANDLFilePath);
if(file.GetExt() == wxT("r33"))
{
 R33File savefile(CurrentIBANDLFilePath);
 if(savefile.IBANDLFileSave(tableDataEditor,textEditElement,textEditGamma,textEditNumber,textEditAbundance,textEditAtomic,textEditIsotopic,infoRemark)) // Save the IBANDL file to the built-in data editor
 {
 wxMessageDialog *info = new wxMessageDialog(NULL, wxT("Current Element exported to file successfully!") , wxT("Export Successful!"), wxOK);
 info->ShowModal();
 }
}
else if (file.GetExt() == wxT("xlsx"))
{
   // Define global options
  int NumberTableCols,NumberTableRows;
  NumberTableRows = tableDataEditor->GetNumberRows();
  for(int i=0; i<tableDataEditor->GetNumberRows(); i++)
  {
    wxString temp = tableDataEditor->GetCellValue(i,0);
    if(temp.Len() == 0)
    {
      NumberTableRows = i;
      break;
    }
  }
  wxArrayString XlsxProfilesOutput;
  XlsxProfilesOutput.Add(wxT("Two Columns, Units in milibarn.keV"));
  XlsxProfilesOutput.Add(wxT("Four Columns, Units in milibarn.keV"));
  wxSingleChoiceDialog *dial = new wxSingleChoiceDialog(NULL, wxT("Selected from the options below, the output format:"), wxT("Select an Output Mode"),XlsxProfilesOutput);
  if (dial->ShowModal() == wxID_OK)
  {
   if(dial->GetSelection() == 0)
     NumberTableCols = 2;
   else
     NumberTableCols = 4;
     // Create the intermediary matrix table
  TableMatrix TransferSigmaTable(NumberTableRows,NumberTableCols);
  if(NumberTableCols == 4)
  {
    TransferSigmaTable.Add(TableNode(wxT("Energy (keV)")));
    TransferSigmaTable.Add(TableNode(wxT("Energy Error (keV)")));
    TransferSigmaTable.Add(TableNode(wxT("Cross-Section (mbarn.keV)")));
    TransferSigmaTable.Add(TableNode(wxT("Cross-Section Error(mbarn.keV)")));
  }
  else
  {
    TransferSigmaTable.Add(TableNode(wxT("Energy (keV)")));
    TransferSigmaTable.Add(TableNode(wxT("Cross-Section (mbarn.keV)")));
  }
  for(int p=0; p<NumberTableRows; p++)
  {
    for(int q=0; q<NumberTableCols;q++)
    {
      if(NumberTableCols == 2) //Two column case, which require special conversions
      {
        if(q == 0)
        {
          wxString temp = tableDataEditor->GetCellValue(p,0);
          double value,sigma;
          if(temp.ToDouble(&value))
              sigma = value;
          else
              sigma = 0;
          TransferSigmaTable.Add(TableNode(wxString::Format("%f",sigma),1));
        }
        else
        {
          wxString temp = tableDataEditor->GetCellValue(p,2);
          double value,sigma;
          if(temp.ToDouble(&value))
              sigma = value;
          else
              sigma = 0;
          TransferSigmaTable.Add(TableNode(wxString::Format("%f",sigma),1));
        }
      }
      else // Four column case, where just copy the results
      {
        TransferSigmaTable.Add(TableNode(tableDataEditor->GetCellValue(p,q),1));
      }
    }
  }
  // Save the table while call the Xlsx file constructor
  XlsxFile XlsxSigmaFile(CurrentIBANDLFilePath,TransferSigmaTable,wxT("A1"));
  XlsxSigmaFile.WriteFile();
  }

}
else
{
 ITNFile savefile(CurrentIBANDLFilePath);
 if(savefile.ITNFileSave(tableDataEditor,textEditElement,textEditGamma,textEditNumber,textEditAbundance,textEditAtomic,textEditIsotopic,infoRemark))
 {
 wxMessageDialog *info = new wxMessageDialog(NULL, wxT("Current Element exported to file successfully!") , wxT("Export Successful!"), wxOK);
 info->ShowModal();
 }
}
}
}
SaveDialog->Close(); // Clean up after ourselves
}

void ERYAPIXEdialogAddDatabase::OnEditClear( wxCommandEvent& event )
{
tableDataEditor->ClearGrid();
textEditElement->Clear();
textEditGamma->Clear();
textEditNumber->Clear();
textEditAbundance->Clear();
textEditAtomic->Clear();
textEditIsotopic->Clear();
infoRemark.Clear();
}

void ERYAPIXEdialogAddDatabase::OnEditSave( wxCommandEvent& event )
{
 if(textEditElement->GetValue() != wxEmptyString && textEditGamma->GetValue() != wxEmptyString  && textEditNumber->GetValue() != wxEmptyString && textEditAbundance->GetValue() != wxEmptyString && textEditAtomic->GetValue() != wxEmptyString && textEditIsotopic->GetValue() != wxEmptyString)
 {
  // Get the current local frame data
 ElementDatabase EditElement(textEditElement,textEditGamma,textEditNumber,textEditAbundance,textEditAtomic,textEditIsotopic,infoRemark,tableDataEditor);
  // Check if their contents are valid
  if(!(EditElement.CheckElement()))
  {
   wxMessageDialog *info = new wxMessageDialog(NULL, wxT("Element Names should be named exclusively with numbers and letters from \"A\"|\"a\" to \"z\"|\"z\".\nThe underscore \"_\" are acceptable, thought.\nOther fields should be exclusively numerical ones.\nPlease rectify the invalid data, and try again.") , wxT("Invalid Data Record"), wxOK|wxICON_ERROR);
   info->ShowModal();
   return;
  }
  else
  {
  // Call the parent frame
  ERYAPIXEdialogDatabaseManager *Parent = (ERYAPIXEdialogDatabaseManager *) GetParent();
  // Store the local data from child frame to parent frame
  Parent->StoreElement(EditElement);
  // Return to parent frame
  Parent->SaveElement(true);
  Close();
  }

 }
else
 {
  wxMessageDialog *info = new wxMessageDialog(NULL, wxT("Not all obligatory parameters are filled.\nPlease fill all necessary data, and try again!") , wxT("Incomplete Record."), wxOK|wxICON_EXCLAMATION);
  info->ShowModal();
 }
}

void ERYAPIXEdialogAddDatabase::OnEditCancel( wxCommandEvent& event )
{
 // Call the parent frame
ERYAPIXEdialogDatabaseManager *Parent = (ERYAPIXEdialogDatabaseManager *) GetParent();
Parent->SaveElement(false);
Close();
}

void ERYAPIXEdialogAddDatabase::OnEditHelp( wxCommandEvent& event )
{
 ERYAPIXEdialogRemark *remark = new ERYAPIXEdialogRemark(this,infoRemark,0);
 remark->ShowModal();
}
