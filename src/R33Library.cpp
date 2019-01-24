/***************************************************************
 * Name:      R33Library.cpp
 * Purpose:   IBANDL file format (*.r33), ITN text file format (*.dat|*.txt), LabView DataLog binary file (*.txt:DTLG) library implementation
 * Author:    Vasco Manteigas (vm.manteigas@campus.fct.utl.pt)
 * Created:   2016-10-01
 * Copyright: Grupo de Reacoes Nucleares - LIBPHYS
 * License:
 **************************************************************/

#include "R33Library.h"

// R33File class implementation
bool R33File::IBANDLFileLoad(wxGrid* &tableDataEditor, wxTextCtrl* &textEditElement, wxTextCtrl* &textEditGamma, wxTextCtrl* &textEditNumber, wxTextCtrl* &textEditAbundance, wxTextCtrl* &textEditAtomic, wxTextCtrl* &textEditIsotopic)
{
 // Ask the user to clear the element parameters, while loads the original IBANDL file.
 wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Delete all Element Parameters?"), wxT("Clear Current Element Data."), wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
 if (dial->ShowModal() == wxID_YES)
 {
  textEditElement->Clear();
  textEditGamma->Clear();
  textEditNumber->Clear();
  textEditAbundance->Clear();
  textEditAtomic->Clear();
  textEditIsotopic->Clear();
 }
 else // Otherwise, only empty parameters are cleared.
 {
 if (textEditElement->GetValue() == wxEmptyString)
  textEditElement->Clear();
 if (textEditGamma->GetValue() == wxEmptyString)
  textEditGamma->Clear();
 if (textEditNumber->GetValue() == wxEmptyString)
  textEditNumber->Clear();
 if (textEditAbundance->GetValue() == wxEmptyString)
  textEditAbundance->Clear();
 if (textEditAtomic->GetValue() == wxEmptyString)
  textEditAtomic->Clear();
 if (textEditIsotopic->GetValue() == wxEmptyString)
  textEditIsotopic->Clear();
 }
 tableDataEditor->ClearGrid(); // But the main speadsheet are always cleared.

 // Our IBANDL file parsing is a very simple one, and only dedicated to search and find the relevant values.
 // A more robust implementation would require to create several classes to handle the diffrent parameters and data blocks.
 // Since our recipe works, it just begin with the file loading
 wxTextFile database(R33FileName);
 database.Open();
 // Find Data block and CommentBlock delimiters
 int beginDataBlock, endDataBlock, commentBlock;
 wxString SigmaUnits = wxT("mb"); //default value
 for(int k=0; k<database.GetLineCount(); k++)
  {
    if(database.GetLine(k).Trim().Len() == 0)
    {
      commentBlock = k + 1;
    }
    else if(database.GetLine(k).Trim() == wxT("Data:"))
    {
      beginDataBlock = k + 1;
    }
    else if(database.GetLine(k).Trim() == wxT("EndData:"))
    {
      endDataBlock = k;
    }
  }
   // Process the first part of file, and extract the relevant information
 for(int i=commentBlock; i<beginDataBlock; i++)
  {
  // Prepare the current line, and select the token separators
  wxString CurrentParameterLine = database.GetLine(i);
  wxArrayString CurrentSeparators;
  CurrentSeparators.Add(wxT(":"));
  CurrentSeparators.Add(wxT("("));
  CurrentSeparators.Add(wxT(")"));
  CurrentSeparators.Add(wxT(","));
  CurrentSeparators.Add(wxT("+"));
  TextLineParser CurrentLine(CurrentParameterLine,CurrentSeparators);
  if(CurrentLine.GetUnexcluded().GetCount()>0)
  {
   if (CurrentLine.GetUnexcluded().Item(0) == wxT("Zeds")) // Process the line Zeds: Qa,QX,QY,Qb
   {
    if (CurrentLine.GetUnexcluded().GetCount()>1)
    {
     textEditNumber->SetValue(CurrentLine.GetUnexcluded().Item(2));
    }
   }
   else if (CurrentLine.GetUnexcluded().Item(0) == wxT("Masses")) // Process the line Masses: ma,mX,mY,mb
   {
    if (CurrentLine.GetUnexcluded().GetCount()>1)
    {
     textEditAtomic->SetValue(CurrentLine.GetUnexcluded().Item(2));
    }
   }
   else if (CurrentLine.GetUnexcluded().Item(0) == wxT("Egamma")) // Process the line Egamma: E0+E1+...
   {
    if (CurrentLine.GetUnexcluded().GetCount()>0)
     {
      textEditGamma->SetValue(CurrentLine.GetUnexcluded().Item(1));
     }
   }
   else if (CurrentLine.GetUnexcluded().Item(0) == wxT("Reaction")) // Process the line Reaction : X(a,b)Y
   {
    if (CurrentLine.GetUnexcluded().GetCount()>1)
    {
     textEditElement->SetValue(CurrentLine.GetUnexcluded().Item(1));
    }
   }
   else if (CurrentLine.GetUnexcluded().Item(0) == wxT("Units")) // Process the units declarations
   {
    if (CurrentLine.GetUnexcluded().GetCount()>1)
    {
     SigmaUnits =  CurrentLine.GetUnexcluded().Item(1);
     if(SigmaUnits != wxT("mb") && SigmaUnits != wxT("tot"))  // Check unit compatibility
     {
     textEditElement->Clear();
     textEditGamma->Clear();
     textEditNumber->Clear();
     textEditAbundance->Clear();
     textEditAtomic->Clear();
     textEditIsotopic->Clear();
     tableDataEditor->ClearGrid();
     wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Error: Unsupported Cross-Section unit:") + SigmaUnits, wxT("Unsupported IBANDL file"), wxOK | wxICON_ERROR);
     dial->ShowModal();
     return false;
     }
    }
   }
  }
 }
 // Finally, process the datasheet of the cross-section table.
 // Process the second part of file
 for(int j=beginDataBlock; j<endDataBlock; j++)
  {
    wxString CurrentCrossSectionLine = database.GetLine(j); // Get the current line
    TextLineParser CrossSectionLine(CurrentCrossSectionLine,wxT(","));
    //Line prefetch
    tableDataEditor->SetCellValue(j - beginDataBlock, 0, wxT("0"));
    tableDataEditor->SetCellValue(j - beginDataBlock, 1, wxT("0"));
    tableDataEditor->SetCellValue(j - beginDataBlock, 2, wxT("0"));
    tableDataEditor->SetCellValue(j - beginDataBlock, 3, wxT("0"));
    for (int k=0; k<CrossSectionLine.GetUnexcluded().GetCount(); k++)
    {
      wxString tempvalue;
      double value;
      if(CrossSectionLine.GetUnexcluded().Item(k).ToDouble(&value))
      {
       if(k>1 && SigmaUnits == wxT("mb"))
       {
        value = value*8*std::acos(0); //multiply by 4*pi, if the units are "mbarn per sr"
       }
       if((std::abs(value) < 1e-3 || std::abs(value) > 1e+6) && value != 0.0)
        tempvalue = wxString::Format("%.3e",value);
       else
        tempvalue = wxString::Format("%f",value);
      }
      else
      {
        tempvalue = wxT("0");
      }
     //Copy the converted value to the table
     if (k<4)
      tableDataEditor->SetCellValue(j - beginDataBlock, k, tempvalue);
    }
  }
  // Finishes the procedure
  database.Close();
  return true;
}

