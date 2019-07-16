/***************************************************************
 * Name:      ParserLibrary.cpp
 * Purpose:   Algebraic and Text parser library functions
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

#include "ParserLibrary.h"
#include <wx/arrimpl.cpp>
WX_DEFINE_OBJARRAY( AlgebraicVariableArray);
WX_DEFINE_OBJARRAY( TokenArray);
WX_DEFINE_OBJARRAY( ParameterArray);

// An internal function (cannot access directly) to find the real array address of a given variable
int AlgebraicVariableVector::FindVariableAdress(wxString varname)
{
  for (int i=0; i<this->GetCount(); i++)
  {
    if (this->Item(i).GetName() == varname)
        return i;
  }
  return wxNOT_FOUND;
}

// Try to find a variable on the array with the same name as argument. Return the successfulness.
bool AlgebraicVariableVector::Find(wxString name)
{
  for (int i=0; i<this->GetCount(); i++)
  {
    if (this->Item(i).GetName() == name)
        return true;
  }
  return false;
}

// Try to find a variable on the array with the same name as argument, and return their value.
double AlgebraicVariableVector::FindGet(wxString name)
{
  for (int i=0; i<this->GetCount(); i++)
  {
    if (this->Item(i).GetName() == name)
        return this->Item(i).GetValue();
  }
  return NAN;
}

// Try to find a variable on the array with the same name as argument, while changes their numeric value. Return the successfulness.
bool AlgebraicVariableVector::FindReplace(wxString name, double newvalue)
{
  for (int i=0; i<this->GetCount(); i++)
  {
    if (this->Item(i).GetName() == name)
    {
       this->Item(i).SetValue(newvalue);
       return true;
    }
  }
  return false;
}

// An internal function (cannot access directly) to find the real array address of a given parameter
int ParameterVector::FindParameterAdress(wxString varname)
{
  for (int i=0; i<this->GetCount(); i++)
  {
    if (this->Item(i).GetName() == varname)
        return i;
  }
  return wxNOT_FOUND;
}

// Try to find a parameter on the array with the same name as argument. Return the successfulness.
bool ParameterVector::Find(wxString name)
{
  for (int i=0; i<this->GetCount(); i++)
  {
    if (this->Item(i).GetName() == name)
        return true;
  }
  return false;
}

// Try to find a parameter on the array with the same name as argument, and return their value.
wxString ParameterVector::FindGet(wxString name)
{
  for (int i=0; i<this->GetCount(); i++)
  {
    if (this->Item(i).GetName() == name)
        return this->Item(i).GetValue();
  }
  return wxEmptyString;
}

// Try to find a parameter on the array with the same name as argument, while changes their numeric value. Return the successfulness.
bool ParameterVector::FindReplace(wxString name, wxString newvalue)
{
  for (int i=0; i<this->GetCount(); i++)
  {
    if (this->Item(i).GetName() == name)
    {
       this->Item(i).SetValue(newvalue);
       return true;
    }
  }
  return false;
}


// Check the array of Tokens and classify along their types.
// The priority rules are the following:
// 0: For numbers, variables, parenthesis, commas and colons, since it had special rules
// 1: For assignment operator "="
// 2: The less "<" and more ">" operators. Also equal "==", different "<>", strict-less "<=", strict-more ">=" operators.
// 3: For sum "+" and minus "-"
// 4: For product "*" and division "/"
// 5: For power "^", where it have right (false flag) associativity
// 6: For the built-in functions, since it had the highest priority on evaluation order.
bool TokenVector::ClassifyAndCheck()
{
 // Begin the main cycle
  for (int i=0; i<this->GetCount(); i++)
  {
   if (this->CheckIsValidNumber(i)) // If the token is a valid number, it will set the default flags as a number
   {
    this->Item(i).SetAllFlags(0,true,true,false,false,false,false);
   }
   else if (this->CheckIsValidSymbol(i)) // If the token is a valid symbol, then it requires further details
   {
    if (this->Item(i).GetValue() == wxT("+"))
     this->Item(i).SetAllFlags(3,true,false,false,false,true,false);
    else if (this->Item(i).GetValue() == wxT("-"))
     this->Item(i).SetAllFlags(3,true,false,false,false,true,false);
    else if (this->Item(i).GetValue() == wxT("*"))
     this->Item(i).SetAllFlags(4,true,false,false,false,true,false);
    else if (this->Item(i).GetValue() == wxT("/"))
     this->Item(i).SetAllFlags(4,true,false,false,false,true,false);
    else if (this->Item(i).GetValue() == wxT("^"))
     this->Item(i).SetAllFlags(5,false,false,false,false,true,false);
    else if (this->Item(i).GetValue() == wxT("<"))
     this->Item(i).SetAllFlags(2,true,false,false,false,true,false);
    else if (this->Item(i).GetValue() == wxT(">"))
     this->Item(i).SetAllFlags(2,true,false,false,false,true,false);
    else if (this->Item(i).GetValue() == wxT("<="))
     this->Item(i).SetAllFlags(2,true,false,false,false,true,false);
    else if (this->Item(i).GetValue() == wxT(">="))
     this->Item(i).SetAllFlags(2,true,false,false,false,true,false);
    else if (this->Item(i).GetValue() == wxT("<>"))
     this->Item(i).SetAllFlags(2,true,false,false,false,true,false);
    else if (this->Item(i).GetValue() == wxT("=="))
     this->Item(i).SetAllFlags(2,true,false,false,false,true,false);
    else if (this->Item(i).GetValue() == wxT("="))
     this->Item(i).SetAllFlags(1,true,false,false,false,true,false);
    else if (this->Item(i).GetValue() == wxT("("))
     this->Item(i).SetAllFlags(0,true,false,false,false,true,false);
    else if (this->Item(i).GetValue() == wxT(")"))
     this->Item(i).SetAllFlags(0,true,false,false,false,true,false);
    else if (this->Item(i).GetValue() == wxT(":"))
     this->Item(i).SetAllFlags(0,true,false,false,false,true,false);
    else if (this->Item(i).GetValue() == wxT(","))
     this->Item(i).SetAllFlags(0,true,false,false,false,true,false);
    else
      return false;
   }
   else if (this->CheckIsValidVariable(i)) // If the token is a valid variable, first we need to check against the list of built-in functions, and special words
   {
    if (this->Item(i).GetValue() == wxT("asin"))
     this->Item(i).SetAllFlags(6,true,false,false,true,false,false);
    else if (this->Item(i).GetValue() == wxT("sin"))
     this->Item(i).SetAllFlags(6,true,false,false,true,false,false);
    else if (this->Item(i).GetValue() == wxT("asinh"))
     this->Item(i).SetAllFlags(6,true,false,false,true,false,false);
    else if (this->Item(i).GetValue() == wxT("sinh"))
     this->Item(i).SetAllFlags(6,true,false,false,true,false,false);
    else if (this->Item(i).GetValue() == wxT("acos"))
     this->Item(i).SetAllFlags(6,true,false,false,true,false,false);
    else if (this->Item(i).GetValue() == wxT("cos"))
     this->Item(i).SetAllFlags(6,true,false,false,true,false,false);
    else if (this->Item(i).GetValue() == wxT("acosh"))
     this->Item(i).SetAllFlags(6,true,false,false,true,false,false);
    else if (this->Item(i).GetValue() == wxT("cosh"))
     this->Item(i).SetAllFlags(6,true,false,false,true,false,false);
    else if (this->Item(i).GetValue() == wxT("atan"))
     this->Item(i).SetAllFlags(6,true,false,false,true,false,false);
    else if (this->Item(i).GetValue() == wxT("tan"))
     this->Item(i).SetAllFlags(6,true,false,false,true,false,false);
    else if (this->Item(i).GetValue() == wxT("atanh"))
     this->Item(i).SetAllFlags(6,true,false,false,true,false,false);
    else if (this->Item(i).GetValue() == wxT("tanh"))
     this->Item(i).SetAllFlags(6,true,false,false,true,false,false);
    else if (this->Item(i).GetValue() == wxT("exp"))
     this->Item(i).SetAllFlags(6,true,false,false,true,false,false);
    else if (this->Item(i).GetValue() == wxT("log"))
     this->Item(i).SetAllFlags(6,true,false,false,true,false,false);
    else if (this->Item(i).GetValue() == wxT("ln"))
     this->Item(i).SetAllFlags(6,true,false,false,true,false,false);
    else if (this->Item(i).GetValue() == wxT("sqrt"))
     this->Item(i).SetAllFlags(6,true,false,false,true,false,false);
    else if (this->Item(i).GetValue() == wxT("fxvar"))
     this->Item(i).SetAllFlags(0,true,false,false,false,false,true);
    else if (this->Item(i).GetValue() == wxT("fyvar"))
     this->Item(i).SetAllFlags(0,true,false,false,false,false,true);
    else if (this->Item(i).GetValue() == wxT("fxmin"))
     this->Item(i).SetAllFlags(0,true,false,false,false,false,true);
    else if (this->Item(i).GetValue() == wxT("fxmax"))
     this->Item(i).SetAllFlags(0,true,false,false,false,false,true);
    else if (this->Item(i).GetValue() == wxT("fnvar"))
     this->Item(i).SetAllFlags(0,true,false,false,false,false,true);
    else
     this->Item(i).SetAllFlags(0,true,false,true,false,false,false); // If don`t belong to the built-in words, then it`s a variable
   }
   else // If nothing are valid, return a failure.
    return false;
  }
  return true; // Return true, since the whole process was done.
}

// Verify the consistency of a number
bool Token::CheckIsValidNumber()
{
 double temp;
 return (this->GetValue().ToDouble(&temp));
}

// Verify the consistency of a number
bool TokenVector::CheckIsValidNumber(int position)
{
 double temp;
 return (this->Item(position).GetValue().ToDouble(&temp));
}

// Verify the consistency of an alfanumeric string
bool Token::CheckIsValidVariable()
{
  wxString temp = wxT("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
  wxString temp0 = wxT("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
  wxString character;
  int counter;
  for (int i=0; i<this->GetValue().Len(); i++)
  {
    character = this->GetValue().GetChar(i);
    if (i == 0)
     counter = temp0.Find(character);
    else
     counter = temp.Find(character);
    if (counter == wxNOT_FOUND)
    {
      return false;
    }
  }
  return true;
}

// Verify the consistency of an alfanumeric string
bool TokenVector::CheckIsValidVariable(int position)
{
  wxString temp = wxT("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
  wxString temp0 = wxT("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
  wxString character;
  int counter;
  for (int i=0; i<this->Item(position).GetValue().Len(); i++)
  {
    character = this->Item(position).GetValue().GetChar(i);
    if (i == 0)
     counter = temp0.Find(character);
    else
     counter = temp.Find(character);
    if (counter == wxNOT_FOUND)
    {
      return false;
    }
  }
  return true;
}

// Verify the consistency of a valid symbol
bool Token::CheckIsValidSymbol()
{
 wxString temp = wxT("()=:,^*/-+<>");
  wxString character;
  int counter;
  for (int i=0; i<this->GetValue().Len(); i++)
  {
    character = this->GetValue().GetChar(i);
    counter = temp.Find(character);
    if (counter == wxNOT_FOUND)
    {
      return false;
    }
  }
  return true;
}

