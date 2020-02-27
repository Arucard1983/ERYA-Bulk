#include "ERYAPIXEdialogERYACalculator.h"
#include "ParserLibrary.h"

ERYAPIXEdialogERYACalculator::ERYAPIXEdialogERYACalculator( wxWindow* parent )
:
dialogERYACalculator( parent )
{
 // Initialize the internal variables
 LastVariables.Clear();
 LastVector.clear();
 LastAns = 0;
 DecimalPrecision = 10;
}

void ERYAPIXEdialogERYACalculator::OnAcos( wxCommandEvent& event )
{
 wxString data = wxT("acos(");
 textCalculatorInput->WriteText(data);
}

void ERYAPIXEdialogERYACalculator::OnAsin( wxCommandEvent& event )
{
 wxString data = wxT("asin(");
 textCalculatorInput->WriteText(data);
}

void ERYAPIXEdialogERYACalculator::OnAtan( wxCommandEvent& event )
{
  wxString data = wxT("atan(");
  textCalculatorInput->WriteText(data);
}

void ERYAPIXEdialogERYACalculator::OnFxvar( wxCommandEvent& event )
{
  wxString data = wxT("fxvar =");
  textCalculatorInput->WriteText(data);
}

void ERYAPIXEdialogERYACalculator::OnLeftParenthesis( wxCommandEvent& event )
{
 wxString data = wxT("(");
 textCalculatorInput->WriteText(data);
}

void ERYAPIXEdialogERYACalculator::OnRightParenthesis( wxCommandEvent& event )
{
  wxString data = wxT(")");
  textCalculatorInput->WriteText(data);
}

void ERYAPIXEdialogERYACalculator::OnSqrt( wxCommandEvent& event )
{
  wxString data = wxT("sqrt(");
 textCalculatorInput->WriteText(data);
}

void ERYAPIXEdialogERYACalculator::OnPower( wxCommandEvent& event )
{
  wxString data = wxT("^");
 textCalculatorInput->WriteText(data);
}

void ERYAPIXEdialogERYACalculator::OnClearInput( wxCommandEvent& event )
{
 textCalculatorInput->Clear();
}

void ERYAPIXEdialogERYACalculator::OnAcosh( wxCommandEvent& event )
{
  wxString data = wxT("acosh(");
 textCalculatorInput->WriteText(data);
}

void ERYAPIXEdialogERYACalculator::OnAsinh( wxCommandEvent& event )
{
  wxString data = wxT("asinh(");
 textCalculatorInput->WriteText(data);
}

void ERYAPIXEdialogERYACalculator::OnAtanh( wxCommandEvent& event )
{
  wxString data = wxT("atanh(");
 textCalculatorInput->WriteText(data);
}

void ERYAPIXEdialogERYACalculator::OnFyvar( wxCommandEvent& event )
{
  wxString data = wxT("fyvar =");
 textCalculatorInput->WriteText(data);
}

void ERYAPIXEdialogERYACalculator::OnSeven( wxCommandEvent& event )
{
  wxString data = wxT("7");
 textCalculatorInput->WriteText(data);
}

void ERYAPIXEdialogERYACalculator::OnEight( wxCommandEvent& event )
{
  wxString data = wxT("8");
 textCalculatorInput->WriteText(data);
}

void ERYAPIXEdialogERYACalculator::OnNine( wxCommandEvent& event )
{
  wxString data = wxT("9");
  textCalculatorInput->WriteText(data);
}

void ERYAPIXEdialogERYACalculator::OnPlus( wxCommandEvent& event )
{
  wxString data = wxT("+");
 textCalculatorInput->WriteText(data);
}

void ERYAPIXEdialogERYACalculator::OnClearOutput( wxCommandEvent& event )
{
 textCalculatorOutput->Clear();
 LastAns = 0;
 LastVariables.Clear();
 LastVector.clear();
}

void ERYAPIXEdialogERYACalculator::OnCos( wxCommandEvent& event )
{
  wxString data = wxT("cos(");
 textCalculatorInput->WriteText(data);
}

void ERYAPIXEdialogERYACalculator::OnSin( wxCommandEvent& event )
{
  wxString data = wxT("sin(");
 textCalculatorInput->WriteText(data);
}

void ERYAPIXEdialogERYACalculator::OnTan( wxCommandEvent& event )
{
  wxString data = wxT("tan(");
 textCalculatorInput->WriteText(data);
}

