/***************************************************************
 * Name:      ReactionYield.cpp
 * Purpose:   ERYA Reaction Yield implementation
 * Author:    Vasco Manteigas (vm.manteigas@campus.fct.utl.pt)
 * Created:   2016-10-01
 * Copyright: Grupo de Reacoes Nucleares - LIBPHYS
 * License:
 **************************************************************/

#include "ReactionYield.h"
#include <wx/arrimpl.cpp>
WX_DEFINE_OBJARRAY( ElementArray);
WX_DEFINE_OBJARRAY( CompoundArray);
WX_DEFINE_OBJARRAY( FittingParameterArray);
WX_DEFINE_OBJARRAY( YieldArray);

// Detector Default Constructor
Detector::Detector(DetectorParameters DetectorSetup)
{
 if (DetectorSetup.GetDetectorEnergy().GetCount()>0)  // Get the mandatory pair of Energy,Sigma data
 {
   Energy = Vector(DetectorSetup.GetDetectorEnergy().GetCount());
   Sigma = Vector(DetectorSetup.GetDetectorEfficiency().GetCount());
   for (int i=0;i<DetectorSetup.GetDetectorEnergy().GetCount();i++)
   {
     double temp1,temp2; // Notice the conversion inside the vector !!! Otherwise it would get random numbers!!!
     wxString stemp1 = DetectorSetup.GetDetectorEnergy().Item(i);
     wxString stemp2 = DetectorSetup.GetDetectorEfficiency().Item(i);
     Energy.SetValue(i,stemp1.ToDouble(&temp1));
     Energy.SetValue(i,temp1);
     Sigma.SetValue(i,stemp2.ToDouble(&temp2));
     Sigma.SetValue(i,temp2);
   }
   // Sorting routine for Energy vector
   int n = Energy.GetNumberElements();
   for (int i=0; i<n-1; i++)
   {
     for(int j=0; j<n-i-1; j++)
     {
      if (Energy.GetValue(j) > Energy.GetValue(j+1))
      {
      double temp1 = Energy.GetValue(j);
      double temp2 = Sigma.GetValue(j);
      Energy.SetValue(j,Energy.GetValue(j+1));
      Sigma.SetValue(j,Sigma.GetValue(j+1));
      Energy.SetValue(j+1,temp1);
      Sigma.SetValue(j+1,temp2);
      }
     }
   }
   // Handles a user-defined fitting function
   if(DetectorSetup.GetDetectorFunction().Len() == 0) //Handles the empty function case, which is fine, as long exists interpolation data.
   {
    if(Energy.GetNumberElements()>0 && Sigma.GetNumberElements()>0)
    {
     Sucess = true;
     DefinedFunction = false;
    }
    else
    {
     Sucess = false;
     DefinedFunction = false;
     wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("There is no Detector efficiency data to handle!") , wxT("Detector Setup Failure"), wxOK | wxICON_ERROR);
     dial->ShowModal();
    }
   }
   else // Parse the custom-defined function
   {
    DetectorFittingFunction = AlgebraicFunction(DetectorSetup.GetDetectorFunction());
    if (DetectorFittingFunction.GetErrorString().Len() == 0)
    {
     if(DetectorFittingFunction.IsFunctionDefined())
     {
      Sucess = true;
      DefinedFunction = true;
     }
     else // Requires a correct function definition, that is, what is the independent and dependent variables ?
     {
      Sucess = false;
      DefinedFunction = false;
      wxMessageDialog *dial = new wxMessageDialog(NULL, DetectorFittingFunction.GetErrorString() , wxT("Detector Setup Failure"), wxOK | wxICON_ERROR);
      dial->ShowModal();
     }
    }
    else // Algebraic or syntax errors will halt the fit procedure
    {
      Sucess = false;
      DefinedFunction = false;
     wxMessageDialog *dial = new wxMessageDialog(NULL, DetectorFittingFunction.GetErrorString() , wxT("Detector Setup Failure"), wxOK | wxICON_ERROR);
     dial->ShowModal();
    }
   }
 }
 else    // You cannot evaluate anything without the Detector Database!
 {
   Sucess = false;
   DefinedFunction = false;
   wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Error: Detector Data Empty!") , wxT("Detector Setup Failure"), wxOK | wxICON_ERROR);
   dial->ShowModal();
 }
}


// Evaluate the numerical result of the function, when an argument is issued
double Detector::GetEval(double arg)
{
  if (DefinedFunction && arg>0)    // Get the energy value, if a fitting function was provided
  {
   double FunctionResult = DetectorFittingFunction.GetFyxEval(arg);
   if(std::isfinite(FunctionResult))
    return FunctionResult;
   else
    return 0; // Avoid overflows, by setting at out-of-boundary result to zero, since the yield will be reset to zero.
  }
  else if ( !DefinedFunction && Energy.GetNumberElements()>0 && arg > 0)   // If any fitting function was not provided, the only hope is to interpolate two pairs of neighbours points
  {
   double epsilon1,epsilon2,E1,E2;
   if (Energy.Maximum()>arg && Energy.Minimum()<arg)  // If the point was outside the bundaries, the final result will be set to zero.
   {
   for (int i=0; i<Energy.GetNumberElements(); i++)  // In some situations, some gamma points matches exactly at some element of main table
    {
    if (Energy.GetValue(i) == arg)
     {
       return Sigma.GetValue(i);
     }
    }
   for (int j=0; j<Energy.GetNumberElements() - 1; j++)  // But in general situations, it is necessary to interpolate two points.
    {
    if (Energy.GetValue(j)<arg && arg<Energy.GetValue(j+1))
     {
      E1 = Energy.GetValue(j);
      E2 = Energy.GetValue(j+1);
      epsilon1 = Sigma.GetValue(j);
      epsilon2 = Sigma.GetValue(j+1);
      double slope = (arg - E1)/(E2 - E1);
      double linear = epsilon1 + slope*(epsilon2 - epsilon1);
      return linear;
     }
    }
    return 0;
   }
   else
  {
   return 0;
  }
  }
  else
  {
   return 0;
  }
}


// Store the elements parameters in pure numerical forms
Element::Element(ElementDatabase ThisElement, ZieglerParameters ThisZiegler, ElementSRIMArray ThisSRIM,  wxString MassFractionGuess, wxString StoichiometricGuess, wxString Group, wxString YieldFit, wxString CalibrationFactor, int ElementCounter)
{
  // Convert the original table of strings to form a numerical table it's straightforward
  Group.ToLong(&CompoundGroup);
  AlgebraicFunction TestMG(MassFractionGuess);
  if(TestMG.GetErrorString().Len()==0)
  {
    Mass = TestMG.GetAnsEval();
  }
  else
  {
    Stoichiometry = 1.0;
  }
  AlgebraicFunction TestSG(StoichiometricGuess);
  if(TestSG.GetErrorString().Len()==0)
  {
    Stoichiometry = TestSG.GetAnsEval();
  }
  else
  {
    Stoichiometry = 1.0;
  }
  if(YieldFit == wxT("Yes"))
  {
   ElementFit = true;
  }
   else
  {
   ElementFit = false;
  }
  ElementID = ElementCounter;
  Symbol = ThisElement.GetElement();
  ThisElement.GetGamma().ToDouble(&GammaPeak);
  ThisElement.GetNumber().ToLong(&Number);
  ThisElement.GetAbundance().ToDouble(&Abundance);
  CalibrationFactor.ToDouble(&SigmaCalibrationFactor);
  if (ThisElement.GetAtomic().Len()>0)
  {
   ThisElement.GetAtomic().ToDouble(&Atomic);
  }
   else
  {
   ThisZiegler.GetZieglerMass().Item(Number-1).ToDouble(&Atomic);
  }
  ThisElement.GetIsotopic().ToDouble(&Isotopic);
  Energy = Vector(ThisElement.GetEnergy().GetCount());
  EnergyError = Vector(ThisElement.GetEnergyError().GetCount());
  Sigma = Vector(ThisElement.GetSigma().GetCount());
  SigmaError = Vector(ThisElement.GetSigmaError().GetCount());
  for(int i=0; i<ThisElement.GetEnergy().GetCount(); i++)
  {
    double temp1, temp2, temp3, temp4;
    Energy.SetValue(i,ThisElement.GetEnergy().Item(i).ToDouble(&temp1));
    EnergyError.SetValue(i,ThisElement.GetEnergyError().Item(i).ToDouble(&temp2));
    Sigma.SetValue(i,ThisElement.GetSigma().Item(i).ToDouble(&temp3));
    SigmaError.SetValue(i,ThisElement.GetSigmaError().Item(i).ToDouble(&temp4));
    Energy.SetValue(i,temp1);
    EnergyError.SetValue(i,temp2);
    Sigma.SetValue(i,temp3);
    SigmaError.SetValue(i,temp4);
  }
  // Sorting routine for Energy vector
   int n = Energy.GetNumberElements();
   for (int i=0; i<n-1; i++)
   {
     for(int j=0; j<n-i-1; j++)
     {
      if (Energy.GetValue(j) > Energy.GetValue(j+1))
      {
      double temp1 = Energy.GetValue(j);
      double temp3 = Sigma.GetValue(j);
      double temp2 = EnergyError.GetValue(j);
      double temp4 = SigmaError.GetValue(j);
      Energy.SetValue(j,Energy.GetValue(j+1));
      Sigma.SetValue(j,Sigma.GetValue(j+1));
      EnergyError.SetValue(j,EnergyError.GetValue(j+1));
      SigmaError.SetValue(j,SigmaError.GetValue(j+1));
      Energy.SetValue(j+1,temp1);
      Sigma.SetValue(j+1,temp3);
      EnergyError.SetValue(j+1,temp2);
      SigmaError.SetValue(j+1,temp4);
      }
     }
   }
// Load the adequate SRIM table from the database
  int ZNumber = static_cast<int>(Number);
  int ZAdress = ThisSRIM.GetElementAdress(ZNumber);
  if((ThisSRIM.GetElementAdress(ZNumber) != wxNOT_FOUND) && (ThisSRIM.Item(ZAdress).GetElementEnergy().GetCount() == ThisSRIM.Item(ZAdress).GetElementStoppingPower().GetCount()))
  {
    SRIMEnergy = Vector(ThisSRIM.Item(ZAdress).GetElementEnergy().GetCount()); //Once confirmed the existence, load and convert the original data into an array of vectors.
    SRIMStoppingPower = Vector(ThisSRIM.Item(ZAdress).GetElementEnergy().GetCount());
    for (int k=0; k<ThisSRIM.Item(ZAdress).GetElementEnergy().GetCount(); k++)
    {
      double temp0,temp1;
      wxString c0 = ThisSRIM.Item(ZAdress).GetElementEnergy().Item(k);
      wxString c1 = ThisSRIM.Item(ZAdress).GetElementStoppingPower().Item(k);
      if(c0.ToDouble(&temp0) && c1.ToDouble(&temp1)) //If the pair of values are numerical data, just copy it.
      {
        SRIMEnergy.SetValue(k,temp0);
        SRIMStoppingPower.SetValue(k,temp1);
      }
    }
    // Sorting routine
    int n = SRIMEnergy.GetNumberElements();
    for (int i=0; i<n-1; i++)
    {
     for(int j=0; j<n-i-1; j++)
     {
      if (SRIMEnergy.GetValue(j) > SRIMEnergy.GetValue(j+1))
      {
      double temp1 = SRIMEnergy.GetValue(j);
      double temp2 = SRIMStoppingPower.GetValue(j);
      SRIMEnergy.SetValue(j,SRIMEnergy.GetValue(j+1));
      SRIMStoppingPower.SetValue(j,SRIMStoppingPower.GetValue(j+1));
      SRIMEnergy.SetValue(j+1,temp1);
      SRIMStoppingPower.SetValue(j+1,temp2);
      }
     }
    }
  }
  else //Without SRIM data, the vectors are left empty
  {
    SRIMEnergy = Vector();
    SRIMStoppingPower = Vector();
  }
  // Load the adequate Ziegler parameters from the database
  if (ThisZiegler.GetZieglerElements().GetCount()>Number)
   {
     ThisZiegler.GetZieglerVersion().ToLong(&ZieglerModel);
     ZieglerTable = Vector(12);
     std::vector<double> ZieglerData;
     ZieglerData.push_back(Number*1.0);
     double temp1, temp2, temp3, temp4, temp5, temp6, temp7, temp8, temp9, temp10, temp11, temp12, extra13;
     ZieglerTable.SetValue(0,ThisZiegler.GetZieglerValues1().Item(Number-1).ToDouble(&temp1));
     ZieglerTable.SetValue(1,ThisZiegler.GetZieglerValues2().Item(Number-1).ToDouble(&temp2));
     ZieglerTable.SetValue(2,ThisZiegler.GetZieglerValues3().Item(Number-1).ToDouble(&temp3));
     ZieglerTable.SetValue(3,ThisZiegler.GetZieglerValues4().Item(Number-1).ToDouble(&temp4));
     ZieglerTable.SetValue(4,ThisZiegler.GetZieglerValues5().Item(Number-1).ToDouble(&temp5));
     ZieglerTable.SetValue(5,ThisZiegler.GetZieglerValues6().Item(Number-1).ToDouble(&temp6));
     ZieglerTable.SetValue(6,ThisZiegler.GetZieglerValues7().Item(Number-1).ToDouble(&temp7));
     ZieglerTable.SetValue(7,ThisZiegler.GetZieglerValues8().Item(Number-1).ToDouble(&temp8));
     ZieglerTable.SetValue(8,ThisZiegler.GetZieglerValues9().Item(Number-1).ToDouble(&temp9));
     ZieglerTable.SetValue(9,ThisZiegler.GetZieglerValues10().Item(Number-1).ToDouble(&temp10));
     ZieglerTable.SetValue(10,ThisZiegler.GetZieglerValues11().Item(Number-1).ToDouble(&temp11));
     ZieglerTable.SetValue(11,ThisZiegler.GetZieglerValues12().Item(Number-1).ToDouble(&temp12));
     ZieglerTable.SetValue(0,temp1);
     ZieglerTable.SetValue(1,temp2);
     ZieglerTable.SetValue(2,temp3);
     ZieglerTable.SetValue(3,temp4);
     ZieglerTable.SetValue(4,temp5);
     ZieglerTable.SetValue(5,temp6);
     ZieglerTable.SetValue(6,temp7);
     ZieglerTable.SetValue(7,temp8);
     ZieglerTable.SetValue(8,temp9);
     ZieglerTable.SetValue(9,temp10);
     ZieglerTable.SetValue(10,temp11);
     ZieglerTable.SetValue(11,temp12);
     ZieglerData.push_back(temp1);
     ZieglerData.push_back(temp2);
     ZieglerData.push_back(temp3);
     ZieglerData.push_back(temp4);
     ZieglerData.push_back(temp5);
     ZieglerData.push_back(temp6);
     ZieglerData.push_back(temp7);
     ZieglerData.push_back(temp8);
     ZieglerData.push_back(temp9);
     ZieglerData.push_back(temp10);
     ZieglerData.push_back(temp11);
     ZieglerData.push_back(temp12);
     ThisZiegler.GetZieglerMass().Item(Number-1).ToDouble(&extra13);
     ZieglerData.push_back(extra13);
     if(ZieglerModel == 2)
     {
      CustomZieglerFunction = AlgebraicFunction(ThisZiegler.GetZieglerFunction());
      if(CustomZieglerFunction.SetFunctionNumberParameter(ZieglerData))
      {
        Sucess = true;
      }
      else
      {
        Sucess = false;
        wxMessageDialog *dial = new wxMessageDialog(NULL, CustomZieglerFunction.GetErrorString() , wxT("Invalid Custom Ziegler Function"), wxOK | wxICON_ERROR);
        dial->ShowModal();
      }
     }
   }
   else // Element not declared, so it will be set equal zero
   {
     ZieglerTable = Vector(12);
     std::vector<double> ZieglerData;
     ZieglerData.push_back(Number*1.0);
     for (int j=0; j<12; j++)
     {
       ZieglerTable.SetValue(j,0);
       ZieglerData.push_back(0.0);
     }
     if(ZieglerModel > 1)
     {
      CustomZieglerFunction = AlgebraicFunction(ThisZiegler.GetZieglerFunction());
      if(CustomZieglerFunction.SetFunctionNumberParameter(ZieglerData))
      {
        Sucess = true;
      }
      else
      {
        Sucess = false;
        wxMessageDialog *dial = new wxMessageDialog(NULL, CustomZieglerFunction.GetErrorString() , wxT("Invalid Custom Ziegler Function"), wxOK | wxICON_ERROR);
        dial->ShowModal();
      }
     }
   }
   Sucess = true;
}