bool R33File::IBANDLFileSave(wxGrid *tableDataEditor, wxTextCtrl* textEditElement, wxTextCtrl* textEditGamma, wxTextCtrl* textEditNumber, wxTextCtrl* textEditAbundance, wxTextCtrl* textEditAtomic, wxTextCtrl* textEditIsotopic)
{
 // Get the elements parameters
 wxString valueEditElement = textEditElement->GetValue();
 wxString valueEditGamma = textEditGamma->GetValue();
 wxString valueEditNumber = textEditNumber->GetValue();
 wxString valueEditAbundance = textEditAbundance->GetValue();
 wxString valueEditAtomic = textEditAtomic->GetValue();
 wxString valueEditIsotopic = textEditIsotopic->GetValue();
 // Create the file, and write the data
 wxTextFile file(R33FileName);
 file.Create();
 file.AddLine( wxT("Comment: ERYA Element Database exported to IBANDL") );
 file.AddLine( wxT("Element,") + valueEditElement );
 file.AddLine( wxT("Gamma Peak,") + valueEditGamma );
 file.AddLine( wxT("Atomic Number,") + valueEditNumber );
 file.AddLine( wxT("Atomic Abundance,") + valueEditAbundance);
 file.AddLine( wxT("Atomic Mass,") + valueEditAtomic);
 file.AddLine( wxT("Isotopic Mass,") + valueEditIsotopic);
 file.AddLine( wxEmptyString);
 file.AddLine( wxT("Version: R33") );
 file.AddLine( wxT("Source: ERYA") );
 file.AddLine( wxT("Name: ERYA v4.20") );
 file.AddLine( wxT("Address1:") );
 file.AddLine( wxT("SubFile:" ) );
 file.AddLine( wxT("Serial Number:") );
 file.AddLine( wxT("X4Number:") );
 file.AddLine( wxT("Reaction:   ") + valueEditElement + wxT("(p,p0)") + valueEditElement);
 file.AddLine( wxT("Distribution: Energy") );
 file.AddLine( wxT("Egamma: ")  + valueEditGamma);
 file.AddLine( wxT("Masses: 1.0, ")  + valueEditAtomic + wxT(", 1.0, ") + valueEditAtomic);
 file.AddLine( wxT("Zeds:   1, ")  + valueEditNumber + wxT(", 1, ") + valueEditNumber);
 file.AddLine( wxT("Qvalue: 0.00,\t0.00,\t0.00,\t0.00,\t0.00") );
 file.AddLine( wxT("Theta:  0.00") );
 file.AddLine( wxT("Sigfactors: 1.00,\t0.00") );
 file.AddLine( wxT("Enfactors: 1.00,\t0.00,\t0.00,\t0.00") );
 file.AddLine( wxT("Units: tot") );
 file.AddLine( wxT("Data:") );
  int tableLimit = tableDataEditor->GetNumberRows();
   for(int i=0;i<tableLimit;i++)
   {
    wxString temp = tableDataEditor->GetCellValue(i,0);
    if(temp.Len()>0)
    {
     file.AddLine( tableDataEditor->GetCellValue(i,0) + wxT(",\t") + tableDataEditor->GetCellValue(i,1) + wxT(",\t") + tableDataEditor->GetCellValue(i,2) + wxT(",\t") + tableDataEditor->GetCellValue(i,3));
    }
   }
 file.AddLine( wxT("EndData:") );
 file.Write();
 file.Close();
 return true;
}