// Verify the consistency of a valid symbol
bool TokenVector::CheckIsValidSymbol(int position)
{
 wxString temp = wxT("()=:,^*/-+<>");
  wxString character;
  int counter;
  for (int i=0; i<this->Item(position).GetValue().Len(); i++)
  {
    character = this->Item(position).GetValue().GetChar(i);
    counter = temp.Find(character);
    if (counter == wxNOT_FOUND)
    {
      return false;
    }
  }
  return true;
}

// Generate the array of tokens from the original string.
// The main three types of tokens are:
// Numeric Token: Which represent a number, including in the scientific notation form.
// Symbol Token: A single character token, which is almost easy to tokenize.
// Variable Token: Any array of alfanumerical characters. The first character should be a letter.
bool AlgebraicFunction::AlgebraicParser()
{
 wxString cache,character;
 for(int i=0; i<InternalFunctionString.Len(); i++)
 {
   character.Clear();
   character = InternalFunctionString.GetChar(i);
   Token *testchar = new Token(character);
   Token *testcache = new Token(cache + character);
   Token *testnumber = new Token(cache + character + wxT("0"));
   // Empty characters will not produce any new token, but will tokenize any remaining characters left on temporary memory
   if (character == wxT(" ") || character == wxT("\t") || character == wxT("\n"))
   {
     if(cache.Len()>0)
     {
       if(InternalFunctionTokens.NewToken(cache))
        {
         cache.Clear();
        }
       else
        {
         ErrorCode = wxT("Syntax Error: Invalid Token \"") + cache + wxT("\"");
         return false;
        }
     }
   }
   // Any operator character, except the "-" or "+" due to the fact may belong to a number token, are simply tokenized
   else if (testchar->CheckIsValidSymbol() && testchar->GetValue() != wxT("+") && testchar->GetValue() != wxT("-"))
   {
     if(cache.Len()>0)
     {
       if(InternalFunctionTokens.NewToken(cache))
        {
         cache.Clear();
         // Since some operators had two characters, pick the next character.
         wxString nextchar,symbol;
         bool IsTwoCharSymbol = false;
         symbol = character; //by default
         if (InternalFunctionString.Len()-i>1)
           nextchar = InternalFunctionString.GetChar(i+1);
         // Check if the first character had only the symbols "<",">","="; in order to find the same thing for the second character.
         if(character == wxT("<") || character == wxT(">") || character == wxT("="))
         {
           if(nextchar == wxT("<") || nextchar == wxT(">") || nextchar == wxT("="))
           {
             symbol = character + nextchar; //A valid two character operator
             IsTwoCharSymbol = true;
           }
           else
           {
             symbol = character; //A valid single character operator
             IsTwoCharSymbol = false;
           }
         }
         if(!(InternalFunctionTokens.NewToken(symbol)))
         {
         ErrorCode = wxT("Syntax Error: Invalid Operator Symbol \"") + symbol + wxT("\"");
         return false;
         }
         else
         {
           if(IsTwoCharSymbol)
            i=i+1; //shift the parsing character main cycle
         }
        }
       else
        {
         ErrorCode = wxT("Syntax Error: Invalid Token \"") + cache + wxT("\"");
         return false;
        }
     }
     else
     {
      cache.Clear();
         // Since some operators had two characters, pick the next character.
         wxString nextchar,symbol;
         bool IsTwoCharSymbol = false;
         symbol = character; //by default
         if (InternalFunctionString.Len()-i>1)
           nextchar = InternalFunctionString.GetChar(i+1);
         // Check if the first character had only the symbols "<",">","="; in order to find the same thing for the second character.
         if(character == wxT("<") || character == wxT(">") || character == wxT("="))
         {
           if(nextchar == wxT("<") || nextchar == wxT(">") || nextchar == wxT("="))
           {
             symbol = character + nextchar; //A valid two character operator
             IsTwoCharSymbol = true;
           }
           else
           {
             symbol = character; //A valid single character operator
             IsTwoCharSymbol = false;
           }
         }
         if(!(InternalFunctionTokens.NewToken(symbol)))
         {
         ErrorCode = wxT("Syntax Error: Invalid Operator Symbol \"") + symbol + wxT("\"");
         return false;
         }
         else
         {
           if(IsTwoCharSymbol)
            i=i+1; //shift the parsing character main cycle
         }
     }
   }
   // Next case will handle the little fry when the current character is a "-" or "+" symbol
   else if (character == wxT("-") || character == wxT("+"))
   {
     // Picking the next character
     wxString nextchar;
     if (InternalFunctionString.Len()-i>1)
      nextchar = InternalFunctionString.GetChar(i+1);
     if(cache.Len()>0)
     {
       // Testing for number compatibility, i.e., (123.5e)(+/-)(0)
       Token *testsci = new Token(cache + character + nextchar);
       if (testsci->CheckIsValidNumber())
       {
         cache = cache + character;
       }
       else
       {
         // Store the current cache, and the new symbol as separate tokens.
       if(InternalFunctionTokens.NewToken(cache))
        {
         cache.Clear();
         if(!(InternalFunctionTokens.NewToken(character)))
         {
         ErrorCode = wxT("Syntax Error: Invalid Operator Symbol \"") + cache + wxT("\"");
         return false;
         }
        }
       else
        {
         ErrorCode = wxT("Syntax Error: Invalid Token \"") + cache + wxT("\"");
         return false;
        }
       }
       delete testsci;
     }
     else
     {
      // Testing for number compatibility, i.e., (+/-)(0)
       Token *testnum = new Token(character + nextchar);
       if (testnum->CheckIsValidNumber())
       {
         if(InternalFunctionTokens.GetCount()>0) //Check if the previous token are a variable, function or a symbol, as long it wans't the right parenthesis ")"
         {
           if(InternalFunctionTokens.Last().GetPriority()>0 || InternalFunctionTokens.Last().GetValue() == wxT("(") ) //On higher priority tokens, or the left parenthesis "(" any plus and minus sign should be the beginning of a number token.
           {
             cache = cache + character;
           }
           else
           {
            if(InternalFunctionTokens.Last().GetValue() == wxT(")") || InternalFunctionTokens.Last().CheckIsValidNumber() || InternalFunctionTokens.Last().CheckIsValidSymbol())
            {
               // The sign should be an independent token, since it was an arithmetic operator
               cache.Clear();
              if(!(InternalFunctionTokens.NewToken(character)))
              {
               ErrorCode = wxT("Syntax Error: Invalid Operator Symbol \"") + character + wxT("\"");
               return false;
              }
            }
            else
            {
             cache = cache + character;
            }
           }
         }
         else //First token should be a number
         {
         cache = cache + character;
         }
       }
       else
       {
         // Otherwise just tokenize the current symbol
        cache.Clear();
        if(!(InternalFunctionTokens.NewToken(character)))
        {
         ErrorCode = wxT("Syntax Error: Invalid Operator Symbol \"") + character + wxT("\"");
         return false;
        }
       }
       delete testnum;
     }
   }
   // If the current character are an alfanumeric one, just add to the current cache
   else if (testcache->CheckIsValidNumber() || testcache->CheckIsValidVariable() || testnumber->CheckIsValidNumber())
    {
         cache = cache + character;
    }
   // And the default case will bring a fatal error
   else
    {
      ErrorCode = wxT("Syntax Error: Invalid or Unknown Symbol \"") + character + wxT("\"");
      return false;
    }
   delete testchar;
   delete testcache;
   delete testnumber;
 }
 // Store the remaining temporary cache as a new token, if applicable
 if(cache.Len()>0)
     {
       if(InternalFunctionTokens.NewToken(cache))
        {
         cache.Clear();
        }
       else
        {
         ErrorCode = wxT("Syntax Error: Invalid Token  \"") + cache + wxT("\"");
         return false;
        }
     }
 // And finishes the procedure
  return true;
}

