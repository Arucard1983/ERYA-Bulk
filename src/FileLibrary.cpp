/***************************************************************
 * Name:      FileLibrary.cpp
 * Purpose:   ERYA file format for the Results(*.epz), Database (*.epd), Detector (*.epsd) and Ziegler(*.epsz) implementation
 * Author:    Vasco Manteigas (vm.manteigas@campus.fct.utl.pt)
 * Created:   2016-10-01
 * Copyright: Grupo de Reacoes Nucleares - LIBPHYS
 * License:
 **************************************************************/

#include "FileLibrary.h"


// Load a ERYA Yield file (*.epz)
bool ERYAPIXEFile::ERYAPIXEFileLoad(wxTextCtrl* &MinimumEnergy, wxTextCtrl* &MaximumEnergy, wxTextCtrl* &EnergyStep, wxTextCtrl* &ProfilingStep, wxTextCtrl* &Charge, wxTextCtrl* &Thickness, wxArrayString& choiceElement, wxArrayString& choiceGP, wxArrayString& checkFit, wxArrayString& textZ, wxArrayString& textCP, wxArrayString& textSG, wxArrayString& textYS, wxArrayString& textYE, wxArrayString& textYF, wxArrayString& textSF, wxArrayString& textSM, wxArrayString& textSE, wxArrayString& TableData, ElementDatabaseArray CurrentDatabase, int&CI, int& MI, int& LT, int& LS, int& LY)
{
  // Begin processing
  TableData.Clear();
  wxArrayString ListLabels,ListElements,ListGammaPeaks;
  wxXmlDocument LocalERYAPIXEFile;
  if(!(LocalERYAPIXEFile.Load(ERYAPIXEFileName)))
  {
    wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Unexpected file format of ") + ERYAPIXEFileName, wxT("ERYA-Bulk Yield File Loading Error"), wxOK | wxICON_ERROR);
    dial->ShowModal();
    return false;
  }
  if(LocalERYAPIXEFile.GetRoot()->GetName() != wxT("ERYA-Bulk_Yields"))
  {
    wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Unexpected file format of ") + ERYAPIXEFileName, wxT("ERYA-Bulk Yield File Loading Error"), wxOK | wxICON_ERROR);
    dial->ShowModal();
    return false;
  }
  wxXmlNode *YieldNode = LocalERYAPIXEFile.GetRoot()->GetChildren();
  while(YieldNode)
  {
    if(YieldNode->GetName() == wxT("Yield_Parameters")) // Get the global parameters
    {
     wxXmlNode *YieldGlobal = YieldNode->GetChildren();
     while(YieldGlobal)
     {
      if(YieldGlobal->GetName() == wxT("Detector_Parameters"))
      {
       wxString c0 = YieldGlobal->GetAttribute(wxT("Minimum_Energy"),wxT("0"));
       wxString c1 = YieldGlobal->GetAttribute(wxT("Maximum_Energy"),wxT("1"));
       wxString c2 = YieldGlobal->GetAttribute(wxT("Energy_Step"),wxT("1"));
       wxString c3 = YieldGlobal->GetAttribute(wxT("Profiling_Step"),wxT("0"));
       wxString c4 = YieldGlobal->GetAttribute(wxT("Charge"),wxT("1"));
       wxString c5 = YieldGlobal->GetAttribute(wxT("Thickness"),wxT("1"));
       wxString c6 = YieldGlobal->GetAttribute(wxT("Current_Iterations"),wxT("0"));
       wxString c7 = YieldGlobal->GetAttribute(wxT("Maximum_Iterations"),wxT("100"));
       wxString c8 = YieldGlobal->GetAttribute(wxT("Decimal_Damping_Factor"),wxT("3"));
       wxString c9 = YieldGlobal->GetAttribute(wxT("Decimal_Stoichiometry_Precision"),wxT("3"));
       wxString c10 = YieldGlobal->GetAttribute(wxT("Decimal_Yield_Precision"),wxT("3"));
       MinimumEnergy->SetValue(c0);
       MaximumEnergy->SetValue(c1);
       EnergyStep->SetValue(c2);
       ProfilingStep->SetValue(c3);
       Charge->SetValue(c4);
       Thickness->SetValue(c5);
       long CurrentIterations,MaximumIterations,LogTau,LogStoichiometry,LogYield;
       c6.ToLong(&CurrentIterations);
       c7.ToLong(&MaximumIterations);
       c8.ToLong(&LogTau);
       c9.ToLong(&LogStoichiometry);
       c10.ToLong(&LogYield);
       CI = CurrentIterations;
       MI = MaximumIterations;
       LT = LogTau;
       LS = LogStoichiometry;
       LY = LogYield;
      }
     YieldGlobal = YieldGlobal->GetNext();
     }
    }
   if(YieldNode->GetName() == wxT("Yield_Data")) // Get the element registers
   {
     wxXmlNode *YieldData = YieldNode->GetChildren();
     while(YieldData)
     {
     if(YieldData->GetName() == wxT("Item"))
     {
       wxString d0,d1,d2,d3,d4,d5,d6,d7,d8,d9,d10,d11;
       // Set default values, for compability of previous ERYA versions;
       d0 = wxT("0");
       d1 = wxT("0");
       d2 = wxT("0");
       d3 = wxT("0");
       d4 = wxT("1");
       d5 = wxT("1");
       d6 = wxT("0");
       d7 = wxT("0");
       d8 = wxT("0");
       d9 = wxT("0");
       d10 = wxT("0");
       d11 = wxT("0");
       // Reading actual values
       wxXmlNode *YieldDataRegister = YieldData->GetChildren();
       while(YieldDataRegister)
       {
       if(YieldDataRegister->GetName() == wxT("Element_Name"))
       {
        wxXmlNode *Register0 = YieldDataRegister->GetChildren();
         if(Register0->GetName() == wxT("value"))
            {
                d0 = Register0->GetNodeContent();
                if(d0.Len() == 0)
                {
                 wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Unexpected incompatible data on file: ") + ERYAPIXEFileName, wxT("ERYA-Bulk Yield File Loading Error"), wxOK | wxICON_ERROR);
                 dial->ShowModal();
                 return false;
                }
            }
       }
       if(YieldDataRegister->GetName() == wxT("Gamma_Peak"))
       {
         wxXmlNode *Register1 = YieldDataRegister->GetChildren();
         if(Register1->GetName() == wxT("value"))
            {
                d1 = Register1->GetNodeContent();
                if(d1.Len() == 0)
                {
                 wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Unexpected incompatible data on file: ") + ERYAPIXEFileName, wxT("ERYA-Bulk Yield File Loading Error"), wxOK | wxICON_ERROR);
                 dial->ShowModal();
                 return false;
                }
            }
       }
       if(YieldDataRegister->GetName() == wxT("Fit_Applied"))
       {
         wxXmlNode *Register2 = YieldDataRegister->GetChildren();
         if(Register2->GetName() == wxT("value"))
            {
                d2 = Register2->GetNodeContent();
                if(d2.Len() == 0)
                {
                  d2 = wxT("0");
                }
            }
       }
       if(YieldDataRegister->GetName() == wxT("Group_Number"))
       {
         wxXmlNode *Register3 = YieldDataRegister->GetChildren();
         if(Register3->GetName() == wxT("value"))
            {
                d3 = Register3->GetNodeContent();
                if(d3.Len() == 0)
                {
                  d3 = wxT("0");
                }
            }
       }
       if(YieldDataRegister->GetName() == wxT("Calibration_Parameter"))
       {
         wxXmlNode *Register4 = YieldDataRegister->GetChildren();
         if(Register4->GetName() == wxT("value"))
            {
                d4 = Register4->GetNodeContent();
                if(d4.Len() == 0)
                {
                  d4 = wxT("1");
                }
            }
       }
       if(YieldDataRegister->GetName() == wxT("Stoichiometric_Guess"))
       {
         wxXmlNode *Register5 = YieldDataRegister->GetChildren();
         if(Register5->GetName() == wxT("value"))
            {
                d5 = Register5->GetNodeContent();
                if(d5.Len() == 0)
                {
                  d5 = wxT("1");
                }
            }
       }
       if(YieldDataRegister->GetName() == wxT("Yield_Simulated"))
       {
         wxXmlNode *Register6 = YieldDataRegister->GetChildren();
         if(Register6->GetName() == wxT("value"))
            {
                d6 = Register6->GetNodeContent();
                if(d6.Len() == 0)
                {
                  d6 = wxT("0");
                }
            }
       }
       if(YieldDataRegister->GetName() == wxT("Yield_Experimental"))
       {
         wxXmlNode *Register7 = YieldDataRegister->GetChildren();
         if(Register7->GetName() == wxT("value"))
            {
                d7 = Register7->GetNodeContent();
                if(d7.Len() == 0)
                {
                  d7 = wxT("0");
                }
            }
       }
       if(YieldDataRegister->GetName() == wxT("Yield_Fitted"))
       {
         wxXmlNode *Register8 = YieldDataRegister->GetChildren();
         if(Register8->GetName() == wxT("value"))
            {
                d8 = Register8->GetNodeContent();
                if(d8.Len() == 0)
                {
                  d8 = wxT("0");
                }
            }
       }
       if(YieldDataRegister->GetName() == wxT("Stoichiometric_Fitted"))
       {
         wxXmlNode *Register9 = YieldDataRegister->GetChildren();
         if(Register9->GetName() == wxT("value"))
            {
                d9 = Register9->GetNodeContent();
                if(d9.Len() == 0)
                {
                  d9 = wxT("0");
                }
            }
       }
       if(YieldDataRegister->GetName() == wxT("Stoichiometric_Fit_Mass"))
       {
         wxXmlNode *Register10 = YieldDataRegister->GetChildren();
         if(Register10->GetName() == wxT("value"))
            {
                d10 = Register10->GetNodeContent();
                if(d10.Len() == 0)
                {
                  d10 = wxT("0");
                }
            }
       }
       if(YieldDataRegister->GetName() == wxT("Stoichiometric_Fit_Error"))
       {
         wxXmlNode *Register11 = YieldDataRegister->GetChildren();
         if(Register11->GetName() == wxT("value"))
            {
                d11 = Register11->GetNodeContent();
                if(d11.Len() == 0)
                {
                  d11 = wxT("0");
                }
            }
       }
        YieldDataRegister = YieldDataRegister->GetNext();
      }
      // Check the element presence
      if(CurrentDatabase.FindElement(d0,d1) == wxNOT_FOUND)
        {
         wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Unknown element ") + d0 + wxT(" and gamma ") + d1 + wxT("\nWhile load the file: ") +  ERYAPIXEFileName, wxT("ERYA-Bulk Yield File Loading Error"), wxOK | wxICON_ERROR);
         dial->ShowModal();
         return false;
        }
      // Add to the output
      choiceElement.Add(d0);
      choiceGP.Add(d1);
      checkFit.Add(d2);
      textZ.Add(d3);
      textCP.Add(d4);
      textSG.Add(d5);
      textYS.Add(d6);
      textYE.Add(d7);
      textYF.Add(d8);
      textSF.Add(d9);
      textSM.Add(d10);
      textSE.Add(d11);
     }
     YieldData = YieldData->GetNext(); // Continue to the next register
   }
   }
    if(YieldNode->GetName() == wxT("Profiling_Data")) //Main data entry point
    {
     wxXmlNode *LayerData = YieldNode->GetChildren();
     while(LayerData)
     {
      if(LayerData->GetName() == wxT("Profiling_Labels")) //Get the table labels
      {
        wxXmlNode *LayerLabels = LayerData->GetChildren();
        while(LayerLabels)
        {
          if(LayerLabels->GetName() == wxT("label"))
          {
            wxString LayerCount = LayerLabels->GetAttribute(wxT("n"),wxT("0"));
            long LayerValue;
            if(LayerCount.ToLong(&LayerValue))
            {
              if(LayerValue > 0)
              {
              wxString LayerString;
              wxXmlNode *LayerStringValue = LayerLabels->GetChildren();
              if(LayerStringValue->GetName() == wxT("value"))
               {
                LayerString = LayerStringValue->GetNodeContent();
               }
              // The first column are reserved for the units, so the Element checking are skipped.
               if(LayerValue > 1)
               {
                 wxArrayString SpecialSymbols;
                 SpecialSymbols.Add("(");
                 SpecialSymbols.Add(")");
                 TextLineParser CurrentLabelData(LayerString, SpecialSymbols);
                 if(CurrentDatabase.FindElement(CurrentLabelData.GetUnexcluded().Item(1),CurrentLabelData.GetUnexcluded().Item(2)) == wxNOT_FOUND)
                 {
                  wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Unknown Element \"") + CurrentLabelData.GetUnexcluded().Item(1) + wxT("\" and Gamma Peak") + CurrentLabelData.GetUnexcluded().Item(2) + wxT("\" keV, \nPlease check if you load the correct Element Database, and try again!"), wxT("Layer Profiling File Loading Error"), wxOK | wxICON_ERROR);
                  dial->ShowModal();
                  return false;
                 }
                 else
                 {
                  ListElements.Add(CurrentLabelData.GetUnexcluded().Item(1));
                  ListGammaPeaks.Add(CurrentLabelData.GetUnexcluded().Item(2));
                  ListLabels.Add(LayerString);
                 }
               }
               else
               {
                 ListLabels.Add(wxT("Energy (keV)"));
               }
              }
              else
              {
               wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Invalid Table Labels!"), wxT("ERYA-Bulk File Loading Error"), wxOK | wxICON_ERROR);
               dial->ShowModal();
               return false;
              }
            }
            else
            {
             wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Invalid Table Labels!"), wxT("ERYA-Bulk File Loading Error"), wxOK | wxICON_ERROR);
             dial->ShowModal();
             return false;
            }
          }
         LayerLabels = LayerLabels->GetNext();
        }
      }
      if(LayerData->GetName() == wxT("Profiling_Table")) //Load the table data itself
      {
        wxXmlNode *LayerRows = LayerData->GetChildren();
        while(LayerRows)
        {
          if(LayerRows->GetName() == wxT("Profiling"))
          {
            wxString LayerCount = LayerRows->GetAttribute(wxT("n"),wxT("0"));
            long LayerValue;
            if(!(LayerCount.ToLong(&LayerValue)))
            {
             wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Invalid Table Dimension!"), wxT("ERYA-Bulk File Loading Error"), wxOK | wxICON_ERROR);
             dial->ShowModal();
             return false;
            }
            wxXmlNode *LayerCols = LayerRows->GetChildren();
            while(LayerCols)
            {
             if(LayerCols->GetName() == wxT("element"))
             {
               wxString ElementCount = LayerCols->GetAttribute(wxT("n"),wxT("0"));
               long ElementValue;
               if(ElementCount.ToLong(&ElementValue))
               {
                wxString ElementString;
                wxXmlNode *ElementStringValue = LayerCols->GetChildren();
                 if(ElementStringValue->GetName() == wxT("value"))
                 {
                  ElementString = ElementStringValue->GetNodeContent();
                  TableData.Add(ElementString);
                 }
               }
               else
               {
               wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Invalid Table Dimension!"), wxT("ERYA-Bulk File Loading Error"), wxOK | wxICON_ERROR);
               dial->ShowModal();
               return false;
               }
             }
             LayerCols = LayerCols->GetNext();
            }
          }
         LayerRows = LayerRows->GetNext();
        }
      }
     LayerData = LayerData->GetNext();
     }
    }
   YieldNode = YieldNode->GetNext();
  }
 return true;
}