// ITNFile class implementation
bool ITNFile::ITNFileLoad(wxGrid* &tableDataEditor, wxTextCtrl* &textEditElement, wxTextCtrl* &textEditGamma, wxTextCtrl* &textEditNumber, wxTextCtrl* &textEditAbundance, wxTextCtrl* &textEditAtomic, wxTextCtrl* &textEditIsotopic)
{
 // Ask the user to clear the element parameters, while loads the original ITN file.
 wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Delete all Element Parameters?"), wxT("Clear Current Element Data."), wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
 if (dial->ShowModal() == wxID_YES)
 {
  textEditElement->Clear();
  textEditGamma->Clear();
  textEditNumber->Clear();
  textEditAbundance->Clear();
  textEditAtomic->Clear();
  textEditIsotopic->Clear();
 }
 else // Otherwise, only empty parameters are cleared.
 {
 if (textEditElement->GetValue() == wxEmptyString)
  textEditElement->Clear();
 if (textEditGamma->GetValue() == wxEmptyString)
  textEditGamma->Clear();
 if (textEditNumber->GetValue() == wxEmptyString)
  textEditNumber->Clear();
 if (textEditAbundance->GetValue() == wxEmptyString)
  textEditAbundance->Clear();
 if (textEditAtomic->GetValue() == wxEmptyString)
  textEditAtomic->Clear();
 if (textEditIsotopic->GetValue() == wxEmptyString)
  textEditIsotopic->Clear();
 }
 tableDataEditor->ClearGrid(); // But the main spreadsheet are always cleared.
 // Set Parameters
 double UnitFactor;
 if (SelectUnits == 0)
  UnitFactor = 1;
 else
  UnitFactor = 1000;
 if (SelectAngles == 0)
  UnitFactor = UnitFactor * (8 * std::acos(0.0)); //4*pi
 else
  UnitFactor =  UnitFactor * 1.0;
 // Process the file
 wxTextFile file(ITNFileName);
 file.Open();
 // Parses the original file
 int CurrentITNLine = 0;
 for(int i=0; i<file.GetLineCount(); i++)
  {
  // Parse the line and counts the number of words
  wxString TestITNLine = file.GetLine(i);
  TextLineParser ITNLine(TestITNLine,wxEmptyString);
  if(ITNLine.GetUnexcluded().GetCount() == 2) // The minimum viable condition is a line with two numerical values.
   {
    // Fetch the two elements
    wxString c0 = ITNLine.GetUnexcluded().Item(0);
    wxString c1 = ITNLine.GetUnexcluded().Item(1);
    // Replace commas by decimal points, if necessary
    int test0 = c0.Replace(wxT(","),wxT("."),true);
    int test1 = c1.Replace(wxT(","),wxT("."),true);
    double nc0,nc1;
    // Verify if the tokens are numerical values
    if(c0.ToDouble(&nc0) && c1.ToDouble(&nc1))
    {
      tableDataEditor->SetCellValue(CurrentITNLine, 0, c0);
      tableDataEditor->SetCellValue(CurrentITNLine, 1, wxT("0"));
      tableDataEditor->SetCellValue(CurrentITNLine, 2, wxString::Format("%f",nc1*UnitFactor));
      tableDataEditor->SetCellValue(CurrentITNLine, 3, wxT("0"));
      CurrentITNLine = CurrentITNLine + 1;
    }
    else //If the user choose to ignore invalid lines, and it will be skipped, the parsing will not be aborted.
    {
      if(!IgnoreParsingErrors)
      {
        wxMessageDialog *error = new wxMessageDialog(NULL, wxT("File Reading Error due to non-numeric value.") , wxT("Invalid File!"), wxOK | wxICON_ERROR);
        error->ShowModal();
        tableDataEditor->ClearGrid();
        return false;
      }
    }
   }
   else
   {
     //Ignore error if the user selected, which then the current line will be discarded
     if(!IgnoreParsingErrors)
     {
      wxMessageDialog *error = new wxMessageDialog(NULL, wxT("Invalid File Format. Expect two numerical columns") , wxT("Invalid File!"), wxOK | wxICON_ERROR);
      error->ShowModal();
      tableDataEditor->ClearGrid();
      return false;
     }
   }
  }
 file.Close();
 return true;
}

bool ITNFile::ITNFileSave(wxGrid *tableDataEditor, wxTextCtrl* textEditElement, wxTextCtrl* textEditGamma, wxTextCtrl* textEditNumber, wxTextCtrl* textEditAbundance, wxTextCtrl* textEditAtomic, wxTextCtrl* textEditIsotopic)
{
 // Store only the cross-section data, since it don-t contain any reference of the element
 wxTextFile file(ITNFileName);
 file.Create();
 for(int i=0; i<tableDataEditor->GetNumberRows(); i++)
 {
   if (tableDataEditor->GetCellValue(i, 0) != wxEmptyString)
   {
    double value;
    wxString CurrentSigma = tableDataEditor->GetCellValue(i,2);
    if(CurrentSigma.ToDouble(&value))
    {
     value = value / 1000;
    }
    else
    {
     wxMessageDialog *error = new wxMessageDialog(NULL, wxT("The cross-section table must be numerical values only!") , wxT("Exporting Failure!"), wxOK | wxICON_ERROR);
     error->ShowModal();
     return false;
    }
    file.AddLine( tableDataEditor->GetCellValue(i,0) + wxT("\t") + wxString::Format("%f",value));
   }
   else
   {
     break;
   }
 }
 file.Write();
 file.Close();
 return true;
}