// Evaluate the numerical values of built-in functions, using the STL library
double AlgebraicFunction::EvaluateFunction(Token f, double arg)
{
 if(f.GetValue() == wxT("sin"))
  return std::sin(arg);
 else if(f.GetValue() == wxT("asin"))
  return std::asin(arg);
 else if(f.GetValue() == wxT("sinh"))
  return std::sinh(arg);
 else if(f.GetValue() == wxT("asinh"))
  return std::asinh(arg);
 else if(f.GetValue() == wxT("cos"))
  return std::cos(arg);
 else if(f.GetValue() == wxT("acos"))
  return std::acos(arg);
 else if(f.GetValue() == wxT("cosh"))
  return std::cosh(arg);
 else if(f.GetValue() == wxT("acosh"))
  return std::acosh(arg);
 else if(f.GetValue() == wxT("tan"))
  return std::tan(arg);
 else if(f.GetValue() == wxT("atan"))
  return std::atan(arg);
 else if(f.GetValue() == wxT("tanh"))
  return std::tanh(arg);
 else if(f.GetValue() == wxT("atanh"))
  return std::atanh(arg);
 else if(f.GetValue() == wxT("sqrt"))
  return std::sqrt(arg);
 else if(f.GetValue() == wxT("exp"))
  return std::exp(arg);
 else if(f.GetValue() == wxT("ln"))
  return std::log(arg);
 else if(f.GetValue() == wxT("log"))
  return std::log10(arg);
 else
  return NAN;
}

// Evaluate numerically the built-in arithmetic operators.
double AlgebraicFunction::EvaluateOperator(Token o, double arg1, double arg2)
{
 if(o.GetValue() == wxT("+"))
  return arg2 + arg1;
 else if(o.GetValue() == wxT("-"))
  return arg2 - arg1;
 else if(o.GetValue() == wxT("*"))
  return arg2 * arg1;
 else if(o.GetValue() == wxT("/"))
  return arg2 / arg1;
 else if(o.GetValue() == wxT("^"))
  return std::pow(arg2,arg1);
 else if(o.GetValue() == wxT("<"))
  return (arg2 < arg1) ? 1.0 : 0.0;
 else if(o.GetValue() == wxT(">"))
  return (arg2 > arg1) ? 1.0 : 0.0;
 else if(o.GetValue() == wxT("<="))
  return (arg2 <= arg1) ? 1.0 : 0.0;
 else if(o.GetValue() == wxT(">="))
  return (arg2 >= arg1) ? 1.0 : 0.0;
 else if(o.GetValue() == wxT("<>"))
  return (arg2 != arg1) ? 1.0 : 0.0;
 else if(o.GetValue() == wxT("=="))
  return (arg2 == arg1) ? 1.0 : 0.0;
 else
  return NAN;
}