// Save an ERYA Yield file (*.epz)
bool ERYAPIXEFile::ERYAPIXEFileSave(wxTextCtrl* MinimumEnergy, wxTextCtrl* MaximumEnergy, wxTextCtrl* EnergyStep, wxTextCtrl* ProfilingStep, wxTextCtrl* Charge, wxTextCtrl* Thickness, ArrayElement choiceElement, ArrayGP choiceGP, ArrayFit checkFit, ArrayZ textZ, ArrayCP textCP, ArraySG textSG, ArrayYS textYS, ArrayYE textYE, ArrayYF textYF, ArraySF textSF, ArraySM textSM, ArraySE textSE, wxGrid* TableData, int CI, int MI, int LT, int LS, int LY)
{
  // Get the current time
  wxDateTime ThisTime = wxDateTime::Now();
  wxString ActualTime =ThisTime.Format(wxT("%Y-%m-%dT%H:%M:%S.00Z"),wxDateTime::WET);
  // Get some global variables
  int ProfilingNumberLayers = TableData->GetNumberRows();
  int ProfilingNumberElements = TableData->GetNumberCols();
  // An xml file when written, all node are declared backwarks, but any atribute of the same node should be declared forwards
  wxXmlDocument LocalERYAPIXE;
  wxXmlNode* yield = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, "ERYA-Bulk_Yields");
  LocalERYAPIXE.SetRoot(yield);
  wxXmlNode* data = new wxXmlNode(yield, wxXML_ELEMENT_NODE, "Yield_Data");
  for(int i=0; i<choiceElement.GetCount(); i++)
  {
   // Some additional preparations
   int CurrentElement = choiceElement.GetCount()-i;
   int ElementSlot = choiceElement.Item(CurrentElement-1)->GetSelection();
   int GammaPeakSlot = choiceGP.Item(CurrentElement-1)->GetSelection();
   wxString AnswerFit;
       if(checkFit.Item(CurrentElement-1)->GetValue())
           AnswerFit = wxT("1");
        else
           AnswerFit = wxT("0");
   wxXmlNode* dataitem = new wxXmlNode(data, wxXML_ELEMENT_NODE, "Item");
   dataitem->AddAttribute(wxT("number"),wxString::Format("%i",CurrentElement));
    wxXmlNode* dataSE = new wxXmlNode(dataitem, wxXML_ELEMENT_NODE, "Stoichiometric_Fit_Error");
     wxXmlNode* dataSEvalue = new wxXmlNode(dataSE, wxXML_ELEMENT_NODE, "value");
       dataSEvalue->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, textSE.Item(CurrentElement-1)->GetValue()));
    wxXmlNode* dataSM = new wxXmlNode(dataitem, wxXML_ELEMENT_NODE, "Stoichiometric_Fit_Mass");
     wxXmlNode* dataSMvalue = new wxXmlNode(dataSM, wxXML_ELEMENT_NODE, "value");
       dataSMvalue->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, textSM.Item(CurrentElement-1)->GetValue()));
    wxXmlNode* dataSF = new wxXmlNode(dataitem, wxXML_ELEMENT_NODE, "Stoichiometric_Fitted");
     wxXmlNode* dataSFvalue = new wxXmlNode(dataSF, wxXML_ELEMENT_NODE, "value");
       dataSFvalue->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, textSF.Item(CurrentElement-1)->GetValue()));
    wxXmlNode* dataYF = new wxXmlNode(dataitem, wxXML_ELEMENT_NODE, "Yield_Fitted");
     wxXmlNode* dataYFvalue = new wxXmlNode(dataYF, wxXML_ELEMENT_NODE, "value");
       dataYFvalue->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, textYF.Item(CurrentElement-1)->GetValue()));
    wxXmlNode* dataYE = new wxXmlNode(dataitem, wxXML_ELEMENT_NODE, "Yield_Experimental");
     wxXmlNode* dataYEvalue = new wxXmlNode(dataYE, wxXML_ELEMENT_NODE, "value");
       dataYEvalue->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, textYE.Item(CurrentElement-1)->GetValue()));
    wxXmlNode* dataYS = new wxXmlNode(dataitem, wxXML_ELEMENT_NODE, "Yield_Simulated");
     wxXmlNode* dataYSvalue = new wxXmlNode(dataYS, wxXML_ELEMENT_NODE, "value");
       dataYSvalue->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, textYS.Item(CurrentElement-1)->GetValue()));
    wxXmlNode* dataSG = new wxXmlNode(dataitem, wxXML_ELEMENT_NODE, "Stoichiometric_Guess");
     wxXmlNode* dataSGvalue = new wxXmlNode(dataSG, wxXML_ELEMENT_NODE, "value");
       dataSGvalue->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, textSG.Item(CurrentElement-1)->GetValue()));
    wxXmlNode* dataCP = new wxXmlNode(dataitem, wxXML_ELEMENT_NODE, "Calibration_Parameter");
     wxXmlNode* dataCPvalue = new wxXmlNode(dataCP, wxXML_ELEMENT_NODE, "value");
       dataCPvalue->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, textCP.Item(CurrentElement-1)->GetValue()));
    wxXmlNode* dataZ = new wxXmlNode(dataitem, wxXML_ELEMENT_NODE, "Group_Number");
     wxXmlNode* dataZvalue = new wxXmlNode(dataZ, wxXML_ELEMENT_NODE, "value");
       dataZvalue->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, textZ.Item(CurrentElement-1)->GetValue()));
    wxXmlNode* dataFit = new wxXmlNode(dataitem, wxXML_ELEMENT_NODE, "Fit_Applied");
     wxXmlNode* dataFitvalue = new wxXmlNode(dataFit, wxXML_ELEMENT_NODE, "value");
       dataFitvalue->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, AnswerFit));
    wxXmlNode* dataGP = new wxXmlNode(dataitem, wxXML_ELEMENT_NODE, "Gamma_Peak");
     wxXmlNode* dataGPvalue = new wxXmlNode(dataGP, wxXML_ELEMENT_NODE, "value");
       dataGPvalue->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, choiceGP.Item(CurrentElement-1)->GetString(GammaPeakSlot)));
    wxXmlNode* dataEN = new wxXmlNode(dataitem, wxXML_ELEMENT_NODE, "Element_Name");
     wxXmlNode* dataENvalue = new wxXmlNode(dataEN, wxXML_ELEMENT_NODE, "value");
       dataENvalue->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, choiceElement.Item(CurrentElement-1)->GetString(ElementSlot)));
  }
  wxXmlNode* setup = new wxXmlNode(yield, wxXML_ELEMENT_NODE, "Yield_Parameters");
   wxXmlNode* detector = new wxXmlNode(setup, wxXML_ELEMENT_NODE, "Detector_Parameters");
   detector->AddAttribute(wxT("Minimum_Energy"),MinimumEnergy->GetValue());
   detector->AddAttribute(wxT("Maximum_Energy"),MaximumEnergy->GetValue());
   detector->AddAttribute(wxT("Energy_Step"),EnergyStep->GetValue());
   detector->AddAttribute(wxT("Profiling_Step"),ProfilingStep->GetValue());
   detector->AddAttribute(wxT("Charge"),Charge->GetValue());
   detector->AddAttribute(wxT("Thickness"),Thickness->GetValue());
   detector->AddAttribute(wxT("Current_Iterations"),wxString::Format("%i",CI));
   detector->AddAttribute(wxT("Maximum_Iterations"),wxString::Format("%i",MI));
   detector->AddAttribute(wxT("Decimal_Damping_Factor"),wxString::Format("%i",LT));
   detector->AddAttribute(wxT("Decimal_Stoichiometry_Precision"),wxString::Format("%i",LS));
   detector->AddAttribute(wxT("Decimal_Yield_Precision"),wxString::Format("%i",LY));
  wxXmlNode* data1 = new wxXmlNode(yield, wxXML_ELEMENT_NODE, "Profiling_Data"); // Profiling root
   wxXmlNode* block1 = new wxXmlNode(data1, wxXML_ELEMENT_NODE, "Profiling_Table");
   // Write the spreadsheet table
 for(int i=0; i<ProfilingNumberLayers; i++)
 {
  wxString CurrentProfilingIndex = wxString::Format("%i",ProfilingNumberLayers-i);
  wxXmlNode* data1row = new wxXmlNode(block1, wxXML_ELEMENT_NODE, "Profiling");
   data1row->AddAttribute(wxT("n"),CurrentProfilingIndex);
  for(int j=0; j<ProfilingNumberElements; j++)
  {
   wxString CurrentElementIndex = wxString::Format("%i",ProfilingNumberElements-j);
   wxXmlNode* data1col = new wxXmlNode(data1row, wxXML_ELEMENT_NODE, "element");
   data1col->AddAttribute(wxT("n"),CurrentElementIndex);
   wxXmlNode* data1value = new wxXmlNode(data1col, wxXML_ELEMENT_NODE, "value");
       data1value->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, TableData->GetCellValue(ProfilingNumberLayers-i-1,ProfilingNumberElements-j-1)));
  }
 }
 // Write the spreadsheet labels
 wxXmlNode* label = new wxXmlNode(data1, wxXML_ELEMENT_NODE, "Profiling_Labels");
 for(int k=0; k<ProfilingNumberElements; k++)
  {
   wxString CurrentLabelIndex = wxString::Format("%i",ProfilingNumberElements-k);
   wxXmlNode* data1label = new wxXmlNode(label, wxXML_ELEMENT_NODE, "label");
   data1label->AddAttribute(wxT("n"),CurrentLabelIndex);
   wxXmlNode* labelvalue = new wxXmlNode(data1label, wxXML_ELEMENT_NODE, "value");
       labelvalue->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, TableData->GetColLabelValue(ProfilingNumberElements-k-1)));
  }
  wxXmlNode* version = new wxXmlNode(yield, wxXML_ELEMENT_NODE, "File_Details");
   wxXmlNode* details = new wxXmlNode(version, wxXML_ELEMENT_NODE, "Contents");
   wxXmlNode* programdatetime = new wxXmlNode(details, wxXML_ELEMENT_NODE, "Date_File_Creation");
     programdatetime->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, ActualTime));
    wxXmlNode* programversion = new wxXmlNode(details, wxXML_ELEMENT_NODE, "Program_Version");
     programversion->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, wxT("4.20")));
    wxXmlNode* programname = new wxXmlNode(details, wxXML_ELEMENT_NODE, "Program_Name");
     programname->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, wxT("ERYA-Bulk")));
 // Save the file
 return LocalERYAPIXE.Save(ERYAPIXEFileName,0);
}