// Calculate the cross-section from the table of data
double Element::EvaluateSigma(double AtEnergy)
{
 if (Energy.GetNumberElements()>0 && AtEnergy > 0)
 {
   double epsilon1,epsilon2,E1,E2;
   // If the point was outside the boundaries, the final result will be set to zero.
   if ((Energy.Minimum() <= AtEnergy) && (AtEnergy <= Energy.Maximum()))
   {
   for (int i=0; i<Energy.GetNumberElements(); i++)  // In some situations, some gamma points matches exactly at some element of main table
    {
    if (Energy.GetValue(i) == AtEnergy)
     {
       return Sigma.GetValue(i);
     }
    }
   for (int j=0; j<Energy.GetNumberElements() - 1; j++)  // But in general situations, it is necessary to interpolate two points.
    {
    if (AtEnergy>Energy.GetValue(j) && AtEnergy<Energy.GetValue(j+1))
     {
      // Boundary values for Energy and Cross-Section
      E1 = Energy.GetValue(j);
      E2 = Energy.GetValue(j+1);
      epsilon1 = Sigma.GetValue(j);
      epsilon2 = Sigma.GetValue(j+1);
      // Apply a linear interpolation: y=a*x+b
      double slope = (epsilon2 - epsilon1) / (E2 - E1); // a
      double initial = epsilon1 - slope * E1; // b
      return slope * AtEnergy + initial;
//      double slope = (AtEnergy - E1)/(E2 - E1);
//      double linear = epsilon1 + slope*(epsilon2 - epsilon1);
//      return linear;
     }
    }
    return 0;
   }
   else
   {
    return 0;
   }
  }
  else
  {
   return 0;
  }
}

// From an initial value, find the next tabulated value on cross-section table
double Element::GetNextEnergyTable(double InitialEnergy)
{
 if (Energy.GetNumberElements()>0 )
 {
  for (int i=0; i<Energy.GetNumberElements(); i++)  // Search from the first to the last increasing value, until find the first value that is greater than the argument
    {
    if (Energy.GetValue(i) > InitialEnergy)
     {
       return Energy.GetValue(i); // Return the required value
     }
    }
   return 0;
 }
 else
 {
   return 0;
 }
}


// Evaluate the Ziegler function
double Element::EvaluateZiegler(double AtEnergy)
{
 if (ZieglerModel == 0) // 1977 Ziegler Model
 {
  if ( AtEnergy >= 0 && AtEnergy < 10)  // At very low energies
  {
    return ZieglerTable.GetValue(0)*std::sqrt(AtEnergy);
  }
  else  if (AtEnergy > 10 && AtEnergy < 1000)  // Medium range energy
  {
    double StoppingLow, StoppingHigh, Stopping;
    StoppingLow = ZieglerTable.GetValue(1)*std::pow(AtEnergy,0.45);
    StoppingHigh = (ZieglerTable.GetValue(2)/AtEnergy)*std::log(1 + (ZieglerTable.GetValue(3)/AtEnergy) + (ZieglerTable.GetValue(4)*AtEnergy));
    Stopping = (StoppingLow*StoppingHigh)/(StoppingHigh+StoppingLow);
    return Stopping;
  }
  else   // At high energies, greater that 1 MeV
  {
    double ProtonMass = 938280; // Proton Mass in keV
    double RelativisticVelocity = (AtEnergy*AtEnergy + 2 * AtEnergy * ProtonMass)/((ProtonMass + AtEnergy) * (ProtonMass + AtEnergy));
    double PolynomialStopping = ZieglerTable.GetValue(7) + ZieglerTable.GetValue(8) * std::log(AtEnergy) + ZieglerTable.GetValue(9) * std::pow(std::log(AtEnergy),2) + ZieglerTable.GetValue(10) * std::pow(std::log(AtEnergy),3) + ZieglerTable.GetValue(11) * std::pow(std::log(AtEnergy),4);
    double LogarithmStopping = std::log(ZieglerTable.GetValue(6)*RelativisticVelocity/(1-RelativisticVelocity)) - RelativisticVelocity;
    double Stopping = (ZieglerTable.GetValue(5)/RelativisticVelocity)*(LogarithmStopping - PolynomialStopping);
    return Stopping;
  }
 }
 else if (ZieglerModel == 1) // 1991 Ziegler Model, which contains a single condition
  {
   if ( AtEnergy >= 0 && AtEnergy < 25)  // At very low energies, to avoid an undefined evaluation
   {
    return 0;
   }
   else
   {
    double StoppingLow, StoppingHigh, Stopping;
    StoppingLow = ZieglerTable.GetValue(0)*std::pow(AtEnergy,ZieglerTable.GetValue(1)) + ZieglerTable.GetValue(2)*std::pow(AtEnergy,ZieglerTable.GetValue(3));
    StoppingHigh = (ZieglerTable.GetValue(4)/std::pow(AtEnergy,ZieglerTable.GetValue(5)))*std::log( (ZieglerTable.GetValue(6)/AtEnergy) + (ZieglerTable.GetValue(7)*AtEnergy));
    Stopping = (StoppingLow*StoppingHigh)/(StoppingHigh+StoppingLow);
    return Stopping;
   }
  }
  else if (ZieglerModel == 2) // Apply a custom function
  {
    return CustomZieglerFunction.GetFyxEval(AtEnergy);
  }
  else if (ZieglerModel == 3) // Apply the SRIM interpolation tables
  {
   if (SRIMEnergy.GetNumberElements()>0 && SRIMStoppingPower.GetNumberElements()>0 && AtEnergy > 0)
   {
   double epsilon1,epsilon2,E1,E2;
   // If the point was outside the boundaries, the final result will be set to zero.
   if ((SRIMEnergy.Minimum() <= AtEnergy) && (AtEnergy <= SRIMEnergy.Maximum()))
   {
   for (int i=0; i<Energy.GetNumberElements(); i++)  // In some situations, some gamma points matches exactly at some element of main table
    {
    if (SRIMEnergy.GetValue(i) == AtEnergy)
     {
       return SRIMStoppingPower.GetValue(i);
     }
    }
    for (int j=0; j<SRIMEnergy.GetNumberElements() - 1; j++)  // But in general situations, it is necessary to interpolate two points.
     {
     if (AtEnergy>SRIMEnergy.GetValue(j) && AtEnergy<SRIMEnergy.GetValue(j+1))
      {
      E1 = SRIMEnergy.GetValue(j);
      E2 = SRIMEnergy.GetValue(j+1);
      epsilon1 = SRIMStoppingPower.GetValue(j);
      epsilon2 = SRIMStoppingPower.GetValue(j+1);
       // Apply a linear interpolation: y=a*x+b
      double slope = (epsilon2 - epsilon1) / (E2 - E1); // a
      double initial = epsilon1 - slope * E1; // b
      return slope * AtEnergy + initial;
//      double slope = (AtEnergy - E1)/(E2 - E1);
//      double linear = epsilon1 + slope*(epsilon2 - epsilon1);
//      return linear;
      }
     }
    return 0;
    }
    else if (AtEnergy > 0 &&  AtEnergy < SRIMEnergy.Minimum()) // Apply a linear continuity to low energies, to avoid false zero values at finite energy losses
    {
      double slope = AtEnergy / SRIMEnergy.Minimum();
      double linear = slope * SRIMStoppingPower.GetValue(SRIMEnergy.AtMinimum());
      return linear;
    }
    else if (AtEnergy > SRIMEnergy.Maximum())  // For higher energies, the stopping power goes to an approximate constant value.
    {
      return SRIMStoppingPower.GetValue(SRIMEnergy.AtMaximum());
    }
   else // Default value
    {
    return 0;
    }
   }
   else // Empty Ziegler stack
   {
   return 0;
   }
  }
  else // Default value
  {
    return 0;
  }
}