// Convert the token array to the RPN format, which is more manageable to evaluate numerical data.
bool AlgebraicFunction::AlgebraicCompiler()
{
 RPNTokenStack.Clear();
 TokenVector LocalOperatorStack;
 // Start the Shunting-Yard main cycle
 for(int i=0; i<InternalFunctionTokens.GetCount(); i++)
 {
  // If the current toke is a number, just copy to the output stack
  if (InternalFunctionTokens.Item(i).IsNumber())
  {
   RPNTokenStack.Add(InternalFunctionTokens.Item(i));
  }
  // A variable follows the same rule, since it is a symbolic place for a number.
  else if (InternalFunctionTokens.Item(i).IsVariable())
  {
   RPNTokenStack.Add(InternalFunctionTokens.Item(i));
  }
  // A parameter follows the same rule of a variable or a number, then just copy to the output stack.
  else if (InternalFunctionTokens.Item(i).IsParameter())
  {
   RPNTokenStack.Add(InternalFunctionTokens.Item(i));
  }
  // The "=" token will copy to the stack, and it is the lowest precedence operator.
  else if (InternalFunctionTokens.Item(i).GetValue() == wxT("="))
  {
   LocalOperatorStack.Add(InternalFunctionTokens.Item(i));
  }
  // Any function token will be sent to the operator stack
  else if (InternalFunctionTokens.Item(i).IsFunction())
  {
   LocalOperatorStack.Add(InternalFunctionTokens.Item(i));
  }
  // The comma "," operator will flush the entire operator stack, and it should not have some symbols like the left parenthesis left. Finally the comma pop to stack
  else if (InternalFunctionTokens.Item(i).GetValue() == wxT(","))
  {
   for(int j=0; j<LocalOperatorStack.GetCount(); j++)
   {
    if (LocalOperatorStack.Item(LocalOperatorStack.GetCount()-1-j).GetValue() == wxT("("))
     {
      ErrorCode = wxT("Syntax Error: Parenthesis \"(\" out-of-place");
      return false;
     }
     else
     {
      RPNTokenStack.Add(LocalOperatorStack.Item(LocalOperatorStack.GetCount()-1-j));
     }
   }
   LocalOperatorStack.Clear();
  }
  // The colon ":" operator will also flush the entire operator stack as the previous case, but at the end, the colon symbol will be exported to the output stack
  else if (InternalFunctionTokens.Item(i).GetValue() == wxT(":"))
  {
   for(int j=0; j<LocalOperatorStack.GetCount(); j++)
   {
    if (LocalOperatorStack.Item(LocalOperatorStack.GetCount()-1-j).GetValue() == wxT("("))
     {
      ErrorCode = wxT("Syntax Error: Parenthesis \"(\" out-of-place");
      return false;
     }
     else
     {
      RPNTokenStack.Add(LocalOperatorStack.Item(LocalOperatorStack.GetCount()-1-j));
     }
   }
   LocalOperatorStack.Clear();
   RPNTokenStack.Add(InternalFunctionTokens.Item(i));
  }
  // If the operator stack is empty, any operator will be sent to the operator stack
  else if (InternalFunctionTokens.Item(i).IsOperator() && LocalOperatorStack.GetCount() == 0 )
  {
   LocalOperatorStack.Add(InternalFunctionTokens.Item(i));
  }
  // If the current token is an arithmetic operator, then start the following cycle...
  else if (InternalFunctionTokens.Item(i).IsOperator() && InternalFunctionTokens.Item(i).GetPriority() > 0 )
  {
    // Start the popping cycle
    int OriginalStackSize = LocalOperatorStack.GetCount();
    for (int j=0; j<OriginalStackSize; j++)
    {
     // If the current token is left associative and their priority are equal or less than the operator stack priority, pop that operator to the output stack
     if (InternalFunctionTokens.Item(i).GetAssociativity() && (InternalFunctionTokens.Item(i).GetPriority() <= LocalOperatorStack.Last().GetPriority()))
     {
      RPNTokenStack.Add(LocalOperatorStack.Item(LocalOperatorStack.GetCount()-1));
      LocalOperatorStack.RemoveAt(LocalOperatorStack.GetCount()-1);
     }
     // If the current token is right associative and their priority are  less than the operator stack priority, pop that operator to the output stack
     else if (!(InternalFunctionTokens.Item(i).GetAssociativity()) && (InternalFunctionTokens.Item(i).GetPriority() < LocalOperatorStack.Last().GetPriority()))
     {
      RPNTokenStack.Add(LocalOperatorStack.Item(LocalOperatorStack.GetCount()-1));
      LocalOperatorStack.RemoveAt(LocalOperatorStack.GetCount()-1);
     }
     else // Stop the popping
     {
      j = OriginalStackSize;
     }
    }
    // Push the current token to the operator stack
    LocalOperatorStack.Add(InternalFunctionTokens.Item(i));
  }
  // A left parenthesis just copy to the stack
  else if (InternalFunctionTokens.Item(i).GetValue() == wxT("("))
  {
   LocalOperatorStack.Add(InternalFunctionTokens.Item(i));
  }
  // A right parenthesis will flush the operator stack until a left parenthesis are found, and both discarded. Also, it should not cross with commas, colons and assignments
  else if (InternalFunctionTokens.Item(i).GetValue() == wxT(")"))
  {
   int OriginalStackSize = LocalOperatorStack.GetCount();
   bool FoundLeftParenthesis = false;
   for(int j=0; j<OriginalStackSize; j++)
   {
    if (LocalOperatorStack.Last().GetValue() == wxT("("))
     {
      // If found a left parenthesis, delete it and finishes the flushing cycle
      LocalOperatorStack.RemoveAt(LocalOperatorStack.GetCount()-1);
       j = OriginalStackSize;
       FoundLeftParenthesis = true;
      // Also check if the first top of the operator stack is a function (since their arguments are bounded by parenthesis)
      if (LocalOperatorStack.GetCount()>0)
      {
        if(LocalOperatorStack.Last().IsFunction()) // If so, then copy the function token to the output stack
        {
          RPNTokenStack.Add(LocalOperatorStack.Last());
          LocalOperatorStack.RemoveAt(LocalOperatorStack.GetCount()-1);
        }
      }
     }
     else if (LocalOperatorStack.Last().GetValue() == wxT("="))
     {
      ErrorCode = wxT("Syntax Error: Assignment operator \"=\" out-of-place");
      return false;
     }
     else if (LocalOperatorStack.Last().GetValue() == wxT(":"))
     {
      ErrorCode = wxT("Syntax Error: Multi-statement operator \":\" out-of-place");
      return false;
     }
     else if (LocalOperatorStack.Last().GetValue() == wxT(","))
     {
      ErrorCode = wxT("Syntax Error: Variable separator \",\" out-of-place");
      return false;
     }
     else
     {
      RPNTokenStack.Add(LocalOperatorStack.Last());
      LocalOperatorStack.RemoveAt(LocalOperatorStack.GetCount()-1);
     }
   }
   if (!(FoundLeftParenthesis))
   {
      ErrorCode = wxT("Syntax Error: Parenthesis Mismatch");
      return false;
   }
  }
  // An unknown token will trigger a fatal error
  else
  {
      ErrorCode = wxT("Fatal Error: Undefined Variable, Function or Operator: \"") + InternalFunctionTokens.Item(i).GetValue() + wxT("\"");
      return false;
  }
 }
 // Flush the remaining operator stack
 for(int k=0; k<LocalOperatorStack.GetCount(); k++)
   {
    if (LocalOperatorStack.Item(LocalOperatorStack.GetCount()-1-k).GetValue() == wxT("("))
     {
      ErrorCode = wxT("Syntax Error: Parenthesis Mismatch");
      return false;
     }
     else
     {
      RPNTokenStack.Add(LocalOperatorStack.Item(LocalOperatorStack.GetCount()-1-k));
     }
   }
   LocalOperatorStack.Clear();
 // Exit the procedure
   return true;
}