// Database file main constructor
DatabaseFile::DatabaseFile(wxString DatabaseFilePath, wxString Version, ElementDatabaseArray CurrentDatabase)
{
 DatabaseFileName = DatabaseFilePath;
 DatabaseFileVersion = Version;
 ParsedDatabase = CurrentDatabase;
}

// A private function to handle the database parsing
bool DatabaseFile::ElementBlockParsing(wxArrayString ElementBlock)
{
  // Setup the needed parameters
   wxString CurrentElement;
   wxString CurrentGamma;
   wxString CurrentNumber;
   wxString CurrentAbundance;
   wxString CurrentAtomic;
   wxString CurrentIsotopic;
   wxArrayString CurrentEnergy;
   wxArrayString CurrentEnergyError;
   wxArrayString CurrentSigma;
   wxArrayString CurrentSigmaError;
   if (DatabaseFileVersion == wxT("txt")) // Process the  LabView database source file format (is a plain text file, luckily)
  {
   wxArrayString CurrentGammaStack; //Required for LabView file format correct handling
   wxArrayString AllEnergyStack; //Required for LabView file format correct handling
   wxArrayString AllSigmaStack; //Required for LabView file format correct handling

    for (int i=0; i<ElementBlock.GetCount(); i++)
   {
    // Get the tokens
    wxArrayString CurrentSeparators;
    CurrentSeparators.Add("(");
    CurrentSeparators.Add(")");
    TextLineParser CurrentBlockLine(ElementBlock.Item(i), CurrentSeparators);
    wxArrayString CurrentTokens = CurrentBlockLine.GetUnexcluded();
    if (CurrentTokens.Item(0) == wxT("_Element") && CurrentTokens.GetCount() > 1) // Get the Element Name
    {
      if(CurrentTokens.Last() != wxT("_Element"))
      CurrentElement = CurrentTokens.Last();
      else
      CurrentElement.Clear();
    }
    else if (CurrentTokens.Item(0) == wxT("_Abundance") && CurrentTokens.GetCount() > 1) // Get the Abundance
    {
      if(CurrentTokens.Last() != wxT("_Abundance"))
      CurrentAbundance = CurrentTokens.Last();
      else
      CurrentAbundance.Clear();
    }
    else if (CurrentTokens.Item(0) == wxT("_AtomicNumber") && CurrentTokens.GetCount() > 1) // Get the Atomic Number in ERYA version
    {
     if(CurrentTokens.Last() != wxT("_AtomicNumber"))
     CurrentNumber = CurrentTokens.Last();
     else
     CurrentNumber.Clear();
    }
    else if (CurrentTokens.Item(0) == wxT("_AtomicMass") && CurrentTokens.GetCount() > 1) // Get the Atomic Mass in ERYA version
    {
     if(CurrentTokens.Last() != wxT("_AtomicMass"))
     CurrentAtomic = CurrentTokens.Last();
     else
     CurrentAtomic.Clear();
    }
    else if (CurrentTokens.Item(0) == wxT("_IsotopicMass") && CurrentTokens.GetCount() > 1) // Get the Isotopic Mass in ERYA version
    {
     if(CurrentTokens.Last() != wxT("_IsotopicMass"))
     CurrentIsotopic = CurrentTokens.Last();
     else
     CurrentIsotopic.Clear();
    }
    else if (CurrentTokens.Item(0) == wxT("_Sigma")) // Get the Gamma Peak in ERYA version, which can have several values
    {
     if (CurrentTokens.GetCount()>1)
      {
       for (int a=1; a<CurrentTokens.GetCount(); a++)
       {
         wxString number = CurrentTokens.Item(a);
         number = number.Left(number.Find(wxT("keV"))); // Remove the "keV" suffix from the number
         CurrentGammaStack.Add(number);
       }
      }
      else // If the gamma peak was not declared, the program will assume an element on ground-state.
      {
        CurrentGamma = wxT("0");
        CurrentGammaStack.Add("0");
      }
    }
    else // Begin the numerical column of data processing, which is a pair of columns for each gamma peak
    {
     if(CurrentTokens.GetCount() == 2 * CurrentGammaStack.GetCount())
     {
         for(int a=0; a<CurrentGammaStack.GetCount(); a++)
         {
          // Old ERYA Format store the cross-section in keV.barn, so it is necessary to convert to keV.milibarn
          double SigmaValue;
          wxString EnergyData = CurrentTokens.Item(2*a);
          wxString SigmaData = CurrentTokens.Item(2*a+1);
          SigmaData.ToDouble(&SigmaValue);
          SigmaValue = SigmaValue * 1000;
          SigmaData = wxString::Format("%f",SigmaValue);
          AllEnergyStack.Add(EnergyData);
          AllSigmaStack.Add(SigmaData);
         }
     }
     else
     {
        wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Invalid Cross-Section Data of Element: ") + CurrentElement, wxT("Element Database File Error"), wxOK | wxICON_ERROR);
        dial->ShowModal();
        return false;
     }
    } // Finishes the current line.
   } // Complete the main cycle.

   // Generates a sequence of independent elements from the previous cross-section data-pairs.
   for(int k=0; k<CurrentGammaStack.GetCount(); k++)
   {
     // At new gamma-peak, means a new element, so the cross-section data will be cleared.
     CurrentEnergy.Clear();
     CurrentEnergyError.Clear();
     CurrentSigma.Clear();
     CurrentSigmaError.Clear();
     for(int z=0; z<AllEnergyStack.GetCount(); z++)
     {
       if(k == z%CurrentGammaStack.GetCount()) // Check if the actual bi-vector adress matches the current gamma-peak
       {
         CurrentEnergy.Add(AllEnergyStack.Item(z));
         CurrentEnergyError.Add("0");
         CurrentSigma.Add(AllSigmaStack.Item(z));
         CurrentSigmaError.Add("0");
       }
     }
     // Add a new element
     if (CurrentElement.Len()>0)
     {
     ElementDatabase StoreNewElement(CurrentElement, CurrentGammaStack.Item(k), CurrentNumber, CurrentAbundance, CurrentAtomic, CurrentIsotopic, CurrentEnergy, CurrentEnergyError, CurrentSigma, CurrentSigmaError);
     ParsedDatabase.Add(StoreNewElement);
     }
     else
     {
     wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Anonymus Element Name found during Database file loading."), wxT("Element Database File Error"), wxOK | wxICON_ERROR);
     dial->ShowModal();
     return false;
     }
   }
   // And finishes the procedure
   if(CurrentGammaStack.GetCount() == 0)
   {
    ElementDatabase StoreNewElement(CurrentElement, wxT("0"), CurrentNumber, CurrentAbundance, CurrentAtomic, CurrentIsotopic, CurrentEnergy, CurrentEnergyError, CurrentSigma, CurrentSigmaError);
    ParsedDatabase.Add(StoreNewElement);
    return true;
   }
    return true;
  }
 else  // Error
 {
  wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Unsupported database file!"), wxT("Invalid Database Format"), wxOK | wxICON_ERROR);
  dial->ShowModal();
  return false;
 }
}