// Evaluates the total stoichiometry of all Elements found on main spreadsheet
double ElementExtra::GetTotalStoichiometry()
{
 TotalStoichiometry = 0;
 for (int i=0; i<this->GetCount(); i++)
 {
  TotalStoichiometry = TotalStoichiometry + this->Item(i).GetStoichiometry();
 }
 return TotalStoichiometry;
}

// Evaluates the total atomic and/or isotopic mass of all Elements found on main spreadsheet
double ElementExtra::GetTotalMass()
{
 TotalMass = 0;
 for (int i=0; i<this->GetCount(); i++)
 {
  TotalMass = TotalMass + this->Item(i).GetStoichiometry() * this->Item(i).GetIsotopicMass();
 }
 return TotalMass;
}

// Evaluates the number of fitting parameters, which is number of Elements marked with "Fit" flag,  and is always less the total number of Elements
int ElementExtra::GetNumberFittingParameters()
{
 NumberParameters = 0;
 for (int i=0; i<this->GetCount(); i++)
 {
  if(this->Item(i).GetElementFit())
  {
   NumberParameters = NumberParameters + 1;
  }
 }
 return NumberParameters;
}

// Return the relevant fitting element adress from the ordened sequence of all elements.
int ElementExtra::GetFittingElementItem(int FindValue)
{
 int FittingElementItem = 0;
 for (int i=0; i<this->GetCount(); i++)
 {
  if(this->Item(i).GetElementFit())
  {
   if(FindValue == FittingElementItem)
    {
     return i;
    }
    else
    {
     FittingElementItem = FittingElementItem + 1;
    }
  }
 }
 return -1;
}

// Update the stoichiometry fromthe selected Elements
bool ElementExtra::SetStoichiometryFromTo(IntegerVector ElementID, Vector NewValues)
{
 for(int i=0; i<ElementID.GetNumberElements(); i++)
 {
  if(ElementID.GetValue(i) < this->GetCount())
  {
   this->Item(ElementID.GetValue(i)).SetStoichiometry(NewValues.GetValue(i));
  }
  else
  {
   return false;
  }
 }
 return true;
}

// Get the stoichiometric values of fitting elements only.
Vector ElementExtra::GetTotalFitStoichiometry()
{
 Vector results = Vector(this->GetNumberFittingParameters());
 int j=0;
 for (int i=0; i<this->GetCount(); i++)
 {
  if(this->Item(i).GetElementFit())
  {
   results.SetValue(j,this->Item(i).GetStoichiometry());
   j = j + 1;
  }
 }
 return results;
}

// Get all the stoichiometric ratios.
Vector ElementExtra::GetAllStoichiometry()
{
 Vector results = Vector(this->GetCount());
 for (int i=0; i<this->GetCount(); i++)
 {
   results.SetValue(i,this->Item(i).GetStoichiometry());
 }
 return results;
}

// Get all the elements mass
Vector ElementExtra::GetAllMass()
{
 Vector results = Vector(this->GetCount());
 for (int i=0; i<this->GetCount(); i++)
 {
   results.SetValue(i,this->Item(i).GetStoichiometry() * this->Item(i).GetIsotopicMass());
 }
 return results;
}

// Evaluate the weight stoichiometry ratio
double ElementExtra::GetWeightStoichiometry(int i)
{
 double WeightStoichiometry = 0;
 for(int k=0; k<this->GetCount(); k++)
 {
   WeightStoichiometry = WeightStoichiometry + this->Item(k).GetStoichiometry() * this->Item(k).GetAtomicMass();
 }
 if(std::abs(WeightStoichiometry)>0)
 {
   return this->Item(i).GetStoichiometry() * this->Item(i).GetAtomicMass() / WeightStoichiometry;
 }
 else
 {
   return 0;
 }
}


// Evaluate the stopping power of all Elements, combined with the Bragg´s Law.
double ElementExtra::EvaluateBragg(double AtEnergy)
{
 double StoppingPowerSum = 0;
 for (int i=0; i<this->GetCount(); i++)
 {
  StoppingPowerSum = StoppingPowerSum + this->Item(i).EvaluateZiegler(AtEnergy) * this->GetWeightStoichiometry(i);
 }
 return StoppingPowerSum;
}

// Evaluate the corrected value for the cross-section
double ElementExtra::EvaluateCrossSection(int ElementID, double AtEnergy, double DeltaEnergy)
{
 if(DeltaEnergy > 0) // Will evaluate an average of sigma(E), by taking a trapezoidal rule over the points between E, and E+dE
 {
  double CurrentEnergy = AtEnergy;
  double FinalEnergy = AtEnergy + DeltaEnergy;
  double SumBlock = 0;
  double NextEnergyValue = 0;
  bool EvaluationComplete = false; // for the do-while cycle
  do
  {
    NextEnergyValue = this->Item(ElementID).GetNextEnergyTable(CurrentEnergy);
   if (NextEnergyValue == 0) // An error
   {
     return this->Item(ElementID).EvaluateSigma(AtEnergy) * this->Item(ElementID).GetSigmaCalibrationFactor();
   }
   else if(NextEnergyValue < FinalEnergy) // Found an inflection point between E and E + dE
   {
     double Block = (NextEnergyValue - CurrentEnergy) * (this->Item(ElementID).EvaluateSigma(CurrentEnergy) * this->Item(ElementID).GetSigmaCalibrationFactor() + this->Item(ElementID).EvaluateSigma(NextEnergyValue) * this->Item(ElementID).GetSigmaCalibrationFactor()) / 2;
     CurrentEnergy = NextEnergyValue;
     SumBlock = SumBlock + Block;
   }
   else
   {
     double Block = (FinalEnergy - CurrentEnergy) * (this->Item(ElementID).EvaluateSigma(CurrentEnergy) * this->Item(ElementID).GetSigmaCalibrationFactor() + this->Item(ElementID).EvaluateSigma(FinalEnergy) * this->Item(ElementID).GetSigmaCalibrationFactor()) / 2;
     SumBlock = SumBlock + Block;
     EvaluationComplete = true;
   }
  }
  while(!EvaluationComplete);
  return SumBlock; // Return the average cross-section., as intended.
 }
 else // Simply return the sigma(E) value
 {
  return this->Item(ElementID).EvaluateSigma(AtEnergy) * this->Item(ElementID).GetSigmaCalibrationFactor();
 }
}

// Return the entire molar mass of all Elements
double ElementExtra::GetMolarMass()
{
 double MolarMass = 0;
 for (int i=0; i<this->GetCount(); i++)
 {
  MolarMass = MolarMass + this->Item(i).GetAtomicMass() * this->Item(i).GetStoichiometry();
 }
 return MolarMass;
}

// Renormalize the stoichiometry of fitting Elements
bool ElementExtra::RenormStoichiometry()
{
 if(this->GetCount() == 0)
  return false;
 double ScaleStoichiometry = 0;
 for(int i=0; i<this->GetCount(); i++)
 {
  if(!(this->Item(i).GetElementFit()))
   ScaleStoichiometry = ScaleStoichiometry + this->Item(i).GetStoichiometry();
 }
 ScaleStoichiometry = 1 - ScaleStoichiometry;
 double PartialStoichiometrySum = 0;
 for(int i=0; i<this->GetCount(); i++)
 {
  if(this->Item(i).GetElementFit())
   PartialStoichiometrySum = PartialStoichiometrySum + this->Item(i).GetStoichiometry();
 }
 for(int i=0; i<this->GetCount(); i++)
 {
  double CurrentValue = this->Item(i).GetStoichiometry();
  if(this->Item(i).GetElementFit())
   this->Item(i).SetStoichiometry(CurrentValue*ScaleStoichiometry/PartialStoichiometrySum);
 }
 return true;
}

// Renormalize the stoichiometry of all Elements
bool ElementExtra::RenormStoichiometryTotal()
{
 double SumStoichiometry = this->GetTotalStoichiometry();
 if(this->GetCount() == 0)
  return false;
 for(int i=0; i<this->GetCount(); i++)
 {
  double temp = this->Item(i).GetStoichiometry();
  this->Item(i).SetStoichiometry(temp/SumStoichiometry);
 }
 return true;
}

// Evaluate the mass fraction of a element respective to the whole sample
double ElementExtra::GetMassFraction(int ElementID)
{
 double TotalStoichiometricMass = 0;
 for(int i=0; i<this->GetCount(); i++)
 {
   TotalStoichiometricMass = TotalStoichiometricMass + this->Item(i).GetStoichiometry() * this->Item(i).GetAtomicMass();
 }
 if (this->GetCount() > 0)
 {
  double WeightElement = this->Item(ElementID).GetStoichiometry() * this->Item(ElementID).GetAtomicMass();
  return WeightElement / TotalStoichiometricMass;
 }
 else
 {
  return 1.0;
 }
}


// Main constructor for Compound, that in this case, a single Element are sent to the designed group
Compound::Compound(Element ThisElement)
{
 CompoundGroupNumber = ThisElement.GetCompoundGroup();
 ChemicalFormula.Add(ThisElement);
}

// Merge two compounds to the original, if it shares the same group. Also return a boolean value if a merge actually occurs
bool Compound::MergeCompound(Compound Additional)
{
 if (Additional.GetGroupNumber() == 0 || CompoundGroupNumber == 0)
  {
  return false;
  }
  else
  {
   if (CompoundGroupNumber == Additional.GetGroupNumber())
   {
    for (int i=0; i<Additional.ChemicalFormula.GetCount(); i++)  // Copy the additional Elements
    {
     ChemicalFormula.Add(Additional.ChemicalFormula.Item(i));
    }
    return true;
   }
   else
   {
    return false;
   }
  }
}