void ERYAPIXEdialogERYACalculator::OnFxmin( wxCommandEvent& event )
{
  wxString data = wxT("fxmin =");
 textCalculatorInput->WriteText(data);
}

void ERYAPIXEdialogERYACalculator::OnFour( wxCommandEvent& event )
{
  wxString data = wxT("4");
 textCalculatorInput->WriteText(data);
}

void ERYAPIXEdialogERYACalculator::OnFive( wxCommandEvent& event )
{
  wxString data = wxT("5");
 textCalculatorInput->WriteText(data);
}

void ERYAPIXEdialogERYACalculator::OnSix( wxCommandEvent& event )
{
  wxString data = wxT("6");
 textCalculatorInput->WriteText(data);
}

void ERYAPIXEdialogERYACalculator::OnMinus( wxCommandEvent& event )
{
  wxString data = wxT("-");
 textCalculatorInput->WriteText(data);
}

void ERYAPIXEdialogERYACalculator::OnComma( wxCommandEvent& event )
{
  wxString data = wxT(",");
 textCalculatorInput->WriteText(data);
}

void ERYAPIXEdialogERYACalculator::OnCosh( wxCommandEvent& event )
{
  wxString data = wxT("cosh(");
 textCalculatorInput->WriteText(data);
}

void ERYAPIXEdialogERYACalculator::OnSinh( wxCommandEvent& event )
{
  wxString data = wxT("sinh(");
 textCalculatorInput->WriteText(data);
}

void ERYAPIXEdialogERYACalculator::OnTanh( wxCommandEvent& event )
{
  wxString data = wxT("tanh(");
 textCalculatorInput->WriteText(data);
}

void ERYAPIXEdialogERYACalculator::OnFxmax( wxCommandEvent& event )
{
  wxString data = wxT("fxmax =");
 textCalculatorInput->WriteText(data);
}

void ERYAPIXEdialogERYACalculator::OnOne( wxCommandEvent& event )
{
  wxString data = wxT("1");
 textCalculatorInput->WriteText(data);
}

void ERYAPIXEdialogERYACalculator::OnTwo( wxCommandEvent& event )
{
  wxString data = wxT("2");
 textCalculatorInput->WriteText(data);
}

void ERYAPIXEdialogERYACalculator::OnThree( wxCommandEvent& event )
{
  wxString data = wxT("3");
 textCalculatorInput->WriteText(data);
}

void ERYAPIXEdialogERYACalculator::OnTimes( wxCommandEvent& event )
{
  wxString data = wxT("*");
 textCalculatorInput->WriteText(data);
}

void ERYAPIXEdialogERYACalculator::OnColon( wxCommandEvent& event )
{
  wxString data = wxT(":");
 textCalculatorInput->WriteText(data);
}

void ERYAPIXEdialogERYACalculator::OnExp( wxCommandEvent& event )
{
  wxString data = wxT("exp(");
 textCalculatorInput->WriteText(data);
}

void ERYAPIXEdialogERYACalculator::OnLog( wxCommandEvent& event )
{
  wxString data = wxT("log(");
 textCalculatorInput->WriteText(data);
}

void ERYAPIXEdialogERYACalculator::OnLn( wxCommandEvent& event )
{
  wxString data = wxT("ln(");
 textCalculatorInput->WriteText(data);
}