// Load the database from the file, where support two different file formats
bool DatabaseFile::ERYAPIXEDatabaseFileLoad()
{
 // Prepare the original file for processing
 ParsedDatabase.Clear();
 // Apply the supported modes
 if ( DatabaseFileVersion == wxT("epd")) // Apply ERYA Version rules
 {
    wxXmlDocument LocalDatabaseFile;
    wxString CurrentElement;
    wxString CurrentGamma;
    wxString CurrentNumber;
    wxString CurrentAbundance;
    wxString CurrentAtomic;
    wxString CurrentIsotopic;
    wxArrayString CurrentEnergy;
    wxArrayString CurrentEnergyError;
    wxArrayString CurrentSigma;
    wxArrayString CurrentSigmaError;
    if(!(LocalDatabaseFile.Load(DatabaseFileName)))
    {
    wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Unexpected file format of ") + DatabaseFileName, wxT("Database File Loading Error"), wxOK | wxICON_ERROR);
    dial->ShowModal();
    return false;
    }
   if(LocalDatabaseFile.GetRoot()->GetName() != wxT("ERYA-Bulk_Database"))
   {
    EnableCompatibilityMode = false;
   }
   else if(LocalDatabaseFile.GetRoot()->GetName() != wxT("ERYA-Profiling_Database"))
   {
    EnableCompatibilityMode = true;
   }
   else
   {
    wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Unexpected file format of ") + DatabaseFileName, wxT("Database File Loading Error"), wxOK | wxICON_ERROR);
    dial->ShowModal();
    return false;
   }
   wxXmlNode *DatabaseNode = LocalDatabaseFile.GetRoot()->GetChildren();
   while(DatabaseNode)
   {
    if(DatabaseNode->GetName() == wxT("Database_Data"))
    {
      wxXmlNode *DatabaseData = DatabaseNode->GetChildren();
      if(DatabaseData->GetName() == wxT("Database_Element"))
      {
        wxXmlNode *DatabaseSector = DatabaseData->GetChildren();
        while(DatabaseSector)
        {
          CurrentEnergy.Clear();
          CurrentEnergyError.Clear();
          CurrentSigma.Clear();
          CurrentSigmaError.Clear();
          if(DatabaseSector->GetName() == wxT("register"))
          {
            CurrentElement = wxT("E") + DatabaseSector->GetAttribute(wxT("n"),wxT("0"));
            CurrentElement = DatabaseSector->GetAttribute(wxT("Element_Name"),CurrentElement);
            CurrentGamma = DatabaseSector->GetAttribute(wxT("Gamma_Peak"),wxT("0"));
            CurrentNumber = DatabaseSector->GetAttribute(wxT("Atomic_Number"),wxT("0"));
            CurrentAbundance = DatabaseSector->GetAttribute(wxT("Abundance"),wxT("0"));
            CurrentAtomic = DatabaseSector->GetAttribute(wxT("Atomic_Mass"),wxT("0"));
            CurrentIsotopic = DatabaseSector->GetAttribute(wxT("Isotopic_Mass"),wxT("0"));
          }
          DatabaseSector = DatabaseSector->GetNext();
          if(DatabaseSector->GetName() == wxT("Cross-Section"))
          {
            wxXmlNode *DatabaseSigma = DatabaseSector->GetChildren();
            while(DatabaseSigma)
            {
             if(DatabaseSigma->GetName() == wxT("sector"))
             {
               wxString c0 = DatabaseSigma->GetAttribute(wxT("Energy"),wxT("0"));
               wxString c1 = DatabaseSigma->GetAttribute(wxT("Energy_Error"),wxT("0"));
               wxString c2 = DatabaseSigma->GetAttribute(wxT("Sigma"),wxT("0"));
               wxString c3 = DatabaseSigma->GetAttribute(wxT("Sigma_Error"),wxT("0"));
               CurrentEnergy.Add(c0);
               CurrentEnergyError.Add(c1);
               CurrentSigma.Add(c2);
               CurrentSigmaError.Add(c3);
             }
             DatabaseSigma = DatabaseSigma->GetNext();
            }
          }
          ElementDatabase StoreNewElement(CurrentElement, CurrentGamma, CurrentNumber, CurrentAbundance, CurrentAtomic, CurrentIsotopic, CurrentEnergy, CurrentEnergyError, CurrentSigma, CurrentSigmaError);
          ParsedDatabase.Add(StoreNewElement);
          DatabaseSector = DatabaseSector->GetNext();
        }
      }
    }
    DatabaseNode = DatabaseNode->GetNext();
   }
  return ParsedDatabase.SortElementDatabase();
 }
 else if (DatabaseFileVersion == wxT("txt")) // Apply old ERYA file format
 {
  LabViewElements LabViewBinaryDatabase(DatabaseFileName);
  if(LabViewBinaryDatabase.IsLabViewElements()) // Experimental LabView Binary Database Support
  {
    if(LabViewBinaryDatabase.IsLabViewElements()) // It is a compatible file ?
    {
     ParsedDatabase = LabViewBinaryDatabase.GetElementsFromLabView();
     return ParsedDatabase.SortElementDatabase();
    }
    else
    {
     wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Fatal Error: Unexpected data structure!"), wxT("LabView Import Error"), wxOK | wxICON_ERROR);
     dial->ShowModal();
     ParsedDatabase.Clear();
     return false;
    }
  }
  else // Apply ASCII version
  {
   wxTextFile file(DatabaseFileName);
   wxArrayString ElementBlock;
   file.Open();
   ElementBlock.Clear();
   ElementBlock.Add(file.GetLine(0));
   for(int i=1; i<file.GetLineCount(); i++)
   {
     wxString TestingLine = file.GetLine(i);
     wxString CheckLine = TestingLine.Left(TestingLine.Find("\t"));
     if ( CheckLine == wxT("_Element") ) // All records start with "_Element"
     {
      if(this->ElementBlockParsing(ElementBlock)) // Process the element database
       ElementBlock.Clear(); // And clears the temporary cache
      else
       return false;
      ElementBlock.Add(file.GetLine(i)); // Add the current line to the cache.
     }
     else
     {
      ElementBlock.Add(file.GetLine(i)); // Add the current line to the cache.
     }
   }
   if (ElementBlock.GetCount()>0) // Check remaining temporary memory
   {
     if(this->ElementBlockParsing(ElementBlock)) // Process the element database
      ElementBlock.Clear(); // And clears the temporary cache
     else
      return false;
   }
   file.Close();
   return ParsedDatabase.SortElementDatabase();
  }
 }
 else
 {
 wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Invalid or unknown database file format"), wxT("Error file loading"), wxOK | wxICON_ERROR);
 dial->ShowModal();
 return false;
 }
}

// Save the current database from memory to a file
bool DatabaseFile::ERYAPIXEDatabaseFileSave()
{
  // Check versions formats
 if (DatabaseFileVersion == wxT("epd"))  // ERYA Version
 {
  // Get the current time
  wxDateTime ThisTime = wxDateTime::Now();
  wxString ActualTime =ThisTime.Format(wxT("%Y-%m-%dT%H:%M:%S.00Z"),wxDateTime::WET);
  // An xml file when written, all node are declared backwarks, but any atribute of the same node should be declared forwards
  wxXmlDocument LocalDatabase;
  wxXmlNode* database = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, "ERYA-Bulk_Database");
  LocalDatabase.SetRoot(database);
  wxXmlNode* data = new wxXmlNode(database, wxXML_ELEMENT_NODE, "Database_Data");
  wxXmlNode* block = new wxXmlNode(data, wxXML_ELEMENT_NODE, "Database_Element");
  for(int i=0; i<ParsedDatabase.GetCount(); i++)
  {
   // Get the current values
   int CurrentValue = ParsedDatabase.GetCount()-i;
   wxString c0 = ParsedDatabase.Item(CurrentValue-1).GetElement();
   wxString c1 = ParsedDatabase.Item(CurrentValue-1).GetGamma();
   wxString c2 = ParsedDatabase.Item(CurrentValue-1).GetNumber();
   wxString c3 = ParsedDatabase.Item(CurrentValue-1).GetAbundance();
   wxString c4 = ParsedDatabase.Item(CurrentValue-1).GetAtomic();
   wxString c5 = ParsedDatabase.Item(CurrentValue-1).GetIsotopic();
   // Allocate the cross-section data
     wxXmlNode* cross = new wxXmlNode(block, wxXML_ELEMENT_NODE, "Cross-Section");
     for(int j=0; j<ParsedDatabase.Item(CurrentValue-1).GetEnergy().GetCount(); j++)
     {
      int CurrentSigma = ParsedDatabase.Item(CurrentValue-1).GetEnergy().GetCount()-j;
      if (ParsedDatabase.Item(CurrentValue-1).GetEnergy().Item(CurrentSigma-1).Len() > 0 && ParsedDatabase.Item(CurrentValue-1).GetSigma().Item(CurrentSigma-1).Len() > 0 )
      {
       wxString s0 = ParsedDatabase.Item(CurrentValue-1).GetEnergy().Item(CurrentSigma-1);
       wxString s1 = ParsedDatabase.Item(CurrentValue-1).GetEnergyError().Item(CurrentSigma-1);
       wxString s2 = ParsedDatabase.Item(CurrentValue-1).GetSigma().Item(CurrentSigma-1);
       wxString s3 = ParsedDatabase.Item(CurrentValue-1).GetSigmaError().Item(CurrentSigma-1);
       wxXmlNode* sigma = new wxXmlNode(cross, wxXML_ELEMENT_NODE, "sector");
       sigma->AddAttribute(wxT("n"),wxString::Format("%i",CurrentSigma));
       sigma->AddAttribute(wxT("Energy"),s0);
       sigma->AddAttribute(wxT("Energy_Error"),s1);
       sigma->AddAttribute(wxT("Sigma"),s2);
       sigma->AddAttribute(wxT("Sigma_Error"),s3);
      }
     }
    // Allocate the elements
    wxXmlNode* header = new wxXmlNode(block, wxXML_ELEMENT_NODE, "register");
    header->AddAttribute(wxT("n"),wxString::Format("%i",CurrentValue));
    header->AddAttribute(wxT("Element_Name"),c0);
    header->AddAttribute(wxT("Gamma_Peak"),c1);
    header->AddAttribute(wxT("Atomic_Number"),c2);
    header->AddAttribute(wxT("Abundance"),c3);
    header->AddAttribute(wxT("Atomic_Mass"),c4);
    header->AddAttribute(wxT("Isotopic_Mass"),c5);
  }
  // Save file header
  wxXmlNode* fileversion = new wxXmlNode(database, wxXML_ELEMENT_NODE, "File_Details");
   wxXmlNode* details = new wxXmlNode(fileversion, wxXML_ELEMENT_NODE, "Contents");
   wxXmlNode* programdatetime = new wxXmlNode(details, wxXML_ELEMENT_NODE, "Date_File_Creation");
     programdatetime->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, ActualTime));
   wxXmlNode* programversion = new wxXmlNode(details, wxXML_ELEMENT_NODE, "Program_Version");
     programversion->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, wxT("4.20")));
    wxXmlNode* programname = new wxXmlNode(details, wxXML_ELEMENT_NODE, "Program_Name");
     programname->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, wxT("ERYA-Bulk")));
 // Save the file
 return LocalDatabase.Save(DatabaseFileName,0);
}
else if (DatabaseFileVersion == wxT("txt")) // LabView Source ERYA Version
{
 wxTextFile file(DatabaseFileName);
 file.Create();
 for(int i=0; i<ParsedDatabase.GetCount(); i++)
 {
  file.AddLine( wxT("_Element\t") + ParsedDatabase.Item(i).GetElement());
  file.AddLine( wxT("_AtomicNumber\t") + ParsedDatabase.Item(i).GetNumber());
  file.AddLine( wxT("_Abundance\t") + ParsedDatabase.Item(i).GetAbundance());
  file.AddLine( wxT("_AtomicMass\t") + ParsedDatabase.Item(i).GetAtomic());
  file.AddLine( wxT("_IsotopicMass\t") + ParsedDatabase.Item(i).GetIsotopic());
  file.AddLine( wxT("_Sigma\t(") + ParsedDatabase.Item(i).GetGamma() + wxT("keV)"));
  for(int j=0; j<ParsedDatabase.Item(i).GetEnergy().GetCount(); j++)
  {
   if (ParsedDatabase.Item(i).GetEnergy().Item(j).Len() > 0 && ParsedDatabase.Item(i).GetSigma().Item(j).Len() > 0)
   {
    // It's necessary to convert the standard keV.mb to the keV.b before store on file
    double SigmaValue;
    wxString EnergyData = ParsedDatabase.Item(i).GetEnergy().Item(j);
    wxString SigmaData = ParsedDatabase.Item(i).GetSigma().Item(j);
    SigmaData.ToDouble(&SigmaValue);
    SigmaValue = SigmaValue / 1000;
    SigmaData = wxString::Format("%f",SigmaValue);
    file.AddLine( EnergyData + wxT("\t") + SigmaData);
   }
  }
 }
 file.Write();
 file.Close();
 return true;
} // End Old ERYA Version
 else  // Error
 {
  wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Cannot save database in this format!"), wxT("Invalid Database Format"), wxOK | wxICON_ERROR);
  dial->ShowModal();
  return false;
 }
}

// Detector file main constructor
DetectorFile::DetectorFile(wxString DetectorFilePath, wxString Version, DetectorParameters CurrentParameters)
{
 DetectorFileName = DetectorFilePath;
 DetectorFileVersion = Version;
 ParsedParameters = CurrentParameters;
}