// Get the number of fitting elements from the selected compound
int Compound::GetNumberFitElements()
{
 int NFE = 0;
 for(int i=0; i<ChemicalFormula.GetCount(); i++)
 {
   if(ChemicalFormula.Item(i).GetElementFit())
     NFE = NFE + 1;
 }
 return NFE;
}

// Generate the compounds from the main spreadsheet itself
CompoundExtra::CompoundExtra(ElementExtra CurrentSelectedElements)
{
 // Create a temporary Compound array, where use the alternative Compound constructor
 for (int i=0; i<CurrentSelectedElements.GetCount(); i++)  // Create the first list of compounds
   {
     Compound SingleElement = Compound(CurrentSelectedElements.Item(i));
     this->Add(SingleElement);
   }
 // Merging main cycle
  for (int j=0; j<this->GetCount(); j++)
   {
     for (int k=j+1; k<this->GetCount(); k++)
     {
       if(this->Item(j).MergeCompound(this->Item(k)))
        {
          this->RemoveAt(k);
          k--;
        }
     }
   }
  // Set flag for sucefulness
   Sucess = true;
}

// If the following Element belogs to a particular compound, it should return the Compound and Element Adress, otherwise it will return false.
bool CompoundExtra::FindElement(int ElementID, int& CompoundAdress, int& ElementAdress)
{
 CompoundAdress = 0;
 ElementAdress = 0;
 for(int i=0; i<this->GetCount(); i++)
 {
  for(int j=0; j<this->Item(i).GetNumberElements(); j++)
  {
    if(this->Item(i).GetElementIDAt(j) == ElementID)
    {
     CompoundAdress = i;
     ElementAdress = j;
     return true;
    }
  }
 }
 return false;
}


// Update the stoichiometry of the selected Element, that belongs to any Compound.
double CompoundExtra::SetStoichiometryAt(int ElementID, double value)
{
 int CompoundAdress, ElementAdress;
 if(this->FindElement(ElementID,CompoundAdress,ElementAdress))
 {
  return this->Item(CompoundAdress).SetStoichiometryAt(ElementAdress,value);
 }
 else
 {
  return 0;
 }
}

// Apply the Stoichiometry renormalization for all compounds (including the special set of isolated elements), for the fitted Elements
bool CompoundExtra::RenormStoichiometry()
{
 // First, get the sum of all stoichiometry
 if(this->GetCount() == 0)
  return false;
 double PartialStoichiometry = 0;
 double ScaleStoichiometry = 0;
 for(int i=0; i<this->GetCount(); i++)
 {
  for(int j=0; j<this->Item(i).GetNumberElements(); j++)
  {
    if(!(this->Item(i).GetElementFitAt(j)))
     ScaleStoichiometry = ScaleStoichiometry + this->Item(i).GetStoichiometryAt(j);
  }
 }
 ScaleStoichiometry = 1 - ScaleStoichiometry;
 for(int i=0; i<this->GetCount(); i++)
 {
  for(int j=0; j<this->Item(i).GetNumberElements(); j++)
  {
    if(this->Item(i).GetElementFitAt(j))
     PartialStoichiometry = PartialStoichiometry + this->Item(i).GetStoichiometryAt(j);
  }
 }
 // And finally, just renormalize the stoichiometry
 for(int i=0; i<this->GetCount(); i++)
 {
  for(int j=0; j<this->Item(i).GetNumberElements(); j++)
  {
    double CurrentValue = this->Item(i).GetStoichiometryAt(j);
    if(this->Item(i).GetElementFitAt(j))
     this->Item(i).SetStoichiometryAt(j,ScaleStoichiometry*CurrentValue/PartialStoichiometry);
  }
 }
 return true;
}

// Apply the Stoichiometry renormalization for all compounds (including the special set of isolated elements)
bool CompoundExtra::RenormStoichiometryTotal()
{
 // First, get the sum of all stoichiometry
 double TotalStoichiometry = 0;
 if(this->GetCount() == 0)
  return false;
 for(int i=0; i<this->GetCount(); i++)
 {
  for(int j=0; j<this->Item(i).GetNumberElements(); j++)
  {
    TotalStoichiometry = TotalStoichiometry + this->Item(i).GetStoichiometryAt(j);
  }
 }
 // And finally, just renormalize the stoichiometry
 for(int i=0; i<this->GetCount(); i++)
 {
  for(int j=0; j<this->Item(i).GetNumberElements(); j++)
  {
    double temp = this->Item(i).GetStoichiometryAt(j);
    this->Item(i).SetStoichiometryAt(j,temp/TotalStoichiometry);
  }
 }
 return true;
}


// Update the stoichiometry on all Compounds from the selected Elements
bool CompoundExtra::SetStoichiometryFromTo(IntegerVector ElementID, Vector NewValues)
{
 for(int i=0; i<ElementID.GetNumberElements(); i++)
 {
  int CompoundAdress,ElementAdress;
  if(this->FindElement(ElementID.GetValue(i),CompoundAdress,ElementAdress))
  {
   this->Item(CompoundAdress).SetStoichiometryAt(ElementAdress,NewValues.GetValue(i));
  }
  else
  {
   return false;
  }
 }
 return true;
}

// Classify the user inputs according to the type:
// 1. Only Atomic Fraction filled, which don't require conversion
// 2. Only Mass Fraction filled, requiring a trivial conversion.
// 3. Atomic Fraction filled with the a single Mass fraction value from each individual compound or independent element, requiring more conversion working.
// 0. Otherwise, return with an error.
bool CompoundExtra::CheckMassAtomicStoichiometry()
{
 // Create the counters and make a single run-check.
 int TotalMassZeros = 0;
 int TotalAtomicOnes = 0;
 int TotalMassNonZeros = 0;
 int TotalElements = 0;
 // An empty array return always false.
 if(this->GetCount() == 0)
  return false;
 for(int i=0; i<this->GetCount(); i++)
 {
  for(int j=0; j<this->Item(i).GetNumberElements(); j++)
  {
    double temp1 =  this->Item(i).GetStoichiometryAt(j); //Get Atomic Fraction
    double temp2 =  this->Item(i).GetMassQuantityAt(j); //Get Atomic Fraction
    // Sum the counters.
    TotalElements = TotalElements + 1;
    if (temp1 == 1)
        TotalAtomicOnes = TotalAtomicOnes + 1;
    else if (temp2 == 0)
        TotalMassZeros = TotalMassZeros + 1;
    else if (temp2 != 0)
        TotalMassNonZeros = TotalMassNonZeros + 1;
    else
        TotalMassNonZeros = TotalMassNonZeros * 1;
  }
 }
 // Classification
 if (TotalElements == TotalMassZeros)
 {
    MassAtomicStoichiometryMode = 0; // Both atomic and mass columns can be empty, since it is a particular case of the atomic filled column, and sets the default flag.
    return true;
 }
 else if ((TotalElements == TotalAtomicOnes) && (TotalMassZeros == 0))
 {
    MassAtomicStoichiometryMode = 1; // Detect a fully mass fraction filled column
    return true;
 }
 else if ((TotalMassNonZeros == this->GetCount()) && (TotalElements <= TotalAtomicOnes))
 {
   bool UniqueMassCompound = true; // Requires to make a second check.
   for(int p=0; p<this->GetCount(); p++)
   {
    UniqueMassCompound = false; // A new compound
    for(int q=0; q<this->Item(p).GetNumberElements(); q++)
    {
      double temp3 =  this->Item(p).GetMassQuantityAt(q);
      if ((temp3 != 0) && !UniqueMassCompound)
      {
        UniqueMassCompound = true; //OK
      }
      else if ((temp3 != 0) && UniqueMassCompound)
      {
        MassAtomicStoichiometryMode = 3; // Invalid input
        return false;
      }
      else
      {
        temp3 = 0;
      }
    }
   }
   MassAtomicStoichiometryMode = 2; // A valid hybrid input
    return true;
 }
 else
 {
    MassAtomicStoichiometryMode = 3; // Invalid input
    return false;
 }
}

bool CompoundExtra::ConvertMassAtomicToStoichiometry()
{
 if(this->GetCount() == 0)
  return false;
 // Atomic Fraction don't require conversions
 if(MassAtomicStoichiometryMode == 0)
 {
    return true;
 }
 else if(MassAtomicStoichiometryMode == 1) // Mass Fraction requires conversion to Atomic Fraction
 {
   double TotalWeightMass = 0; // First find the total weight
   for(int i=0; i<this->GetCount(); i++)
   {
    for(int j=0; j<this->Item(i).GetNumberElements(); j++)
    {
     TotalWeightMass = TotalWeightMass + this->Item(i).GetMassQuantityAt(j) / this->Item(i).GetAtomicMassAt(j);
    }
   }
    // And the atomic fraction of each element are now evaluated.
   for(int i=0; i<this->GetCount(); i++)
   {
    for(int j=0; j<this->Item(i).GetNumberElements(); j++)
    {
     double temp = this->Item(i).GetMassQuantityAt(j) / this->Item(i).GetAtomicMassAt(j);
     this->Item(i).SetStoichiometryAt(j,temp/TotalWeightMass);
    }
   }
  return true;
 }
 else if(MassAtomicStoichiometryMode == 2) // A mixture of compounds wityh fixed atomic ratios but with their mass known, along independent elements with their mass known
 {
   // For each independent element or compound find the absolute mass value, and convert the relative atomic ratios to mass ratios
   for(int i=0; i<this->GetCount(); i++)
   {
    double CompoundMass = 0; // Then find the total weight
    for(int j=0; j<this->Item(i).GetNumberElements(); j++)
    {
     CompoundMass = CompoundMass + this->Item(i).GetMassQuantityAt(j);
    }
    double CompoundStoichiometry = 0; // Convert the compound's atomic ratios back to mass fractions
    for(int j=0; j<this->Item(i).GetNumberElements(); j++)
    {
     CompoundStoichiometry = CompoundStoichiometry + this->Item(i).GetStoichiometryAt(j) * this->Item(i).GetAtomicMassAt(j);
    }
    for(int j=0; j<this->Item(i).GetNumberElements(); j++)
    {
     double temp = this->Item(i).GetStoichiometryAt(j) * this->Item(i).GetAtomicMassAt(j);
     this->Item(i).SetMassQuantityAt(j,temp*CompoundMass/CompoundStoichiometry);
    }
   }
  // With the temporary sample mass fraction evaluated, it is straighforward to convert back to atomic fraction
  double TotalWeightMass = 0; // Then find the total weight
   for(int i=0; i<this->GetCount(); i++)
   {
    for(int j=0; j<this->Item(i).GetNumberElements(); j++)
    {
     TotalWeightMass = TotalWeightMass + this->Item(i).GetMassQuantityAt(j) / this->Item(i).GetAtomicMassAt(j);
    }
   }
    // And the atomic fraction of each element are now evaluated.
   for(int i=0; i<this->GetCount(); i++)
   {
    for(int j=0; j<this->Item(i).GetNumberElements(); j++)
    {
     double temp = this->Item(i).GetMassQuantityAt(j) / this->Item(i).GetAtomicMassAt(j);
     this->Item(i).SetStoichiometryAt(j,temp/TotalWeightMass);
    }
   }
  return true;
 }
 else // Unsupported mode.
 {
   return false;
 }
}