void ERYAPIXEdialogERYACalculator::OnFunctionVar( wxCommandEvent& event )
{
 wxTextEntryDialog *dialog = new wxTextEntryDialog(this,wxT("Value of x=... at "),wxT("Evaluate f(x), at x=?"),wxEmptyString); //Input variable value
 if (dialog->ShowModal() == wxID_OK)
 {
 double fdata;
 wxString data = dialog->GetValue();
  if(data.ToDouble(&fdata))
  {
   AlgebraicFunction *fx = new AlgebraicFunction(textCalculatorInput->GetValue()); // Compile the user defined function
   if(fx->IsFunctionWithVectorDefined())
    fx->SetFunctionNumberParameter(LastVector); // Update the vector parameters
   if(fx->GetErrorString().Len()>0)
   {
    textCalculatorOutput->SetValue(fx->GetErrorString()); //Error due to syntax errors
   }
   else
   {
    if(fx->IsFunctionDefined())
    {
     textCalculatorOutput->SetValue(this->GetConversion(fx->GetFyxEval(fdata),false)); // Get the y = f(x) value
     LastVariables.Clear(); // Dump Variables data
     for(int i=0; i<fx->GetOnlyConstants().GetCount(); i++)
     {
       wxString temp = fx->GetOnlyConstants().Item(i).GetName() + wxT(" = ") + this->GetConversion(fx->GetOnlyConstants().Item(i).GetValue(),false);
       if (i != (fx->GetOnlyConstants().GetCount()-1))
        temp = temp + wxT(",");
       LastVariables = LastVariables + temp + wxT(" ");
     }
     if((fx->GetOnlyConstants().GetCount()>0) && (fx->GetOnlyVariables().GetCount()>0) )
       LastVariables = LastVariables + wxT(", ");
     for(int j=0; j<fx->GetOnlyVariables().GetCount(); j++)
     {
       wxString temp = fx->GetOnlyVariables().Item(j).GetName() + wxT(" = ") + this->GetConversion(fx->GetOnlyVariables().Item(j).GetValue(),false);
       if (j != (fx->GetOnlyVariables().GetCount()-1))
        temp = temp + wxT(",");
       LastVariables = LastVariables + temp + wxT(" ");
     }
    }
    else
    {
      textCalculatorOutput->SetValue(wxT("Declaration Error: The function is not defined.")); // Error due to non-defined function
    }
   }
   delete fx;
  }
  else
  {
   textCalculatorOutput->SetValue(wxT("Declaration Error: Expected a numerical value for function variable.")); // Error due to non-numeric input.
  }
 }
 delete dialog;
}

void ERYAPIXEdialogERYACalculator::OnZero( wxCommandEvent& event )
{
  wxString data = wxT("0");
 textCalculatorInput->WriteText(data);
}

void ERYAPIXEdialogERYACalculator::OnDecimal( wxCommandEvent& event )
{
  wxString data = wxT(".");
 textCalculatorInput->WriteText(data);
}

void ERYAPIXEdialogERYACalculator::OnExponential( wxCommandEvent& event )
{
  wxString data = wxT("E");
 textCalculatorInput->WriteText(data);
}

void ERYAPIXEdialogERYACalculator::OnDivide( wxCommandEvent& event )
{
  wxString data = wxT("/");
 textCalculatorInput->WriteText(data);
}

void ERYAPIXEdialogERYACalculator::OnReturn( wxCommandEvent& event )
{
 AlgebraicFunction *fx = new AlgebraicFunction(textCalculatorInput->GetValue());
 if(fx->GetErrorString().Len()>0)
 {
  textCalculatorOutput->SetValue(fx->GetErrorString());
 }
 else
 {
  textCalculatorOutput->SetValue(this->GetConversion(fx->GetAnsEval(),false));
  LastAns = fx->GetAnsEval();
  LastVariables.Clear();
     for(int i=0; i<fx->GetOnlyConstants().GetCount(); i++)
     {
       wxString temp = fx->GetOnlyConstants().Item(i).GetName() + wxT(" = ") + this->GetConversion(fx->GetOnlyConstants().Item(i).GetValue(),false);
       if (i != (fx->GetOnlyConstants().GetCount()-1))
        temp = temp + wxT(",");
       LastVariables = LastVariables + temp + wxT(" ");
     }
     if((fx->GetOnlyConstants().GetCount()>0) && (fx->GetOnlyVariables().GetCount()>0) )
       LastVariables = LastVariables + wxT(", ");
     for(int j=0; j<fx->GetOnlyVariables().GetCount(); j++)
     {
       wxString temp = fx->GetOnlyVariables().Item(j).GetName() + wxT(" = ") + this->GetConversion(fx->GetOnlyVariables().Item(j).GetValue(),false);
       if (j != (fx->GetOnlyVariables().GetCount()-1))
        temp = temp + wxT(",");
       LastVariables = LastVariables + temp + wxT(" ");
     }
 }
 delete fx;
}

void ERYAPIXEdialogERYACalculator::OnHelp( wxCommandEvent& event )
{
 Close();
}

void ERYAPIXEdialogERYACalculator::OnBackSpace( wxCommandEvent& event )
{
 textCalculatorInput->Remove(textCalculatorInput->GetLastPosition()-1,textCalculatorInput->GetLastPosition());
}

void ERYAPIXEdialogERYACalculator::OnAssign( wxCommandEvent& event )
{
 wxString data = wxT(" = ");
 textCalculatorInput->WriteText(data);
}