// Loads the Detector settings from the file, which can be the default standard, or the old version from ITN
bool DetectorFile::DetectorFileLoad()
{
 ParsedParameters.Clear();
 if (DetectorFileVersion == wxT("epsd"))
  {
    wxXmlDocument LocalDetectorFile;
    if(!(LocalDetectorFile.Load(DetectorFileName)))
    {
    wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Unexpected file format of ") + DetectorFileName, wxT("Detector File Loading Error"), wxOK | wxICON_ERROR);
    dial->ShowModal();
    return false;
    }
   if(LocalDetectorFile.GetRoot()->GetName() != wxT("ERYA-Bulk_Detector"))
   {
     EnableCompatibilityMode = false;
   }
   else if(LocalDetectorFile.GetRoot()->GetName() != wxT("ERYA-Profiling_Detector"))
   {
     EnableCompatibilityMode = true;
   }
   else
   {
    wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Unexpected file format of ") + DetectorFileName, wxT("Detector File Loading Error"), wxOK | wxICON_ERROR);
    dial->ShowModal();
    return false;
   }
   wxXmlNode *DetectorNode = LocalDetectorFile.GetRoot()->GetChildren();
   while(DetectorNode)
   {
     if(DetectorNode->GetName() == wxT("Detector_Data")) //Main data entry point
    {
     wxXmlNode *DetectorData = DetectorNode->GetChildren();
     while(DetectorData)
     {
      if(DetectorData->GetName() == wxT("Detector_Function")) //Load the custom function
      {
        wxString CustomFunction;
        wxXmlNode *DetectorFunction = DetectorData->GetChildren();
        if(DetectorFunction->GetName() == wxT("macro"))
         {
           CustomFunction = DetectorFunction->GetNodeContent();
         }
        ParsedParameters.SetDetectorFunction(CustomFunction);
      }
      if(DetectorData->GetName() == wxT("Energy_Efficiency")) //Get the table data
      {
        wxXmlNode *DetectorElement = DetectorData->GetChildren();
        while(DetectorElement)
        {
          if(DetectorElement->GetName() == wxT("register"))
          {
            wxString ActualEnergy = DetectorElement->GetAttribute(wxT("Energy"),wxT("0"));
            wxString ActualEfficiency = DetectorElement->GetAttribute(wxT("Efficiency"),wxT("0"));
            ParsedParameters.AddEnergyEfficiencyData(ActualEnergy,ActualEfficiency);
          }
         DetectorElement = DetectorElement->GetNext();
        }
      }
     DetectorData = DetectorData->GetNext();
     }
    }
    DetectorNode = DetectorNode->GetNext();
   }
   return true;
  }
  else if(DetectorFileVersion == wxT("txt"))
  {
   // The LabView ERYA format are simply a file with two columns of numbers, and the model function are hardcoded to the program
    wxTextFile file(DetectorFileName);
   file.Open();
  for(int i=0; i<file.GetLineCount(); i++)
  {
    wxString CurrentLine = file.GetLine(i); // Get the current line
    TextLineParser CurrentLineData(CurrentLine, wxEmptyString); // Define the custom parser rules
    if(CurrentLineData.GetUnexcluded().GetCount() == 2)
    {
     ParsedParameters.AddEnergyEfficiencyData(CurrentLineData.GetUnexcluded().Item(0), CurrentLineData.GetUnexcluded().Item(1));
    }
    else
    {
    wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Fatal Error: Invalid file format!"), wxT("Import Error"), wxOK | wxICON_ERROR);
    dial->ShowModal();
    ParsedParameters.Clear();
    return false;
    }
  }
   ParsedParameters.SetDetectorFunction(wxEmptyString);
   file.Close();
   return true;
  }
  else if(DetectorFileVersion == wxT("xlsx"))
 {
  XlsxFile DetectorXlsxFile(DetectorFileName);
  if(DetectorXlsxFile.LoadFile())
  {
   TableMatrix DetectorMatrix = DetectorXlsxFile.GetTableMatrix();
   int NumberElements,NumberValues;
   DetectorMatrix.GetRealMatrixSize(NumberElements,NumberValues);
   if(NumberElements > 1 && NumberValues == 2)
   {
     // Check and verify if a line are a pure numerical, otherwise, ignore the line since it is a label
     for(int i=0; i<NumberElements; i++)
     {
      int temp;
      double v0,v1;
      wxString c0 = DetectorMatrix.GetTableMatrixValueAt(i,0,temp);
      wxString c1 = DetectorMatrix.GetTableMatrixValueAt(i,1,temp);
      // Store the line
      if(c0.ToDouble(&v0) && c1.ToDouble(&v1))
       ParsedParameters.AddEnergyEfficiencyData(c0,c1);
     }
    return true;
   }
   else
   {
     wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Expected a 2 column table"), wxT("Import Error"), wxOK | wxICON_ERROR);
    dial->ShowModal();
    ParsedParameters.Clear();
    return false;
   }
  }
  else
  {
   wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Fatal Error: Invalid file format!"), wxT("Import Error"), wxOK | wxICON_ERROR);
    dial->ShowModal();
    ParsedParameters.Clear();
    return false;
  }
 }
 else
 {
 wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Unknown Detector Setup File Format!"), wxT("Invalid Format"), wxOK | wxICON_ERROR);
 dial->ShowModal();
 return false;
 }
}

bool DetectorFile::DetectorFileSave()
{
 if (DetectorFileVersion == wxT("epsd")) // Save in the native ERYA format
 {
  // Get the current time
  wxDateTime ThisTime = wxDateTime::Now();
  wxString ActualTime =ThisTime.Format(wxT("%Y-%m-%dT%H:%M:%S.00Z"),wxDateTime::WET);
  // An xml file when written, all node are declared backwarks, but any atribute of the same node should be declared forwards
  wxXmlDocument LocalDetector;
  wxXmlNode* detector = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, "ERYA-Bulk_Detector");
  LocalDetector.SetRoot(detector);
  wxXmlNode* data = new wxXmlNode(detector, wxXML_ELEMENT_NODE, "Detector_Data");
  wxXmlNode* block = new wxXmlNode(data, wxXML_ELEMENT_NODE, "Energy_Efficiency");
  for(int i=0; i<ParsedParameters.GetDetectorEnergy().GetCount(); i++)
  {
   // Get the current values
   int CurrentValue = ParsedParameters.GetDetectorEnergy().GetCount()-i;
   wxString c0 = ParsedParameters.GetDetectorEnergy().Item(CurrentValue-1);
   wxString c1 = ParsedParameters.GetDetectorEfficiency().Item(CurrentValue-1);
   // Allocate the elements
   if (c0.Len()>0 && c1.Len()>0)
   {
    wxXmlNode* sigma = new wxXmlNode(block, wxXML_ELEMENT_NODE, "register");
    sigma->AddAttribute(wxT("n"),wxString::Format("%i",CurrentValue));
    sigma->AddAttribute(wxT("Energy"),c0);
    sigma->AddAttribute(wxT("Efficiency"),c1);
   }
  }
  // Store the custom function
  wxXmlNode *dfunction = new wxXmlNode(data, wxXML_ELEMENT_NODE, "Detector_Function");
   wxXmlNode *definition = new wxXmlNode(dfunction, wxXML_ELEMENT_NODE, "macro");
    definition->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, ParsedParameters.GetDetectorFunction()));
  // Save file header
  wxXmlNode* fileversion = new wxXmlNode(detector, wxXML_ELEMENT_NODE, "File_Details");
   wxXmlNode* details = new wxXmlNode(fileversion, wxXML_ELEMENT_NODE, "Contents");
   wxXmlNode* programdatetime = new wxXmlNode(details, wxXML_ELEMENT_NODE, "Date_File_Creation");
     programdatetime->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, ActualTime));
   wxXmlNode* programversion = new wxXmlNode(details, wxXML_ELEMENT_NODE, "Program_Version");
     programversion->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, wxT("4.20")));
    wxXmlNode* programname = new wxXmlNode(details, wxXML_ELEMENT_NODE, "Program_Name");
     programname->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, wxT("ERYA-Bulk")));
 // Save the file
 return LocalDetector.Save(DetectorFileName,0);
 }
 else if (DetectorFileVersion == wxT("txt"))
 {
 wxTextFile file(DetectorFileName);
 file.Create();
 for(int i=0; i<ParsedParameters.GetDetectorEnergy().GetCount(); i++)
 {
  if (ParsedParameters.GetDetectorEnergy().Item(i).Len() > 0 && ParsedParameters.GetDetectorEfficiency().Item(i).Len() > 0 )
  {
   file.AddLine( ParsedParameters.GetDetectorEnergy().Item(i) + wxT("\t") + ParsedParameters.GetDetectorEfficiency().Item(i));
  }
 }
 file.Write();
 file.Close();
 return true;
 }
 else if (DetectorFileVersion == wxT("xlsx"))
 {
  TableMatrix DetectorMatrix(ParsedParameters.GetDetectorEnergy().GetCount(),2);
  DetectorMatrix.Add(TableNode(wxT("Energy (keV)"),0));
  DetectorMatrix.Add(TableNode(wxT("Absolute Efficiency"),0));
  for(int i=0; i<ParsedParameters.GetDetectorEnergy().GetCount(); i++)
  {
   if (ParsedParameters.GetDetectorEnergy().Item(i).Len() > 0 && ParsedParameters.GetDetectorEfficiency().Item(i).Len() > 0 )
   {
     DetectorMatrix.Add(TableNode(ParsedParameters.GetDetectorEnergy().Item(i),1));
     DetectorMatrix.Add(TableNode(ParsedParameters.GetDetectorEfficiency().Item(i),1));
   }
  }
  XlsxFile DetectorXlsxFile(DetectorFileName, DetectorMatrix, wxT("A1"));
  DetectorXlsxFile.WriteFile();
  return true;
 }
 else
 {
 wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Cannot save detector setup data in this format!"), wxT("Invalid Format"), wxOK | wxICON_ERROR);
 dial->ShowModal();
 return false;
 }
}

// Ziegler File main constructor
ZieglerFile::ZieglerFile(wxString ZieglerFilePath, wxString Version, ZieglerParameters CurrentParameters, ElementSRIMArray CurrentTables, int Argument)
{
 ZieglerFileName = ZieglerFilePath;
 ZieglerFileVersion = Version;
 ParsedParameters = CurrentParameters;
 ParsedTables = CurrentTables;
 TableMode = Argument; // If argument is zero, force Ziegler's Parameters, otherwise it will use the SRIM table corresponding to the Atomic Number
}