IntegerVector CompoundExtra::GetElementsID()
{
 IntegerVector InitialID = IntegerVector(this->GetCount());
 int k = 0;
 for(int i=0; i<this->GetCount(); i++)
 {
  for(int j=0; j<this->Item(i).GetNumberElements(); j++)
  {
    double temp = this->Item(i).GetElementIDAt(j);
    InitialID.SetValue(k,temp);
    k = k + 1;
  }
 }
 return InitialID;
}

Vector CompoundExtra::GetAllStoichiometry()
{
 Vector InitialID = Vector(this->GetCount());
 int k = 0;
 for(int i=0; i<this->GetCount(); i++)
 {
  for(int j=0; j<this->Item(i).GetNumberElements(); j++)
  {
    double temp = this->Item(i).GetStoichiometryAt(j);
    InitialID.SetValue(k,temp);
    k = k + 1;
  }
 }
 return InitialID;
}

// Apply the new stoichiometric index, while maintain a fixed ratio between their elements members of the same compound.
bool FittingParameter::SetStoichiometry(double newvalue)
{
 // Find the ratio, i.e., x = newvalue/c[0]
 if (GroupElementsStoichiometry.GetNumberElements() > 0)
 {
  double FixedRatio = newvalue / GroupElementsStoichiometry.GetValue(0);
   for (int i=0; i<GroupElementsStoichiometry.GetNumberElements(); i++) // Apply the rule of three
   {
     double OldElementRatio = GroupElementsStoichiometry.GetValue(i);
     double NewElementRatio = OldElementRatio * FixedRatio;
     GroupElementsStoichiometry.SetValue(i,NewElementRatio);
   }
   return true;
 }
 else
 {
    return false; // Illegal value
 }
}

// Define the array of Fitting Parameters constructor
FittingParameterVector::FittingParameterVector(ElementExtra AllSelectedElements, CompoundExtra AllSelectedCompounds)
{
 // Single Fitting Elements will generate single fitting variables
 for(int i=0; i<AllSelectedElements.GetCount(); i++)
 {
   if(AllSelectedElements.Item(i).GetElementFit() && AllSelectedElements.Item(i).GetCompoundGroup() == 0)
   {
     IntegerVector ThisElementID(1);
     Vector ThisElementStoichiometry(1);
     ThisElementID.SetValue(0,AllSelectedElements.Item(i).GetElementID());
     ThisElementStoichiometry.SetValue(0,AllSelectedElements.Item(i).GetStoichiometry());
     FittingParameter temp(ThisElementID,ThisElementStoichiometry,0);
     this->Add(temp);
   }
 }
 // Compounds will store several fitting elements, but it will be stored as a fixed ratio.
 for(int j=0; j<AllSelectedCompounds.GetCount(); j++)
 {
   if(AllSelectedCompounds.Item(j).GetNumberFitElements()>0 && AllSelectedCompounds.Item(j).GetGroupNumber()>0)
   {
     IntegerVector ThisCompoundID(AllSelectedCompounds.Item(j).GetNumberFitElements());
     Vector ThisCompoundStoichiometry(AllSelectedCompounds.Item(j).GetNumberFitElements());
     int ThisCID = AllSelectedCompounds.Item(j).GetGroupNumber();
     int z = AllSelectedCompounds.Item(j).GetNumberElements();
     int Count = 0;
     for (int k=0; k<z; k++)
     {
       if(AllSelectedCompounds.Item(j).GetElementFitAt(k))
       {
        ThisCompoundID.SetValue(Count,AllSelectedCompounds.Item(j).GetElementIDAt(k));
        ThisCompoundStoichiometry.SetValue(Count,AllSelectedCompounds.Item(j).GetStoichiometryAt(k));
        Count = Count + 1;
       }
     }
     FittingParameter temp(ThisCompoundID,ThisCompoundStoichiometry,ThisCID);
     this->Add(temp);
   }
 }
}

// Count the number of fitting elements.
int FittingParameterVector::ElementCount()
{
 int x=0;
 for(int i=0; i<this->GetCount(); i++)
 {
  x = x + this->Item(i).GetNumberElements();
 }
 return x;
}

// Get the numerical vector which contains the relevant fitting parameters. Any compound will get the first element, since the others are not independent
Vector FittingParameterVector::GetCurrentFittingParameters()
{
 Vector InitialFit = Vector(this->GetNumberFittingParameters());
 for(int i=0; i<this->GetNumberFittingParameters(); i++)
 {
   InitialFit.SetValue(i,this->Item(i).GetStoichiometry().GetValue(0));
 }
 return InitialFit;
}

// Get the element ID of all fitting Elements
IntegerVector FittingParameterVector::GetElementParameterID()
{
 IntegerVector InitialID = IntegerVector(this->GetNumberElements());
 int k=0;
 for(int i=0; i<this->GetNumberFittingParameters(); i++)
 {
   for(int j=0; j<this->Item(i).GetGroupElementsID().GetNumberElements(); j++)
   {
    InitialID.SetValue(k,this->Item(i).GetGroupElementsID().GetValue(j));
    k = k + 1;
   }
 }
 return InitialID;
}

// Get the element ID of the relevant fitting Elements
IntegerVector FittingParameterVector::GetFittingParameterID()
{
 IntegerVector InitialID = IntegerVector(this->GetNumberFittingParameters());
 for(int i=0; i<this->GetNumberFittingParameters(); i++)
 {
   InitialID.SetValue(i,this->Item(i).GetGroupElementsID().GetValue(0));
 }
 return InitialID;
}

// Update the Stoichiometry of several independent Elements at once
bool FittingParameterVector::SetFittingStoichiometry(IntegerVector ElementsID, Vector Data)
{
 if(ElementsID.GetNumberElements() == Data.GetNumberElements())
 {
  for(int i=0; i<ElementsID.GetNumberElements(); i++)
  {
    this->SetStoichiometryAt(ElementsID.GetValue(i),Data.GetValue(i));
  }
  return true;
 }
 return false;
}

// Get the independent fitting parameters
Vector FittingParameterVector::GetFittingStoichiometry()
{
 Vector UpdateStoichiometry = Vector(this->GetNumberFittingParameters());
 for(int i=0; i<this->GetNumberFittingParameters(); i++)
 {
   UpdateStoichiometry.SetValue(i,this->Item(i).GetStoichiometry().GetValue(0));
 }
 return UpdateStoichiometry;
}

// Get the Stoichiometry of all elements that had a fit flag
Vector FittingParameterVector::GetAllFittingStoichiometry()
{
 Vector UpdateStoichiometry = Vector(this->GetNumberElements());
 int k=0;
 for(int i=0; i<this->GetNumberFittingParameters(); i++)
 {
   for(int j=0; j<this->Item(i).GetStoichiometry().GetNumberElements(); j++)
   {
    UpdateStoichiometry.SetValue(k,this->Item(i).GetStoichiometry().GetValue(j));
    k = k + 1;
   }
 }
 return UpdateStoichiometry;
}

// For a selected Element Parameter ID, return the list of all elements dependent of the selected Element
IntegerVector FittingParameterVector::GetElementIDAt(int ElementID)
{
 for(int i=0; i<this->GetNumberFittingParameters(); i++)
 {
   if(this->Item(i).GetGroupElementsID().GetValue(0) == ElementID)
    return this->Item(i).GetGroupElementsID();
 }
 return IntegerVector();
}

// Update a stoichiometric value for the selected Element ID
double FittingParameterVector::SetStoichiometryAt(int ElementID, double value)
{
 for(int i=0; i<this->GetNumberFittingParameters(); i++)
 {
   if(this->Item(i).GetGroupElementsID().GetValue(0) == ElementID)
   {
     if(this->Item(i).SetStoichiometry(value))
      return this->Item(i).GetStoichiometry().GetValue(0);
     else
      return 0;
   }
 }
 return 0;
}

// Return the stoichiometric value for the selected Element ID, and dependent elements
Vector FittingParameterVector::GetStoichiometryAt(int ElementID)
{
 for(int i=0; i<this->GetNumberFittingParameters(); i++)
 {
   if(this->Item(i).GetGroupElementsID().GetValue(0) == ElementID)
   {
     return this->Item(i).GetStoichiometry();
   }
 }
 return VectorNull(0);
}

// Yield Calculation Main Constructor
Yield::Yield(double Emin, double Emax, double DE, double ProfilingStep, double Charge, double Thickness, int ElementPosition, Detector ThoseDetector, ElementExtra ThoseElements, CompoundExtra ThoseCompounds)
{
 // Obtain the Element Adress, and the Compound Adress
 int temp0,temp1;
 ThoseCompounds.FindElement(ElementPosition,temp0,temp1);
 YieldElementPosition = ThoseElements.Item(ElementPosition).GetElementID();
 YieldCompoundPosition = temp0;
 // Evaluate the theorical yield without fitting
 CurrentYield = this->FunctionYield(Emin,Emax,DE,ProfilingStep,Charge,Thickness,ThoseDetector,ThoseElements,ThoseCompounds);
}

// Evaluate the Yield at a certain range of energies.
double Yield::FunctionYield(double Emin, double Emax, double DE, double ProfilingStep, double Charge, double Thickness, Detector ThoseDetector, ElementExtra ThoseElements, CompoundExtra ThoseCompounds)
{
 // Evaluate the initial Yield, which is a simple product.
 double DetectorEfficiency = ThoseDetector.GetEval(ThoseElements.Item(YieldElementPosition).GetGammaPeak());
 double ThisAtomicMass = ThoseElements.Item(YieldElementPosition).GetAtomicMass();
 double ThisAbundance = ThoseElements.Item(YieldElementPosition).GetAbundance();
 double ThisMassFraction = ThoseElements.GetMassFraction(YieldElementPosition);
 double AvogradoNumber = 6.02214076e+23;
 double ElementaryCharge = 1.602176634e-19;
 double ElectronVolt = 1.0e-3; // Convert eV to keV
 double NumberAtoms = 1.0e+15; // Due to Ziegler Units: ev/10^15 at/cm^2
 double MicroColoumb = 1.0e-6; // Convert Colomb to Micro-Colomb
 double MiliBarnUnit = 1.0e-27; // Convert milibarn to sq.cm
 double MicroGram = 1.0e-6; // Convert micro-grams to grams.
 double ZieglerFactor = NumberAtoms/(MicroGram*ElectronVolt); //Fix Ziegler Function Units
 double ThisNumberCharges = Charge; //Charge in micro-Coloumb
 double YieldFactor = AvogradoNumber * MicroColoumb * MiliBarnUnit * MicroGram / ElementaryCharge; // Set constant conversion
 double BraggFactor = AvogradoNumber / ZieglerFactor; // Set constant conversion
 double YieldNull = DetectorEfficiency * ThisNumberCharges * ThisMassFraction * ThisAbundance * YieldFactor / ThisAtomicMass;

  // Start the main yield evaluation cycle, where some values requires to inquire the entire array of Elements.
  double YieldSum = 0;
  int NumberLayers = std::ceil((Emax - Emin)/DE);
  double DepthSum = 0;
  if(Thickness > 0) // For thick samples, the integration begins at maximum energy, while also accounts the beam depth
  {
   for (int i=0; i<NumberLayers; i++)
   {
    // Evaluate the cross-section and stopping power of all elements, on initial, middle and final interval boundaries.
    double CurrentEnergy = Emax - i*DE;
    double CurrentSigma = ThoseElements.EvaluateCrossSection(YieldElementPosition,CurrentEnergy-DE,DE);
    double CurrentStoppingPower = ThoseElements.EvaluateBragg(CurrentEnergy-DE/2) * BraggFactor / ThoseElements.GetMolarMass();
    double CurrentDepth = DE / CurrentStoppingPower;
    // Evaluate the integral
    double CurrentFunction;
    if (CurrentStoppingPower == 0)
      CurrentFunction = 0;
    else
      CurrentFunction = CurrentSigma / CurrentStoppingPower;
    YieldSum = YieldSum + CurrentFunction;
    DepthSum = DepthSum + CurrentDepth;
    if(DepthSum > Thickness) // Reach the sample physical boundary
        break;
   }
  }
  else // ulk samples are plain integration
  {
   for (int i=0; i<NumberLayers; i++)
   {
    // Evaluate the cross-section and stopping power of all elements, on initial, middle and final interval boundaries.
    double CurrentEnergy = Emin + i*DE;
    double CurrentSigma = ThoseElements.EvaluateCrossSection(YieldElementPosition,CurrentEnergy,DE);
    double CurrentStoppingPower = ThoseElements.EvaluateBragg(CurrentEnergy+DE/2) * BraggFactor / ThoseElements.GetMolarMass();
    // Evaluate the integral
    double CurrentFunction;
    if (CurrentStoppingPower == 0)
      CurrentFunction = 0;
    else
      CurrentFunction = CurrentSigma / CurrentStoppingPower;
    YieldSum = YieldSum + CurrentFunction;
   }
  }
  // The final result is the product of the integral and the conversion factor
   double YieldTotal = YieldNull * YieldSum;
   return YieldTotal;
}