// Implements a postfix calculator, using the tokens previously converted to the RPN stack, and return the numerical values.
bool AlgebraicFunction::AlgebraicCalculator()
{
 // The internal register stack required for temporary memory use the variable class, using the following convention:
 // A standard number will be treated as a _NULL_ variable.
 // A variable will have the default "true" value, otherwise a constant it will be "false".
 // A parameter accepts strings as arguments, and it will have a separate stack.
 AlgebraicVariableVector RegisterStack;
 RPNFunctionStack.Clear();

 // Begin the RPN processing main cycle, which is similar to a computer low-level program
 for (int i=0; i<RPNTokenStack.GetCount(); i++)
 {
   // Saving the current token to the function stack
   RPNFunctionStack.Add(RPNTokenStack.Item(i));
   // A number token are converted to a double precision constant and pop to the register stack.
   if (RPNTokenStack.Item(i).IsNumber())
   {
    double temp;
    RPNTokenStack.Item(i).GetValue().ToDouble(&temp);
    RegisterStack.NewConstant(wxT("_NULL_"),temp);
   }
   // A parameter is also a special type constant.
   else if (RPNTokenStack.Item(i).IsParameter())
   {
    RegisterStack.NewConstant(RPNTokenStack.Item(i).GetValue(),0.0);
   }
   // A variable are simply moved to the register stack with the same name, and a null value.
   else if (RPNTokenStack.Item(i).IsVariable())
   {
    RegisterStack.NewConstant(RPNTokenStack.Item(i).GetValue(),1.0);
   }
   // The assigment operator expect two arguments from the register stack, where the first argument will be copied to the second one.
   else if (RPNTokenStack.Item(i).GetValue() == wxT("="))
   {
     if(RegisterStack.GetCount()>1) // Requires at least two arguments
     {
        // The left argument is the new variable, or the parameter
        wxString tempvar = RegisterStack.Item(RegisterStack.GetCount()-2).GetName(); // Get the variable name
        if (InternalFunctionVariables.Find(tempvar) || FunctionParameters.Find(tempvar)) // check the presence of the new declared variable/constant
        {
          if(RegisterStack.Item(RegisterStack.GetCount()-2).IsConstant() && InternalFunctionVariables.Find(tempvar)) // A constant cannot be overwritten
          {
            // Unless the constant is a function variable
            if ( tempvar == FunctionParameters.FindGet(wxT("fyvar")) || tempvar == FunctionParameters.FindGet(wxT("fxvar")) || tempvar == FunctionParameters.FindGet(wxT("fxmin")) || tempvar == FunctionParameters.FindGet(wxT("fxmax")) || tempvar == FunctionParameters.FindGet(wxT("fnvar")))
            {
             double temp = RegisterStack.Item(RegisterStack.GetCount()-1).GetValue(); // Get the variable value
             InternalFunctionVariables.FindReplace(tempvar,temp); // Store the new variable
             RegisterStack.RemoveAt(RegisterStack.GetCount()-1); // Delete the two arguments from the register stack
             RegisterStack.RemoveAt(RegisterStack.GetCount()-1);
            }
            else
            {
             ErrorCode = wxT("Declaration Error: Cannot redefine the constant, \"") + RegisterStack.Item(RegisterStack.GetCount()-2).GetName() + wxT("\".\nSince it already values \"") + RegisterStack.Item(RegisterStack.GetCount()-2).GetName() + wxT(" = ") + wxString::Format( wxT("%.10e"), InternalFunctionVariables.FindGet(RegisterStack.Item(RegisterStack.GetCount()-2).GetName())) + wxT("\""); // A constant cannot be redefined!
             return false;
            }
          }
          else if(RegisterStack.Item(RegisterStack.GetCount()-2).IsConstant() && FunctionParameters.Find(tempvar)) // A parameter cannot be overwritten
          {
            ErrorCode = wxT("Declaration Error: Cannot overwrite the parameter, \"") + RegisterStack.Item(RegisterStack.GetCount()-2).GetName() + wxT("\".\nSince it already values \"") + RegisterStack.Item(RegisterStack.GetCount()-2).GetName() + wxT(" = ") + FunctionParameters.FindGet(RegisterStack.Item(RegisterStack.GetCount()-2).GetName()) + wxT("\""); // A constant cannot be redefined!
             return false;
          }
          else if(RegisterStack.Item(RegisterStack.GetCount()-2).IsVariable() && InternalFunctionVariables.Find(tempvar)) // A declared variable can be overwritten by the new value
          {
            double temp = RegisterStack.Item(RegisterStack.GetCount()-1).GetValue(); // Get the variable value
            InternalFunctionVariables.FindReplace(tempvar,temp); // Store the new variable
            RegisterStack.RemoveAt(RegisterStack.GetCount()-1); // Delete the two arguments from the register stack
            RegisterStack.RemoveAt(RegisterStack.GetCount()-1);
          }
          else // An illegal operation with the assignment operator
          {
            ErrorCode = wxT("Declaration Error: Invalid variable name format, \"") + RegisterStack.Item(RegisterStack.GetCount()-2).GetName() + wxT("\", for the assignment operator \"=\".");
            return false;
          }
        }
        else // It's a new variable/parameter/constant
        {
         if(RegisterStack.Item(RegisterStack.GetCount()-2).IsConstant()) // Handles the constant, and potential parameters
          {
            // Define the potential parameter candidates, and a temporary numerical value
            wxString ParameterType = RegisterStack.Item(RegisterStack.GetCount()-2).GetName();
            wxString ParameterTerm = RegisterStack.Item(RegisterStack.GetCount()-1).GetName();
            double ConstantValue;
            // Begin the validity of the parameters first, and later the internal algebraic variables
            if (ParameterType == wxT("fxvar") || ParameterType == wxT("fyvar") || ParameterType == wxT("fxmin") || ParameterType == wxT("fxmax")) // The y-var and x-var of a function requires a valid name
            {
              if (ParameterTerm != wxT("_NULL_"))
              {
                // Define a new parameter, and also a standard variable, since their argument are any variable, as long do not exist!
                if(FunctionParameters.Find(ParameterType))
                {
                 ErrorCode = wxT("Declaration Error: Function Parameter \"") + ParameterType + wxT("\" already values \"") + FunctionParameters.FindGet(ParameterType) + wxT("\", and cannot be overwritten.");
                 return false;
                }
                else
                {
                 FunctionParameters.NewParameter(ParameterType,ParameterTerm);
                 InternalFunctionVariables.NewVariable(ParameterTerm,1.0);
                  RegisterStack.RemoveAt(RegisterStack.GetCount()-1); // Delete the two arguments from the register stack
                  RegisterStack.RemoveAt(RegisterStack.GetCount()-1);
                }
              }
              else
              {
               ErrorCode = wxT("Declaration Error: Function Parameter \"") + ParameterType + wxT("\" cannot accept numerical values.\"");
               return false;
              }
            }
            else if (ParameterType == wxT("fnvar")) // A parameter macro to create 16 new variables with the form <name>, <name>1,..., <name>15
            {
              if (ParameterTerm != wxT("_NULL_"))
              {
                // Define a new parameter, and also a standard variable, since their argument are any variable, as long do not exist!
                if(FunctionParameters.Find(ParameterType))
                {
                 ErrorCode = wxT("Declaration Error: Function Parameter \"") + ParameterType + wxT("\" already values \"") + FunctionParameters.FindGet(ParameterType) + wxT("\", and cannot be overwritten.");
                 return false;
                }
                else
                {
                 FunctionParameters.NewParameter(ParameterType,ParameterTerm);
                 InternalFunctionVariables.NewVariable(ParameterTerm,1.0);
                  RegisterStack.RemoveAt(RegisterStack.GetCount()-1); // Delete the two arguments from the register stack
                  RegisterStack.RemoveAt(RegisterStack.GetCount()-1);
                  for(int z=1; z<16; z++)
                  {
                    wxString ParameterTermData = ParameterTerm + wxString::Format("%d",z);
                    InternalFunctionVariables.NewVariable(ParameterTermData,1.0);
                  }
                }
              }
              else
              {
               ErrorCode = wxT("Declaration Error: Function Parameter \"") + ParameterType + wxT("\" cannot accept numerical values.\"");
               return false;
              }
            }
            // Define a new constant while expect a numerical value, or another constant or variable
           else
           {
            // Get the numerical value of the new constant
            if(InternalFunctionVariables.Find(RegisterStack.Item(RegisterStack.GetCount()-1).GetName())) // Verify if the variable are defined
            {
            ConstantValue = InternalFunctionVariables.FindGet(RegisterStack.Item(RegisterStack.GetCount()-1).GetName()); // Get the variable value
            }
            else if (RegisterStack.Item(RegisterStack.GetCount()-1).GetName() == wxT("_NULL_"))
            {
            ConstantValue = RegisterStack.Item(RegisterStack.GetCount()-1).GetValue(); // Get the variable value
            }
            else
            {
            ErrorCode = wxT("Declaration Error: Undefined Variable: \"") + RegisterStack.Item(RegisterStack.GetCount()-1).GetName() + wxT("\"");
            return false;
            }
            InternalFunctionVariables.NewConstant(ParameterType,ConstantValue); // Store the new constant
            RegisterStack.RemoveAt(RegisterStack.GetCount()-1); // Delete the two arguments from the register stack
            RegisterStack.RemoveAt(RegisterStack.GetCount()-1);
           }
          }
          else if(RegisterStack.Item(RegisterStack.GetCount()-2).IsVariable()) // Handles the variable case.
          {
             double ConstantValue;
             // Get the numerical value of the new constant
            if(InternalFunctionVariables.Find(RegisterStack.Item(RegisterStack.GetCount()-1).GetName())) // Verify if the variable are defined
            {
            ConstantValue = InternalFunctionVariables.FindGet(RegisterStack.Item(RegisterStack.GetCount()-1).GetName()); // Get the variable value
            }
            else if (RegisterStack.Item(RegisterStack.GetCount()-1).GetName() == wxT("_NULL_"))
            {
            ConstantValue = RegisterStack.Item(RegisterStack.GetCount()-1).GetValue(); // Get the variable value
            }
            else
            {
            ErrorCode = wxT("Declaration Error: Undefined Variable: \"") + RegisterStack.Item(RegisterStack.GetCount()-1).GetName() + wxT("\"");
            return false;
            }
            InternalFunctionVariables.NewVariable(RegisterStack.Item(RegisterStack.GetCount()-2).GetName(),ConstantValue); // Store the new constant
            RegisterStack.RemoveAt(RegisterStack.GetCount()-1); // Delete the two arguments from the register stack
            RegisterStack.RemoveAt(RegisterStack.GetCount()-1);
          }
          else  // Invalid type
          {
            ErrorCode = wxT("Declaration Error: Invalid variable name format, \"") + RegisterStack.Item(RegisterStack.GetCount()-2).GetName() + wxT("\", for the assignment operator \"=\".");
            return false;
          }
        }
     }
     else // Invalid number of arguments
     {
      ErrorCode = wxT("Declaration Error: Assignment operator \"=\" without valid arguments");
      return false;
     }
   }
  // The colon ":" operator will delete the entire register stack, while try to store the last numerical value to the output variable.
  else if (RPNTokenStack.Item(i).GetValue() == wxT(":"))
  {
    // First of all, the function stack are cleared...
    RPNFunctionStack.Clear();
    RegisterStack.Clear();
    if (RegisterStack.GetCount() == 0) // Empty register stack
    {
     RegisterStack.Clear();
    }
    else if (RegisterStack.GetCount() == 1) // One value stored
    {
     if (RegisterStack.Item(RegisterStack.GetCount()-1).GetName() == wxT("_NULL_")) // Numerical tokens will be stored to the output variable
      {
       FunctionValue = RegisterStack.Item(RegisterStack.GetCount()-1).GetValue();
       RegisterStack.Clear();
      }
      else if(InternalFunctionVariables.Find(RegisterStack.Item(RegisterStack.GetCount()-1).GetName()))
      {
       FunctionValue = InternalFunctionVariables.FindGet(RegisterStack.Item(RegisterStack.GetCount()-1).GetName());
       RegisterStack.Clear();
      }
      else if(FunctionParameters.Find(RegisterStack.Item(RegisterStack.GetCount()-1).GetName()))
      {
        ErrorCode = wxT("Syntax Error: Cannot use parameter: \"") + RegisterStack.Item(RegisterStack.GetCount()-1).GetName() + wxT("\" outside definition.");
        return false;
      }
      else
      {
        ErrorCode = wxT("Declaration Error: Undefined Variable: \"") + RegisterStack.Item(RegisterStack.GetCount()-1).GetName() + wxT("\"");
        return false;
      }
    }
    else
    {
      ErrorCode = wxT("Syntax Error: Invalid numerical expression"); // Invalid syntax format.
      return false;
    }
  }
 // Verify the evaluation of a function, while take care the potential numerical overflows using the STL library.
  else if (RPNTokenStack.Item(i).IsFunction())
  {
    if (RegisterStack.GetCount() > 0)
    {
      if (RegisterStack.Item(RegisterStack.GetCount()-1).GetName() != wxT("_NULL_")) // If the argument are a variable
      {
        if (InternalFunctionVariables.Find(RegisterStack.Item(RegisterStack.GetCount()-1).GetName()))
        {
          double temp = InternalFunctionVariables.FindGet(RegisterStack.Item(RegisterStack.GetCount()-1).GetName());
          double f = this->EvaluateFunction(RPNTokenStack.Item(i),temp);
          RegisterStack.RemoveAt(RegisterStack.GetCount()-1);
          RegisterStack.NewVariable(wxT("_NULL_"),f);
        }
        else
        {
          ErrorCode = wxT("Declaration Error: Undefined Variable: \"") + RegisterStack.Item(RegisterStack.GetCount()-1).GetName() + wxT("\"");
          return false;
        }
      }
      else // If the argument are a number
      {
       double f = this->EvaluateFunction(RPNTokenStack.Item(i),RegisterStack.Item(RegisterStack.GetCount()-1).GetValue());
       RegisterStack.RemoveAt(RegisterStack.GetCount()-1);
       RegisterStack.NewVariable(wxT("_NULL_"),f);
      }
    }
    else
    {
      ErrorCode = wxT("Syntax Error: Function \"") + RPNTokenStack.Item(i).GetValue() + wxT("\" without valid argument."); // Invalid syntax format.
      return false;
    }
  }
 // Evaluate an arithmetic operator, and use the STL library to handle invalid numerical results
  else if (RPNTokenStack.Item(i).IsOperator() && RPNTokenStack.Item(i).GetPriority() > 0)
  {
    if (RegisterStack.GetCount() > 1)
    {
      double temp1,temp2;
      wxString tempvar1,tempvar2;
      // First argument
      if (RegisterStack.Item(RegisterStack.GetCount()-1).GetName() != wxT("_NULL_"))
      {
       tempvar1 = RegisterStack.Item(RegisterStack.GetCount()-1).GetName();
       if (InternalFunctionVariables.Find(RegisterStack.Item(RegisterStack.GetCount()-1).GetName()))
        {
          temp1 = InternalFunctionVariables.FindGet(RegisterStack.Item(RegisterStack.GetCount()-1).GetName());
        }
        else
        {
          ErrorCode = wxT("Declaration Error: Undefined Variable: \"") + RegisterStack.Item(RegisterStack.GetCount()-1).GetName() + wxT("\"");
          return false;
        }
      }
      else
      {
        temp1 = RegisterStack.Item(RegisterStack.GetCount()-1).GetValue();
      }
      // Second argument
      if (RegisterStack.Item(RegisterStack.GetCount()-2).GetName() != wxT("_NULL_"))
      {
       tempvar2 = RegisterStack.Item(RegisterStack.GetCount()-2).GetName();
       if (InternalFunctionVariables.Find(RegisterStack.Item(RegisterStack.GetCount()-2).GetName()))
        {
          temp2 = InternalFunctionVariables.FindGet(RegisterStack.Item(RegisterStack.GetCount()-2).GetName());
        }
        else
        {
          ErrorCode = wxT("Declaration Error: Undefined Variable: \"") + RegisterStack.Item(RegisterStack.GetCount()-2).GetName() + wxT("\"");
          return false;
        }
      }
      else
      {
        temp2 = RegisterStack.Item(RegisterStack.GetCount()-2).GetValue();
      }
      // Evaluate the result
      double f = this->EvaluateOperator(RPNTokenStack.Item(i),temp1,temp2);
      RegisterStack.RemoveAt(RegisterStack.GetCount()-1);
      RegisterStack.RemoveAt(RegisterStack.GetCount()-1);
      RegisterStack.NewVariable(wxT("_NULL_"),f);
    }
    else
    {
      ErrorCode = wxT("Syntax Error: Arithmetic Operator \"") + RPNTokenStack.Item(i).GetValue() + wxT("\" without valid argument."); // Invalid syntax format.
      return false;
    }
  }
  // Case default will also trigger an exception
  else
  {
    ErrorCode = wxT("Syntax Error: Invalid or Unknown Operator: \"") + RPNTokenStack.Item(i).GetValue() + wxT("\"."); // Invalid syntax format.
    return false;
  }
 }
 // Evaluate the remaining register stack members
    if (RegisterStack.GetCount() == 0) // Empty register stack
    {
     FunctionValue = 0;
     RegisterStack.Clear();
    }
    else if (RegisterStack.GetCount() == 1) // One value stored
    {
     // The function stack should be deleted
     RPNFunctionStack.Clear();
     if (RegisterStack.Item(RegisterStack.GetCount()-1).GetName() == wxT("_NULL_")) // Numerical tokens will be store the output variable
      {
       FunctionValue = RegisterStack.Item(RegisterStack.GetCount()-1).GetValue();
       RegisterStack.Clear();
      }
      else if(InternalFunctionVariables.Find(RegisterStack.Item(RegisterStack.GetCount()-1).GetName()))
      {
       FunctionValue = InternalFunctionVariables.FindGet(RegisterStack.Item(RegisterStack.GetCount()-1).GetName());
       RegisterStack.Clear();
      }
      else if(FunctionParameters.Find(RegisterStack.Item(RegisterStack.GetCount()-1).GetName()))
      {
        ErrorCode = wxT("Syntax Error: Cannot use parameter: \"") + RegisterStack.Item(RegisterStack.GetCount()-1).GetName() + wxT("\" outside definition.");
        return false;
      }
      else if(!(InternalFunctionVariables.Find(RegisterStack.Item(RegisterStack.GetCount()-1).GetName())))
      {
       ErrorCode = wxT("Declaration Error: Undefined Variable: \"") + RegisterStack.Item(RegisterStack.GetCount()-1).GetName() + wxT("\"");
       return false;
      }
      else
      {
       ErrorCode = ErrorCode = wxT("Syntax Error: Invalid numerical expression"); // Invalid expression
       return false;
      }
    }
    else
    {
      ErrorCode = wxT("Syntax Error: Invalid numerical expression"); // Invalid syntax format.
      return false;
    }
  return true; // Finish procedure
}