bool ZieglerFile::ZieglerFileLoad()
{
 if (ZieglerFileVersion == wxT("epsz")) // Load the ERYA's Ziegler native format.
 {
  ParsedParameters.Clear();
  ParsedTables.Clear();
  wxXmlDocument LocalZieglerFile;
  if(!(LocalZieglerFile.Load(ZieglerFileName)))
  {
    wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Unexpected file format of ") + ZieglerFileName, wxT("Ziegler File Loading Error"), wxOK | wxICON_ERROR);
    dial->ShowModal();
    return false;
  }
  if(!(LocalZieglerFile.GetRoot()->GetName() == wxT("ERYA-Bulk_Ziegler") || LocalZieglerFile.GetRoot()->GetName() == wxT("ERYA-Profiling_Ziegler")))
  {
    wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Unexpected file format of ") + ZieglerFileName, wxT("Ziegler File Loading Error"), wxOK | wxICON_ERROR);
    dial->ShowModal();
    return false;
  }
  wxXmlNode *ZieglerNode = LocalZieglerFile.GetRoot()->GetChildren();
  while(ZieglerNode)
  {
    if(ZieglerNode->GetName() == wxT("Ziegler_Data")) //Main data entry point
    {
     wxXmlNode *ZieglerData = ZieglerNode->GetChildren();
     while(ZieglerData)
     {
      if(ZieglerData->GetName() == wxT("Ziegler_Function")) //Set the custom function
      {
        wxString CustomFunction;
        wxXmlNode *ZieglerFunction = ZieglerData->GetChildren();
        if(ZieglerFunction->GetName() == wxT("macro"))
         {
           CustomFunction = ZieglerFunction->GetNodeContent();
         }
        ParsedParameters.SetZieglerFunction(CustomFunction);
      }
      if(ZieglerData->GetName() == wxT("Ziegler_Version")) //Set the version
      {
        wxString ActualZieglerVersion = ZieglerData->GetAttribute(wxT("zv"),wxT("0")); //Get the desired value
        ParsedParameters.SetZieglerVersion(ActualZieglerVersion);
      }
      if(ZieglerData->GetName() == wxT("Ziegler_Parameters")) //Get the table data
      {
        wxXmlNode *ZieglerElement = ZieglerData->GetChildren();
        while(ZieglerElement)
        {
          if(ZieglerElement->GetName() == wxT("Element_Number")) // Read one element array
          {
            wxString c0,c1,c2,c3,c4,c5,c6,c7,c8,c9,c10,c11,c12,c13;
            wxXmlNode *ZieglerElementData = ZieglerElement->GetChildren();
            while(ZieglerElementData)
            {
              if(ZieglerElementData->GetName() == wxT("Element_Name"))
              {
                wxXmlNode *ZieglerData0 = ZieglerElementData->GetChildren();
                if(ZieglerData0->GetName() == wxT("value"))
                {
                  c0 = ZieglerData0->GetNodeContent();
                }
              }
              if(ZieglerElementData->GetName() == wxT("A-1"))
              {
                wxXmlNode *ZieglerData1 = ZieglerElementData->GetChildren();
                if(ZieglerData1->GetName() == wxT("value"))
                {
                  c1 = ZieglerData1->GetNodeContent();
                }
              }
              if(ZieglerElementData->GetName() == wxT("A-2"))
              {
                wxXmlNode *ZieglerData2 = ZieglerElementData->GetChildren();
                if(ZieglerData2->GetName() == wxT("value"))
                {
                  c2 = ZieglerData2->GetNodeContent();
                }
              }
              if(ZieglerElementData->GetName() == wxT("A-3"))
              {
                wxXmlNode *ZieglerData3 = ZieglerElementData->GetChildren();
                if(ZieglerData3->GetName() == wxT("value"))
                {
                  c3 = ZieglerData3->GetNodeContent();
                }
              }
              if(ZieglerElementData->GetName() == wxT("A-4"))
              {
                wxXmlNode *ZieglerData4 = ZieglerElementData->GetChildren();
                if(ZieglerData4->GetName() == wxT("value"))
                {
                  c4 = ZieglerData4->GetNodeContent();
                }
              }
              if(ZieglerElementData->GetName() == wxT("A-5"))
              {
                wxXmlNode *ZieglerData5 = ZieglerElementData->GetChildren();
                if(ZieglerData5->GetName() == wxT("value"))
                {
                  c5 = ZieglerData5->GetNodeContent();
                }
              }
              if(ZieglerElementData->GetName() == wxT("A-6"))
              {
                wxXmlNode *ZieglerData6 = ZieglerElementData->GetChildren();
                if(ZieglerData6->GetName() == wxT("value"))
                {
                  c6 = ZieglerData6->GetNodeContent();
                }
              }
              if(ZieglerElementData->GetName() == wxT("A-7"))
              {
                wxXmlNode *ZieglerData7 = ZieglerElementData->GetChildren();
                if(ZieglerData7->GetName() == wxT("value"))
                {
                  c7 = ZieglerData7->GetNodeContent();
                }
              }
              if(ZieglerElementData->GetName() == wxT("A-8"))
              {
                wxXmlNode *ZieglerData8 = ZieglerElementData->GetChildren();
                if(ZieglerData8->GetName() == wxT("value"))
                {
                  c8 = ZieglerData8->GetNodeContent();
                }
              }
              if(ZieglerElementData->GetName() == wxT("A-9"))
              {
                wxXmlNode *ZieglerData9 = ZieglerElementData->GetChildren();
                if(ZieglerData9->GetName() == wxT("value"))
                {
                  c9 = ZieglerData9->GetNodeContent();
                }
              }
              if(ZieglerElementData->GetName() == wxT("A-10"))
              {
                wxXmlNode *ZieglerData10 = ZieglerElementData->GetChildren();
                if(ZieglerData10->GetName() == wxT("value"))
                {
                  c10 = ZieglerData10->GetNodeContent();
                }
              }
              if(ZieglerElementData->GetName() == wxT("A-11"))
              {
                wxXmlNode *ZieglerData11 = ZieglerElementData->GetChildren();
                if(ZieglerData11->GetName() == wxT("value"))
                {
                  c11 = ZieglerData11->GetNodeContent();
                }
              }
              if(ZieglerElementData->GetName() == wxT("A-12"))
              {
                wxXmlNode *ZieglerData12 = ZieglerElementData->GetChildren();
                if(ZieglerData12->GetName() == wxT("value"))
                {
                  c12 = ZieglerData12->GetNodeContent();
                }
              }
              if(ZieglerElementData->GetName() == wxT("Atomic_Mass"))
              {
                wxXmlNode *ZieglerData13 = ZieglerElementData->GetChildren();
                if(ZieglerData13->GetName() == wxT("value"))
                {
                  c13 = ZieglerData13->GetNodeContent();
                }
              }
              ZieglerElementData = ZieglerElementData->GetNext();
            }
           // Store the line
           ParsedParameters.AddZieglerParameterData(c0,c1,c2,c3,c4,c5,c6,c7,c8,c9,c10,c11,c12,c13);
          }
          ZieglerElement = ZieglerElement->GetNext();
        }
      }
      if(ZieglerData->GetName() == wxT("Ziegler_SRIM_Tables")) //Get the top node of SRIM tables
      {
        wxXmlNode *SRIMElement = ZieglerData->GetChildren();
        while(SRIMElement)
        {
         if(SRIMElement->GetName() == wxT("Ziegler_SRIM_Element")) //Read the element contents
         {
           wxString AtomicNumber = SRIMElement->GetAttribute(wxT("atomic"),wxT("1"));
           wxArrayString SRIMEnergy, SRIMStoppingPower;
           wxXmlNode *SRIMData = SRIMElement->GetChildren();
           while(SRIMData)
           {
            if(SRIMData->GetName() == wxT("register")) // Read each pair of values from the vtable on stack
            {
            wxString ActualEnergy = SRIMData->GetAttribute(wxT("Energy"),wxT("0"));
            wxString ActualStoppingPower = SRIMData->GetAttribute(wxT("Stopping_Power"),wxT("0"));
            SRIMEnergy.Add(ActualEnergy);
            SRIMStoppingPower.Add(ActualStoppingPower);
            }
            SRIMData = SRIMData->GetNext();
           }
           long ZElement;
           int Z;
           if(AtomicNumber.ToLong(&ZElement))
            Z = static_cast<int>(ZElement);
           else
            Z = 1;
           ParsedTables.Replace(Z,SRIMEnergy,SRIMStoppingPower); //Store the complete element table on memory.
         }
         SRIMElement = SRIMElement->GetNext();
        }
      }
     ZieglerData = ZieglerData->GetNext();
     }
    }
   ZieglerNode = ZieglerNode->GetNext(); //Pass to next element
  }
  return true;
 }
 else if (ZieglerFileVersion == wxT("txt")) // Database files with such extension can be a genuine text file, or a LabView binary file
 {
  LabViewZiegler LabViewBinaryZiegler(ZieglerFileName);
  SRIMFile SRIMFileZiegler(ZieglerFileName);
  if(LabViewBinaryZiegler.IsLabViewFile()) // Use the LabView Binary Database Loader. First check the header file.
  {
    if(LabViewBinaryZiegler.IsLabViewZiegler()) // It is a compatible file ?
    {
     ParsedParameters.Clear();
     ParsedParameters = LabViewBinaryZiegler.GetZieglerParametersFromLabView();
     ParsedParameters.FixElementNames();
     return true;
    }
    else
    {
     wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Fatal Error: Unexpected data structure!"), wxT("LabView Import Error"), wxOK | wxICON_ERROR);
     dial->ShowModal();
     ParsedParameters.Clear();
     return false;
    }
  }
  else if(SRIMFileZiegler.IsSRIMFile()) // Check if the file is a SRIM file
  {
   if(SRIMFileZiegler.SRIMImport()) // Extract the data inside the SRIM file
   {
     wxArrayString DataElementEnergy = SRIMFileZiegler.GetEnergy();
     wxArrayString DataElementStoppingPower = SRIMFileZiegler.GetStoppingPower();
     int DataElementNumber = SRIMFileZiegler.GetAtomic();
     if(DataElementNumber == TableMode)
     {
       ParsedTables.Replace(DataElementNumber,DataElementEnergy,DataElementStoppingPower);
       return true;
     }
     else
     {
       wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("This SRIM file will overwrite the Stopping-Power of\n Element with Atomic Number ") + wxString::Format("%d",DataElementNumber) + wxT("\nWhile you have selected on SRIM spreadsheet the Element with Atomic Number ") + wxString::Format("%d", TableMode) , wxT("SRIM Import Warning. Replace anyway ?"), wxYES_NO | wxNO_DEFAULT | wxICON_WARNING);
       if (dial->ShowModal() == wxID_YES) // Replace anyway-
       {
        ParsedTables.Replace(DataElementNumber,DataElementEnergy,DataElementStoppingPower);
        return true;
       }
       else
       {
        return false;
       }
     }
   }
   else
   {
     wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Fatal Error: Unexpected data structure!"), wxT("SRIM Import Error"), wxOK | wxICON_ERROR);
     dial->ShowModal();
     ParsedParameters.Clear();
     return false;
   }
  }
  else // It's a generic text file format
  {
   wxTextFile file(ZieglerFileName);
   file.Open();
   if (TableMode == 0)
    ParsedParameters.Clear();
   wxArrayString Energy, StoppingPower;
   for(int j=0; j<file.GetLineCount(); j++)
   {
    wxString CurrentLine = file.GetLine(j); // Get the current line
    TextLineParser CurrentLineData(CurrentLine, wxEmptyString); // Define the custom parser rules
    if(CurrentLineData.GetUnexcluded().GetCount() == 14 && TableMode == 0) // Ziegler Table
    {
     // If the file is the legacy format, the program will treat the element symbol using the Z<n> format
     ParsedParameters.AddZieglerParameterData( wxT("Z") + CurrentLineData.GetUnexcluded().Item(0), CurrentLineData.GetUnexcluded().Item(1), CurrentLineData.GetUnexcluded().Item(2), CurrentLineData.GetUnexcluded().Item(3), CurrentLineData.GetUnexcluded().Item(4), CurrentLineData.GetUnexcluded().Item(5), CurrentLineData.GetUnexcluded().Item(6), CurrentLineData.GetUnexcluded().Item(7), CurrentLineData.GetUnexcluded().Item(8), CurrentLineData.GetUnexcluded().Item(9), CurrentLineData.GetUnexcluded().Item(10), CurrentLineData.GetUnexcluded().Item(11), CurrentLineData.GetUnexcluded().Item(12), CurrentLineData.GetUnexcluded().Item(13));
     // The difference of the two versions, is that the 1977 version require 12 parameters, and the 1991 only 8.
     double test1,test2,test3,test4;
     CurrentLineData.GetUnexcluded().Item(9).ToDouble(&test1);
     CurrentLineData.GetUnexcluded().Item(10).ToDouble(&test2);
     CurrentLineData.GetUnexcluded().Item(11).ToDouble(&test3);
     CurrentLineData.GetUnexcluded().Item(12).ToDouble(&test4);
     if (test1 < 1e-10 && test2 < 1e-10 && test3 < 1e-10 && test4 < 1e-10)
         ParsedParameters.SetZieglerVersion(wxT("1"));
     else
         ParsedParameters.SetZieglerVersion(wxT("0"));
    }
    else if(CurrentLineData.GetUnexcluded().GetCount() == 2 && TableMode > 0) // A single SRIM Table
    {
      Energy.Add(CurrentLineData.GetUnexcluded().Item(0));
      StoppingPower.Add(CurrentLineData.GetUnexcluded().Item(1));
      ParsedParameters.SetZieglerVersion(wxT("3"));
    }
    else
    {
    wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Fatal Error: Invalid file format!"), wxT("Import Error"), wxOK | wxICON_ERROR);
    dial->ShowModal();
    ParsedParameters.Clear();
    return false;
    }
   }
   file.Close();
   if (TableMode == 0)
    ParsedParameters.FixElementNames();
   else
    return ParsedTables.Replace(TableMode,Energy,StoppingPower);
   return true;
  }
 }
 else if (ZieglerFileVersion == wxT("xlsx")) //Load the table from an Excel Xlsx file format, which calls own methods
 {
  XlsxFile ZieglerXlsxFile(ZieglerFileName);
  if(ZieglerXlsxFile.LoadFile())
  {
   TableMatrix ZieglerMatrix = ZieglerXlsxFile.GetTableMatrix();
   int NumberElements,NumberValues;
   ZieglerMatrix.GetRealMatrixSize(NumberElements,NumberValues);
   if(NumberElements > 1 && NumberValues == 14 && TableMode == 0)
   {
     ParsedParameters.Clear();
     // Check if the entire line are numerical, except the first column.
     for(int i=0; i<NumberElements; i++)
     {
      int temp;
      double v0,v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12,v13;
      wxString c0 = ZieglerMatrix.GetTableMatrixValueAt(i,0,temp);
      wxString c1 = ZieglerMatrix.GetTableMatrixValueAt(i,1,temp);
      wxString c2 = ZieglerMatrix.GetTableMatrixValueAt(i,2,temp);
      wxString c3 = ZieglerMatrix.GetTableMatrixValueAt(i,3,temp);
      wxString c4 = ZieglerMatrix.GetTableMatrixValueAt(i,4,temp);
      wxString c5 = ZieglerMatrix.GetTableMatrixValueAt(i,5,temp);
      wxString c6 = ZieglerMatrix.GetTableMatrixValueAt(i,6,temp);
      wxString c7 = ZieglerMatrix.GetTableMatrixValueAt(i,7,temp);
      wxString c8 = ZieglerMatrix.GetTableMatrixValueAt(i,8,temp);
      wxString c9 = ZieglerMatrix.GetTableMatrixValueAt(i,9,temp);
      wxString c10 = ZieglerMatrix.GetTableMatrixValueAt(i,10,temp);
      wxString c11 = ZieglerMatrix.GetTableMatrixValueAt(i,11,temp);
      wxString c12 = ZieglerMatrix.GetTableMatrixValueAt(i,12,temp);
      wxString c13 = ZieglerMatrix.GetTableMatrixValueAt(i,13,temp);
      // Check the first column
      if(c0.ToDouble(&v0))
       c0 = wxT("Z") + c0;
      // Store the line
      if(c1.ToDouble(&v1) && c2.ToDouble(&v2) && c3.ToDouble(&v3) && c4.ToDouble(&v4) && c5.ToDouble(&v5) && c6.ToDouble(&v6) && c7.ToDouble(&v7) && c8.ToDouble(&v8) && c9.ToDouble(&v9) && c10.ToDouble(&v10) && c11.ToDouble(&v11) && c12.ToDouble(&v12) && c13.ToDouble(&v13))
       {
       ParsedParameters.AddZieglerParameterData(c0,c1,c2,c3,c4,c5,c6,c7,c8,c9,c10,c11,c12,c13);
      // The difference of the two versions, is that the 1977 version require 12 parameters, and the 1991 only 8.
       double test1,test2,test3,test4;
       c9.ToDouble(&test1);
       c10.ToDouble(&test2);
       c11.ToDouble(&test3);
       c12.ToDouble(&test4);
       if (test1 < 1e-10 && test2 < 1e-10 && test3 < 1e-10 && test4 < 1e-10)
         ParsedParameters.SetZieglerVersion(wxT("1"));
       else
         ParsedParameters.SetZieglerVersion(wxT("0"));
       }
     }
    ParsedParameters.FixElementNames();
    return true;
   }
   else if (NumberElements > 1 && NumberValues == 2 && TableMode > 0)
   {
    wxArrayString Energy, StoppingPower;
    for(int i=0; i<NumberElements; i++)
     {
      int temp;
      double v0,v1;
      wxString c0 = ZieglerMatrix.GetTableMatrixValueAt(i,0,temp);
      wxString c1 = ZieglerMatrix.GetTableMatrixValueAt(i,1,temp);
      // Check if the two values are numeric terms, otherwise are discarded.
      if(c0.ToDouble(&v0) && c1.ToDouble(&v1))
       {
         Energy.Add(c0);
         StoppingPower.Add(c1);
       }
     }
    return ParsedTables.Replace(TableMode,Energy,StoppingPower);
   }
   else
   {
     wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Expected a table with 2(SRIM),\n14(ERYA-Bulk)\nor 16(ERYA-Profiling) columns."), wxT("Excel Import Error"), wxOK | wxICON_ERROR);
    dial->ShowModal();
    ParsedParameters.Clear();
    return false;
   }
  }
  else
  {
   wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Fatal Error: Invalid file format!"), wxT("Import Error"), wxOK | wxICON_ERROR);
    dial->ShowModal();
    ParsedParameters.Clear();
    return false;
  }
 }
 else
 {
 wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Cannot load Ziegler Parameters file in this format!"), wxT("Invalid Format"), wxOK | wxICON_ERROR);
 dial->ShowModal();
 return false;
 }
}