// Levenberg-Marquardt optimized algorithm for stoichiometric fitting.
// Using the algorithm published by K. Madsen, H.B.Nilsen, O. Tingleff, from "Methods for Non-Linear Least Square Problems".
// Our function are defined by a vector of functions defined as f(x) = Yield(E)_exp - Yield(E)_theo, where the parameters are the
// independent stoichiometric parameters which should be renormalized each turn, since the yield are invariant due such renormalization.
bool YieldFitting::LevenbergMarquardtYield(double Emin, double Emax, double DE, double ProfilingStep, double Charge, double Thickness, Detector ThoseDetector, ElementExtra& ThoseElements, CompoundExtra& ThoseCompounds)
{
  // Initial setup
  double e1,e2,mu,nu,tau,rho;
  tau = LMTau;
  nu = 10;
  e1 = LMStoichiometry;
  e2 = LMYield;
  LMNumberInteractions = 0;
  int kmax = LMMaxInteractions;

  // Create an additional modal progress bar:
  wxProgressDialog *progress = new wxProgressDialog(wxT("Numerical Simulation Progress"), wxT("Starting, please wait..."),LMMaxInteractions,NULL,wxPD_CAN_ABORT|wxPD_APP_MODAL|wxPD_AUTO_HIDE);

  // Initial Stoichiometry and Initial Yield.
  // Notice the stoichiometric parameters (the fitting variables) can be shorter than the number of fitting yields.
  // And also it is necessary to store the element positions, to update the correct values!
  FittingParameterVector ThoseFits(ThoseElements,ThoseCompounds);
  Vector FittingExperimentalYield = Vector(ThoseFits.GetNumberElements());
  Vector FittingTheoricalYield = Vector(ThoseFits.GetNumberElements());
  Vector StoichiometricParameters = ThoseFits.GetFittingStoichiometry();
  IntegerVector FitElementAdress = ThoseFits.GetFittingParameterID();
  IntegerVector AllElementAdress = ThoseFits.GetElementParameterID();
  for (int i=0; i<FittingExperimentalYield.GetNumberElements(); i++)
  {
   double tempYield = this->Item(AllElementAdress.GetValue(i)).FunctionYield(Emin, Emax, DE, ProfilingStep,Charge, Thickness, ThoseDetector, ThoseElements, ThoseCompounds);
   double tempExp = ExperimentalYield.GetValue(AllElementAdress.GetValue(i));
   FittingTheoricalYield.SetValue(i,tempYield);
   FittingExperimentalYield.SetValue(i,tempExp);
  }

  // Initial vectors and matrices
  Vector f = VectorMinus(FittingExperimentalYield,FittingTheoricalYield);
  Matrix Jacobian = this->GradientFunctionYield(Emin,Emax,DE,ProfilingStep,Charge,Thickness,ThoseDetector,ThoseElements,ThoseCompounds);
  Matrix A = MatrixProduct(MatrixTranspose(Jacobian),Jacobian);
  Matrix g = MatrixProduct(MatrixTranspose(Jacobian),GetMatrix(f));
  Vector tau0 = A.GetNumberRows();
  for (int z=0; z<tau0.GetNumberElements(); z++)
  {
   tau0.SetValue(z,A.GetValue(z,z));
  }
  mu = tau * tau0.Maximum();

  // User abort fitting
  if(progress->WasCancelled())
    {
     progress->Destroy();
     ErrorMensage = wxT("Stop! The fitting was aborted by the user!");
     return false;
    }

  // Since if on zeroth step got a solution, then just evaluate some additional evaluations, and then closes the procedure successfully.
  if (GetVector(g).InfNorm() < e1)
  {
   FittingParameterVector FinalSolution = FittingParameterVector(ThoseElements,ThoseCompounds);
   FinalSolution.SetFittingStoichiometry(FinalSolution.GetFittingParameterID(),FinalSolution.GetFittingStoichiometry());
   ThoseElements.SetStoichiometryFromTo(FinalSolution.GetElementParameterID(),FinalSolution.GetAllFittingStoichiometry());
   ThoseElements.RenormStoichiometry();
   ThoseCompounds.SetStoichiometryFromTo(FinalSolution.GetElementParameterID(),FinalSolution.GetAllFittingStoichiometry());
   ThoseCompounds.RenormStoichiometry();
   FitError = GetVector(g).InfNorm();
   progress->Destroy();
   return true;
  }

 // Levenberg-Marquardt algorithm main cycle

 for(int k=0; k<kmax; k++)
 {
  LMNumberInteractions = LMNumberInteractions + 1;
  // Update the progress bar...
   progress->Update(k,wxT("Fitting Step #") + wxString::Format("%i",k) + wxT(" of ") + wxString::Format("%i",kmax));
  // Matrix solving: (A + mu  I) h = -g
  Matrix ADamped = MatrixAddition(A,MatrixScalarProduct(mu,MatrixIdentity(A.GetNumberRows())));
  if(ADamped.GetOverflow() || !(ADamped.GetStatus()))
  {
   ErrorMensage = wxT("Jacobian Matrix Dimension Error at step ") + wxString::Format("%d",LMNumberInteractions);
   progress->Destroy();
   return false;
  }
  Matrix h = MatrixSolve(ADamped,MatrixScalarProduct(-1.0,g));
  if(!(h.GetStatus())  || h.GetOverflow())
  {
   ErrorMensage = wxT("Jacobian Matrix Dimension Error at step ") + wxString::Format("%d",LMNumberInteractions);
   progress->Destroy();
   return false;
  }
  // Check the validity of the determined approximation
  if (GetVector(h).EuclideNorm() < e2 * (StoichiometricParameters.EuclideNorm() + e2))
  {
    // Get an actual solution
    Vector CheckStoichiometry = VectorSum(StoichiometricParameters,GetVector(h));
    FittingParameterVector FinalSolution = FittingParameterVector(ThoseElements,ThoseCompounds);
    FinalSolution.SetFittingStoichiometry(FinalSolution.GetFittingParameterID(),CheckStoichiometry);
    ThoseElements.SetStoichiometryFromTo(FinalSolution.GetElementParameterID(),FinalSolution.GetAllFittingStoichiometry());
    ThoseElements.RenormStoichiometry();
    ThoseCompounds.SetStoichiometryFromTo(FinalSolution.GetElementParameterID(),FinalSolution.GetAllFittingStoichiometry());
    ThoseCompounds.RenormStoichiometry();
    FitError = GetVector(h).EuclideNorm();
    progress->Destroy();
    return true;
  }
  else //Find another solution
  {
    // Evaluate the gain ratio
    Vector NewStoichiometry = VectorSum(StoichiometricParameters,GetVector(h));
    // Get the old and updated vales for the yields
    Vector Oldf = Vector(ThoseFits.GetNumberElements());
    Vector Newf = Vector(ThoseFits.GetNumberElements());
    // First get the previous values, and renormalize
    FittingParameterVector OldStoichiometry = FittingParameterVector(ThoseElements,ThoseCompounds);
    OldStoichiometry.SetFittingStoichiometry(OldStoichiometry.GetFittingParameterID(),StoichiometricParameters);
    ThoseElements.SetStoichiometryFromTo(OldStoichiometry.GetElementParameterID(),OldStoichiometry.GetAllFittingStoichiometry());
    ThoseElements.RenormStoichiometry();
    ThoseCompounds.SetStoichiometryFromTo(OldStoichiometry.GetElementParameterID(),OldStoichiometry.GetAllFittingStoichiometry());
    ThoseCompounds.RenormStoichiometry();
    for (int i=0; i<ThoseFits.GetNumberElements(); i++)
    {
     double tempYield = this->Item(ThoseFits.GetElementParameterID().GetValue(i)).FunctionYield(Emin, Emax, DE, ProfilingStep, Charge, Thickness, ThoseDetector, ThoseElements, ThoseCompounds);
     double tempExp = ExperimentalYield.GetValue(ThoseFits.GetElementParameterID().GetValue(i));
     Oldf.SetValue(i,tempExp - tempYield);
    }
    // Then get the new values, and renormalize
    FittingParameterVector TestStoichiometry = FittingParameterVector(ThoseElements,ThoseCompounds);
    TestStoichiometry.SetFittingStoichiometry(TestStoichiometry.GetFittingParameterID(),NewStoichiometry);
    ThoseElements.SetStoichiometryFromTo(TestStoichiometry.GetElementParameterID(),TestStoichiometry.GetAllFittingStoichiometry());
    ThoseElements.RenormStoichiometry();
    ThoseCompounds.SetStoichiometryFromTo(TestStoichiometry.GetElementParameterID(),TestStoichiometry.GetAllFittingStoichiometry());
    ThoseCompounds.RenormStoichiometry();
    for (int i=0; i<ThoseFits.GetNumberElements(); i++)
    {
     double tempYield = this->Item(ThoseFits.GetElementParameterID().GetValue(i)).FunctionYield(Emin, Emax, DE, ProfilingStep,Charge, Thickness, ThoseDetector, ThoseElements, ThoseCompounds);
     double tempExp = ExperimentalYield.GetValue(ThoseFits.GetElementParameterID().GetValue(i));
     Newf.SetValue(i,tempExp - tempYield);
    }
    // Evaluate the internal products
    double OldYieldNorm = Oldf.EuclideNorm() * Oldf.EuclideNorm() / 2;
    double NewYieldNorm = Newf.EuclideNorm() * Newf.EuclideNorm() / 2;
    // Evaluate the gain ratio
    Matrix GainDenominator = MatrixSubtraction(MatrixScalarProduct(mu,h),g);
    Matrix CrossGainDenominator = MatrixProduct(MatrixTranspose(h),GainDenominator);
    double rho = 2*(OldYieldNorm-NewYieldNorm)/CrossGainDenominator.GetValue(0,0);
    // And the renormalized new stoichiometry
    Vector NormNewStoichiomety = TestStoichiometry.GetFittingStoichiometry();

    // User abort fitting
    if(progress->WasCancelled())
    {
     progress->Destroy();
     ErrorMensage = wxT("Stop! The fitting was aborted by the user!");
     return false;
    }

    if(rho>0) // Accepted value
    {
     StoichiometricParameters = NormNewStoichiomety; //Update the testing stoichiometry to definitive
     f = Newf; //Update the residue function
     Jacobian = this->GradientFunctionYield(Emin,Emax,DE,ProfilingStep,Charge,Thickness,ThoseDetector,ThoseElements,ThoseCompounds); //Update the Jacobian
     A = MatrixProduct(MatrixTranspose(Jacobian),Jacobian); //Update the Hessian matrix
     g = MatrixProduct(MatrixTranspose(Jacobian),GetMatrix(f)); //Update the values
     // Update prameters
     Vector test(2);
     test.SetValue(0,1/3);
     test.SetValue(1,1 - std::pow( 2*rho-1 ,3));
     tau = tau * test.Maximum();
     nu = 10;
     // Check the validity of the determined approximation
     if (GetVector(g).InfNorm() < e1)
     {
      // Get an actual solution
      FittingParameterVector FinalSolution = FittingParameterVector(ThoseElements,ThoseCompounds);
      FinalSolution.SetFittingStoichiometry(FinalSolution.GetFittingParameterID(),StoichiometricParameters);
      ThoseElements.SetStoichiometryFromTo(FinalSolution.GetElementParameterID(),FinalSolution.GetAllFittingStoichiometry());
      ThoseElements.RenormStoichiometry();
      ThoseCompounds.SetStoichiometryFromTo(FinalSolution.GetElementParameterID(),FinalSolution.GetAllFittingStoichiometry());
      ThoseCompounds.RenormStoichiometry();
      FitError = GetVector(g).InfNorm();
      progress->Destroy();
      return true;
     }
    }
    else //Convergence failure, try another parameters
    {
      mu = mu * nu;
      nu = 10 * nu;
    }
    // User abort fitting
    if(progress->WasCancelled())
    {
     progress->Destroy();
     ErrorMensage = wxT("Stop! The fitting was aborted by the user!");
     return false;
    }
  }
 } // End Levenberg-Marquardt main cycle


 // Failure
 progress->Destroy();
 ErrorMensage = wxT("Cannot find a suitable solution! Please check the initial conditions, and try again.");
 return false;
}