// A simpler postfix calculator to handle the function already defined, since the constants, variables are already defined in previous step
double AlgebraicFunction::FunctionCalculator()
{
 // Create a local register stack
 AlgebraicVariableVector FunctionStack;

 // Begin the RPN processing main cycle, which is similar to a computer processor
 for (int i=0; i<RPNFunctionStack.GetCount(); i++)
 {
    // A number token are converted to a double precision variable and pop to the register stack.
   if (RPNFunctionStack.Item(i).IsNumber())
   {
    double temp;
    RPNFunctionStack.Item(i).GetValue().ToDouble(&temp);
    FunctionStack.NewVariable(wxT("_NULL_"),temp);
   }
   // A variable are simply moved to the register stack with the same name, and a null value.
   else if (RPNFunctionStack.Item(i).IsVariable())
   {
    FunctionStack.NewConstant(RPNFunctionStack.Item(i).GetValue(),1.0);
   }
   // The assignment operator expect two arguments from the register stack, where the is the independent variable will assign the function value to the dependent variable
   else if (RPNFunctionStack.Item(i).GetValue() == wxT("="))
   {
     if(FunctionStack.GetCount()>1) // Requires at least two arguments
     {
        // Declare the temporary variables for easy management
        wxString tempvar;
        double temp;
        // Check the validity of the first argument, which can be a number or an already known variable
        if (FunctionStack.Item(FunctionStack.GetCount()-1).GetName() == wxT("_NULL_"))
        {
         temp = FunctionStack.Item(FunctionStack.GetCount()-1).GetValue(); // Get the variable value
        }
        else
        {
          if(InternalFunctionVariables.Find(FunctionStack.Item(FunctionStack.GetCount()-1).GetName())) // Verify if the variable are defined
          {
            temp = InternalFunctionVariables.FindGet(FunctionStack.Item(FunctionStack.GetCount()-1).GetName()); // Get the variable value
          }
          else
          {
            ErrorCode = wxT("Declaration Error: Undefined Variable: \"") + FunctionStack.Item(FunctionStack.GetCount()-1).GetName() + wxT("\"");
            return NAN;
          }
        }
        // The second argument is the new variable
        tempvar = FunctionStack.Item(FunctionStack.GetCount()-2).GetName(); // Get the variable name
        if (InternalFunctionVariables.Find(tempvar)) // check the validity of the variable name
        {
          InternalFunctionVariables.FindReplace(tempvar,temp); // Store the new variable
          FunctionStack.RemoveAt(FunctionStack.GetCount()-1); // Delete the two arguments from the register stack
          FunctionStack.RemoveAt(FunctionStack.GetCount()-1);
        }
        else
        {
           ErrorCode = wxT("Function Error:  Undefined Variable: \"") + FunctionStack.Item(FunctionStack.GetCount()-2).GetName() + wxT("\"");
           return NAN;
        }
     }
     else
     {
      ErrorCode = wxT("Function Error: Assignment operator \"=\" without valid arguments!"); // Invalid syntax format.
      return NAN;
     }
   }
   // Verify the evaluation of a function, while take care the potential numerical overflows using the STL library.
  else if (RPNFunctionStack.Item(i).IsFunction())
  {
    if (FunctionStack.GetCount() > 0)
    {
      if (FunctionStack.Item(FunctionStack.GetCount()-1).GetName() != wxT("_NULL_")) // If the argument are a variable
      {
        if (InternalFunctionVariables.Find(FunctionStack.Item(FunctionStack.GetCount()-1).GetName()))
        {
          double temp = InternalFunctionVariables.FindGet(FunctionStack.Item(FunctionStack.GetCount()-1).GetName());
          double f = this->EvaluateFunction(RPNFunctionStack.Item(i),temp);
          FunctionStack.RemoveAt(FunctionStack.GetCount()-1);
          FunctionStack.NewVariable(wxT("_NULL_"),f);
        }
        else
        {
          ErrorCode = wxT("Declaration Error: Undefined Variable: \"") + FunctionStack.Item(FunctionStack.GetCount()-1).GetName() + wxT("\"");
          return NAN;
        }
      }
      else // If the argument are a number
      {
       double f = this->EvaluateFunction(RPNFunctionStack.Item(i),FunctionStack.Item(FunctionStack.GetCount()-1).GetValue());
       FunctionStack.RemoveAt(FunctionStack.GetCount()-1);
       FunctionStack.NewVariable(wxT("_NULL_"),f);
      }
    }
    else
    {
      ErrorCode = wxT("Syntax Error: Function \"") + RPNFunctionStack.Item(i).GetValue() + wxT("\" without valid argument."); // Invalid syntax format.
      return NAN;
    }
  }
 // Evaluate an arithmetic operator, and use the STL library to handle invalid numerical results
  else if (RPNFunctionStack.Item(i).IsOperator() && RPNFunctionStack.Item(i).GetPriority() > 0)
  {
    if (FunctionStack.GetCount() > 1)
    {
      double temp1,temp2;
      wxString tempvar1,tempvar2;
      // First argument
      if (FunctionStack.Item(FunctionStack.GetCount()-1).GetName() != wxT("_NULL_"))
      {
       tempvar1 = FunctionStack.Item(FunctionStack.GetCount()-1).GetName();
       if (InternalFunctionVariables.Find(FunctionStack.Item(FunctionStack.GetCount()-1).GetName()))
        {
          temp1 = InternalFunctionVariables.FindGet(FunctionStack.Item(FunctionStack.GetCount()-1).GetName());
        }
        else
        {
          ErrorCode = wxT("Declaration Error: Undefined Variable: \"") + FunctionStack.Item(FunctionStack.GetCount()-1).GetName() + wxT("\"");
          return NAN;
        }
      }
      else
      {
        temp1 = FunctionStack.Item(FunctionStack.GetCount()-1).GetValue();
      }
      // Second argument
      if (FunctionStack.Item(FunctionStack.GetCount()-2).GetName() != wxT("_NULL_"))
      {
       tempvar2 = FunctionStack.Item(FunctionStack.GetCount()-2).GetName();
       if (InternalFunctionVariables.Find(FunctionStack.Item(FunctionStack.GetCount()-2).GetName()))
        {
          temp2 = InternalFunctionVariables.FindGet(FunctionStack.Item(FunctionStack.GetCount()-2).GetName());
        }
        else
        {
          ErrorCode = wxT("Declaration Error: Undefined Variable: \"") + FunctionStack.Item(FunctionStack.GetCount()-2).GetName() + wxT("\"");
          return NAN;
        }
      }
      else
      {
        temp2 = FunctionStack.Item(FunctionStack.GetCount()-2).GetValue();
      }
      // Evaluate the result
      double f = this->EvaluateOperator(RPNFunctionStack.Item(i),temp1,temp2);
      FunctionStack.RemoveAt(FunctionStack.GetCount()-1);
      FunctionStack.RemoveAt(FunctionStack.GetCount()-1);
      FunctionStack.NewVariable(wxT("_NULL_"),f);
    }
    else
    {
      ErrorCode = wxT("Syntax Error: Arithmetic Operator \"") + RPNFunctionStack.Item(i).GetValue() + wxT("\" without valid argument."); // Invalid syntax format.
      return NAN;
    }
  }
  // Case default will also trigger an exception
  else
  {
    ErrorCode = wxT("Syntax Error: Invalid or Unknown Operator: \"") + RPNFunctionStack.Item(i).GetValue() + wxT("\"."); // Invalid syntax format.
    return NAN;
  }
 }
 // Get the function dependent variable and return their value.
 if(FunctionStack.GetCount() == 0)
 {
  if(FunctionParameters.Find(wxT("fyvar")))
  {
   wxString FunctionImage = FunctionParameters.FindGet(wxT("fyvar"));
   if(InternalFunctionVariables.Find(FunctionImage))
   {
    return InternalFunctionVariables.FindGet(FunctionImage); // return the function value
   }
   else
   {
    ErrorCode = wxT("Function Error: Undefined Function Variable: \"") + FunctionImage + wxT("\".");
    return NAN;
   }
  }
  else
  {
    ErrorCode = wxT("Function Error: Incorrect Function Declaration.");
    return NAN;
  }
 }
 else
 {
    ErrorCode = wxT("Syntax Error: Invalid numerical expression"); // Invalid syntax format.
    return NAN;
 }
}