void ERYAPIXEdialogERYACalculator::OnDisplayProgram( wxCommandEvent& event )
{
 textCalculatorOutput->Clear();
 textCalculatorOutput->WriteText(LastVariables);
}

void ERYAPIXEdialogERYACalculator::OnDisplayAnswer( wxCommandEvent& event )
{
 wxTextEntryDialog *dialog = new wxTextEntryDialog(this,wxT("Vector Numerical Values"),wxT("Fill up to 16 numerical values, separated by commas, to replace the \"fnvar\" vector values."),wxEmptyString); //Expect a valid variable
 if (dialog->ShowModal() == wxID_OK)
 {
  wxString data = dialog->GetValue();
  TextLineParser ListVector(data,wxT(","));
  wxArrayString TokenVector = ListVector.GetUnexcluded();
  if(TokenVector.GetCount()>0 && TokenVector.GetCount()<17)
  {
   LastVector.clear();
   for(int i=0; i<TokenVector.GetCount(); i++)
   {
    double temp;
    if(TokenVector.Item(i).ToDouble(&temp))
    {
      LastVector.push_back(temp);
    }
    else
    {
     textCalculatorOutput->SetValue(wxT("Declaration Error: Expected a numerical value for all vector terms")); // Error due to non-numeric input.
    }
   }
   textCalculatorOutput->SetValue(this->GetConversion(0.0,false));
  }
  else
  {
    textCalculatorOutput->SetValue(wxT("Declaration Error: ERYA Macro cannot handle a vector with more than 16 elements.")); // Error due to non-numeric input.
  }
 }
 delete dialog;
}

void ERYAPIXEdialogERYACalculator::OnLess( wxCommandEvent& event )
{
  wxString data = wxT("<");
 textCalculatorInput->WriteText(data);
}

void ERYAPIXEdialogERYACalculator::OnMore( wxCommandEvent& event )
{
 wxString data = wxT(">");
 textCalculatorInput->WriteText(data);
}

void ERYAPIXEdialogERYACalculator::OnVectorMacro( wxCommandEvent& event )
{
 wxString data = wxT("fnvar =");
 textCalculatorInput->WriteText(data);
}

void ERYAPIXEdialogERYACalculator::OnSetDecimals( wxCommandEvent& event )
{
 wxNumberEntryDialog *decimal = new wxNumberEntryDialog(this,wxT("Define the number of decimal numbers that ERYA calculator should handle:"), wxT("Number of Decimal Digits"), wxT("ERYA Precision Settings"),10,6,15);
 if(decimal->ShowModal() == wxID_OK)
 {
  long temp = decimal->GetValue();
  DecimalPrecision = static_cast<int>(temp);
 }
 delete decimal;
}

void ERYAPIXEdialogERYACalculator::OnInputVariable( wxCommandEvent& event )
{
 wxTextEntryDialog *dialog = new wxTextEntryDialog(this,wxT("Variable Name ?"),wxT("Insert a valid variable name"),wxEmptyString); //Expect a valid variable
 if (dialog->ShowModal() == wxID_OK)
 {
 wxString data = dialog->GetValue();
  AlgebraicFunction *fx = new AlgebraicFunction(data+wxT("=0")); // Test the variable compatibility
  double test = fx->GetAnsEval();
  if(fx->GetErrorString().Len()>0)
   {
    textCalculatorOutput->SetValue(fx->GetErrorString() + wxT(" : Invalid Variable Name")); //Error due to syntax errors
   }
  else if (fx->GetOnlyConstants().GetCount() != 1)
  {
    textCalculatorOutput->SetValue(wxT("Declaration Error : Invalid Variable Name")); //Error due to syntax errors
  }
  else
  {
    textCalculatorInput->SetValue(data + wxT(" = ")); //Valid variable
  }
 }
 delete dialog;
}

wxString ERYAPIXEdialogERYACalculator::GetConversion(double Var, bool Sci)
{
  wxString NumberDecimalDigits = wxString::Format("%i",DecimalPrecision); //Convert the decimal points precision as a string.
  if((std::abs(Var) < 0.01) || (std::abs(Var) > 1e+10))
    Sci = true; // Override the scientific notation for large or small numbers.
  wxString NumberFormat;
  if (Sci)
    NumberFormat = wxT("%.") + NumberDecimalDigits + wxT("e");
  else
    NumberFormat = wxT("%.") + NumberDecimalDigits + wxT("f");
  // Return the number format
  return wxString::Format(NumberFormat,Var);
}