// Handles and parses a single SRIM Stopping-Powers Table for a pure element
bool SRIMFile::SRIMImport()
{
 // Conversion factor
 double UnitConversionFactor = 1;
 // Open the SRIM file, and start parsing
 wxTextFile database(SRIMFileName);
 database.Open();
 // Using unique keywords, find the line numbers that contains the relevant data blocks.
 int Ion, Atom, Units, InitialTable, FinalTable,Convert;
 for(int k=0; k<database.GetLineCount(); k++)
  {
    if(database.GetLine(k).Contains(wxT("Ion =")))
    {
      Ion = k; //Get the Ion name line
    }
    else if(database.GetLine(k).Contains(wxT("Composition")))
    {
      Atom = k + 4; //Get the Atomic Number Line
    }
    else if(database.GetLine(k).Contains(wxT("Stopping Units =")))
    {
      Units = k; // Get the Units line
    }
    else if(database.GetLine(k).Contains(wxT("Straggling")))
    {
      InitialTable = k + 2; // Get the initial table line
    }
    else if(database.GetLine(k).Contains(wxT("Multiply")))
    {
      FinalTable = k - 2; // Get the final table line
    }
    else if(database.GetLine(k).Contains(wxT("(C) ")))
    {
      Convert = k - 3; // Get the conversion constants
    }
  }
 // if any of delimiters are null, then the file is invalid.
 if (Ion == 0 || Atom == 0 || Units == 0 || InitialTable == 0 || FinalTable == 0)
 {
    wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Invalid SRIM file format"), wxT("SRIM Import Error"), wxOK | wxICON_ERROR);
     dial->ShowModal();
     return false;
 }

 // Get the incident ion name. Only Hydrogen are supported
 wxString CurrentIonLine = database.GetLine(Ion);
  wxArrayString CurrentSeparators;
  CurrentSeparators.Add(wxT("["));
  CurrentSeparators.Add(wxT("]"));
  TextLineParser CurrentLine(CurrentIonLine,CurrentSeparators);
  if(CurrentLine.GetUnexcluded().GetCount()>4)
  {
    long IonAtomicNumber;
    wxString TestAtomicNumber = CurrentLine.GetUnexcluded().Item(3); //Like [1], for Hydrogen.
    if(TestAtomicNumber.ToLong(&IonAtomicNumber))
    {
      if(IonAtomicNumber != 1) // If the ion is not hydrogen, reject the table.
      {
       wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("ERYA only support proton beams."), wxT("SRIM Import Error"), wxOK | wxICON_ERROR);
       dial->ShowModal();
       return false;
      }
    }
    else
    {
       wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("ERYA only support proton beams."), wxT("SRIM Import Error"), wxOK | wxICON_ERROR);
       dial->ShowModal();
       return false;
    }
  }
  else
  {
    wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("ERYA only support proton beams."), wxT("SRIM Import Error"), wxOK | wxICON_ERROR);
     dial->ShowModal();
     return false;
  }
 // Get the target atomic number. Also it should be a single element.
 wxString CurrentAtomLine = database.GetLine(Atom);
  TextLineParser CurrentAtomData(CurrentAtomLine,wxEmptyString);
  if(!(database.GetLine(Atom+1).Contains(wxT("=="))))
  {
    wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("ERYA only support single elements from SRIM Stopping-Powers tables."), wxT("SRIM Import Error"), wxOK | wxICON_ERROR);
     dial->ShowModal();
     return false;
  }
  if(CurrentAtomData.GetAllTokens().GetCount()==4)
  {
    long IonAtomicNumber;
    wxString TestAtomicNumber = CurrentAtomData.GetAllTokens().Item(1); // Obtain the target atomic number
    if(TestAtomicNumber.ToLong(&IonAtomicNumber))
    {
      AtomicElement = static_cast<int>(IonAtomicNumber); // Store the Atomic Number of the selected element
    }
    else
    {
      wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Expected an atomic number defined from SRIM target element.\nObtain :") + TestAtomicNumber, wxT("SRIM Import Error"), wxOK | wxICON_ERROR);
     dial->ShowModal();
     return false;
    }
  }
  else
  {
    wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Expected an atomic number defined from SRIM target element."), wxT("SRIM Import Error"), wxOK | wxICON_ERROR);
     dial->ShowModal();
     return false;
  }
  // Load the correct line with the unit conversion factor
 wxString ConversionLine = database.GetLine(Convert);
 wxArrayString ConversionSeparators;
 ConversionSeparators.Add(wxT("("));
 ConversionSeparators.Add(wxT(")"));
 ConversionSeparators.Add(wxT("/"));
 ConversionSeparators.Add(wxT("="));
 TextLineParser ConversionData(ConversionLine,ConversionSeparators);
 if(ConversionData.GetUnexcluded().GetCount()==5)
 {
  // Check the following units
    wxString TestUnit0 = ConversionData.GetUnexcluded().Item(0);
    wxString TestUnit1 = ConversionData.GetUnexcluded().Item(1);
    wxString TestUnit2 = ConversionData.GetUnexcluded().Item(2);
    wxString TestUnit3 = ConversionData.GetUnexcluded().Item(3);
    wxString TestUnit4 = ConversionData.GetUnexcluded().Item(4);
    int test1 = TestUnit0.Replace(wxT(","),wxT("."),true); //Replace commas to points.
    if(TestUnit1.Contains(wxT("eV")) && TestUnit2.Contains(wxT("1E15")) && TestUnit3.Contains(wxT("atoms")) && TestUnit4.Contains(wxT("cm2"))) //ev/(1E15 atm/cm^2), which is the default ERYA units!
    {
      if(!(TestUnit0.ToDouble(&UnitConversionFactor)))
      {
       wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Non-numeric unit conversion value found: ") + TestUnit0, wxT("SRIM Import Error"), wxOK | wxICON_ERROR);
       dial->ShowModal();
       return false;
      }
    }
    else
    {
       wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Unexpected Stopping-Power Convert Unit: ") + TestUnit1 + wxT(":") +  TestUnit2 + wxT(":") +  TestUnit3 +  TestUnit2 + wxT(":") +  TestUnit4, wxT("SRIM Import Error"), wxOK | wxICON_ERROR);
     dial->ShowModal();
     return false;
    }
 }
 else
 {
    wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Unexpected Stopping-Power Unit."), wxT("SRIM Import Error"), wxOK | wxICON_ERROR);
     dial->ShowModal();
     return false;
 }
 // Starts the table parsing, once the atomic number, conversion factors and file signatures are checked forehand
 ElementEnergy.Clear();
 ElementStoppingPower.Clear();
 for(int i=InitialTable; i<=FinalTable; i++)
 {
   wxString CurrentTableLine = database.GetLine(i);
   TextLineParser CurrentTableData(CurrentTableLine,wxEmptyString);
   if(CurrentTableData.GetAllTokens().GetCount()>4) //It requires the first four columns of data.
   {
    wxString TestUnit1 = CurrentTableData.GetAllTokens().Item(0); //Energy
    wxString TestUnit2 = CurrentTableData.GetAllTokens().Item(1); // Energy Unit
    wxString TestUnit3 = CurrentTableData.GetAllTokens().Item(2); // dE/dx atomic
    wxString TestUnit4 = CurrentTableData.GetAllTokens().Item(3); // dE/dx nuclear
    double Energy, EnergyUnit, TotalEnergy, DeltaAtomic, DeltaNuclear, StoppingPower;
    if(TestUnit2.Contains(wxT("keV")))
    {
       EnergyUnit = 1.0;
    }

    else if(TestUnit2.Contains(wxT("MeV")))
    {
        EnergyUnit = 1000.0;
    }
    else
    {
    wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Unexpected Energy Suffix: ") + TestUnit2, wxT("SRIM Import Error"), wxOK | wxICON_ERROR);
     dial->ShowModal();
     return false;
    }
    int test1 = TestUnit1.Replace(wxT(","),wxT("."),true); //Replace commas to points.
    int test2 = TestUnit3.Replace(wxT(","),wxT("."),true); //Replace commas to points.
    int test3 = TestUnit4.Replace(wxT(","),wxT("."),true); //Replace commas to points.
     if(!(TestUnit1.ToDouble(&Energy))) // Get energy
     {
     wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Non-numeric value found: ") + TestUnit1, wxT("SRIM Import Error"), wxOK | wxICON_ERROR);
     dial->ShowModal();
     return false;
     }
     if(!(TestUnit3.ToDouble(&DeltaAtomic))) // Get delta atomic
     {
     wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Non-numeric value found: ") + TestUnit3, wxT("SRIM Import Error"), wxOK | wxICON_ERROR);
     dial->ShowModal();
     return false;
     }
     if(!(TestUnit4.ToDouble(&DeltaNuclear))) // Get delta nuclear
     {
     wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Non-numeric value found: ") + TestUnit4, wxT("SRIM Import Error"), wxOK | wxICON_ERROR);
     dial->ShowModal();
     return false;
     }
    // Evaluate the pair of values
    TotalEnergy = Energy * EnergyUnit;
    StoppingPower = (DeltaAtomic + DeltaNuclear)*UnitConversionFactor;
    // And stores on class private data
    ElementEnergy.Add(wxString::Format("%f",TotalEnergy));
    ElementStoppingPower.Add(wxString::Format("%f",StoppingPower));
   }
 }
 // Complete with success
 database.Close();
 return true;
}