// Obtain from the function itself the array of variables that are only constants
AlgebraicVariableVector AlgebraicFunction::GetOnlyConstants()
{
 AlgebraicVariableVector OnlyConstants;
 for (int i=0;i<InternalFunctionVariables.GetCount();i++)
 {
  if(InternalFunctionVariables.Item(i).IsConstant())
   OnlyConstants.Add(InternalFunctionVariables.Item(i));
 }
 return OnlyConstants;
}

// Obtain from the function itself the array of variables that are only variables
AlgebraicVariableVector AlgebraicFunction::GetOnlyVariables()
{
 AlgebraicVariableVector OnlyVariables;
 for (int i=0;i<InternalFunctionVariables.GetCount();i++)
 {
  if(InternalFunctionVariables.Item(i).IsVariable())
   OnlyVariables.Add(InternalFunctionVariables.Item(i));
 }
 return OnlyVariables;
}

// Obtain from the function itself the variables, excluding the reserved for the function itself
AlgebraicVariableVector AlgebraicFunction::GetNonFVariables()
{
 AlgebraicVariableVector NonFVariables;
 wxString fx = FunctionParameters.FindGet(wxT("fxvar"));
 wxString fy = FunctionParameters.FindGet(wxT("fyvar"));
 for (int i=0;i<InternalFunctionVariables.GetCount();i++)
 {
  if(InternalFunctionVariables.Item(i).IsVariable())
  {
    if((InternalFunctionVariables.Item(i).GetName() != fx) && (InternalFunctionVariables.Item(i).GetName() != fy))
        NonFVariables.Add(InternalFunctionVariables.Item(i));
  }
 }
 return NonFVariables;
}


// A quick check of function consistency: The main variable functions should be declared, and no compilations errors of the internal function should exist.
bool AlgebraicFunction::IsFunctionDefined()
{
 if(FunctionParameters.Find(wxT("fxvar")) && FunctionParameters.Find(wxT("fyvar")))
 {
  wxString fx = FunctionParameters.FindGet(wxT("fxvar"));
  wxString fy = FunctionParameters.FindGet(wxT("fyvar"));
  if(InternalFunctionVariables.Find(fx) && InternalFunctionVariables.Find(fy) && fx.Len()>0 && fy.Len()>0)
  {
    return true;
  }
  else
  {
    if(ErrorCode.Len() == 0)
     ErrorCode = wxT("Function Error: The Function Variables are not Defined!");
    return false;
  }
 }
 else
 {
  return false;
 }
}

