/***************************************************************
 * Name:      DatabaseLibrary.cpp
 * Purpose:   ERYA-PIXE database classes prototypes
 * Author:    Vasco Manteigas (vm.manteigas@campus.fct.utl.pt)
 * Created:   2016-10-01
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

#include "DatabaseLibrary.h"
#include "PlotLibrary.h"
#include <wx/arrimpl.cpp>
WX_DEFINE_OBJARRAY(ElementDatabaseList);
WX_DEFINE_OBJARRAY(ElementSRIMList);

// Element Database main constructor
ElementDatabase::ElementDatabase(wxString Name, wxString Gamma, wxString Number, wxString Abundance, wxString Atomic, wxString Isotopic, wxArrayString Energy, wxArrayString EnergyError, wxArrayString Sigma, wxArrayString SigmaError, wxString Info)
{
  dataEditElement = Name;
  dataEditGamma = Gamma;
  dataEditNumber = Number;
  dataEditAbundance = Abundance;
  dataEditAtomic = Atomic;
  dataEditIsotopic = Isotopic;
  dataEnergy = Energy;
  dataEnergyError = EnergyError;
  dataSigma = Sigma;
  dataSigmaError = SigmaError;
  infoElement = Info;
}

// Element Database standard constructor
ElementDatabase::ElementDatabase(wxTextCtrl* EditElement, wxTextCtrl* EditGamma, wxTextCtrl* EditNumber, wxTextCtrl* EditAbundance, wxTextCtrl* EditAtomic, wxTextCtrl* EditIsotopic, wxString EditInfo, wxGrid* DataEditor)
{
   dataEditElement = EditElement->GetValue();
   dataEditGamma = EditGamma->GetValue();
   dataEditNumber = EditNumber->GetValue();
   dataEditAbundance = EditAbundance->GetValue();
   dataEditAtomic = EditAtomic->GetValue();
   dataEditIsotopic = EditIsotopic->GetValue();
   infoElement = EditInfo;
   dataEnergy.Clear();
   dataEnergyError.Clear();
   dataSigma.Clear();
   dataSigmaError.Clear();
   for(int i=0;i<DataEditor->GetNumberRows();i++)
   {
   wxString temp0 = DataEditor->GetCellValue(i,0);
   wxString temp1 = DataEditor->GetCellValue(i,1);
   wxString temp2 = DataEditor->GetCellValue(i,2);
   wxString temp3 = DataEditor->GetCellValue(i,3);
   dataEnergy.Add(temp0);
   dataEnergyError.Add(temp1);
   dataSigma.Add(temp2);
   dataSigmaError.Add(temp3);
   }
}

// Enforce a strict name convention for Elements:
// Their name should consist with number and/or letters, and the underscore characters
// Other fields are pure numerical values
bool ElementDatabase::CheckElement()
{
  //Check name validity
  if(dataEditElement.Len()==0)
    return false;
  wxString chars = wxT("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_");
  for(int i=0; i<dataEditElement.Len(); i++)
  {
   wxString character = dataEditElement.GetChar(i);
   int counter;
   counter = chars.Find(character);
   if(counter == wxNOT_FOUND)
    return false;
  }
  // Check all numeric values
  double testGamma, testNumber, testAbundance, testAtomic, testIsotopic;
  if(!(dataEditGamma.ToDouble(&testGamma)))
    return false;
  else if(!(dataEditNumber.ToDouble(&testNumber)))
    return false;
  else if(!(dataEditAbundance.ToDouble(&testAbundance)))
    return false;
  else if(!(dataEditAtomic.ToDouble(&testAtomic)))
    return false;
  else if(!(dataEditIsotopic.ToDouble(&testIsotopic)))
    return false;
  // Pass all tests
  else
   return true;
}

// Obtain the relevant Element information to the Element Editor
bool ElementDatabase::GetAllElementInfo(wxTextCtrl* &SetElement, wxTextCtrl* &SetGammaPeak, wxTextCtrl* &SetNumber, wxTextCtrl* &SetAbundance, wxTextCtrl* &SetMass, wxTextCtrl* &SetIsotopic, wxString &SetInfo, wxGrid* &SetEnergySigmaErrorTable)
{
     SetElement->SetValue(this->GetElement());
     SetGammaPeak->SetValue(this->GetGamma());
     SetNumber->SetValue(this->GetNumber());
     SetAbundance->SetValue(this->GetAbundance());
     SetMass->SetValue(this->GetAtomic());
     SetIsotopic->SetValue(this->GetIsotopic());
     SetInfo = this->GetInfo();
    for (int i=0; i<this->GetEnergy().GetCount(); i++)
   {
    wxString temp1 = this->GetEnergy().Item(i);
    wxString temp2 = this->GetEnergyError().Item(i);
    wxString temp3 = this->GetSigma().Item(i);
    wxString temp4 = this->GetSigmaError().Item(i);
    if (temp1.Len()>0 && temp2.Len()>0 && temp3.Len()>0 && temp4.Len()>0)
    {
      SetEnergySigmaErrorTable->SetCellValue(i,0,temp1);
      SetEnergySigmaErrorTable->SetCellValue(i,1,temp2);
      SetEnergySigmaErrorTable->SetCellValue(i,2,temp3);
      SetEnergySigmaErrorTable->SetCellValue(i,3,temp4);
    }
    else
    {
      return true;
    }
  }
  return true;
}

// Convert a character to an integer, defining a base-36 integer
int ElementDatabaseArray::CharToNumber(wxString c)
{
 if(c.Len()>1)
  {
    return wxNOT_FOUND;
  }
  else
  {
    if(c == wxT("0"))
     return 0;
    else if(c == wxT("1"))
     return 1;
    else if(c == wxT("2"))
     return 2;
    else if(c == wxT("3"))
     return 3;
    else if(c == wxT("4"))
     return 4;
    else if(c == wxT("5"))
     return 5;
    else if(c == wxT("6"))
     return 6;
    else if(c == wxT("7"))
     return 7;
    else if(c == wxT("8"))
     return 8;
    else if(c == wxT("9"))
     return 9;
    else if(c == wxT("A"))
     return 10;
    else if(c == wxT("a"))
     return 10;
    else if(c == wxT("B"))
     return 11;
    else if(c == wxT("b"))
     return 11;
    else if(c == wxT("C"))
     return 12;
    else if(c == wxT("c"))
     return 12;
    else if(c == wxT("D"))
     return 13;
    else if(c == wxT("d"))
     return 13;
    else if(c == wxT("E"))
     return 14;
    else if(c == wxT("e"))
     return 14;
    else if(c == wxT("F"))
     return 15;
    else if(c == wxT("f"))
     return 15;
    else if(c == wxT("G"))
     return 16;
    else if(c == wxT("g"))
     return 16;
    else if(c == wxT("H"))
     return 17;
    else if(c == wxT("h"))
     return 17;
    else if(c == wxT("I"))
     return 18;
    else if(c == wxT("i"))
     return 18;
    else if(c == wxT("J"))
     return 19;
    else if(c == wxT("j"))
     return 19;
    else if(c == wxT("K"))
     return 20;
    else if(c == wxT("k"))
     return 20;
    else if(c == wxT("L"))
     return 21;
    else if(c == wxT("l"))
     return 21;
    else if(c == wxT("M"))
     return 22;
    else if(c == wxT("m"))
     return 22;
    else if(c == wxT("N"))
     return 23;
    else if(c == wxT("n"))
     return 23;
    else if(c == wxT("O"))
     return 24;
    else if(c == wxT("o"))
     return 24;
    else if(c == wxT("P"))
     return 25;
    else if(c == wxT("p"))
     return 25;
    else if(c == wxT("Q"))
     return 26;
    else if(c == wxT("q"))
     return 26;
    else if(c == wxT("R"))
     return 27;
    else if(c == wxT("r"))
     return 27;
    else if(c == wxT("S"))
     return 28;
    else if(c == wxT("s"))
     return 28;
    else if(c == wxT("T"))
     return 29;
    else if(c == wxT("t"))
     return 29;
    else if(c == wxT("U"))
     return 30;
    else if(c == wxT("u"))
     return 30;
    else if(c == wxT("V"))
     return 31;
    else if(c == wxT("v"))
     return 31;
    else if(c == wxT("W"))
     return 32;
    else if(c == wxT("w"))
     return 32;
    else if(c == wxT("X"))
     return 33;
    else if(c == wxT("x"))
     return 33;
    else if(c == wxT("Y"))
     return 34;
    else if(c == wxT("y"))
     return 34;
    else if(c == wxT("Z"))
     return 35;
    else if(c == wxT("z"))
     return 35;
    else
     return 0;
  }
}


// Convert a string to an integer, using a base-36 float number
double ElementDatabaseArray::StringToNumber(wxString s)
{
 double Result = 0;
 for(int i=0; i<s.Len(); i++)
  {
   wxString ic = s.GetChar(i);
   if( this->CharToNumber(ic) != wxNOT_FOUND)
    {
      double tempvalue = CharToNumber(ic) * 1.0;
      double tempindex = -1.0*i;
      Result = Result + tempvalue * std::pow(36,tempindex) ;
    }
    else
    {
      return NAN;
    }
  }
 return Result;
}

// Check the order of two strings, and verify if the first are greater than the second, which would trigger the true flag, or get false otherwise.
bool ElementDatabaseArray::SwapStrings(wxString a, wxString b)
{
 // Make the comparation
 if(std::isfinite(this->StringToNumber(a)) && std::isfinite(this->StringToNumber(b)))
  return (this->StringToNumber(a) > this->StringToNumber(b));
 else
  return false;
}

// Sort the Element Database using a two-stage sorting method: First the elements are sorted by own name, and once done, each Element block are sorted by
// own Excitation value.
bool ElementDatabaseArray::SortElementDatabase()
{
  wxArrayString ElementList;
  // Zeroth Step: Check the consistency of the Elements names
   for(int k=0; k < this->GetCount() ; k++)
     {
      if ( this->StringToNumber(this->Item(k).GetElement()) == wxNOT_FOUND )
      {
       return false;
      }
     }
 // First Step: Apply the sorting algorithm to the Element Name.
  int n = this->GetCount();
   for (int i=0; i<n-1; i++)
   {
     for(int j=0; j<n-i-1; j++)
     {
      if ( this->SwapStrings(this->Item(j).GetElement(), this->Item(j+1).GetElement()) )
      {
       ElementDatabase Temporary = this->Item(j);
       this->RemoveAt(j);
       this->Insert(Temporary,j+1);
      }
     }
   }
 // Second Step: Once the Names are sorted, check if the element contains several excitation levels (gamma peaks), and sort them
   int m = this->GetCount();
   for (int p=0; p<m-1; p++)
   {
     for(int q=0; q<m-p-1; q++)
     {
      double gamma1,gamma2;
      if(this->Item(q).GetGamma().ToDouble(&gamma1) && this->Item(q+1).GetGamma().ToDouble(&gamma2))
      {
       if ( (this->StringToNumber(this->Item(q).GetElement()) == this->StringToNumber(this->Item(q+1).GetElement())) && (gamma1 > gamma2) )
       {
       ElementDatabase Temporary = this->Item(q);
       this->RemoveAt(q);
       this->Insert(Temporary,q+1);
       }
      }
      else
      {
       return false;
      }
     }
   }
  // And return
  return true;
}

// Get the real database adress of the requested element
int ElementDatabaseArray::FindElement(wxString GetElementName, wxString GetGammaPeak)
{
 // Check the element presence on the database
 for(int i=0; i<this->GetCount(); i++)
 {
  if ((this->Item(i).GetElement() == GetElementName) && (this->Item(i).GetGamma() == GetGammaPeak))
  {
     return i;
  }
 }
  return wxNOT_FOUND;
}



// Add a new element to the database, where it will check if the the element (The name and the excitation state are the main exclusive characteristics)
// already exits (it will get a true value if it is a new element), but if finds an equal element, then it will replace and get true if the Overwrite flag
// are true, otherwise, get false.
// After a sucefull operation, the database are sorted by the Element Name, and then by the Gamma Peak
bool ElementDatabaseArray::NewElement(ElementDatabase CandidateElement, bool Overwrite)
{
 // Any Element candidate should have, at least, a name and a gamma!
 if(CandidateElement.GetGamma().Len() == 0 || CandidateElement.GetElement().Len() == 0)
  return false;
 // Verify if their names are plenty valid
 if(!(CandidateElement.CheckElement()))
  return false;
 // Check the element presence on the database
 int CheckCandidateAdress = this->FindElement(CandidateElement.GetElement(),CandidateElement.GetGamma());
 // If the overwrite flag was enabled, then replace it
 if (CheckCandidateAdress != wxNOT_FOUND)
 {
  if(Overwrite)
  {
    this->RemoveAt(CheckCandidateAdress);
    this->Add(CandidateElement);
    // And return while sort the database
    return this->SortElementDatabase();
  }
  else // A protected Element cannot be overwritten, unless the user acceps to rename the element
  {
    wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("The Element \"") + CandidateElement.GetElement() + wxT("\", with \"") + CandidateElement.GetGamma() + wxT("\" keV.\nAlready exists on database!\nIf you add, the Element will be renamed.\nAdd anyway?"), wxT("Element Duplication!"), wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
    if (dial->ShowModal() == wxID_YES)
    {
      //Test for duplications
      int NumberGuest = 0;
      bool FixName = false;
      wxString FinalNewName;
      wxString TestNewName = CandidateElement.GetElement() + wxT("_") + wxString::Format("%i",NumberGuest);
        do
        {
          if(this->FindElement(TestNewName,CandidateElement.GetGamma()) != wxNOT_FOUND)
          {
            NumberGuest = NumberGuest + 1;
            TestNewName = CandidateElement.GetElement() + wxT("_") + wxString::Format("%i",NumberGuest);
          }
          else
          {
            FixName = true;
          }
        }while(!FixName);
        FinalNewName = TestNewName;
        CandidateElement.SetElement(FinalNewName);
        this->Add(CandidateElement);
        // And return while sort the database
        return this->SortElementDatabase();
    }
    else
    {
      return false;
    }
  }
 }
 else
 {
  // If it was not found, just add it.
  this->Add(CandidateElement);
  // And sort the database
  return this->SortElementDatabase();
 }
}

// A convenient function to delete the selected element from the Database Manager, while also updates the database automatically
bool ElementDatabaseArray::RemoveElement(wxString GetElement, wxString GetGammaPeak, wxChoice* &ListElement, wxChoice* &ListGammaPeak, wxTextCtrl* &SetNumber, wxTextCtrl* &SetAbundance, wxTextCtrl* &SetMass, wxTextCtrl* &SetIsotopic, wxGrid* &SetEnergySigma, mpWindow* &SetEnergyPlot)
{
 // Define an internal flag
 bool FoundElement = false;
 // Check the element presence on the database
 for(int i=0; i<this->GetCount(); i++)
 {
  if ( GetElement == this->Item(i).GetElement() && GetGammaPeak == this->Item(i).GetGamma())
  {
    this->RemoveAt(i);
    FoundElement = true;
  }
 }
 // If the element was actually removed, then update the database
 if (FoundElement)
  {
    if(this->GetCount() > 0)
     {
      if(this->SortElementDatabase())
        {
          // Rebuild the Database Manager with the information from the first element from database
          return this->FindGetAllElementInfo(ListElement->GetString(0), ListGammaPeak->GetString(0), ListElement, ListGammaPeak, SetNumber, SetAbundance, SetMass, SetIsotopic, SetEnergySigma, SetEnergyPlot);
        }
        else
        {
        this->Clear();
        ListElement->Clear();
        ListGammaPeak->Clear();
        SetAbundance->Clear();
        SetIsotopic->Clear();
        SetMass->Clear();
        SetNumber->Clear();
        SetEnergyPlot->DelAllLayers(false,true);
        wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("The Element Database consistency are damaged. Please, reload with a new one."), wxT("Database is Corrupted!"), wxOK | wxICON_ERROR);
        dial->ShowModal();
        return false; // Database error.
        }
     }
     else // Delete the contents of the Database Manager, since it was empty!
     {
       ListElement->Clear();
       ListGammaPeak->Clear();
       SetAbundance->Clear();
       SetIsotopic->Clear();
       SetMass->Clear();
       SetNumber->Clear();
       SetEnergyPlot->DelAllLayers(false,true);
       return true;
     }
  }
  else
  {
    return false; // Element not found, so no deletion at all.
  }
}

// Update the Database manager from the selected element.
bool ElementDatabaseArray::FindGetAllElementInfo(wxString GetElement, wxString GetGammaPeak, wxChoice* &ListElement, wxChoice* &ListGammaPeak, wxTextCtrl* &SetNumber, wxTextCtrl* &SetAbundance, wxTextCtrl* &SetMass, wxTextCtrl* &SetIsotopic, wxGrid* &SetEnergySigma, mpWindow* &SetEnergyPlot)
{
  // Clear old tables
  SetNumber->Clear();
  SetAbundance->Clear();
  SetMass->Clear();
  SetIsotopic->Clear();
  SetEnergySigma->ClearGrid();
  SetEnergyPlot->DelAllLayers(false,true);
  // Requires a non-empty database frist.
  if (this->GetCount()>0 && ResetElementGammaMenu(ListElement,ListGammaPeak))
   {
      // Check the Element Menu, get their position, and then refresh the Gamma Peak Menu, and obtain their adress.
      int ElementPosition,GammaPosition;
      ElementPosition = ListElement->FindString(GetElement);
      if(this->RebuildElementGammaMenu(ListElement,ListGammaPeak,ElementPosition))
       GammaPosition = ListGammaPeak->FindString(GetGammaPeak);
      else
       return false;

      if( ElementPosition != wxNOT_FOUND && GammaPosition != wxNOT_FOUND) // No need to refresh the pull-downs, and set them.
       {
        ListElement->SetSelection(ElementPosition);
        ListGammaPeak->SetSelection(GammaPosition);
       }
      else if( ElementPosition != wxNOT_FOUND && GammaPosition == wxNOT_FOUND) // Reset the Gamma Peak Menu to the first choice.
       {
        ListElement->SetSelection(ElementPosition);
        ListGammaPeak->SetSelection(0);
        GetGammaPeak = ListGammaPeak->GetString(0);
       }
      else // Reset both Menus to the first one, and return a false flag, which means an error.
       {
        ListElement->SetSelection(0);
        ListGammaPeak->SetSelection(0);
        return false;
       }

   // Find the element adress
     int ElementAdress = this->FindElement(GetElement,GetGammaPeak);
     if( ElementAdress == wxNOT_FOUND)
      return false;


   // Update the Database Manager element information
     SetNumber->SetValue(this->Item(ElementAdress).GetNumber());
     SetAbundance->SetValue(this->Item(ElementAdress).GetAbundance());
     SetMass->SetValue(this->Item(ElementAdress).GetAtomic());
     SetIsotopic->SetValue(this->Item(ElementAdress).GetIsotopic());
     for (int i=0; i<this->Item(ElementAdress).GetEnergy().GetCount(); i++)
     {
      wxString temp1 = this->Item(ElementAdress).GetEnergy().Item(i);
      wxString temp2 = this->Item(ElementAdress).GetSigma().Item(i);
      if (temp1.Len()>0 && temp2.Len()>0)
      {
      SetEnergySigma->SetCellValue(i,0,temp1);
      SetEnergySigma->SetCellValue(i,1,temp2);
      }
     }
    // Make the plot
    // Prepare data
        std::vector<double> ElementEnergy;
        std::vector<double> ElementSigma;
        double EnergyMin, EnergyMax, SigmaMin, SigmaMax;
        ElementEnergySigmaData *data = new ElementEnergySigmaData();
        data->ExportGridVector(SetEnergySigma,ElementEnergy,ElementSigma,EnergyMin,EnergyMax,SigmaMin,SigmaMax);
        data->SetData(ElementEnergy,ElementSigma);
         // Format the axes and plot
         mpLayer *axeEnergy = new mpScaleX(wxT("Energy"),mpALIGN_BORDER_BOTTOM,false);
         mpLayer *axeSigma = new mpScaleY(wxT("Cross-Section"),mpALIGN_BORDER_LEFT,false);
         mpLayer *plot = data;
         plot->SetContinuity(true);
         wxPen plotcolor(*wxBLUE, 1, wxSOLID);
         plot->SetPen(plotcolor);
         axeEnergy->SetDrawOutsideMargins(false);
         axeSigma->SetDrawOutsideMargins(false);
         plot->SetDrawOutsideMargins(false);
          // Draw the plot elements on selected frame
         SetEnergyPlot->DelAllLayers(false,true);
         SetEnergyPlot->AddLayer(axeEnergy);
         SetEnergyPlot->AddLayer(axeSigma);
         SetEnergyPlot->AddLayer(plot);
         SetEnergyPlot->Fit(EnergyMin,EnergyMax,SigmaMin,SigmaMax);
         return true;
   }
   else
   {
      return false;
   }
}


// Generate the two pull-downs menus from the current database. The Element Index will generate the gamma peak options from the selected Element.
bool ElementDatabaseArray::RebuildElementGammaMenu(wxChoice* &ElementList, wxChoice* &GammaPeakList, int SelectedElementIndex)
{
 if (this->GetCount() == 0)
 {
  ElementList->Clear();
  GammaPeakList->Clear();
  return false;
 }
 if(SelectedElementIndex < 0) // Recycle the Element menu
 {
  SelectedElementIndex = 0;
  ElementList->Clear();
  GammaPeakList->Clear();
  // Generate the Element menu
  for(int i=0; i<this->GetCount(); i++)
  {
  wxString ActualElementName = this->Item(i).GetElement();
  // Avoid Element Name duplications
  if (ElementList->FindString(ActualElementName) == wxNOT_FOUND )
      ElementList->Append(ActualElementName);
  }
 }
 else
 {
  GammaPeakList->Clear(); // It is not necessary to delete the Element Menu
 }
 // Get the Element name from the menu
  wxString SelectedElement = ElementList->GetString(SelectedElementIndex);
  if (SelectedElement.Len()>0)
  {
    for(int j=0; j<this->GetCount(); j++) // Generate the Gamma Peak menu
    {
      if(SelectedElement == this->Item(j).GetElement())
      {
        wxString ActualGammaPeak = this->Item(j).GetGamma();
       // Avoid Gamma Peak duplications, since it would trigger an error
       if (GammaPeakList->FindString(ActualGammaPeak) == wxNOT_FOUND )
        GammaPeakList->Append(ActualGammaPeak);
       else
        return false;
      }
    }
  }
  else // Undeclared Element
  {
    ElementList->Clear();
    GammaPeakList->Clear();
    return false;
  }
 return true;
}

// Get the array string of all elements on database
wxArrayString ElementDatabaseArray::GetAllListElements()
{
 wxArrayString AllListElements;
 AllListElements.Clear();
 for(int i=0; i<this->GetCount(); i++)
 {
  wxString ActualElementName = this->Item(i).GetElement();
  bool IsNewElement = true;
  for(int j=0; j<AllListElements.GetCount(); j++)
  {
   if(ActualElementName == AllListElements.Item(j))
     IsNewElement = false;
  }
  if(IsNewElement)
   AllListElements.Add(ActualElementName);
 }
 return AllListElements;
}

// Get the gamma peaks of the selected element
wxArrayString ElementDatabaseArray::GetAllListGammaPeaks(wxString SearchElement)
{
  wxArrayString AllListGammaPeaks;
  AllListGammaPeaks.Clear();
  for(int i=0; i<this->GetCount(); i++)
  {
  wxString ActualElementName = this->Item(i).GetElement();
   if(SearchElement == ActualElementName)
   {
    bool IsNewGammaPeak = true;
    wxString ActualGammaPeak = this->Item(i).GetGamma();
    for(int j=0; j<AllListGammaPeaks.GetCount(); j++)
    {
     if(ActualGammaPeak == AllListGammaPeaks.Item(j))
     IsNewGammaPeak = false;
    }
   if(IsNewGammaPeak)
     AllListGammaPeaks.Add(ActualGammaPeak);
   }
 }
 return AllListGammaPeaks;
}

// Detector Parameters main constructor
DetectorParameters::DetectorParameters(wxTextCtrl* textFunctionEficiency, wxGrid* tableConstantEficiency, wxString &info)
{
   DetectorFunction = textFunctionEficiency->GetValue();
   DetectorEnergy.Clear();
   DetectorEfficiency.Clear();
   infoDetector = info;
   for(int i=0;i<tableConstantEficiency->GetNumberRows();i++)
   {
   wxString temp0 = tableConstantEficiency->GetCellValue(i,0);
   wxString temp1 = tableConstantEficiency->GetCellValue(i,1);
   DetectorEnergy.Add(temp0);
   DetectorEfficiency.Add(temp1);
   }
}

// Export the current contents of the detector to the Detector Editor Controls
bool DetectorParameters::GetDisplay(wxTextCtrl* &SetDetectorFunction, wxGrid* &DetectorTable)
{
 SetDetectorFunction->SetValue(DetectorFunction);
 DetectorTable->ClearGrid();
 if (DetectorEnergy.GetCount() == DetectorEfficiency.GetCount())
 {
  for(int j=0; j<DetectorEnergy.GetCount(); j++)
  {
    DetectorTable->SetCellValue(j, 0, DetectorEnergy.Item(j));
    DetectorTable->SetCellValue(j, 1, DetectorEfficiency.Item(j));
  }
  return true;
 }
 else
 {
  return false;
 }
}

// Clear the contents of Detector Parameters
void DetectorParameters::Clear()
{
   DetectorFunction.Clear();
   DetectorEnergy.Clear();
   DetectorEfficiency.Clear();
   return;
}

// SRIM Table main constructor
ElementSRIM::ElementSRIM(int Number, wxGrid* tableSRIMData)
{
 Atomic = Number;
 ElementEnergy.Clear();
 ElementStoppingPower.Clear();
   for(int i=0;i<tableSRIMData->GetNumberRows();i++)
   {
   wxString temp0 = tableSRIMData->GetCellValue(i,0);
   wxString temp1 = tableSRIMData->GetCellValue(i,1);
   ElementEnergy.Add(temp0);
   ElementStoppingPower.Add(temp1);
   }
}

// Displays the selected table on the spreadsheet widget
bool ElementSRIM::GetDisplay(wxGrid* &tableSRIMData)
{
 if (ElementEnergy.GetCount() == ElementStoppingPower.GetCount())
 {
  for(int j=0; j<ElementEnergy.GetCount(); j++)
  {
    tableSRIMData->SetCellValue(j, 0, ElementEnergy.Item(j));
    tableSRIMData->SetCellValue(j, 1, ElementStoppingPower.Item(j));
  }
  return true;
 }
 else
 {
  return false;
 }
}

// Function related to several SRIM tables
bool ElementSRIMArray::SortArray()
{
  // Sorting routine
  int m = this->GetCount();
   for (int p=0; p<m-1; p++)
   {
     for(int q=0; q<m-p-1; q++)
     {
       if ( this->Item(q).GetElementAtomic() > this->Item(q+1).GetElementAtomic() )
       {
       ElementSRIM Temporary = this->Item(q);
       this->RemoveAt(q);
       this->Insert(Temporary,q+1);
       }
     }
   }
  // And return
  return true;
}

bool ElementSRIMArray::IfExist(int Number)
{
 for(int i=0; i<this->GetCount(); i++)
 {
   if(this->Item(i).GetElementAtomic() == Number)
    return true;
 }
 return false;
}

bool ElementSRIMArray::GetDisplay(int Number, wxGrid* &tableSRIMData)
{
 if(this->IfExist(Number))
 {
  int Adress = this->GetElementAdress(Number);
  return this->Item(Adress).GetDisplay(tableSRIMData);
 }
 else
 {
   return false;
 }
}

bool ElementSRIMArray::Append(int Number, wxGrid* tableSRIMData)
{
 if(this->IfExist(Number))
 {
  return false;
 }
 else
 {
  this->Add(ElementSRIM(Number,tableSRIMData));
  return this->SortArray();
 }
}

bool ElementSRIMArray::Replace(int Number, wxGrid* tableSRIMData)
{
 if(this->IfExist(Number))
 {
  if(this->Delete(Number))
  {
    this->Add(ElementSRIM(Number,tableSRIMData));
    return this->SortArray();
  }
  else
  {
    return false;
  }
 }
 else
 {
  this->Add(ElementSRIM(Number,tableSRIMData));
  return this->SortArray();
 }
}

bool ElementSRIMArray::Replace(int Number, wxArrayString Energy, wxArrayString StoppingPower)
{
 if(this->IfExist(Number))
 {
  if(this->Delete(Number))
  {
    this->Add(ElementSRIM(Number,Energy,StoppingPower));
    return this->SortArray();
  }
  else
  {
    return false;
  }
 }
 else
 {
  this->Add(ElementSRIM(Number,Energy,StoppingPower));
  return this->SortArray();
 }
}

bool ElementSRIMArray::Delete(int Number)
{
 if(this->IfExist(Number))
 {
  int k = this->GetElementAdress(Number);
  this->RemoveAt(k);
  return this->SortArray();
 }
 else
 {
  return false;
 }
}

int  ElementSRIMArray::GetElementAdress(int Number)
{
 for(int i=0; i<this->GetCount(); i++)
 {
   if(this->Item(i).GetElementAtomic() == Number)
    return i;
 }
 return wxNOT_FOUND;
}

// Ziegler Parameters main constructor
ZieglerParameters::ZieglerParameters( wxChoice* choiceZieglerVersion, wxGrid* tableZieglerParameters, wxString &info)
{
   infoZiegler = info;
   ZieglerVersion = wxString::Format("%d",choiceZieglerVersion->GetSelection());
   ZieglerElements.Clear();
   ZieglerValues1.Clear();
   ZieglerValues2.Clear();
   ZieglerValues3.Clear();
   ZieglerValues4.Clear();
   ZieglerValues5.Clear();
   ZieglerValues6.Clear();
   ZieglerValues7.Clear();
   ZieglerValues8.Clear();
   ZieglerValues9.Clear();
   ZieglerValues10.Clear();
   ZieglerValues11.Clear();
   ZieglerValues12.Clear();
   ZieglerMass.Clear();
   for(int i=0;i<tableZieglerParameters->GetNumberRows();i++)
   {
   wxString temp0 = tableZieglerParameters->GetCellValue(i,0);
   wxString temp1 = tableZieglerParameters->GetCellValue(i,1);
   wxString temp2 = tableZieglerParameters->GetCellValue(i,2);
   wxString temp3 = tableZieglerParameters->GetCellValue(i,3);
   wxString temp4 = tableZieglerParameters->GetCellValue(i,4);
   wxString temp5 = tableZieglerParameters->GetCellValue(i,5);
   wxString temp6 = tableZieglerParameters->GetCellValue(i,6);
   wxString temp7 = tableZieglerParameters->GetCellValue(i,7);
   wxString temp8 = tableZieglerParameters->GetCellValue(i,8);
   wxString temp9 = tableZieglerParameters->GetCellValue(i,9);
   wxString temp10 = tableZieglerParameters->GetCellValue(i,10);
   wxString temp11 = tableZieglerParameters->GetCellValue(i,11);
   wxString temp12 = tableZieglerParameters->GetCellValue(i,12);
   wxString temp13 = tableZieglerParameters->GetCellValue(i,13);
   ZieglerElements.Add(temp0);
   ZieglerValues1.Add(temp1);
   ZieglerValues2.Add(temp2);
   ZieglerValues3.Add(temp3);
   ZieglerValues4.Add(temp4);
   ZieglerValues5.Add(temp5);
   ZieglerValues6.Add(temp6);
   ZieglerValues7.Add(temp7);
   ZieglerValues8.Add(temp8);
   ZieglerValues9.Add(temp9);
   ZieglerValues10.Add(temp10);
   ZieglerValues11.Add(temp11);
   ZieglerValues12.Add(temp12);
   ZieglerMass.Add(temp13);
   }
}

// Ziegler Parameters main constructor
ZieglerParameters::ZieglerParameters( wxTextCtrl* textZieglerFunction, wxChoice* choiceZieglerVersion, wxGrid* tableZieglerParameters, wxString &info)
{
   infoZiegler = info;
   ZieglerVersion = wxString::Format("%d",choiceZieglerVersion->GetSelection());
   ZieglerFunction = textZieglerFunction->GetValue();
   ZieglerElements.Clear();
   ZieglerValues1.Clear();
   ZieglerValues2.Clear();
   ZieglerValues3.Clear();
   ZieglerValues4.Clear();
   ZieglerValues5.Clear();
   ZieglerValues6.Clear();
   ZieglerValues7.Clear();
   ZieglerValues8.Clear();
   ZieglerValues9.Clear();
   ZieglerValues10.Clear();
   ZieglerValues11.Clear();
   ZieglerValues12.Clear();
   ZieglerMass.Clear();
   for(int i=0;i<tableZieglerParameters->GetNumberRows();i++)
   {
   wxString temp0 = tableZieglerParameters->GetCellValue(i,0);
   wxString temp1 = tableZieglerParameters->GetCellValue(i,1);
   wxString temp2 = tableZieglerParameters->GetCellValue(i,2);
   wxString temp3 = tableZieglerParameters->GetCellValue(i,3);
   wxString temp4 = tableZieglerParameters->GetCellValue(i,4);
   wxString temp5 = tableZieglerParameters->GetCellValue(i,5);
   wxString temp6 = tableZieglerParameters->GetCellValue(i,6);
   wxString temp7 = tableZieglerParameters->GetCellValue(i,7);
   wxString temp8 = tableZieglerParameters->GetCellValue(i,8);
   wxString temp9 = tableZieglerParameters->GetCellValue(i,9);
   wxString temp10 = tableZieglerParameters->GetCellValue(i,10);
   wxString temp11 = tableZieglerParameters->GetCellValue(i,11);
   wxString temp12 = tableZieglerParameters->GetCellValue(i,12);
   wxString temp13 = tableZieglerParameters->GetCellValue(i,13);
   ZieglerElements.Add(temp0);
   ZieglerValues1.Add(temp1);
   ZieglerValues2.Add(temp2);
   ZieglerValues3.Add(temp3);
   ZieglerValues4.Add(temp4);
   ZieglerValues5.Add(temp5);
   ZieglerValues6.Add(temp6);
   ZieglerValues7.Add(temp7);
   ZieglerValues8.Add(temp8);
   ZieglerValues9.Add(temp9);
   ZieglerValues10.Add(temp10);
   ZieglerValues11.Add(temp11);
   ZieglerValues12.Add(temp12);
   ZieglerMass.Add(temp13);
   }
}



// Add a new element for the Ziegler Parameters
void ZieglerParameters::AddZieglerParameterData(wxString e, wxString v1, wxString v2, wxString v3, wxString v4, wxString v5, wxString v6, wxString v7, wxString v8, wxString v9, wxString v10, wxString v11, wxString v12, wxString m)
{
 ZieglerElements.Add(e);
 ZieglerValues1.Add(v1);
 ZieglerValues2.Add(v2);
 ZieglerValues3.Add(v3);
 ZieglerValues4.Add(v4);
 ZieglerValues5.Add(v5);
 ZieglerValues6.Add(v6);
 ZieglerValues7.Add(v7);
 ZieglerValues8.Add(v8);
 ZieglerValues9.Add(v9);
 ZieglerValues10.Add(v10);
 ZieglerValues11.Add(v11);
 ZieglerValues12.Add(v12);
 ZieglerMass.Add(m);
 return;
}

// Export the current contents of the Ziegler to the Ziegler Editor Controls
bool ZieglerParameters::GetDisplay(wxChoice* &CurrentZieglerVersion, wxGrid* &ZieglerTable)
{
  ZieglerFunction.Clear();
  long z;
  if(ZieglerVersion.ToLong(&z))
   CurrentZieglerVersion->SetSelection(z);
  else
   return false;
  ZieglerTable->ClearGrid();
  if(ZieglerElements.GetCount() > ZieglerTable->GetNumberRows())
    return false;
  for(int i=0;i<ZieglerElements.GetCount();i++)
   {
    ZieglerTable->SetCellValue(i,0,ZieglerElements.Item(i));
    ZieglerTable->SetCellValue(i,1,ZieglerValues1.Item(i));
    ZieglerTable->SetCellValue(i,2,ZieglerValues2.Item(i));
    ZieglerTable->SetCellValue(i,3,ZieglerValues3.Item(i));
    ZieglerTable->SetCellValue(i,4,ZieglerValues4.Item(i));
    ZieglerTable->SetCellValue(i,5,ZieglerValues5.Item(i));
    ZieglerTable->SetCellValue(i,6,ZieglerValues6.Item(i));
    ZieglerTable->SetCellValue(i,7,ZieglerValues7.Item(i));
    ZieglerTable->SetCellValue(i,8,ZieglerValues8.Item(i));
    ZieglerTable->SetCellValue(i,9,ZieglerValues9.Item(i));
    ZieglerTable->SetCellValue(i,10,ZieglerValues10.Item(i));
    ZieglerTable->SetCellValue(i,11,ZieglerValues11.Item(i));
    ZieglerTable->SetCellValue(i,12,ZieglerValues12.Item(i));
    ZieglerTable->SetCellValue(i,13,ZieglerMass.Item(i));
   }
  return true;
}

// Export the current contents of the Ziegler to the Ziegler Editor Controls, including a custom function
bool ZieglerParameters::GetDisplay(wxTextCtrl* &textZieglerFunction, wxChoice* &CurrentZieglerVersion, wxGrid* &ZieglerTable)
{
  textZieglerFunction->SetValue(ZieglerFunction);
  long z;
  if(ZieglerVersion.ToLong(&z))
   CurrentZieglerVersion->SetSelection(z);
  else
   return false;
  ZieglerTable->ClearGrid();
  if(ZieglerElements.GetCount() > ZieglerTable->GetNumberRows())
    return false;
  for(int i=0;i<ZieglerElements.GetCount();i++)
   {
    ZieglerTable->SetCellValue(i,0,ZieglerElements.Item(i));
    ZieglerTable->SetCellValue(i,1,ZieglerValues1.Item(i));
    ZieglerTable->SetCellValue(i,2,ZieglerValues2.Item(i));
    ZieglerTable->SetCellValue(i,3,ZieglerValues3.Item(i));
    ZieglerTable->SetCellValue(i,4,ZieglerValues4.Item(i));
    ZieglerTable->SetCellValue(i,5,ZieglerValues5.Item(i));
    ZieglerTable->SetCellValue(i,6,ZieglerValues6.Item(i));
    ZieglerTable->SetCellValue(i,7,ZieglerValues7.Item(i));
    ZieglerTable->SetCellValue(i,8,ZieglerValues8.Item(i));
    ZieglerTable->SetCellValue(i,9,ZieglerValues9.Item(i));
    ZieglerTable->SetCellValue(i,10,ZieglerValues10.Item(i));
    ZieglerTable->SetCellValue(i,11,ZieglerValues11.Item(i));
    ZieglerTable->SetCellValue(i,12,ZieglerValues12.Item(i));
    ZieglerTable->SetCellValue(i,13,ZieglerMass.Item(i));
   }
  return true;
}

// Clear all contents of Ziegler Parameters
void ZieglerParameters::Clear()
{
   ZieglerElements.Clear();
   ZieglerValues1.Clear();
   ZieglerValues2.Clear();
   ZieglerValues3.Clear();
   ZieglerValues4.Clear();
   ZieglerValues5.Clear();
   ZieglerValues6.Clear();
   ZieglerValues7.Clear();
   ZieglerValues8.Clear();
   ZieglerValues9.Clear();
   ZieglerValues10.Clear();
   ZieglerValues11.Clear();
   ZieglerValues12.Clear();
   ZieglerMass.Clear();
   ZieglerVersion.Clear();
   return;
}

bool ZieglerParameters::FixElementNames()
{
 //Internal elements dictionary
 wxArrayString ListElementNames;
 ListElementNames.Add(wxT("H")); ListElementNames.Add(wxT("He")); ListElementNames.Add(wxT("Li")); ListElementNames.Add(wxT("Be")); ListElementNames.Add(wxT("B")); ListElementNames.Add(wxT("C"));
 ListElementNames.Add(wxT("N")); ListElementNames.Add(wxT("O")); ListElementNames.Add(wxT("F")); ListElementNames.Add(wxT("Ne")); ListElementNames.Add(wxT("Na")); ListElementNames.Add(wxT("Mg"));
 ListElementNames.Add(wxT("Al")); ListElementNames.Add(wxT("Si")); ListElementNames.Add(wxT("P")); ListElementNames.Add(wxT("S")); ListElementNames.Add(wxT("Cl")); ListElementNames.Add(wxT("Ar"));
 ListElementNames.Add(wxT("K")); ListElementNames.Add(wxT("Ca")); ListElementNames.Add(wxT("Sc")); ListElementNames.Add(wxT("Ti")); ListElementNames.Add(wxT("V")); ListElementNames.Add(wxT("Cr"));
 ListElementNames.Add(wxT("Mn")); ListElementNames.Add(wxT("Fe")); ListElementNames.Add(wxT("Co")); ListElementNames.Add(wxT("Ni")); ListElementNames.Add(wxT("Cu")); ListElementNames.Add(wxT("Zn"));
 ListElementNames.Add(wxT("Ga")); ListElementNames.Add(wxT("Ge")); ListElementNames.Add(wxT("As")); ListElementNames.Add(wxT("Se")); ListElementNames.Add(wxT("Br")); ListElementNames.Add(wxT("Kr"));
 ListElementNames.Add(wxT("Rb")); ListElementNames.Add(wxT("Sr")); ListElementNames.Add(wxT("Y")); ListElementNames.Add(wxT("Zr")); ListElementNames.Add(wxT("Nb")); ListElementNames.Add(wxT("Mo"));
 ListElementNames.Add(wxT("Tc")); ListElementNames.Add(wxT("Ru")); ListElementNames.Add(wxT("Rh")); ListElementNames.Add(wxT("Pd")); ListElementNames.Add(wxT("Ag")); ListElementNames.Add(wxT("Cd"));
 ListElementNames.Add(wxT("In")); ListElementNames.Add(wxT("Sn")); ListElementNames.Add(wxT("Sb")); ListElementNames.Add(wxT("Te")); ListElementNames.Add(wxT("I")); ListElementNames.Add(wxT("Xe"));
 ListElementNames.Add(wxT("Cs")); ListElementNames.Add(wxT("Ba")); ListElementNames.Add(wxT("La")); ListElementNames.Add(wxT("Ce")); ListElementNames.Add(wxT("Pr")); ListElementNames.Add(wxT("Nd"));
 ListElementNames.Add(wxT("Pm")); ListElementNames.Add(wxT("Sm")); ListElementNames.Add(wxT("Eu")); ListElementNames.Add(wxT("Gd")); ListElementNames.Add(wxT("Tb")); ListElementNames.Add(wxT("Dy"));
 ListElementNames.Add(wxT("Ho")); ListElementNames.Add(wxT("Er")); ListElementNames.Add(wxT("Tm")); ListElementNames.Add(wxT("Yb")); ListElementNames.Add(wxT("Lu")); ListElementNames.Add(wxT("Hf"));
 ListElementNames.Add(wxT("Ta")); ListElementNames.Add(wxT("W")); ListElementNames.Add(wxT("Re")); ListElementNames.Add(wxT("Os")); ListElementNames.Add(wxT("Ir")); ListElementNames.Add(wxT("Pt"));
 ListElementNames.Add(wxT("Au")); ListElementNames.Add(wxT("Hg")); ListElementNames.Add(wxT("Tl")); ListElementNames.Add(wxT("Pb")); ListElementNames.Add(wxT("Bi")); ListElementNames.Add(wxT("Po"));
 ListElementNames.Add(wxT("At")); ListElementNames.Add(wxT("Rn")); ListElementNames.Add(wxT("Fr")); ListElementNames.Add(wxT("Ra")); ListElementNames.Add(wxT("Ac")); ListElementNames.Add(wxT("Th"));
 ListElementNames.Add(wxT("Pa")); ListElementNames.Add(wxT("U")); ListElementNames.Add(wxT("Np")); ListElementNames.Add(wxT("Pu")); ListElementNames.Add(wxT("Am")); ListElementNames.Add(wxT("Cm"));
 ListElementNames.Add(wxT("Bk")); ListElementNames.Add(wxT("Cf")); ListElementNames.Add(wxT("Es")); ListElementNames.Add(wxT("Fm")); ListElementNames.Add(wxT("Md")); ListElementNames.Add(wxT("No"));
 ListElementNames.Add(wxT("Lr")); ListElementNames.Add(wxT("Rf")); ListElementNames.Add(wxT("Db")); ListElementNames.Add(wxT("Sg")); ListElementNames.Add(wxT("Bh")); ListElementNames.Add(wxT("Hs"));
 ListElementNames.Add(wxT("Mt")); ListElementNames.Add(wxT("Ds")); ListElementNames.Add(wxT("Rg")); ListElementNames.Add(wxT("Cn")); ListElementNames.Add(wxT("Nh")); ListElementNames.Add(wxT("Fl"));
 ListElementNames.Add(wxT("Mc")); ListElementNames.Add(wxT("Lv")); ListElementNames.Add(wxT("Ts")); ListElementNames.Add(wxT("Og"));
 for(int k=0; k<ZieglerElements.GetCount(); k++)
 {
  if(k<ListElementNames.GetCount())
   ZieglerElements.Item(k) = ListElementNames.Item(k);
 }
 return true;
}