// A quick function to check if the file is a potential SRIM file
bool SRIMFile::IsSRIMFile()
{
 // Check the magic keywords
 wxTextFile database(SRIMFileName);
 database.Open();
 // Using unique keywords, find the line numbers that contains the relevant data blocks.
 int Magic, Ion, Atom, Units, InitialTable, FinalTable;
 for(int k=0; k<database.GetLineCount(); k++)
  {
    if(database.GetLine(k).Contains(wxT("Ion =")))
    {
      Ion = k; //Get the Ion name line
    }
    else if(database.GetLine(k).Contains(wxT("Composition")))
    {
      Atom = k + 4; //Get the Atomic Number Line
    }
    else if(database.GetLine(k).Contains(wxT("Stopping Units =")))
    {
      Units = k; // Get the Units line
    }
    else if(database.GetLine(k).Contains(wxT("Straggling")))
    {
      InitialTable = k + 2; // Get the initial table line
    }
    else if(database.GetLine(k).Contains(wxT("Multiply")))
    {
      FinalTable = k - 2; // Get the final table line
    }
    else if(database.GetLine(k).Contains(wxT("SRIM version")))
    {
      Magic = k; // Get the SRIM Version string
    }
  }
 // if any of delimiters are null, then the file is invalid.
 if (Ion == 0 || Atom == 0 || Units == 0 || InitialTable == 0 || FinalTable == 0 || Magic == 0)
    return false;
 database.Close();
 return true;
}

// LabView Database File Initialization Function.
// The LabView Database file also had between the 16th byte file, and until the FileOffsetAdress, some special codes and strings, which should be an internal structure declaration.
// Our implementation was derived from reverse engineering, and some third-party research.
// Since ERYA hard-codes a fixed structure, due to the fact the LabView ERYA had already a fixed definition, each derived class had the optimized code to load the necessary data.
void LabViewFile::Initialization(wxString LabViewFilePath)
{
 // Begin the binary file first tests to ensure the file are on a compatible format
 LabViewFileName = LabViewFilePath;
 FileDescriptor = wxT("LabView DataLog");
 // Open the file, and read the file header
 wxFile FileTest(LabViewFileName,wxFile::read);
 FileTest.Open(LabViewFileName,wxFile::read,wxS_DEFAULT);
 unsigned char FileHeader[5],FileVersion[5],FileAdress[5],FilePointer[5],FileOffset[5];
 int FileCheck;
 FileCheck = FileTest.Read(FileHeader,4);
 LabViewFileHeader = wxString::FromAscii(FileHeader);
 LabViewFileHeader = LabViewFileHeader.Left(4); // In some compilers, may parse a fifth unwanted character, then just trim it.
 // Get the file Version, however it is not really important
 FileCheck = FileTest.Seek(4,wxFromStart);
 FileCheck = FileTest.Read(FileVersion,4);
 LabViewFileVersion3 = FileVersion[3];
 LabViewFileVersion2 = FileVersion[2];
 LabViewFileVersion1 = FileVersion[1];
 LabViewFileVersion0 = FileVersion[0];
 // Get the file Adress pointer, which is the number of records.
 FileCheck = FileTest.Seek(8,wxFromStart);
 FileCheck = FileTest.Read(FileAdress,4);
 NumberDescriptor = this->CharToInt32(FileAdress);
 // Get the file Offset pointer
 FileCheck = FileTest.Seek(12,wxFromStart);
 FileCheck = FileTest.Read(FilePointer,4);
 FileOffsetPointer = this->CharToInt32(FilePointer);
 // Now jump to the file offset adress.
 FileCheck = FileTest.Seek(FileOffsetPointer,wxFromStart);
 FileCheck = FileTest.Read(FileOffset,4);
 FileOffsetAdress = this->CharToInt32(FileOffset);
 // And the essential stuff was done.
 FileTest.Close();
 return;
}

// Some utilities and wrappers to aid the binary file reading, and get the integers and decimals. LabView format is big-endian... but several CPU architectures are little-endian.
unsigned int LabViewFile::CharToInt16(unsigned char* x)
{
 unsigned int i = x[0]*256 + x[1];
 return i;
}

unsigned int LabViewFile::CharToInt32(unsigned char* x)
{
 unsigned int i = x[0]*256*256*256 + x[1]*256*256 + x[2]*256 + x[3];
 return i;
}

