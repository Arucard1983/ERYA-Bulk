/***************************************************************
 * Name:      ParserLibrary.h
 * Purpose:   Algebraic and Text parser library functions
 * Author:    Vasco Manteigas (vm.manteigas@campus.fct.utl.pt)
 * Created:   2016-10-01
 * Copyright: Grupo de Reacoes Nucleares - LIBPHYS
 * License:
 **************************************************************/

 #ifndef PARSERLIBRARY_H_INCLUDED
 #define PARSERLIBRARY_H_INCLUDED
 #include "ERYAPIXE.h"
 #include "LinearAlgebra.h"
 #include <cmath>

class AlgebraicVariable;
class Token;
class Parameter;
WX_DECLARE_OBJARRAY( AlgebraicVariable, AlgebraicVariableArray);
WX_DECLARE_OBJARRAY( Token, TokenArray);
WX_DECLARE_OBJARRAY( Parameter, ParameterArray);

/***************************************************************
 * Define a dual variable/content in a single object, to handle
 * numerical variables
 * An aditional boolean flag enables to set a variable(true),
 * or a fixed constant (false)
 **************************************************************/
class AlgebraicVariable
{
 private:
 wxString VariableName;
 double VariableValue;
 bool VariableFlag;
 public:
 AlgebraicVariable(wxString name, double value){ VariableName=name; VariableValue=value; VariableFlag=true; };
 AlgebraicVariable(wxString name, double value, bool flag){ VariableName=name; VariableValue=value; VariableFlag=flag; };
 bool IsVariable(){return VariableFlag;};
 bool IsConstant(){return !(VariableFlag);};
 wxString GetName(){return VariableName;};
 double GetValue(){return VariableValue;};
 double SetValue(double newvalue){ VariableValue = newvalue; return VariableValue;};
};

class AlgebraicVariableVector : public AlgebraicVariableArray
{
 private:
 int FindVariableAdress(wxString varname);
 public:
 AlgebraicVariableVector(){};
 bool Find(wxString name);
 double FindGet(wxString name);
 bool FindReplace(wxString name, double newvalue);
 bool NewVariable(wxString name, double value){AlgebraicVariable temp(name,value,true); this->Add(temp); return true;};
 bool NewConstant(wxString name, double value){AlgebraicVariable temp(name,value,false); this->Add(temp); return true;};
};

/***************************************************************
 * Manages the tokens generated during tokenization.
 * Some basic syntax and control rules are also implemented
 * The associativity are defined as left=true, and right=false...
 * Since the default operator associativity are left.
 **************************************************************/
class Token
{
 private:
 wxString TokenElement;
 int Priority;
 bool Associativity;
 bool Number;
 bool Variable;
 bool Function;
 bool Operator;
 bool ReservedWord;
 public:
 Token(wxString element){ TokenElement=element; };
 wxString GetValue(){ return TokenElement; };
 bool SetAllFlags(int p, bool a, bool n, bool v, bool f, bool o, bool w){ Priority=p; Associativity=a; Number=n; Variable=v; Function=f; Operator=o; ReservedWord=w; return true;};
 int  GetPriority(){ return Priority; };
 bool GetAssociativity(){ return Associativity; };
 bool IsNumber(){ return Number; };
 bool IsVariable(){ return Variable; };
 bool IsFunction(){ return Function; };
 bool IsOperator(){ return Operator; };
 bool IsParameter(){ return ReservedWord; };
 bool CheckIsValidNumber();
 bool CheckIsValidVariable();
 bool CheckIsValidSymbol();
};

class TokenVector : public TokenArray
{
 private:
 bool ClassifyAndCheck();
 bool CheckIsValidNumber(int position);
 bool CheckIsValidVariable(int position);
 bool CheckIsValidSymbol(int position);
 public:
 TokenVector(){};
 bool NewToken(wxString t){ this->Add(Token(t)); return ClassifyAndCheck(); };
};

/***************************************************************
 * Manages special variables that only had non-numerical values
 * They are called parameters, and handles some internal settings
 **************************************************************/
class Parameter
{
 private:
 wxString ParameterName;
 wxString ParameterValue;
 public:
 Parameter(wxString name, wxString value){ ParameterName=name; ParameterValue=value; };
 wxString GetName(){ return ParameterName; };
 wxString GetValue(){ return ParameterValue; };
 wxString SetValue(wxString newvalue){ ParameterValue = newvalue; return ParameterValue;};
};

class ParameterVector : public ParameterArray
{
 private:
 int FindParameterAdress(wxString varname);
 public:
 ParameterVector(){};
 bool Find(wxString name);
 wxString FindGet(wxString name);
 bool FindReplace(wxString name, wxString newvalue);
 bool NewParameter(wxString name, wxString value){Parameter temp(name,value); this->Add(temp); return true;};
};


/***************************************************************
 * Handles the evaluation of an algebraic function
 **************************************************************/
class AlgebraicFunction
{
 private:
 TokenVector InternalFunctionTokens;
 TokenVector RPNTokenStack;
 TokenVector RPNFunctionStack;
 AlgebraicVariableVector InternalFunctionVariables;
 ParameterVector FunctionParameters;
 wxString InternalFunctionString;
 wxString ErrorCode;
 double FunctionValue;
 bool AlgebraicParser();
 bool AlgebraicCompiler();
 bool AlgebraicCalculator();
 double FunctionCalculator();
 double EvaluateFunction(Token f, double arg);
 double EvaluateOperator(Token o, double arg1, double arg2);
 public:
 AlgebraicFunction(){};
 AlgebraicFunction(wxString OriginalExpression);
 wxString GetErrorString(){ return ErrorCode; };
 double GetAnsEval(){return FunctionValue;};
 double GetFyxEval(double argument);
 bool IsFunctionDefined();
 bool IsFunctionWithVectorDefined();
 bool FindReplaceVariableValue(wxString var, double value);
 bool SetFunctionNumberParameter(std::vector<double> value);
 AlgebraicVariableVector GetOnlyConstants();
 AlgebraicVariableVector GetOnlyVariables();
 AlgebraicVariableVector GetNonFVariables();
};


/***************************************************************
 * Generic constructor for a text line parser
 * The constructor can accept a list of token selectors,
 * which as reserved symbols that break the line.
 * Two list are created: the first contains the original line,
 * and the second are like the first one, but without the
 * selectors list.
 **************************************************************/
 class TextLineParser
 {
  private:
  wxArrayString TokenizedLine;
  wxArrayString ReducedLine;
  public:
  TextLineParser(wxString InputLine, wxString TokenSymbol);
  TextLineParser(wxString InputLine, wxArrayString TokenSymbol);
  wxArrayString GetAllTokens(){ return TokenizedLine;};
  wxArrayString GetUnexcluded(){ return ReducedLine;};
 };

 #endif // PARSERLIBRARY_H_INCLUDED