// A quick check of function consistency: The main variable functions should be declared, and no compilations errors of the internal function should exist.
bool AlgebraicFunction::IsFunctionWithVectorDefined()
{
 if(FunctionParameters.Find(wxT("fxvar")) && FunctionParameters.Find(wxT("fyvar")) && FunctionParameters.Find(wxT("fnvar")))
 {
  wxString fx = FunctionParameters.FindGet(wxT("fxvar"));
  wxString fy = FunctionParameters.FindGet(wxT("fyvar"));
  wxString fn = FunctionParameters.FindGet(wxT("fnvar"));
  if(InternalFunctionVariables.Find(fx) && InternalFunctionVariables.Find(fy) && InternalFunctionVariables.Find(fn) && fx.Len()>0 && fy.Len()>0 && fn.Len()>0)
  {
    return true;
  }
  else
  {
    if(ErrorCode.Len() == 0)
     ErrorCode = wxT("Function Error: The Function Variables are not Defined!");
    return false;
  }
 }
 else
 {
  return false;
 }
}

// Check the function consistency with a limited domain
bool AlgebraicFunction::IsDomainDefined()
{
 if(this->IsFunctionDefined())
 {
  if(FunctionParameters.Find(wxT("fxmin")) && FunctionParameters.Find(wxT("fxmax")))
  {
  wxString fx = FunctionParameters.FindGet(wxT("fxmin"));
  wxString fy = FunctionParameters.FindGet(wxT("fxmax"));
  if(InternalFunctionVariables.Find(fx) && InternalFunctionVariables.Find(fy) && fx.Len()>0 && fy.Len()>0)
   {
    return true;
   }
   else
   {
    if(ErrorCode.Len() == 0)
     ErrorCode = wxT("Function Error: The Function Should have a Limited Domain!");
    return false;
   }
  }
 else
  {
  return false;
  }
 }
 else
 {
  return false;
 }
}

// Get the minimum and maximum of the function, if the user defined a limited domain, otherwise, returns NAN.
double AlgebraicFunction::GetFunctionMinimum()
{
 if(FunctionParameters.Find(wxT("fxmin")))
 {
  wxString fx = FunctionParameters.FindGet(wxT("fxmin"));
  if(InternalFunctionVariables.Find(fx) && fx.Len()>0)
  {
    return InternalFunctionVariables.FindGet(fx);
  }
  else
  {
    return NAN;
  }
 }
 else
 {
  return NAN;
 }
}

double AlgebraicFunction::GetFunctionMaximum()
{
 if(FunctionParameters.Find(wxT("fxmax")))
 {
  wxString fx = FunctionParameters.FindGet(wxT("fxmax"));
  if(InternalFunctionVariables.Find(fx) && fx.Len()>0)
  {
    return InternalFunctionVariables.FindGet(fx);
  }
  else
  {
    return NAN;
  }
 }
 else
 {
  return NAN;
 }
}

// Evaluate a value of the function, while check the presence of the independent and dependent function variable. Will return NAN in case of error.
double AlgebraicFunction::GetFyxEval(double argument)
{
 if(this->IsFunctionDefined())
 {
  wxString fx = FunctionParameters.FindGet(wxT("fxvar"));
  wxString fy = FunctionParameters.FindGet(wxT("fyvar"));
  wxString fm = FunctionParameters.FindGet(wxT("fxmax"));
  wxString fn = FunctionParameters.FindGet(wxT("fxmin"));
  InternalFunctionVariables.FindReplace(fx,argument);
  double result = this->FunctionCalculator();
  InternalFunctionVariables.FindReplace(fy,result);
  if(fm.Len() == 0 && fn.Len() == 0) // No restricted domain variables
  {
   return result;
  }
  else // Get the restricted domain values
  {
   double xleft,xright;
   xleft = InternalFunctionVariables.FindGet(fn);
   xright = InternalFunctionVariables.FindGet(fm);
   if((argument>=xleft)&&(argument<=xright))
    return result;
   else
    return 0.0;
  }
 }
 else
 {
  ErrorCode = wxT("Function Error: Incorrect Function Declaration.");
  return NAN;
 }
}

// Replace any variable stored on the function. Constants cannot be overwritten.
bool AlgebraicFunction::FindReplaceVariableValue(wxString var, double value)
{
  return InternalFunctionVariables.FindReplace(var,value);
}

// Designed to allocate a pseudo-vector of several elements, where the string defined by "fnvar" are the initial term, and <n>1,<n>2... are the next ones
bool AlgebraicFunction::SetFunctionNumberParameter(std::vector<double> value)
{
 if(this->IsFunctionDefined())
 {
   if(FunctionParameters.Find(wxT("fnvar")) && value.size()>0)
   {
    wxString fn = FunctionParameters.FindGet(wxT("fnvar"));
    InternalFunctionVariables.FindReplace(fn,value.at(0));
    if(value.size()>1)
    {
     for(int i=1; i<value.size(); i++)
     {
       wxString fv = fn + wxString::Format("%d",i);
       InternalFunctionVariables.FindReplace(fv,value.at(i));
     }
    }
    return true;
   }
   else
   {
   ErrorCode = wxT("Function Error: Undefined Vector Name");
   return false;
   }
 }
 else
 {
   return false;
 }
}

// Main Function Constructor, which simply guides the three main algebraic procedures to obtain a numeric value from the user input
AlgebraicFunction::AlgebraicFunction(wxString OriginalExpression)
{
 InternalFunctionString = OriginalExpression;
 if(this->AlgebraicParser())
 {
  if(this->AlgebraicCompiler())
  {
   if(this->AlgebraicCalculator())
   {
     ErrorCode.Clear();
   }
  }
 }
}



// Line tokenizer constructor
TextLineParser::TextLineParser(wxString InputLine, wxString TokenSymbol)
{
 wxString cache;
 TokenizedLine.Clear();
 ReducedLine.Clear();
 for (int i=0; i<InputLine.Len(); i++)
 {
   wxString character = InputLine.GetChar(i);
   // empty spaces are the mandatory tokenization separator
   if(character == wxT(" ") || character == wxT("\t") || character == wxT("\n"))
   {
     if(cache.Len()>0)
     {
       TokenizedLine.Add(cache);
       ReducedLine.Add(cache);
       cache.Clear();
     }
   }
   else if(character == TokenSymbol) // If the character are the token symbol, then the temporary cache are stored as a token, and the symbol too.
   {
     if(cache.Len()>0)
     {
       TokenizedLine.Add(cache);
       TokenizedLine.Add(TokenSymbol);
       ReducedLine.Add(cache);
       cache.Clear();
     }
     else
     {
       TokenizedLine.Add(TokenSymbol);
     }
   }
   else // Add the current character to the temporary cache.
   {
     cache = cache + character;
   }
 }
 // Store the remaining temporary cache as the last token
  if(cache.Len()>0)
     {
       TokenizedLine.Add(cache);
       ReducedLine.Add(cache);
       cache.Clear();
     }
}

// Alternative line tokenizer constructor
TextLineParser::TextLineParser(wxString InputLine, wxArrayString TokenSymbol)
{
 wxString cache;
 TokenizedLine.Clear();
 ReducedLine.Clear();
 wxString SymbolList;
 for (int j=0; j<TokenSymbol.GetCount(); j++)
 {
   SymbolList = SymbolList + TokenSymbol.Item(j);
 }
 for (int i=0; i<InputLine.Len(); i++)
 {
   wxString character = InputLine.GetChar(i);
   // empty spaces are the mandatory tokenization separator
   if(character == wxT(" ") || character == wxT("\t") || character == wxT("\n"))
   {
     if(cache.Len()>0)
     {
       TokenizedLine.Add(cache);
       ReducedLine.Add(cache);
       cache.Clear();
     }
   }
   else if(SymbolList.Find(character) != wxNOT_FOUND) // If the character are the token symbol, then the temporary cache are stored as a token, and the symbol too.
   {
     if(cache.Len()>0)
     {
       TokenizedLine.Add(cache);
       TokenizedLine.Add(character);
       ReducedLine.Add(cache);
       cache.Clear();
     }
     else
     {
       TokenizedLine.Add(character);
     }
   }
   else // Add the current character to the temporary cache.
   {
     cache = cache + character;
   }
 }
 // Store the remaining temporary cache as the last token
  if(cache.Len()>0)
     {
       TokenizedLine.Add(cache);
       ReducedLine.Add(cache);
       cache.Clear();
     }
}