double LabViewFile::CharToDouble(unsigned char* x)
{
 // The IEEE-754 double float number on hexadecimal format requires some delicate tasks, since it will follow the pattern 1-byte*11-byte*52-byte
 unsigned int x7,x6,x5,x4,x3,x2,x1,x0,x1e,x1m,exp;
 double z,zd,sign,expf;
 x0 = x[0];
 x1 = x[1];
 x2 = x[2];
 x3 = x[3];
 x4 = x[4];
 x5 = x[5];
 x6 = x[6];
 x7 = x[7];
 // Get the number sign, and fix the header
 if(x0 >= 128)
 {
   x0 = x0 - 128;
   sign = -1.0;
 }
 else
 {
   sign = 1.0;
 }
 // Split the second byte, since the first half belongs to the exponential, and the second half to the mantissa.
 x1e = x1 / 16;
 x1m = x1 % 16;
 // Evaluate the exponential part. Note: It is necessary to split the integer and fractional convertion to avoid mistakes!
 exp = x0*16+x1e;
 expf = 1.0*exp - 1023.0;
 // Evaluate the mantissa
 zd = 1 + x1m*std::pow(2,-4) + x2*std::pow(2,-12) + x3*std::pow(2,-20) + x4*std::pow(2,-28) + x5*std::pow(2,-36) + x6*std::pow(2,-44) + x7*std::pow(2,-52);
 // Evaluate the final result:
 z = sign * std::pow(2,expf) * zd;
 return z;
}

// Check if the file are a valid LabView Database
bool LabViewFile::IsLabViewFile()
{
  if(LabViewFileHeader == wxT("DTLG"))
  {
   return true;
  }
  else
  {
   return false;
  }
}


// Verify if the LabView File are a compatible Ziegler matrix, this means, it should have at least 14 columns, any higher than that, are ignored.
bool LabViewZiegler::IsLabViewZiegler()
{
 if(this->IsLabViewFile())
 {
 unsigned char ZieglerRows[5],ZieglerColumns[5];
 int FileCheck;
 wxFile FileTest(this->GetLabViewFileName(),wxFile::read);
 FileTest.Open(this->GetLabViewFileName(),wxFile::read,wxS_DEFAULT);
 FileCheck = FileTest.Seek(this->GetFileOffsetAdress(),wxFromStart);
 FileCheck = FileTest.Read(ZieglerRows,4);
 ZieglerNumberRows = this->CharToInt32(ZieglerRows);
 FileCheck = FileTest.Seek(this->GetFileOffsetAdress()+4,wxFromStart);
 FileCheck = FileTest.Read(ZieglerColumns,4);
 ZieglerNumberColumns = this->CharToInt32(ZieglerColumns);
 FileTest.Close();
 if( (ZieglerNumberRows > 0) && (ZieglerNumberColumns > 13) )
  return true;
 else
  return false;
 }
 else
 {
   return false;
 }
}

// Get the numerical Ziegler Parameters, which is simply a matrix with 8-byte hexadecimal IEEE-754 numbers
ZieglerParameters LabViewZiegler::GetZieglerParametersFromLabView()
{
 int FileCheck;
 int CurrentFileAdress = this->GetFileOffsetAdress() + 8;
 std::vector<double> TempValues;
 ZieglerParameters ZieglerMatrix;
 ZieglerMatrix.Clear();
 wxFile FileTest(this->GetLabViewFileName(),wxFile::read);
 FileTest.Open(this->GetLabViewFileName(),wxFile::read,wxS_DEFAULT);
 // Now start the extraction of several 8-byte IEEE-754 double float number
 for (int i=0; i < ZieglerNumberRows; i++)
 {
   for (int j=0; j < ZieglerNumberColumns; j++)
   {
     FileCheck = FileTest.Seek(CurrentFileAdress,wxFromStart);
     unsigned char CurrentData[9];
     FileCheck = FileTest.Read(CurrentData,8);
     double temp = this->CharToDouble(CurrentData);
     TempValues.push_back(temp);
     CurrentFileAdress = CurrentFileAdress + 8;
   }
  // Finishes one line extraction, now let's make the convertion!
   if(TempValues.size() > 13)
   {
     wxString A0 = wxString::Format("%.8f",TempValues.at(0));
     wxString A1 = wxString::Format("%.8f",TempValues.at(1));
     wxString A2 = wxString::Format("%.8f",TempValues.at(2));
     wxString A3 = wxString::Format("%.8f",TempValues.at(3));
     wxString A4 = wxString::Format("%.8f",TempValues.at(4));
     wxString A5 = wxString::Format("%.8f",TempValues.at(5));
     wxString A6 = wxString::Format("%.8f",TempValues.at(6));
     wxString A7 = wxString::Format("%.8f",TempValues.at(7));
     wxString A8 = wxString::Format("%.8f",TempValues.at(8));
     wxString A9 = wxString::Format("%.8f",TempValues.at(9));
     wxString A10 = wxString::Format("%.8f",TempValues.at(10));
     wxString A11 = wxString::Format("%.8f",TempValues.at(11));
     wxString A12 = wxString::Format("%.8f",TempValues.at(12));
     wxString A13 = wxString::Format("%.8f",TempValues.at(13));
     A0 = wxT("Z") + wxString::Format("%i",i+1);
     ZieglerMatrix.AddZieglerParameterData(A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13);
     if(TempValues.at(9) * TempValues.at(10) * TempValues.at(11) * TempValues.at(12) < 1e-20) // Guess the 1977 or 1991 Ziegler version
       ZieglerMatrix.SetZieglerVersion(wxT("1"));
     else
       ZieglerMatrix.SetZieglerVersion(wxT("0"));
     TempValues.clear();
   }
   else
   {
     TempValues.clear();
   }
 }
 // Closing the file, and returning the exported data.
 FileTest.Close();
 return ZieglerMatrix;
}

// Check the validity of a compatible Element Database from a binary LabView file.
bool LabViewElements::IsLabViewElements()
{
 if(this->IsLabViewFile())
 {
 unsigned char RecordCount[5];
 int FileCheck;
 wxFile FileTest(this->GetLabViewFileName(),wxFile::read);
 FileTest.Open(this->GetLabViewFileName(),wxFile::read,wxS_DEFAULT);
 FileCheck = FileTest.Seek(this->GetFileOffsetAdress(),wxFromStart);
 FileCheck = FileTest.Read(RecordCount,4);
 ElementsNumberRecords = this->CharToInt32(RecordCount);
 FileTest.Close();
 if( ElementsNumberRecords > 0)
  return true;
 else
  return false;
 }
 else
 {
   return false;
 }
}