// Implement the Ziegler Parameters saving as a file in native epsz format, and legacy txt format
bool ZieglerFile::ZieglerFileSave()
{
 if (ZieglerFileVersion == wxT("epsz")) // Save on native ERYA-Bulk format, which is a plain xml file
 {
  // Get the current time
  wxDateTime ThisTime = wxDateTime::Now();
  wxString ActualTime =ThisTime.Format(wxT("%Y-%m-%dT%H:%M:%S.00Z"),wxDateTime::WET);
  // An xml file when written, all node are declared backwarks, but any atribute of the same node should be declared forwards
  wxXmlDocument LocalZiegler;
  wxXmlNode* ziegler = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, "ERYA-Bulk_Ziegler");
  LocalZiegler.SetRoot(ziegler);
  wxXmlNode* data = new wxXmlNode(ziegler, wxXML_ELEMENT_NODE, "Ziegler_Data");
  wxXmlNode* tables = new wxXmlNode(data, wxXML_ELEMENT_NODE, "Ziegler_SRIM_Tables");
  for(int k=0; k<ParsedTables.GetCount(); k++)
  {
   // Set the table backwards.
   int CurrentAtomic = ParsedTables.GetCount()-k;
   int AtomicNumber = ParsedTables.Item(CurrentAtomic-1).GetElementAtomic();
   wxXmlNode* tableSRIM = new wxXmlNode(tables, wxXML_ELEMENT_NODE, "Ziegler_SRIM_Element");
    tableSRIM->AddAttribute(wxT("number"),wxString::Format("%i",CurrentAtomic));
    tableSRIM->AddAttribute(wxT("atomic"),wxString::Format("%i",AtomicNumber));
    for(int m=0; m<ParsedTables.Item(CurrentAtomic-1).GetElementEnergy().GetCount(); m++)
   {
   // Get the current values
   int CurrentValue = ParsedTables.Item(CurrentAtomic-1).GetElementEnergy().GetCount()-m;
   wxString c0 = ParsedTables.Item(CurrentAtomic-1).GetElementEnergy().Item(CurrentValue-1);
   wxString c1 = ParsedTables.Item(CurrentAtomic-1).GetElementStoppingPower().Item(CurrentValue-1);
   // Allocate the elements
   if (c0.Len()>0 && c1.Len()>0)
   {
    wxXmlNode* sigma = new wxXmlNode(tableSRIM, wxXML_ELEMENT_NODE, "register");
    sigma->AddAttribute(wxT("n"),wxString::Format("%i",CurrentValue));
    sigma->AddAttribute(wxT("Energy"),c0);
    sigma->AddAttribute(wxT("Stopping_Power"),c1);
   }
  }
  }
  wxXmlNode* parameters = new wxXmlNode(data, wxXML_ELEMENT_NODE, "Ziegler_Parameters");
  for(int i=0; i<ParsedParameters.GetZieglerElements().GetCount(); i++)
  {
   // Get the current values
   int CurrentElement = ParsedParameters.GetZieglerElements().GetCount()-i;
   wxString c0 = ParsedParameters.GetZieglerElements().Item(CurrentElement-1);
   wxString c1 = ParsedParameters.GetZieglerValues1().Item(CurrentElement-1);
   wxString c2 = ParsedParameters.GetZieglerValues2().Item(CurrentElement-1);
   wxString c3 = ParsedParameters.GetZieglerValues3().Item(CurrentElement-1);
   wxString c4 = ParsedParameters.GetZieglerValues4().Item(CurrentElement-1);
   wxString c5 = ParsedParameters.GetZieglerValues5().Item(CurrentElement-1);
   wxString c6 = ParsedParameters.GetZieglerValues6().Item(CurrentElement-1);
   wxString c7 = ParsedParameters.GetZieglerValues7().Item(CurrentElement-1);
   wxString c8 = ParsedParameters.GetZieglerValues8().Item(CurrentElement-1);
   wxString c9 = ParsedParameters.GetZieglerValues9().Item(CurrentElement-1);
   wxString c10 = ParsedParameters.GetZieglerValues10().Item(CurrentElement-1);
   wxString c11 = ParsedParameters.GetZieglerValues11().Item(CurrentElement-1);
   wxString c12 = ParsedParameters.GetZieglerValues12().Item(CurrentElement-1);
   wxString c13 = ParsedParameters.GetZieglerMass().Item(CurrentElement-1);
   // Allocate the elements
   if (c0.Len()>0 && c1.Len()>0 && c2.Len()>0 && c3.Len()>0 && c4.Len()>0 && c5.Len()>0 && c6.Len()>0 && c7.Len()>0 && c8.Len()>0 && c9.Len()>0 && c10.Len()>0 && c11.Len()>0 && c12.Len()>0 && c13.Len()>0)
   {
   wxXmlNode* parameternumber = new wxXmlNode(parameters, wxXML_ELEMENT_NODE, "Element_Number");
   parameternumber->AddAttribute(wxT("number"),wxString::Format("%i",CurrentElement));
    wxXmlNode* datac13 = new wxXmlNode(parameternumber, wxXML_ELEMENT_NODE, "Atomic_Mass");
     wxXmlNode* datac13value = new wxXmlNode(datac13, wxXML_ELEMENT_NODE, "value");
       datac13value->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, c13));
    wxXmlNode* datac12 = new wxXmlNode(parameternumber, wxXML_ELEMENT_NODE, "A-12");
     wxXmlNode* datac12value = new wxXmlNode(datac12, wxXML_ELEMENT_NODE, "value");
       datac12value->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, c12));
    wxXmlNode* datac11 = new wxXmlNode(parameternumber, wxXML_ELEMENT_NODE, "A-11");
     wxXmlNode* datac11value = new wxXmlNode(datac11, wxXML_ELEMENT_NODE, "value");
       datac11value->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, c11));
    wxXmlNode* datac10 = new wxXmlNode(parameternumber, wxXML_ELEMENT_NODE, "A-10");
     wxXmlNode* datac10value = new wxXmlNode(datac10, wxXML_ELEMENT_NODE, "value");
       datac10value->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, c10));
    wxXmlNode* datac9 = new wxXmlNode(parameternumber, wxXML_ELEMENT_NODE, "A-9");
     wxXmlNode* datac9value = new wxXmlNode(datac9, wxXML_ELEMENT_NODE, "value");
       datac9value->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, c9));
    wxXmlNode* datac8 = new wxXmlNode(parameternumber, wxXML_ELEMENT_NODE, "A-8");
     wxXmlNode* datac8value = new wxXmlNode(datac8, wxXML_ELEMENT_NODE, "value");
       datac8value->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, c8));
    wxXmlNode* datac7 = new wxXmlNode(parameternumber, wxXML_ELEMENT_NODE, "A-7");
     wxXmlNode* datac7value = new wxXmlNode(datac7, wxXML_ELEMENT_NODE, "value");
       datac7value->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, c7));
    wxXmlNode* datac6 = new wxXmlNode(parameternumber, wxXML_ELEMENT_NODE, "A-6");
     wxXmlNode* datac6value = new wxXmlNode(datac6, wxXML_ELEMENT_NODE, "value");
       datac6value->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, c6));
    wxXmlNode* datac5 = new wxXmlNode(parameternumber, wxXML_ELEMENT_NODE, "A-5");
     wxXmlNode* datac5value = new wxXmlNode(datac5, wxXML_ELEMENT_NODE, "value");
       datac5value->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, c5));
    wxXmlNode* datac4 = new wxXmlNode(parameternumber, wxXML_ELEMENT_NODE, "A-4");
     wxXmlNode* datac4value = new wxXmlNode(datac4, wxXML_ELEMENT_NODE, "value");
       datac4value->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, c4));
    wxXmlNode* datac3 = new wxXmlNode(parameternumber, wxXML_ELEMENT_NODE, "A-3");
     wxXmlNode* datac3value = new wxXmlNode(datac3, wxXML_ELEMENT_NODE, "value");
       datac3value->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, c3));
    wxXmlNode* datac2 = new wxXmlNode(parameternumber, wxXML_ELEMENT_NODE, "A-2");
     wxXmlNode* datac2value = new wxXmlNode(datac2, wxXML_ELEMENT_NODE, "value");
       datac2value->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, c2));
    wxXmlNode* datac1 = new wxXmlNode(parameternumber, wxXML_ELEMENT_NODE, "A-1");
     wxXmlNode* datac1value = new wxXmlNode(datac1, wxXML_ELEMENT_NODE, "value");
       datac1value->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, c1));
    wxXmlNode* datac0 = new wxXmlNode(parameternumber, wxXML_ELEMENT_NODE, "Element_Name");
     wxXmlNode* datac0value = new wxXmlNode(datac0, wxXML_ELEMENT_NODE, "value");
       datac0value->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, c0));
   }
  }
   // Store the custom function
  wxXmlNode *dfunction = new wxXmlNode(data, wxXML_ELEMENT_NODE, "Ziegler_Function");
   wxXmlNode *definition = new wxXmlNode(dfunction, wxXML_ELEMENT_NODE, "macro");
    definition->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, ParsedParameters.GetZieglerFunction()));
 // Store file headers and versions-
  wxXmlNode* version = new wxXmlNode(data, wxXML_ELEMENT_NODE, "Ziegler_Version");
   version->AddAttribute(wxT("zv"),ParsedParameters.GetZieglerVersion());
  wxXmlNode* fileversion = new wxXmlNode(ziegler, wxXML_ELEMENT_NODE, "File_Details");
   wxXmlNode* details = new wxXmlNode(fileversion, wxXML_ELEMENT_NODE, "Contents");
   wxXmlNode* programdatetime = new wxXmlNode(details, wxXML_ELEMENT_NODE, "Date_File_Creation");
     programdatetime->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, ActualTime));
   wxXmlNode* programversion = new wxXmlNode(details, wxXML_ELEMENT_NODE, "Program_Version");
     programversion->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, wxT("4.20")));
    wxXmlNode* programname = new wxXmlNode(details, wxXML_ELEMENT_NODE, "Program_Name");
     programname->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, wxT("ERYA-Bulk")));
 // Save the file
 return LocalZiegler.Save(ZieglerFileName,0);
 }
 else if (ZieglerFileVersion == wxT("txt")) // Save on LabView Source format, i.e., a standard text file
 {
  wxTextFile file(ZieglerFileName);
  file.Create();
  if(TableMode == 0)
  {
  for(int j=0; j<ParsedParameters.GetZieglerElements().GetCount(); j++)
  {
   wxString c0 = wxString::Format("%i", j + 1);
   wxString c1 = ParsedParameters.GetZieglerValues1().Item(j);
   wxString c2 = ParsedParameters.GetZieglerValues2().Item(j);
   wxString c3 = ParsedParameters.GetZieglerValues3().Item(j);
   wxString c4 = ParsedParameters.GetZieglerValues4().Item(j);
   wxString c5 = ParsedParameters.GetZieglerValues5().Item(j);
   wxString c6 = ParsedParameters.GetZieglerValues6().Item(j);
   wxString c7 = ParsedParameters.GetZieglerValues7().Item(j);
   wxString c8 = ParsedParameters.GetZieglerValues8().Item(j);
   wxString c9 = ParsedParameters.GetZieglerValues9().Item(j);
   wxString c10 = ParsedParameters.GetZieglerValues10().Item(j);
   wxString c11 = ParsedParameters.GetZieglerValues11().Item(j);
   wxString c12 = ParsedParameters.GetZieglerValues12().Item(j);
   wxString c13 = ParsedParameters.GetZieglerMass().Item(j);
   if (c0.Len()>0 && c1.Len()>0 && c2.Len()>0 && c3.Len()>0 && c4.Len()>0 && c5.Len()>0 && c6.Len()>0 && c7.Len()>0 && c8.Len()>0 && c9.Len()>0 && c10.Len()>0 && c11.Len()>0 && c12.Len()>0 && c13.Len()>0)
  file.AddLine( c0 + wxT("\t") + c1 + wxT("\t") + c2 + wxT("\t") + c3 + wxT("\t") + c4 + wxT("\t") + c5 + wxT("\t") + c6 + wxT("\t") + c7 + wxT("\t") + c8 + wxT("\t") + c9 + wxT("\t") + c10 + wxT("\t") + c11 + wxT("\t") + c12 + wxT("\t") + c13 );
  }
  }
  else
  {
   int CurrentAdress = ParsedTables.GetElementAdress(TableMode);
   for(int j=0; j<ParsedTables.Item(CurrentAdress).GetElementEnergy().GetCount(); j++)
   {
   wxString c0 = ParsedTables.Item(CurrentAdress).GetElementEnergy().Item(j);
   wxString c1 = ParsedTables.Item(CurrentAdress).GetElementStoppingPower().Item(j);
   if (c0.Len()>0 && c1.Len()>0)
   file.AddLine( c0 + wxT("\t") + c1 );
   }
  }
 file.Write();
 file.Close();
 return true;
 }
 else if (ZieglerFileVersion == wxT("xlsx")) //Save as an Excel Xlsx file format, which calls own methods
 {
  //First transform the Ziegler array in to a TableMatrix data block
  if(TableMode == 0)
  {
  TableMatrix ZieglerMatrix(ParsedParameters.GetZieglerElements().GetCount(),14);
  ZieglerMatrix.Add(TableNode(wxT("Element Name"),0));
  ZieglerMatrix.Add(TableNode(wxT("A-1"),0));
  ZieglerMatrix.Add(TableNode(wxT("A-2"),0));
  ZieglerMatrix.Add(TableNode(wxT("A-3"),0));
  ZieglerMatrix.Add(TableNode(wxT("A-4"),0));
  ZieglerMatrix.Add(TableNode(wxT("A-5"),0));
  ZieglerMatrix.Add(TableNode(wxT("A-6"),0));
  ZieglerMatrix.Add(TableNode(wxT("A-7"),0));
  ZieglerMatrix.Add(TableNode(wxT("A-8"),0));
  ZieglerMatrix.Add(TableNode(wxT("A-9"),0));
  ZieglerMatrix.Add(TableNode(wxT("A-10"),0));
  ZieglerMatrix.Add(TableNode(wxT("A-11"),0));
  ZieglerMatrix.Add(TableNode(wxT("A-12"),0));
  ZieglerMatrix.Add(TableNode(wxT("Atomic Mass"),0));
  for(int j=0; j<ParsedParameters.GetZieglerElements().GetCount(); j++)
  {
   wxString c0 = ParsedParameters.GetZieglerElements().Item(j);
   wxString c1 = ParsedParameters.GetZieglerValues1().Item(j);
   wxString c2 = ParsedParameters.GetZieglerValues2().Item(j);
   wxString c3 = ParsedParameters.GetZieglerValues3().Item(j);
   wxString c4 = ParsedParameters.GetZieglerValues4().Item(j);
   wxString c5 = ParsedParameters.GetZieglerValues5().Item(j);
   wxString c6 = ParsedParameters.GetZieglerValues6().Item(j);
   wxString c7 = ParsedParameters.GetZieglerValues7().Item(j);
   wxString c8 = ParsedParameters.GetZieglerValues8().Item(j);
   wxString c9 = ParsedParameters.GetZieglerValues9().Item(j);
   wxString c10 = ParsedParameters.GetZieglerValues10().Item(j);
   wxString c11 = ParsedParameters.GetZieglerValues11().Item(j);
   wxString c12 = ParsedParameters.GetZieglerValues12().Item(j);
   wxString c13 = ParsedParameters.GetZieglerMass().Item(j);
    if (c0.Len()>0 && c1.Len()>0 && c2.Len()>0 && c3.Len()>0 && c4.Len()>0 && c5.Len()>0 && c6.Len()>0 && c7.Len()>0 && c8.Len()>0 && c9.Len()>0 && c10.Len()>0 && c11.Len()>0 && c12.Len()>0 && c13.Len()>0)
    {
         ZieglerMatrix.Add(TableNode(c0,0));
         ZieglerMatrix.Add(TableNode(c1,1));
         ZieglerMatrix.Add(TableNode(c2,1));
         ZieglerMatrix.Add(TableNode(c3,1));
         ZieglerMatrix.Add(TableNode(c4,1));
         ZieglerMatrix.Add(TableNode(c5,1));
         ZieglerMatrix.Add(TableNode(c6,1));
         ZieglerMatrix.Add(TableNode(c7,1));
         ZieglerMatrix.Add(TableNode(c8,1));
         ZieglerMatrix.Add(TableNode(c9,1));
         ZieglerMatrix.Add(TableNode(c10,1));
         ZieglerMatrix.Add(TableNode(c11,1));
         ZieglerMatrix.Add(TableNode(c12,1));
         ZieglerMatrix.Add(TableNode(c13,1));
    }
  }
  //Finally, save the content as a Xlsx file
  XlsxFile ZieglerXlsxFile(ZieglerFileName, ZieglerMatrix, wxT("A1"));
  ZieglerXlsxFile.WriteFile();
  return true;
  }
  else
  {
  TableMatrix ZieglerMatrix(ParsedParameters.GetZieglerElements().GetCount(),2);
  ZieglerMatrix.Add(TableNode(wxT("Energy"),0));
  ZieglerMatrix.Add(TableNode(wxT("Stopping Power"),0));
  int CurrentAdress = ParsedTables.GetElementAdress(TableMode);
  for(int j=0; j<ParsedTables.Item(CurrentAdress).GetElementEnergy().GetCount(); j++)
  {
   wxString c0 = ParsedTables.Item(CurrentAdress).GetElementEnergy().Item(j);
   wxString c1 = ParsedTables.Item(CurrentAdress).GetElementStoppingPower().Item(j);
    if (c0.Len()>0 && c1.Len()>0)
    {
         ZieglerMatrix.Add(TableNode(c0,1));
         ZieglerMatrix.Add(TableNode(c1,1));
    }
  }
  //Finally, save the content as a Xlsx file
  XlsxFile ZieglerXlsxFile(ZieglerFileName, ZieglerMatrix, wxT("A1"));
  ZieglerXlsxFile.WriteFile();
  return true;
  }
 }
 else
 {
 wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Cannot save Ziegler Parameters file in this format!"), wxT("Invalid Format"), wxOK | wxICON_ERROR);
 dial->ShowModal();
 return false;
 }
}