// Get the gradient of the Yield vector function, in function of their stoichiometry parameters. This means, a matrix where the rows are the several Yields, and columns equal to the number of fitting parameters
Matrix YieldFitting::GradientFunctionYield(double Emin, double Emax, double DE, double ProfilingStep, double Charge, double Thickness, Detector ThoseDetector, ElementExtra& ThoseElements, CompoundExtra& ThoseCompounds)
{
 // The Jacobian had `i` times the number of Yields/Elements and `j` times the number of fitting independent parameters
 // Thus J(i,j) = d(Y(i))/d(x(j))
 FittingParameterVector LocalParameters = FittingParameterVector(ThoseElements,ThoseCompounds);
 Matrix Gradient = Matrix(LocalParameters.GetNumberElements(),LocalParameters.GetNumberFittingParameters());
 // Obtain the Elements adress
 IntegerVector tempPositions = LocalParameters.GetFittingParameterID(); // Column real adress
 IntegerVector tempElementAdress = LocalParameters.GetElementParameterID(); // Row real adress
 for (int i = 0; i < Gradient.GetNumberRows(); i++)
  {
   for (int j = 0; j < Gradient.GetNumberColumns(); j++)
   {
    // Set the current Yield function, the current fitting parameter, and all element adress associated to the fitting parameter.
    int CurrentParameter = tempPositions.GetValue(j);
    int CurrentYield = tempElementAdress.GetValue(i);
    IntegerVector CurrentElements = LocalParameters.GetElementIDAt(CurrentParameter);
    // Get the diffrentials around the relevant variable
    double DiffLeft = ThoseElements.Item(CurrentParameter).GetStoichiometry() - 1e-9;
    double DiffRight = ThoseElements.Item(CurrentParameter).GetStoichiometry() + 1e-9;
    double Original = ThoseElements.Item(CurrentParameter).GetStoichiometry();
    // Update the stoichiometry of Elements and associated Compounds
    LocalParameters.SetStoichiometryAt(CurrentParameter,DiffLeft);
    ThoseElements.SetStoichiometryFromTo(CurrentElements,LocalParameters.GetStoichiometryAt(CurrentParameter));
    ThoseCompounds.SetStoichiometryFromTo(CurrentElements,LocalParameters.GetStoichiometryAt(CurrentParameter));
    double YieldLeftValue = this->Item(CurrentYield).FunctionYield(Emin,Emax,DE,ProfilingStep,Charge,Thickness,ThoseDetector,ThoseElements,ThoseCompounds);
    LocalParameters.SetStoichiometryAt(CurrentParameter,DiffRight);
    ThoseElements.SetStoichiometryFromTo(CurrentElements,LocalParameters.GetStoichiometryAt(CurrentParameter));
    ThoseCompounds.SetStoichiometryFromTo(CurrentElements,LocalParameters.GetStoichiometryAt(CurrentParameter));
    double YieldRightValue = this->Item(CurrentYield).FunctionYield(Emin,Emax,DE,ProfilingStep,Charge,Thickness,ThoseDetector,ThoseElements,ThoseCompounds);
    LocalParameters.SetStoichiometryAt(CurrentParameter,Original);
    ThoseElements.SetStoichiometryFromTo(CurrentElements,LocalParameters.GetStoichiometryAt(CurrentParameter));
    ThoseCompounds.SetStoichiometryFromTo(CurrentElements,LocalParameters.GetStoichiometryAt(CurrentParameter));
    // Evaluate the numerical derivative, which the result should be place in reserve ordem, due to the fact that (Yexp - Yint)' = -Yint
    double Result = (YieldLeftValue - YieldRightValue)/2e-9;
    Gradient.SetValue(i,j,Result);
   }
  }
  // return the jacobian matrix
  Gradient.SetStatus(true);
  return Gradient;
}

// YieldFitting main constructor
YieldFitting::YieldFitting(double Emin, double Emax, double DE, double ProfilingStep, double Charge, double Thickness, Vector InitialYield, Detector ThoseDetector, ElementExtra ThoseElements, CompoundExtra ThoseCompounds, FittingParameterVector ThoseFits, int mi, int lt, int ly, int ls)
{
  // Set the user-defined precision parameters
  LMMaxInteractions = mi;
  LMTau = std::pow(10,-1.0*lt);
  LMYield = std::pow(10,-1.0*ly);
  LMStoichiometry = std::pow(10,-1.0*ls);
  // Evaluate the initial yields
  OriginalYield = Vector(InitialYield.GetNumberElements());
  for (int i = 0; i < InitialYield.GetNumberElements(); i++)
  {
     Yield SomeYield = Yield(Emin, Emax, DE, ProfilingStep, Charge, Thickness, i, ThoseDetector, ThoseElements, ThoseCompounds);
     OriginalYield.SetValue(i,SomeYield.GetYield());
     this->Add(SomeYield);
  }
 // Define the relevant numerical vectors
  ExperimentalYield = InitialYield;
  FittedYield = OriginalYield;
  FittedStoichiometric = VectorScalarProduct(1/ThoseElements.GetTotalStoichiometry(),ThoseElements.GetAllStoichiometry());
  MassStoichiometric = VectorScalarProduct(1/ThoseElements.GetTotalMass(),ThoseElements.GetAllMass());
  ErrorStoichiometric = VectorNull(ThoseElements.GetCount());
  LMNumberInteractions = 0;

  // If the user select at least one fitting parameter, start the Levenberg-Marquardt algorithm
 if (ThoseFits.GetNumberFittingParameters() > 0)
 {
  // Apply the Levenberg-Marquardt fitting algorithm
  if(!(this->LevenbergMarquardtYield(Emin, Emax, DE, ProfilingStep, Charge, Thickness, ThoseDetector, ThoseElements, ThoseCompounds)))
   {
    if(ErrorMensage.Len() == 0)
     ErrorMensage = wxT("Please check the Experimental Yield and other parameters, and try again.");
     wxMessageDialog *dial = new wxMessageDialog(NULL, ErrorMensage , wxT("Yield not Fittable!"), wxOK | wxICON_ERROR);
     dial->ShowModal();
   }
   else
   {
     // Evaluate the Fitted Yields and Atomic Mass from the Fitted Stoichiometry
     for(int i=0; i<ThoseElements.GetCount(); i++)
     {
      FittedYield.SetValue(i,this->Item(i).FunctionYield(Emin,Emax,DE,ProfilingStep,Charge,Thickness,ThoseDetector,ThoseElements,ThoseCompounds));
      FittedStoichiometric.SetValue(i,ThoseElements.Item(i).GetStoichiometry());
      MassStoichiometric.SetValue(i,ThoseElements.Item(i).GetStoichiometry()*ThoseElements.Item(i).GetIsotopicMass()/ThoseElements.GetTotalMass());
      ErrorStoichiometric.SetValue(i,ThoseElements.Item(i).GetStoichiometry()*FitError);
     }
   }
 }
}

// Return partial Yields, if necessary
double YieldFitting::GetYieldAt(int Adress, double Emin, double Emax, double DE, double DetectorAngle, double Charge, double Thickness, Detector ThoseDetector, ElementExtra ThoseElements, CompoundExtra ThoseCompounds)
{
 return this->Item(Adress).FunctionYield(Emin,Emax,DE,DetectorAngle,Charge,Thickness,ThoseDetector,ThoseElements,ThoseCompounds);
}

// Reaction Yield main constructor
ReactionYield::ReactionYield(ElementDatabaseArray MainDatabase, DetectorParameters DetectorSetup, ZieglerParameters ZieglerSetup, ElementSRIMArray SRIMSetup, wxString GetMinimumEnergy, wxString GetMaximumEnergy, wxString GetEnergyStep, wxString GetProfilingStep, wxString GetCharge, wxString GetThickness, wxArrayString GetCalibrationArray, wxArrayString GetGroupArray, wxArrayString GetElementArray, wxArrayString GetFitArray, wxArrayString GetMG, wxArrayString GetSG, wxArrayString& SetYS, wxArrayString GetYE, wxArrayString& SetYF, wxArrayString& SetSF, wxArrayString& SetSM, wxArrayString& SetSE, int mi, int lt, int ly, int ls)
{
  if(!(this->EvaluateYield(MainDatabase, DetectorSetup, ZieglerSetup, SRIMSetup, GetMinimumEnergy, GetMaximumEnergy, GetEnergyStep, GetProfilingStep, GetCharge, GetThickness, GetCalibrationArray, GetGroupArray, GetElementArray, GetFitArray, GetMG, GetSG, SetYS, GetYE, SetYF, SetSF, SetSM, SetSE, mi, lt, ly, ls)))
  {
   SetYS.Clear();
   SetYF.Clear();
   SetSE.Clear();
   SetSF.Clear();
   SetSM.Clear();
   ErrorStatus = false;
  }
  else
  {
   ErrorStatus = true;
  }
}