// Extract the relevant data structures from the LabView binary file for Element database
// The expected structure of each register block are the following:
   // 1. Number of Gamma Peaks, declared by a 32-bit integer.
   // 1.1 If the last number is greater than zero, then load the Gamma Peaks, as 32-bit integers.
   // 1.2 If it is zero, then add as a 0-keV element.
   // 2. The lenght of Element Name string
   // 2.1 If the lenght is not null, then read the specified number of bytes, and export as a wxString.
   // 2.2 If empty, add the current record as a name itself.(User mistake).
   // 3. Read the Atomic Number, as a 16-bit integer.
   // 4. Read three 8-bit IEEE-754 numbers. And get the Abundance, Atomic Mass and Isotopic Mass.
   // 5. Read two 32-bit integers, to get a floating point matrix size(Like the Ziegler case).
   // 5.1 If the last two numbers are greater than zero, then read the next number of rows plus columns, as an array of 8-bit IEEE-754 numbers.
   // 5.2 Otherwise, there is no Cross-Section data.
// Once completed, for each Gamma Peak numbers, export the data to the ElementDatabaseArray class.
ElementDatabaseArray LabViewElements::GetElementsFromLabView()
{
  ElementDatabaseArray ElementRecords;
  wxFile FileTest(this->GetLabViewFileName(),wxFile::read);
  FileTest.Open(this->GetLabViewFileName(),wxFile::read,wxS_DEFAULT);
  // The LabView file already specify the number of elements on their database.
  int CurrentFileAdress = this->GetFileOffsetAdress() + 4;
  for (int z=0; z < ElementsNumberRecords; z++)
  {
    // 0. Set global variables
    int LabViewNumberGamma;
    std::vector<int> LabViewGammaPeak;
    int LabViewElementLenght;
    wxString LabViewElementName;
    int LabViewNumber;
    double LabViewAbundance, LabViewAtomic, LabViewIsotopic;
    int EnergySize,SigmaSize;
    std::vector<double> LabViewEnergy, LabViewSigma;
    unsigned char GammaPeakCount[5],ElementLenghtCount[5],AtomicCount[3],AbundanceCount[9],MassCount[9],IsotopicCount[9],EnergyCount[5],SigmaCount[5];
    int FileCheck;
    wxArrayString ImportElementEnergy,ImportElementSigma,ImportElementEnergyError,ImportElementSigmaError;
    // 1. Read Gamma Peaks
    FileCheck = FileTest.Seek(CurrentFileAdress,wxFromStart);
    FileCheck = FileTest.Read(GammaPeakCount,4);
    LabViewNumberGamma = this->CharToInt32(GammaPeakCount);
    CurrentFileAdress = CurrentFileAdress + 4;
    if (LabViewNumberGamma > 0)
    {
      for(int i=0; i < LabViewNumberGamma; i++)
      {
        unsigned char GammaPeakValue[5];
        int GammaPeakCurrent;
        FileCheck = FileTest.Seek(CurrentFileAdress,wxFromStart);
        FileCheck = FileTest.Read(GammaPeakValue,4);
        GammaPeakCurrent = this->CharToInt32(GammaPeakValue);
        LabViewGammaPeak.push_back(GammaPeakCurrent);
        CurrentFileAdress = CurrentFileAdress + 4;
      }
    }
    else
    {
      LabViewGammaPeak.push_back(0);
    }
    // 2. Read Element Name
    FileCheck = FileTest.Seek(CurrentFileAdress,wxFromStart);
    FileCheck = FileTest.Read(ElementLenghtCount,4);
    LabViewElementLenght = this->CharToInt32(ElementLenghtCount);
    CurrentFileAdress = CurrentFileAdress + 4;
    if (LabViewElementLenght > 0) //Use a char-by-char reading to minimize problems.
    {
      for (int r=0; r < LabViewElementLenght; r++)
      {
        unsigned char ElementChar[2];
        FileCheck = FileTest.Seek(CurrentFileAdress,wxFromStart);
        FileCheck = FileTest.Read(ElementChar,1);
        char EC = ElementChar[0];
        wxString wxEC = wxString::FromAscii(EC);
        CurrentFileAdress = CurrentFileAdress + 1;
        LabViewElementName = LabViewElementName + wxEC;
      }
      LabViewElementName = LabViewElementName.Trim(); //The last operation remove spaces, tabs, newlines and other unnecessary characters.
    }
    else
    {
      LabViewElementName = wxT("E") + wxString::Format("%i",z+1); // If a user make a mistake, and save on original LabView ERYA, an element without name.
    }
    // 3. Read Atomic Number
    FileCheck = FileTest.Seek(CurrentFileAdress,wxFromStart);
    FileCheck = FileTest.Read(AtomicCount,2);
    LabViewNumber = this->CharToInt16(AtomicCount);
    CurrentFileAdress = CurrentFileAdress + 2;
    // 4. Read the Elements Abundance, Atomic and Isotopic Mass
    FileCheck = FileTest.Seek(CurrentFileAdress,wxFromStart);
    FileCheck = FileTest.Read(AbundanceCount,8);
    LabViewAbundance = this->CharToDouble(AbundanceCount);
    CurrentFileAdress = CurrentFileAdress + 8;
    FileCheck = FileTest.Seek(CurrentFileAdress,wxFromStart);
    FileCheck = FileTest.Read(MassCount,8);
    LabViewAtomic = this->CharToDouble(MassCount);
    CurrentFileAdress = CurrentFileAdress + 8;
    FileCheck = FileTest.Seek(CurrentFileAdress,wxFromStart);
    FileCheck = FileTest.Read(IsotopicCount,8);
    LabViewIsotopic = this->CharToDouble(IsotopicCount);
    CurrentFileAdress = CurrentFileAdress + 8;
    // 5. Read Cross-Section Matrix
    FileCheck = FileTest.Seek(CurrentFileAdress,wxFromStart);
    FileCheck = FileTest.Read(EnergyCount,4);
    EnergySize = this->CharToInt32(EnergyCount);
    CurrentFileAdress = CurrentFileAdress + 4;
    FileCheck = FileTest.Seek(CurrentFileAdress,wxFromStart);
    FileCheck = FileTest.Read(SigmaCount,4);
    SigmaSize = this->CharToInt32(SigmaCount);
    CurrentFileAdress = CurrentFileAdress + 4;
    if((EnergySize > 0) && (SigmaSize > 0)) // An empty matrix means that no cross-section data was provided.
    {
      for(int i=0; i<EnergySize; i++)
      {
        for(int j=0; j<SigmaSize; j++)
        {
          // Read the current 8-bit IEEE-754 double floating point number
          unsigned char CurrentData[9];
          FileCheck = FileTest.Seek(CurrentFileAdress,wxFromStart);
          FileCheck = FileTest.Read(CurrentData,8);
          double temp = this->CharToDouble(CurrentData);
          CurrentFileAdress = CurrentFileAdress + 8;
          if((j%2)== 0) // split the data to the correct vectors.
          {
            LabViewEnergy.push_back(temp);  // Energy em keV
          }
          else
          {
            LabViewSigma.push_back(temp*1000); // Energy em mbarn.keV, with on-fly conversion.
          }
        }
      }
    }
    // 6. Data Processing
    if (LabViewNumberGamma > 0)
    {
     for(int k=0; k<LabViewNumberGamma; k++) // Extract several cross-section of the same Element for each Gamma-Peak
     {
      ImportElementEnergy.Clear();
      ImportElementSigma.Clear();
      ImportElementEnergyError.Clear();
      ImportElementSigmaError.Clear();
      for (int m=0; m<LabViewEnergy.size(); m++)
      {
        if(k == (m%LabViewNumberGamma)) // Select the correct gamma peak
        {
          wxString temp0 = wxString::Format("%f",LabViewEnergy.at(m));
          wxString temp1 = wxString::Format("%f",LabViewSigma.at(m));
          ImportElementEnergy.Add(temp0);
          ImportElementSigma.Add(temp1);
          ImportElementEnergyError.Add(wxT("0"));
          ImportElementSigmaError.Add(wxT("0"));
        }
      }
      // Make additional conversions
      wxString ActualGammaPeak = wxString::Format("%i",LabViewGammaPeak.at(k));
      wxString ActualElementNumber = wxString::Format("%i",LabViewNumber);
      wxString ActualAbundance = wxString::Format("%f",LabViewAbundance);
      wxString ActualAtomic = wxString::Format("%f",LabViewAtomic);
      wxString ActualIsotopic = wxString::Format("%f",LabViewIsotopic);
      // Check the presence of duplications, and if it happens, changes the name slightly
      if(ElementRecords.FindElement(LabViewElementName,ActualGammaPeak) != wxNOT_FOUND)
      {
        int NumberGuest = 0;
        bool FixName = false;
        wxString TestNewName = LabViewElementName + wxT("_") + wxString::Format("%i",NumberGuest);
        do
        {
          if(ElementRecords.FindElement(TestNewName,ActualGammaPeak) != wxNOT_FOUND)
          {
            NumberGuest = NumberGuest + 1;
            TestNewName = LabViewElementName + wxT("_") + wxString::Format("%i",NumberGuest);
          }
          else
          {
            FixName = true;
          }
        }while(!FixName);
        LabViewElementName = TestNewName;
      }
      // And store the data to the Database class
      ElementDatabase StoreNewElement(LabViewElementName, ActualGammaPeak, ActualElementNumber, ActualAbundance, ActualAtomic, ActualIsotopic, ImportElementEnergy, ImportElementEnergyError, ImportElementSigma, ImportElementSigmaError);
      ElementRecords.Add(StoreNewElement);
     }
    }
    else // Treat the case when no gamma-peak references was made.
    {
      // Convert the cross-section data
      ImportElementEnergy.Clear();
      ImportElementSigma.Clear();
      ImportElementEnergyError.Clear();
      ImportElementSigmaError.Clear();
      for (int m=0; m<LabViewEnergy.size(); m++)
      {
          wxString temp0 = wxString::Format("%f",LabViewEnergy.at(m));
          wxString temp1 = wxString::Format("%f",LabViewSigma.at(m));
          ImportElementEnergy.Add(temp0);
          ImportElementSigma.Add(temp1);
          ImportElementEnergyError.Add(wxT("0"));
          ImportElementSigmaError.Add(wxT("0"));
      }
      // Convert the Element main characteristics
      wxString ActualGammaPeak = wxT("0");
      wxString ActualElementNumber = wxString::Format("%i",LabViewNumber);
      wxString ActualAbundance = wxString::Format("%f",LabViewAbundance);
      wxString ActualAtomic = wxString::Format("%f",LabViewAtomic);
      wxString ActualIsotopic = wxString::Format("%f",LabViewIsotopic);
      // Check the presence of duplications, and if it happens, changes the name slightly
      if(ElementRecords.FindElement(LabViewElementName,ActualGammaPeak) != wxNOT_FOUND)
      {
        int NumberGuest = 0;
        bool FixName = false;
        wxString TestNewName = LabViewElementName + wxT("_") + wxString::Format("%i",NumberGuest);
        do
        {
          if(ElementRecords.FindElement(TestNewName,ActualGammaPeak) != wxNOT_FOUND)
          {
            NumberGuest = NumberGuest + 1;
            TestNewName = LabViewElementName + wxT("_") + wxString::Format("%i",NumberGuest);
          }
          else
          {
            FixName = true;
          }
        }while(!FixName);
        LabViewElementName = TestNewName;
      }
      // And store the data to the Database class
      ElementDatabase StoreNewElement(LabViewElementName, ActualGammaPeak, ActualElementNumber, ActualAbundance, ActualAtomic, ActualIsotopic, ImportElementEnergy, ImportElementEnergyError, ImportElementSigma, ImportElementSigmaError);
      ElementRecords.Add(StoreNewElement);
    }
    // Clean temporary vectors
    LabViewEnergy.clear();
    LabViewSigma.clear();
    LabViewGammaPeak.clear();
    // Final file record cycle
  }
  FileTest.Close();
  return ElementRecords;
}