// The main-entry routine for Input/Output data. Notice this function had 20 parameters!
bool ReactionYield::EvaluateYield(ElementDatabaseArray MainDatabase, DetectorParameters DetectorSetup, ZieglerParameters ZieglerSetup, ElementSRIMArray SRIMSetup, wxString GetMinimumEnergy, wxString GetMaximumEnergy, wxString GetEnergyStep, wxString GetProfilingStep, wxString GetCharge, wxString GetThickness, wxArrayString GetCalibrationArray, wxArrayString GetGroupArray, wxArrayString GetElementArray, wxArrayString GetFitArray, wxArrayString GetMG, wxArrayString GetSG, wxArrayString& SetYS, wxArrayString GetYE, wxArrayString& SetYF, wxArrayString& SetSF, wxArrayString& SetSM, wxArrayString& SetSE, int mi, int lt, int ly, int ls)
{
 // Initial setup, and calculation of physical initial parameters
 double ElectricCharge,MinimumEnergy,MaximumEnergy,EnergyStep,Thickness,ProfilingStep;
 CompoundArray CurrentCompounds;
 ElementExtra CurrentElements;
 SetYS.Clear();
 SetYF.Clear();
 SetSE.Clear();
 SetSF.Clear();
 SetSM.Clear();
 if(GetCharge.Len()>0 && GetMinimumEnergy.Len()>0 && GetMaximumEnergy.Len()>0 && GetEnergyStep.Len()>0 && GetThickness.Len()>0 && GetProfilingStep.Len()>0)
 {
 if (GetCharge.ToDouble(&ElectricCharge) && GetMinimumEnergy.ToDouble(&MinimumEnergy) && GetMaximumEnergy.ToDouble(&MaximumEnergy) && GetEnergyStep.ToDouble(&EnergyStep) && GetThickness.ToDouble(&Thickness) && GetProfilingStep.ToDouble(&ProfilingStep))
 {
  if (ElectricCharge > 0 && MinimumEnergy >= 0 && MaximumEnergy > 0 && EnergyStep > 0 && Thickness >= 0 && MinimumEnergy < MaximumEnergy && ProfilingStep >= 0)
  {
    if (GetYE.GetCount() == GetSG.GetCount() && GetElementArray.GetCount() == GetSG.GetCount() && GetMG.GetCount() == GetSG.GetCount())
    {
     // 1. Define Detector Function
     Detector SomeDetector = Detector(DetectorSetup);
    if (!(SomeDetector.CheckDetector()))
    {
     wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Please check if the Detector Setup data are correctly assigned, and try again.") , wxT("Invalid Detector Definition!"), wxOK | wxICON_ERROR);
     dial->ShowModal();
     return false;
    }
    // 2. Load the elements, one by one, and pack them to one single array
    for (int i = 0; i < GetElementArray.GetCount(); i++)
    {
     long element;
     GetElementArray.Item(i).ToLong(&element);
     Element SomeElement = Element(MainDatabase.Item(element), ZieglerSetup, SRIMSetup, GetMG.Item(i), GetSG.Item(i), GetGroupArray.Item(i), GetFitArray.Item(i), GetCalibrationArray.Item(i), i);
     if(SomeElement.IsCorrect())
     {
      CurrentElements.Add(SomeElement);
     }
     else
     {
      wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Please check if the Elements and Compounds are correctly assigned, and try again.") , wxT("Invalid Compound Definition!"), wxOK | wxICON_ERROR);
      dial->ShowModal();
      return false;
     }
    }
    // 3. Read the elements declared from the spreadsheet and build the nominal array of Compounds. Any error will close the procedure.
    CompoundExtra AdditionalCompounds = CompoundExtra(CurrentElements);
    if (!(AdditionalCompounds.CheckCompoundArray()))
    {
     wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Please check if the Elements and Compounds are correctly assigned, and try again.") , wxT("Invalid Compound Definition!"), wxOK | wxICON_ERROR);
     dial->ShowModal();
     return false;
    }
    // 4. Check the consistency of the dual mass and atomic fraction inputs, taking note from the compounds rules.
    // It expects only three cases: Only the mass fraction are filled, or only the atomic fraction are filled, and finally the atomic fraction are fully filled combined with some terms of the
    // mass fraction, where each compound or isolated element are declared with a mass fraction. otherwise it will return a consitency error.
    // It will then return the atomic fraction from all elements.
    if (!(AdditionalCompounds.CheckMassAtomicStoichiometry()))
    {
     wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Incoherent mass or atomic fraction definition of the entire sample.\nVerify the contents of the Compounds, Mass and Atomic Composition columns, and try again\nRefer to the User Manual about the validity of inputs, for more information.") , wxT("Invalid Sample Definition!"), wxOK | wxICON_ERROR);
     dial->ShowModal();
     return false;
    }
   if (!(AdditionalCompounds.ConvertMassAtomicToStoichiometry()))
    {
     wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Incorrect definition of the sample inputs.\nVerify the contents of the Compounds, Mass and Atomic Composition columns, and try again\nRefer to the User Manual about the validity of inputs, for more information.") , wxT("Invalid Sample Definition!"), wxOK | wxICON_ERROR);
     dial->ShowModal();
     return false;
    }
     // 5. Define the Fitting Parameters, after apply the renormalization
   IntegerVector ConversionID;
   Vector ConversionData;
   CurrentElements.SetStoichiometryFromTo(ConversionID,ConversionData);
   CurrentElements.RenormStoichiometryTotal();
   AdditionalCompounds.RenormStoichiometryTotal();
   FittingParameterVector AllRelevantFitting = FittingParameterVector(CurrentElements, AdditionalCompounds);
  // 6. Yield Calculation Main Cycle
  Vector ExperimentalYield = Vector(GetYE.GetCount());
  for (int i = 0; i < GetYE.GetCount(); i++)
  {
   double ThisExperimentalYield;
   ExperimentalYield.SetValue(i,GetYE.Item(i).ToDouble(&ThisExperimentalYield));
   ExperimentalYield.SetValue(i,ThisExperimentalYield);
  }
  YieldFitting CurrentYields = YieldFitting(MinimumEnergy,MaximumEnergy,EnergyStep,1.0,ElectricCharge,Thickness,ExperimentalYield,SomeDetector,CurrentElements,AdditionalCompounds,AllRelevantFitting,mi,lt,ly,ls);
  // 7. Yield Output Main Cycle
  for (int i = 0; i < CurrentYields.GetCount(); i++)
  {
   double ThisYield = CurrentYields.Item(i).GetYield();
   wxString value0 = wxString::Format("%f", ThisYield);
   SetYS.Add(value0);
   double ThisFit = CurrentYields.GetFit(i);
   wxString value1 = wxString::Format("%f", ThisFit);
   SetYF.Add(value1);
   double ThisStoichiometry = CurrentYields.GetStoichiometry(i);
   wxString value2 = wxString::Format("%f", ThisStoichiometry);
   SetSF.Add(value2);
   double ThisMass = CurrentYields.GetMass(i);
   wxString value3 = wxString::Format("%f", ThisMass);
   SetSM.Add(value3);
   double ThisError = CurrentYields.GetError(i);
   wxString value4 = wxString::Format("%.9e", ThisError);
   SetSE.Add(value4);
  }
  // 8. And finish the whole evaluation routine
   LMN = CurrentYields.GetFittingIteractions();
   OptionalDetectorFunction = SomeDetector.GetFittingFunction();
   if(ProfilingStep > 0) //Create a result table for several energies between Emin and Emax
   {
    // Allocate the corrected matrix dimensions
    int NumberEnergySteps = std::ceil((MaximumEnergy-MinimumEnergy)/ProfilingStep)+1;
    if (MinimumEnergy + NumberEnergySteps*ProfilingStep < MaximumEnergy)
      NumberEnergySteps = NumberEnergySteps + 1;
    ElementsProfiling = Matrix(NumberEnergySteps,CurrentYields.GetCount()+1);
    long TotalEnergySteps = ElementsProfiling.GetNumberColumns() * ElementsProfiling.GetNumberRows() + ElementsProfiling.GetNumberColumns();
    long StepNumber = 0;
    // Create an additional modal progress bar:
    wxProgressDialog *progress = new wxProgressDialog(wxT("Numerical Profiling Progress"), wxT("Starting, please wait..."),TotalEnergySteps,NULL,wxPD_CAN_ABORT|wxPD_APP_MODAL|wxPD_AUTO_HIDE);
    // Write matrix values
    for(int p = 0; p < ElementsProfiling.GetNumberRows(); p++)
     {
       for (int q = 0; q < ElementsProfiling.GetNumberColumns(); q++)
       {
         progress->Update(StepNumber,wxT("Calculation Progress...") + wxString::Format("%.3f",(100.0*StepNumber)/(TotalEnergySteps)) + wxT("%"));
         StepNumber = StepNumber + 1;
         if(progress->WasCancelled())
         {
          progress->Destroy();
          wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("If necessary, change the Profiling Energy Step, and try again.\nYou can set to \"0\" to skip this step.") , wxT("Profiling Aborted by the User..."), wxOK | wxICON_ERROR);
          dial->ShowModal();
          return false;
         }
         if(q == 0)
          ElementsProfiling.SetValue(p,q,MinimumEnergy+p*ProfilingStep);
         else
          ElementsProfiling.SetValue(p,q,CurrentYields.GetYieldAt(q-1,MinimumEnergy,MinimumEnergy+p*ProfilingStep,EnergyStep,1.0,ElectricCharge,Thickness,SomeDetector,CurrentElements,AdditionalCompounds));
       }
     }
    for (int z=0; z < ElementsProfiling.GetNumberColumns(); z++)
    {
      progress->Update(StepNumber,wxT("Calculation Progress...") + wxString::Format("%.3f",(100.0*StepNumber)/(TotalEnergySteps)) + wxT("%"));
      StepNumber = StepNumber + 1;
      if(progress->WasCancelled())
         {
          progress->Destroy();
          wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("If necessary, change the Profiling Energy Step, and try again.\nYou can set to \"0\" to skip this step.") , wxT("Profiling Aborted by the User..."), wxOK | wxICON_ERROR);
          dial->ShowModal();
          return false;
         }
      if(z == 0)
          ElementsProfiling.SetValue(ElementsProfiling.GetNumberRows()-1,z,MaximumEnergy);
         else
          ElementsProfiling.SetValue(ElementsProfiling.GetNumberRows()-1,z,CurrentYields.GetYieldAt(z-1,MinimumEnergy,MaximumEnergy,EnergyStep,1.0,ElectricCharge,Thickness,SomeDetector,CurrentElements,AdditionalCompounds));
    }
    progress->Destroy();
   }
   return true;
  }
  else // For some reason, the number of Elements and relevant parameters are not equal
   {
    wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Please check the values on main spreadsheet, and try again.") , wxT("Invalid Table Data!"), wxOK | wxICON_ERROR);
    dial->ShowModal();
     return false;
   }
  }
  else // Out-of-Range error
  {
    wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("All detector parameters and the detector energy range should be positive.") , wxT("Invalid Parameters!"), wxOK | wxICON_ERROR);
    dial->ShowModal();
   return false;
  }
 }
 else  // Invalid User Input
 {
    wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("All initial values should be numeric values, please fix it and try again.") , wxT("Invalid Parameters!"), wxOK | wxICON_ERROR);
    dial->ShowModal();
   return false;
 }
 }
 else // Incomplete data
 {
    wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Please fill the initial values parameters on bottom page, and try again.") , wxT("Incomplete Data!"), wxOK | wxICON_ERROR);
    dial->ShowModal();
   return false;
 }
}
