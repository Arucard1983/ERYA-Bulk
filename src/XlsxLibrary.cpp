/***************************************************************
 * Name:      XlsxLibrary.cpp
 * Purpose:   Simple Xlsx (Excel 2007/2010) Writer/Reader port to wxWidgets
 * Author:    Vasco Manteigas (vm.manteigas@campus.fct.utl.pt)
 * Created:   2017-03-01
 * Copyright: Grupo de Reacoes Nucleares - LIBPHYS
 * License:
 **************************************************************/

#include "XlsxLibrary.h"
#include <wx/arrimpl.cpp>
WX_DEFINE_OBJARRAY( TableNodeArray);

// Table nodes default constructors
TableNode::TableNode(wxString InputElement)
{
 if(InputElement.Len()>0)
 {
   TableElement = InputElement;
   TableType = 0;
 }
 else
 {
   TableElement = wxEmptyString;
   TableType = 3;
 }
}

// For number type, check the case when exists a float zero, were Excel likes to convert to a string!!!
TableNode::TableNode(wxString InputElement, int Type)
{
 if(InputElement.Len()>0)
 {
   TableElement = InputElement;
   TableType = Type;
   if(Type == 1)
   {
    double test;
    if(InputElement.ToDouble(&test))
    {
     if(test == 0.0)
     {
       TableElement = wxT("0");
       TableType = Type;
     }
    }
    else
    {
     TableElement = wxT("0");
     TableType = Type;
    }
   }
 }
 else
 {
   TableElement = wxEmptyString;
   TableType = 3;
 }
}

// Allocate a matrix with r*c filled with default values
void TableMatrix::SetDimension(int Rows, int Columns)
{
 for(int i=0; i<Rows; i++)
 {
    for(int j=0; j<Columns; j++)
    {
      this->Add(TableNode(wxEmptyString));
    }
 }
 ExpectedNumberRows = Rows;
 ExpectedNumberColumns = Columns;
 return;
}

// Get the matrix real position from the two-dimension grid
int TableMatrix::GetNodePosition(int Row, int Column)
{
  if((ExpectedNumberRows>0) && (ExpectedNumberColumns>0))
  {
    int RealAdress = Row * ExpectedNumberColumns + Column;
    return RealAdress;
  }
  else
  {
    return -1;
  }
}

// Return the matrix Elements that are strings, which is needed to build the sharedstrings.xml file
wxArrayString TableMatrix::GetAllStrings()
{
 wxArrayString AllLabels;
 for(int i=0; i<this->GetCount(); i++)
 {
  if(this->Item(i).GetType() == 0)
  {
    bool FoundString = false;
    for(int j=0; j<AllLabels.GetCount(); j++)
    {
      if( AllLabels.Item(j) == this->Item(i).GetElement())
          FoundString = true;
    }
    if(!FoundString)
      AllLabels.Add(this->Item(i).GetElement());
  }
 }
 return AllLabels;
}

// Get the value from the matrix
wxString TableMatrix::GetTableMatrixValueAt(int Row, int Column, int& Type)
{
  int MatrixAdress = this->GetNodePosition(Row,Column);
  if((MatrixAdress < 0) || (MatrixAdress > this->GetCount()))
  {
    return wxEmptyString;
  }
  else
  {
    Type = this->Item(MatrixAdress).GetType();
    return this->Item(MatrixAdress).GetElement();
  }
}

// Edit a matrix value
bool TableMatrix::SetTableMatrixValueAt(int Row, int Column, wxString NewData, int Type)
{
  int MatrixAdress = this->GetNodePosition(Row,Column);
  if((MatrixAdress < 0) || (MatrixAdress > this->GetCount()))
  {
    return false;
  }
  else
  {
    this->Item(MatrixAdress).SetElement(NewData,Type);
    return true;
  }
}

// Return the usefull acessible data size, which may not be equal to the declared size
void TableMatrix::GetRealMatrixSize(int& Row, int& Column)
{
  if ((ExpectedNumberColumns > 0) && (ExpectedNumberRows > 0))
  {
    int RealNumberRows = this->GetCount()/ExpectedNumberColumns;
    Row = RealNumberRows;
    if (Row > 0)
     Column = ExpectedNumberColumns;
    else
     Column = 0;
    return;
  }
  else
  {
    Row = 0;
    Column = 0;
    return;
  }
}

// Base-26 decoder
wxString XlsxFile::GetLetter(int Value)
{
  if((Value < 0) || (Value > 25))
  {
    return wxEmptyString;
  }
  else
  {
    if(Value == 0)
     return wxT("A");
    else if(Value == 1)
     return wxT("B");
    else if(Value == 2)
     return wxT("C");
    else if(Value == 3)
     return wxT("D");
    else if(Value == 4)
     return wxT("E");
    else if(Value == 5)
     return wxT("F");
    else if(Value == 6)
     return wxT("G");
    else if(Value == 7)
     return wxT("H");
    else if(Value == 8)
     return wxT("I");
    else if(Value == 9)
     return wxT("J");
    else if(Value == 10)
     return wxT("K");
    else if(Value == 11)
     return wxT("L");
    else if(Value == 12)
     return wxT("M");
    else if(Value == 13)
     return wxT("N");
    else if(Value == 14)
     return wxT("O");
    else if(Value == 15)
     return wxT("P");
    else if(Value == 16)
     return wxT("Q");
    else if(Value == 17)
     return wxT("R");
    else if(Value == 18)
     return wxT("S");
    else if(Value == 19)
     return wxT("T");
    else if(Value == 20)
     return wxT("U");
    else if(Value == 21)
     return wxT("V");
    else if(Value == 22)
     return wxT("W");
    else if(Value == 23)
     return wxT("X");
    else if(Value == 24)
     return wxT("Y");
    else if(Value == 25)
     return wxT("Z");
    else
     return wxEmptyString;
  }
}


// Xlsx Adress to Cell Translator
wxString XlsxFile::GetXlsxAdress(int Row, int Column)
{
  // Columns are translated to a base-26 letter based number
  wxString InverseNumber,FinalNumber;
  int Number = Column;
  do
  {
    int IntNumber = Number / 26;
    int RemNumber = Number % 26;
    InverseNumber = InverseNumber + this->GetLetter(RemNumber);
    Number = IntNumber;
  }while(Number > 0);
  // Reverse the string
  for(int i=0; i<InverseNumber.Len(); i++)
  {
    wxString temp = InverseNumber.GetChar(InverseNumber.Len() - i - 1);
    FinalNumber = FinalNumber + temp;
  }
  // Rows are trivial
  wxString RowNumber = wxString::Format("%i", Row + 1);
  // Return the Cell reference
  wxString CellReference = FinalNumber + RowNumber;
  return CellReference;
}

// Get the Xlsx cell adress from a given matrix position
bool XlsxFile::SetXlsxAdress(int& Row, int& Column, wxString CellAdress)
{
  // Is expected a serie of letters, then a serie of numbers. A mixed situation means an invalid cell reference
   wxString Letters = wxT("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
   wxString Numbers = wxT("0123456789");
   wxString character;
   int ColumnAdress = 0;
   int RowAdress = 0;
   Row = 0;
   Column = 0;
   bool FoundNumber = false;
   for(int i=0; i<CellAdress.Len(); i++)
   {
     character = CellAdress.GetChar(i);
     int test1 = Letters.Find(character);
     int test2 = Numbers.Find(character);
     if ((i==0) && (test1 == wxNOT_FOUND) ) // First character should be a letter!
     {
       return false;
     }
     else if ((test1 == wxNOT_FOUND) && (test2 == wxNOT_FOUND) ) // Invalid characters means an error.
     {
       return false;
     }
     else if ((test1 != wxNOT_FOUND) && (test2 == wxNOT_FOUND) && !FoundNumber ) // Letters should be in first place
     {
       ColumnAdress = ColumnAdress*26+test1+1;
     }
     else if ((test1 == wxNOT_FOUND) && (test2 != wxNOT_FOUND) ) // Numbers should be in second place, and also trigger the flag
     {
       FoundNumber = true;
       RowAdress = RowAdress*10+test2;
     }
     else // General failure error
     {
       return false;
     }
   }
   // Final result
   if ((ColumnAdress < 0 ) && (RowAdress < 0))
   {
     return false;
   }
   else
   {
     Row = RowAdress-1;
     Column = ColumnAdress-1;
     return true;
   }
}

// Xlsx file alternative constructor, where the table matrix and the initial cell adress is already defined
XlsxFile::XlsxFile(wxString LocalFileName, TableMatrix LocalSpreadsheet, wxString CellAdress)
{
 //Define the target filename target path
 XlsxFileName = LocalFileName;
 // Define the initial cell adress
 this->SetXlsxAdress(InitialRow,InitialColumn,CellAdress);
 // Store the initial table matrix;
 InternalTable = LocalSpreadsheet;
}


// Decode a domain range of a spreadsheet matrix, in the form of <Initial Cell>:<Final Cell>
bool XlsxFile::GetXlsxMatrixAdress(int& InitialRowValue, int& InitialColumnValue, int& RealNumberRows, int& RealNumberColumns, wxString CellRangeAdress)
{
 wxString InitialCell, FinalCell;
 int FoundSeparator = CellRangeAdress.Find(wxT(":"));
 if(FoundSeparator != wxNOT_FOUND)
 {
  InitialCell = CellRangeAdress.Mid(0,FoundSeparator);
  FinalCell = CellRangeAdress.Mid(FoundSeparator+1);
 }
 else
 {
  InitialCell = CellRangeAdress;
  FinalCell = CellRangeAdress;
 }
 int IR,IC,FR,FC;
 this->SetXlsxAdress(IR,IC,InitialCell);
 this->SetXlsxAdress(FR,FC,FinalCell);
 InitialRowValue = IR;
 InitialColumnValue = IC;
 RealNumberRows = FR-IR+1;
 RealNumberColumns = FC-IC+1;
 return true;
}

// Xlsx file writer
bool XlsxFile::WriteFile()
{
   // Build a minimal Xlsx file, and compress as a zip file
   wxFFileOutputStream XlsxArchive(XlsxFileName);
   wxZipOutputStream XlsxContents(XlsxArchive);
   wxTextOutputStream XlsxIndex(XlsxContents);
   wxTextOutputStream XlsxApp(XlsxContents);
   wxTextOutputStream XlsxCore(XlsxContents);
   wxTextOutputStream XlsxRels(XlsxContents);
   wxTextOutputStream XlsxWRels(XlsxContents);
   wxTextOutputStream XlsxStyles(XlsxContents);
   wxTextOutputStream XlsxWBook(XlsxContents);
   wxTextOutputStream XlsxShare(XlsxContents);
   wxTextOutputStream XlsxTable(XlsxContents);
   wxString separator = wxFileName::GetPathSeparator();
   XlsxContents.PutNextEntry(wxT("[Content_Types].xml"));
   XlsxIndex.WriteString(this->GetXlsxIndex());
   XlsxContents.PutNextEntry(wxT("_rels") + separator + wxT(".rels"));
   XlsxRels.WriteString(this->GetXlsxRels());
   XlsxContents.PutNextEntry(wxT("docProps") + separator + wxT("app.xml"));
   XlsxApp.WriteString(this->GetXlsxApp());
   XlsxContents.PutNextEntry(wxT("docProps") + separator + wxT("core.xml"));
   XlsxCore.WriteString(this->GetXlsxCore());
   XlsxContents.PutNextEntry(wxT("xl") + separator + wxT("_rels") + wxFileName::GetPathSeparator() + wxT("workbook.xml.rels"));
   XlsxWRels.WriteString(this->GetXlsxWRels());
   XlsxContents.PutNextEntry(wxT("xl") + separator + wxT("sharedStrings.xml"));
   XlsxShare.WriteString(this->CreateXlsxShared());
   XlsxContents.PutNextEntry(wxT("xl") + separator + wxT("styles.xml"));
   XlsxStyles.WriteString(this->GetXlsxStyles());
   XlsxContents.PutNextEntry(wxT("xl") + separator + wxT("workbook.xml"));
   XlsxWBook.WriteString(this->GetXlsxWBook());
   XlsxContents.PutNextEntry(wxT("xl") + separator + wxT("worksheets") + wxFileName::GetPathSeparator() + wxT("sheet1.xml"));
   XlsxTable.WriteString(CreateXlsxSheet());
   XlsxContents.Close();
   return true;
}

// Xlsx file loader
bool XlsxFile::LoadFile()
{
  // Read the xlsx file contents, which is a zip file, and select two of them.
  bool HaveStrings;
  wxFileSystem XlsxArchive;
  wxFSFile *XlsxSheetFile = XlsxArchive.OpenFile(wxFileSystem::FileNameToURL(XlsxFileName) + wxT("#zip:") + wxT("xl") + wxFileName::GetPathSeparator() + wxT("worksheets") + wxFileName::GetPathSeparator() + wxT("sheet1.xml"));
  wxFSFile *XlsxStringFile = XlsxArchive.OpenFile(wxFileSystem::FileNameToURL(XlsxFileName) + wxT("#zip:") + wxT("xl") + wxFileName::GetPathSeparator() + wxT("sharedStrings.xml"));
  wxXmlDocument XlsxSheet;
  wxXmlDocument XlsxString;
  if(XlsxSheetFile) // Load xl/workbooks/Sheet1.xml
  {
    wxInputStream *XlsxSheetStream = XlsxSheetFile->GetStream();
    XlsxSheet.Load(*XlsxSheetStream);
  }
  else
  {
    wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Cannot load file ") + XlsxFileName, wxT("Xlsx File Loading Error"), wxOK | wxICON_ERROR);
    dial->ShowModal();
    return false;
  }
  if(XlsxStringFile) // Load xl/sharedStrings.xml
  {
    wxInputStream *XlsxStringStream = XlsxStringFile->GetStream();
    XlsxString.Load(*XlsxStringStream);
    HaveStrings = true;
  }
  else
  {
    HaveStrings = false;
  }

  //First get the strings table
  wxArrayString SharedStrings;
  if(HaveStrings)
  {
  if(XlsxString.GetRoot()->GetName() != wxT("sst"))
  {
    wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Unexpected file format of ") + XlsxFileName, wxT("Xlsx File Loading Error"), wxOK | wxICON_ERROR);
    dial->ShowModal();
    return false;
  }
  wxXmlNode *StringNode = XlsxString.GetRoot()->GetChildren();
  while(StringNode)
  {
    wxString StringValue;
    if(StringNode->GetName() == wxT("si"))
    {
      wxXmlNode *ElementNode = StringNode->GetChildren();
      if(ElementNode->GetName() == wxT("t"))
      {
        StringValue = ElementNode->GetNodeContent(); //Get the desired value
      }
    }
   SharedStrings.Add(StringValue);
   StringNode = StringNode->GetNext(); //Pass to next element
  }
  }

  //Next is the sheet parsing, which may require the value of the string's array parsed before.
  wxString BlockDimension;
  int MatrixRows,MatrixColumns;
  if(XlsxSheet.GetRoot()->GetName() != wxT("worksheet"))
  {
    wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Unexpected file format of ") + XlsxFileName, wxT("Xlsx File Loading Error"), wxOK | wxICON_ERROR);
    dial->ShowModal();
    return false;
  }
  wxXmlNode *SheetDimension = XlsxSheet.GetRoot()->GetChildren();
  while(SheetDimension)
  {
    if(SheetDimension->GetName() == wxT("dimension"))
    {
      BlockDimension = SheetDimension->GetAttribute("ref","A1"); //Get the attribute a=b, while "b" is a default value
    }
   SheetDimension = SheetDimension->GetNext(); //Pass to next element
  }

  // Decode and allocate the matrix
  if(!(this->GetXlsxMatrixAdress(InitialRow, InitialColumn, MatrixRows, MatrixColumns, BlockDimension)))
   {
    wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Invalid spreadsheet data on file ") + XlsxFileName, wxT("Xlsx File Loading Error"), wxOK | wxICON_ERROR);
    dial->ShowModal();
    return false;
   }
  InternalTable.Clear();
  InternalTable.SetDimension(MatrixRows,MatrixColumns);

 // Read the source table itself
 wxXmlNode *SheetTable = XlsxSheet.GetRoot()->GetChildren();
  while(SheetTable)
  {
    if(SheetTable->GetName() == wxT("sheetData"))
    {
      wxXmlNode *SheetRow = SheetTable->GetChildren();
      while(SheetRow)
      {
       if(SheetRow->GetName() == wxT("row")) // Read a row
       {
          wxXmlNode *SheetColumn = SheetRow->GetChildren();
          while(SheetColumn)
          {
            if(SheetColumn->GetName() == wxT("c")) // Read a column
            {
             // Extract the desired and supported data
             wxString SheetCellValue, SheetCellAdress,SheetCellType;
             SheetCellAdress = SheetColumn->GetAttribute(wxT("r"),wxT("A1")); //Get cell adress
             SheetCellType = SheetColumn->GetAttribute(wxT("t"),wxT("n")); // Get cell type
             wxXmlNode *SheetValue = SheetColumn->GetChildren();
             // scan for the correct line
             while(SheetValue)
             {
              if(SheetValue->GetName() == wxT("v"))
              {
               SheetCellValue = SheetValue->GetNodeContent(); //Get the desired value
              }
              SheetValue = SheetValue->GetNext();
             }
             // Convert the results
             int RelativeRow, RelativeColumn, RelativeType;
             this->SetXlsxAdress(RelativeRow,RelativeColumn,SheetCellAdress);
             if(SheetCellType == wxT("s")) // String type
             {
               long adress;
               SheetCellValue.ToLong(&adress);
               if(adress > SharedStrings.GetCount())
               {
                wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Invalid spreadsheet data on file ") + XlsxFileName, wxT("Xlsx File Loading Error"), wxOK | wxICON_ERROR);
                dial->ShowModal();
                return false;
               }
               else
               {
                RelativeType = 0;
                SheetCellValue = SharedStrings.Item(adress);
               }
             }
             else if(SheetCellType == wxT("n")) // Number type
             {
               RelativeType = 1;
             }
             else if(SheetCellType == wxT("b")) // Boolean type
             {
               RelativeType = 2;
               if(SheetCellValue == wxT("0"))
                SheetCellValue = wxT("false");
               else
                SheetCellValue = wxT("true");
             }
             else //Unsupported type, reset to null
             {
               RelativeType = 0;
               SheetCellValue = wxT("0");
             }
             // And finally, store the value on export matrix
             InternalTable.SetTableMatrixValueAt(RelativeRow-InitialRow,RelativeColumn-InitialColumn,SheetCellValue,RelativeType);
            }
           SheetColumn = SheetColumn->GetNext();
          }
       }
       SheetRow = SheetRow->GetNext();
      }
    }
   SheetTable = SheetTable->GetNext(); //Pass to next element
  }
  // Return the results
  return true;
}

// Get the actual time:
wxString XlsxFile::GetLocalTime()
{
 wxString LocalTime;
 wxDateTime ThisTime = wxDateTime::Now();
 LocalTime = ThisTime.Format(wxT("%Y-%m-%dT%H:%M:%S.00Z"),wxDateTime::WET);
 return LocalTime;
}

// Some hardcoded Xml files just to avoid reading problems when opened on Microsoft Office or LibreOffice, and thus are simply strings.
// Some default values are exported from a sample xlsx file converted by LibreOffice, or by some third-party reference material while make some adaptations.

// [Content_Types].xml
wxString XlsxFile::GetXlsxIndex()
{
 // Notice the nodes should be declared backwards, but the attributes not!
 wxXmlDocument LocalXlsxIndex;
 wxXmlNode* index = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, "Types");
 LocalXlsxIndex.SetRoot(index);
 index->AddAttribute(wxT("xmlns"),wxT("http://schemas.openxmlformats.org/package/2006/content-types"));
 wxXmlNode* core = new wxXmlNode(index, wxXML_ELEMENT_NODE, "Override");
 core->AddAttribute(wxT("PartName"),wxT("/docProps/core.xml"));
 core->AddAttribute(wxT("ContentType"),wxT("application/vnd.openxmlformats-package.core-properties+xml"));
 wxXmlNode* share = new wxXmlNode(index, wxXML_ELEMENT_NODE, "Override");
 share->AddAttribute(wxT("PartName"),wxT("/xl/sharedStrings.xml"));
 share->AddAttribute(wxT("ContentType"),wxT("application/vnd.openxmlformats-officedocument.spreadsheetml.sharedStrings+xml"));
 wxXmlNode* sheet = new wxXmlNode(index, wxXML_ELEMENT_NODE, "Override");
 sheet->AddAttribute(wxT("PartName"),wxT("/xl/worksheets/sheet1.xml"));
 sheet->AddAttribute(wxT("ContentType"),wxT("application/vnd.openxmlformats-officedocument.spreadsheetml.worksheet+xml"));
 wxXmlNode* app = new wxXmlNode(index, wxXML_ELEMENT_NODE, "Override");
 app->AddAttribute(wxT("PartName"),wxT("/docProps/app.xml"));
 app->AddAttribute(wxT("ContentType"),wxT("application/vnd.openxmlformats-officedocument.extended-properties+xml"));
 wxXmlNode* workbook = new wxXmlNode(index, wxXML_ELEMENT_NODE, "Override");
 workbook->AddAttribute(wxT("PartName"),wxT("/xl/workbook.xml"));
 workbook->AddAttribute(wxT("ContentType"),wxT("application/vnd.openxmlformats-officedocument.spreadsheetml.sheet.main+xml"));
 wxXmlNode* ext = new wxXmlNode(index, wxXML_ELEMENT_NODE, "Default");
 ext->AddAttribute(wxT("Extension"),wxT("xml"));
 ext->AddAttribute(wxT("ContentType"),wxT("application/xml"));
 wxXmlNode* rel = new wxXmlNode(index, wxXML_ELEMENT_NODE, "Default");
 rel->AddAttribute(wxT("Extension"),wxT("rels"));
 rel->AddAttribute(wxT("ContentType"),wxT("application/vnd.openxmlformats-package.relationships+xml"));
 wxXmlNode* styles = new wxXmlNode(index, wxXML_ELEMENT_NODE, "Override");
 styles->AddAttribute(wxT("PartName"),wxT("/xl/styles.xml"));
 styles->AddAttribute(wxT("ContentType"),wxT("application/vnd.openxmlformats-officedocument.spreadsheetml.styles+xml"));
 //Export the file as a wxString
 wxStringOutputStream IndexStream;
 LocalXlsxIndex.Save(IndexStream);
 return IndexStream.GetString();
}

// docProps/app.xml
wxString XlsxFile::GetXlsxApp()
{
 // Notice the nodes should be declared backwards, but the attributes not!
 wxXmlDocument LocalXlsxApp;
 wxXmlNode* app = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, "Properties");
 LocalXlsxApp.SetRoot(app);
 app->AddAttribute(wxT("xmlns"),wxT("http://schemas.openxmlformats.org/officeDocument/2006/extended-properties"));
 app->AddAttribute(wxT("xmlns:vt"),wxT("http://schemas.openxmlformats.org/officeDocument/2006/docPropsVTypes"));
 wxXmlNode* version = new wxXmlNode(app, wxXML_ELEMENT_NODE, "AppVersion");
 version->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, wxT("4.60")));
 wxXmlNode* links = new wxXmlNode(app, wxXML_ELEMENT_NODE, "HyperlinksChanged");
 links->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, wxT("false")));
 wxXmlNode* doc = new wxXmlNode(app, wxXML_ELEMENT_NODE, "SharedDoc");
 doc->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, wxT("false")));
 wxXmlNode* update = new wxXmlNode(app, wxXML_ELEMENT_NODE, "LinksUpToDate");
 update->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, wxT("false")));
 wxXmlNode* title = new wxXmlNode(app, wxXML_ELEMENT_NODE, "TitlesOfParts");
 wxXmlNode* vt = new wxXmlNode(title, wxXML_ELEMENT_NODE, "vt:vector");
 vt->AddAttribute(wxT("size"),wxT("1"));
 vt->AddAttribute(wxT("baseType"),wxT("lpstr"));
 wxXmlNode* lp = new wxXmlNode(vt, wxXML_ELEMENT_NODE, "vt:lpstr");
 lp->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, wxT("Sheet1")));
 wxXmlNode* pairs = new wxXmlNode(app, wxXML_ELEMENT_NODE, "HeadingPairs");
 wxXmlNode* vt1 = new wxXmlNode(pairs, wxXML_ELEMENT_NODE, "vt:vector");
 vt1->AddAttribute(wxT("size"),wxT("2"));
 vt1->AddAttribute(wxT("baseType"),wxT("variant"));
 wxXmlNode* vt2 = new wxXmlNode(vt1, wxXML_ELEMENT_NODE, "vt:variant");
 wxXmlNode* i4 = new wxXmlNode(vt2, wxXML_ELEMENT_NODE, "vt:i4");
 i4->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, wxT("1"))); // Number of Sheets(!)
 wxXmlNode* vt3 = new wxXmlNode(vt1, wxXML_ELEMENT_NODE, "vt:variant");
 wxXmlNode* z = new wxXmlNode(vt3, wxXML_ELEMENT_NODE, "vt:lpstr");
 z->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, wxT("Worksheets"))); // Sheet's folder
 wxXmlNode* crop = new wxXmlNode(app, wxXML_ELEMENT_NODE, "ScaleCrop");
 crop->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, wxT("false")));
 wxXmlNode* security = new wxXmlNode(app, wxXML_ELEMENT_NODE, "DocSecurity");
 security->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, wxT("0")));
 wxXmlNode* appname = new wxXmlNode(app, wxXML_ELEMENT_NODE, "Application");
 appname->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, wxT("ERYA-Bulk")));
 //Export the file as a wxString
 wxStringOutputStream AppStream;
 LocalXlsxApp.Save(AppStream);
 return AppStream.GetString();
}

// docProps/core.xml
wxString XlsxFile::GetXlsxCore()
{
  // Get the current date and hour
  wxString ActualDateTime = this->GetLocalTime();
  // Notice the nodes should be declared backwards, but the attributes not!
 wxXmlDocument LocalXlsxCore;
 wxXmlNode* core = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, "cp:coreProperties");
 LocalXlsxCore.SetRoot(core);
 core->AddAttribute(wxT("xmlns:cp"),wxT("http://schemas.openxmlformats.org/package/2006/metadata/core-properties"));
 core->AddAttribute(wxT("xmlns:dc"),wxT("http://purl.org/dc/elements/1.1/"));
 core->AddAttribute(wxT("xmlns:dcmitype"),wxT("http://purl.org/dc/dcmitype/"));
 core->AddAttribute(wxT("xmlns:dcterms"),wxT("http://purl.org/dc/terms/"));
 core->AddAttribute(wxT("xmlns:xsi"),wxT("http://www.w3.org/2001/XMLSchema-instance"));
 wxXmlNode* date = new wxXmlNode(core, wxXML_ELEMENT_NODE, "dcterms:created");
 date->AddAttribute(wxT("xsi:type"),wxT("dcterms:W3CDTF"));
 date->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, ActualDateTime));
 wxXmlNode* rev = new wxXmlNode(core, wxXML_ELEMENT_NODE, "cp:revision");
 rev->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, wxT("0")));
 wxXmlNode* maker = new wxXmlNode(core, wxXML_ELEMENT_NODE, "dc:creator");
 maker->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, wxT("ERYA-Bulk")));
 //Export the file as a wxString
 wxStringOutputStream CoreStream;
 LocalXlsxCore.Save(CoreStream);
 return CoreStream.GetString();
}

// _rels/.rels
wxString XlsxFile::GetXlsxRels()
{
 // Notice the nodes should be declared backwards, but the attributes not!
 wxXmlDocument LocalXlsxRels;
 wxXmlNode* rels = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, "Relationships");
 LocalXlsxRels.SetRoot(rels);
 rels->AddAttribute(wxT("xmlns"),wxT("http://schemas.openxmlformats.org/package/2006/relationships"));
 wxXmlNode* id3 = new wxXmlNode(rels, wxXML_ELEMENT_NODE, "Relationship");
 id3->AddAttribute(wxT("Id"),wxT("rId3"));
 id3->AddAttribute(wxT("Type"),wxT("http://schemas.openxmlformats.org/officeDocument/2006/relationships/extended-properties"));
 id3->AddAttribute(wxT("Target"),wxT("docProps/app.xml"));
 wxXmlNode* id2 = new wxXmlNode(rels, wxXML_ELEMENT_NODE, "Relationship");
 id2->AddAttribute(wxT("Id"),wxT("rId2"));
 id2->AddAttribute(wxT("Type"),wxT("http://schemas.openxmlformats.org/package/2006/relationships/metadata/core-properties"));
 id2->AddAttribute(wxT("Target"),wxT("docProps/core.xml"));
 wxXmlNode* id1 = new wxXmlNode(rels, wxXML_ELEMENT_NODE, "Relationship");
 id1->AddAttribute(wxT("Id"),wxT("rId1"));
 id1->AddAttribute(wxT("Type"),wxT("http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument"));
 id1->AddAttribute(wxT("Target"),wxT("xl/workbook.xml"));
 //Export the file as a wxString
 wxStringOutputStream RelsStream;
 LocalXlsxRels.Save(RelsStream);
 return RelsStream.GetString();
}

// xl/_rels/workbook.xml.rels
wxString XlsxFile::GetXlsxWRels()
{
  // Notice the nodes should be declared backwards, but the attributes not!
 wxXmlDocument LocalXlsxWRels;
 wxXmlNode* wrels = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, "Relationships");
 LocalXlsxWRels.SetRoot(wrels);
 wrels->AddAttribute(wxT("xmlns"),wxT("http://schemas.openxmlformats.org/package/2006/relationships"));
 wxXmlNode* id3 = new wxXmlNode(wrels, wxXML_ELEMENT_NODE, "Relationship");
 id3->AddAttribute(wxT("Id"),wxT("rId3"));
 id3->AddAttribute(wxT("Type"),wxT("http://schemas.openxmlformats.org/officeDocument/2006/relationships/styles"));
 id3->AddAttribute(wxT("Target"),wxT("styles.xml"));
 wxXmlNode* id2 = new wxXmlNode(wrels, wxXML_ELEMENT_NODE, "Relationship");
 id2->AddAttribute(wxT("Id"),wxT("rId2"));
 id2->AddAttribute(wxT("Type"),wxT("http://schemas.openxmlformats.org/officeDocument/2006/relationships/sharedStrings"));
 id2->AddAttribute(wxT("Target"),wxT("sharedStrings.xml"));
 wxXmlNode* id1 = new wxXmlNode(wrels, wxXML_ELEMENT_NODE, "Relationship");
 id1->AddAttribute(wxT("Id"),wxT("rId1"));
 id1->AddAttribute(wxT("Type"),wxT("http://schemas.openxmlformats.org/officeDocument/2006/relationships/worksheet"));
 id1->AddAttribute(wxT("Target"),wxT("worksheets/sheet1.xml"));
 //Export the file as a wxString
 wxStringOutputStream RelsWStream;
 LocalXlsxWRels.Save(RelsWStream);
 return RelsWStream.GetString();
}

// xl/styles.xml
wxString XlsxFile::GetXlsxStyles()
{
 wxXmlDocument LocalXlsxStyle;
 wxXmlNode* style = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, "styleSheet");
 LocalXlsxStyle.SetRoot(style);
 style->AddAttribute(wxT("xmlns"),wxT("http://schemas.openxmlformats.org/spreadsheetml/2006/main"));
 wxXmlNode* stylecell = new wxXmlNode(style, wxXML_ELEMENT_NODE, "cellStyles");
 stylecell->AddAttribute(wxT("count"),wxT("1"));
 wxXmlNode* cellsystem = new wxXmlNode(stylecell, wxXML_ELEMENT_NODE, "cellStyle");
 cellsystem->AddAttribute(wxT("name"),wxT("Normal"));
 cellsystem->AddAttribute(wxT("xfId"),wxT("0"));
 cellsystem->AddAttribute(wxT("builtinId"),wxT("0"));
 wxXmlNode* cellxfs = new wxXmlNode(style, wxXML_ELEMENT_NODE, "cellXfs");
 cellxfs->AddAttribute(wxT("count"),wxT("1"));
 wxXmlNode* celsystem = new wxXmlNode(cellxfs, wxXML_ELEMENT_NODE, "xf");
 celsystem->AddAttribute(wxT("numFmtId"),wxT("0"));
 celsystem->AddAttribute(wxT("fontId"),wxT("0"));
 celsystem->AddAttribute(wxT("fillId"),wxT("0"));
 celsystem->AddAttribute(wxT("borderId"),wxT("0"));
 celsystem->AddAttribute(wxT("xfId"),wxT("0"));
 wxXmlNode* stylexfs = new wxXmlNode(style, wxXML_ELEMENT_NODE, "cellStyleXfs");
 stylexfs->AddAttribute(wxT("count"),wxT("1"));
 wxXmlNode* xfssystem = new wxXmlNode(stylexfs, wxXML_ELEMENT_NODE, "xf");
 xfssystem->AddAttribute(wxT("numFmtId"),wxT("0"));
 xfssystem->AddAttribute(wxT("fontId"),wxT("0"));
 xfssystem->AddAttribute(wxT("fillId"),wxT("0"));
 xfssystem->AddAttribute(wxT("borderId"),wxT("0"));
 wxXmlNode* borders = new wxXmlNode(style, wxXML_ELEMENT_NODE, "borders");
 borders->AddAttribute(wxT("count"),wxT("1"));
 wxXmlNode* bordersystem0 = new wxXmlNode(borders, wxXML_ELEMENT_NODE, "border");
 wxXmlNode* bordersystem1 = new wxXmlNode(bordersystem0, wxXML_ELEMENT_NODE, "diagonal");
 wxXmlNode* bordersystem2 = new wxXmlNode(bordersystem0, wxXML_ELEMENT_NODE, "bottom");
 wxXmlNode* bordersystem3 = new wxXmlNode(bordersystem0, wxXML_ELEMENT_NODE, "top");
 wxXmlNode* bordersystem4 = new wxXmlNode(bordersystem0, wxXML_ELEMENT_NODE, "right");
 wxXmlNode* bordersystem5 = new wxXmlNode(bordersystem0, wxXML_ELEMENT_NODE, "left");
 wxXmlNode* fills = new wxXmlNode(style, wxXML_ELEMENT_NODE, "fills");
 fills->AddAttribute(wxT("count"),wxT("1"));
 wxXmlNode* fillsystem = new wxXmlNode(fills, wxXML_ELEMENT_NODE, "fill");
 wxXmlNode* fillpattern = new wxXmlNode(fillsystem, wxXML_ELEMENT_NODE, "patternFill");
 fillpattern->AddAttribute(wxT("patternType"),wxT("none"));
 wxXmlNode* fonts = new wxXmlNode(style, wxXML_ELEMENT_NODE, "fonts");
 fonts->AddAttribute(wxT("count"),wxT("1"));
 wxXmlNode* fontsystem = new wxXmlNode(fonts, wxXML_ELEMENT_NODE, "font");
 wxXmlNode* family = new wxXmlNode(fontsystem, wxXML_ELEMENT_NODE, "family");
 family->AddAttribute(wxT("val"),wxT("2"));
 wxXmlNode* name = new wxXmlNode(fontsystem, wxXML_ELEMENT_NODE, "name");
 name->AddAttribute(wxT("val"),wxT("Calibri"));
 wxXmlNode* sz = new wxXmlNode(fontsystem, wxXML_ELEMENT_NODE, "sz");
 sz->AddAttribute(wxT("val"),wxT("12"));
 //Export the file as a wxString
 wxStringOutputStream RelsStyle;
 LocalXlsxStyle.Save(RelsStyle);
 return RelsStyle.GetString();
}

// xl/workbook.xml
wxString XlsxFile::GetXlsxWBook()
{
 wxXmlDocument LocalXlsxWBook;
 wxXmlNode* book = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, "workbook");
 LocalXlsxWBook.SetRoot(book);
 book->AddAttribute(wxT("xmlns"),wxT("http://schemas.openxmlformats.org/spreadsheetml/2006/main"));
 book->AddAttribute(wxT("xmlns:r"),wxT("http://schemas.openxmlformats.org/officeDocument/2006/relationships"));
 wxXmlNode* sheet = new wxXmlNode(book, wxXML_ELEMENT_NODE, "sheets");
 wxXmlNode* sheet1 = new wxXmlNode(sheet, wxXML_ELEMENT_NODE, "sheet");
 sheet1->AddAttribute(wxT("name"),wxT("Sheet1"));
 sheet1->AddAttribute(wxT("sheetId"),wxT("1"));
 sheet1->AddAttribute(wxT("r:id"),wxT("rId1"));
 //Export the file as a wxString
 wxStringOutputStream RelsWBook;
 LocalXlsxWBook.Save(RelsWBook);
 return RelsWBook.GetString();
}


// Generate xl/sharedstrings.xml
wxString XlsxFile::CreateXlsxShared()
{
 // Get the number of strings, which is needed as argument
 wxArrayString AllSharedStrings = InternalTable.GetAllStrings();
 int NumberStrings = AllSharedStrings.GetCount();
 wxString NS = wxString::Format("%i",NumberStrings);
 // Build the xml file itself
 wxXmlDocument LocalXlsxShared;
 wxXmlNode* shared = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, "sst");
 LocalXlsxShared.SetRoot(shared);
 shared->AddAttribute(wxT("xmlns"),wxT("http://schemas.openxmlformats.org/spreadsheetml/2006/main")); //Which are name=value tags
 shared->AddAttribute(wxT("count"),NS);
 shared->AddAttribute(wxT("uniqueCount"),NS);
 // Add write the data. notice the data stream should be allocated backwards!
 for(int i=0; i<AllSharedStrings.GetCount(); i++)
 {
   wxXmlNode* block = new wxXmlNode(shared, wxXML_ELEMENT_NODE, "si");
   wxXmlNode* value = new wxXmlNode(block, wxXML_ELEMENT_NODE, "t");
   value->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, AllSharedStrings.Item(AllSharedStrings.GetCount() - i - 1))); // Set a value, like <tag>value</value>
 }
 //Export the file as a wxString
 wxStringOutputStream SharedStream;
 LocalXlsxShared.Save(SharedStream);
 return SharedStream.GetString();
}

// Generate xl/worksheets/sheet1.xml
// All data are written as a single matrix block, where in the most regular situation it would not happen.
wxString XlsxFile::CreateXlsxSheet()
{
 // Get the matrix block boundaries
 int MaxYieldRows, MaxYieldCols;
 InternalTable.GetRealMatrixSize(MaxYieldRows,MaxYieldCols);
 // Set the boundaries
 wxString DimensionCell = this->GetXlsxAdress(InitialRow,InitialColumn) +  wxT(":") + this->GetXlsxAdress(InitialRow + MaxYieldRows-1,InitialColumn + MaxYieldCols-1);
 wxString MinSpan = wxString::Format("%i",InitialColumn+1);
 wxString MaxSpan = wxString::Format("%i",InitialColumn+MaxYieldCols);
 wxString TableRowSpan = MinSpan + wxT(":") + MaxSpan;
 // Build the xml file itself, however it should be allocated backwards!!!
 wxXmlDocument LocalXlsxSheet;
 wxXmlNode* sheet = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, "worksheet");
 LocalXlsxSheet.SetRoot(sheet);
 sheet->AddAttribute(wxT("xmlns"),wxT("http://schemas.openxmlformats.org/spreadsheetml/2006/main"));
 sheet->AddAttribute(wxT("xmlns:r"),wxT("http://schemas.openxmlformats.org/officeDocument/2006/relationships"));
 wxXmlNode* margins = new wxXmlNode(sheet, wxXML_ELEMENT_NODE, "pageMargins");
 margins->AddAttribute(wxT("left"),wxT("0.7"));
 margins->AddAttribute(wxT("right"),wxT("0.7"));
 margins->AddAttribute(wxT("top"),wxT("0.75"));
 margins->AddAttribute(wxT("bottom"),wxT("0.75"));
 margins->AddAttribute(wxT("header"),wxT("0.3"));
 margins->AddAttribute(wxT("footer"),wxT("0.3"));
 wxXmlNode* data = new wxXmlNode(sheet, wxXML_ELEMENT_NODE, "sheetData");
 for(int i=0; i<MaxYieldRows; i++)
 {
   // Check if exist empty rows
   int NumberEmptyCells = 0;
   for(int k=0; k<MaxYieldCols; k++)
   {
     int TestCellType;
     wxString TestCellValue = InternalTable.GetTableMatrixValueAt(MaxYieldRows-i-1, MaxYieldCols-k-1, TestCellType);
     if(TestCellType == 3) //Found an empty cell
        NumberEmptyCells = NumberEmptyCells + 1;
   }
   // Avoid to create an empty row, as defined by own Microsoft Standards!
   if(MaxYieldCols != NumberEmptyCells)
   {
    // Setting some necessary local variables
   wxString CurrentRowIndex = wxString::Format("%i",InitialRow+MaxYieldRows-i);
   wxString MinSpan = wxString::Format("%i",InitialColumn+1);
   wxString MaxSpan = wxString::Format("%i",InitialColumn+MaxYieldCols);
   wxString TableRowSpan = MinSpan + wxT(":") + MaxSpan;
   // Allocate the Row XML node...
   wxXmlNode* datarow = new wxXmlNode(data, wxXML_ELEMENT_NODE, "row");
   datarow->AddAttribute(wxT("r"),CurrentRowIndex);
   datarow->AddAttribute(wxT("spans"),TableRowSpan);
   for (int j=0; j<MaxYieldCols; j++)
   {
     // Get Table matrix information, first!
     int CellType;
     wxString CellValue = InternalTable.GetTableMatrixValueAt(MaxYieldRows-i-1, MaxYieldCols-j-1, CellType);
     wxString SetCellType;
     bool CreateCell = true; //Needed to avoid creating empty cells.
     if (CellType == 0) // Get the string value
     {
       SetCellType = wxT("s");
       wxArrayString StringList = InternalTable.GetAllStrings();
       for (int z=0; z<StringList.GetCount(); z++)
       {
         if(StringList.Item(z) == CellValue) // The string count begins at zero, instead several structures that begins at one...
         {
            CellValue = wxString::Format("%i",z);
         }
       }
     }
     else if (CellType == 1)
     {
        SetCellType = wxT("n"); // A number
     }
     else if (CellType == 2)
     {
      SetCellType = wxT("b"); // A binary value
      if(CellValue == wxT("true"))
        CellValue = wxString::Format("%i",1);
      else
        CellValue = wxString::Format("%i",0);
     }
     else
     {
        CreateCell = false;
     }
     if(CreateCell) //If the cell is not empty, create it.
     {
      wxString CellAdress = this->GetXlsxAdress(InitialRow + MaxYieldRows-i-1, InitialColumn + MaxYieldCols-j-1);
     // Write the cell contents
      wxXmlNode* datacol = new wxXmlNode(datarow, wxXML_ELEMENT_NODE, "c");
      datacol->AddAttribute(wxT("r"),CellAdress);
      datacol->AddAttribute(wxT("t"),SetCellType);
      wxXmlNode* datavalue = new wxXmlNode(datacol, wxXML_ELEMENT_NODE, "v");
       datavalue->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, CellValue));
     }
   }
   }
 }
 wxXmlNode* format = new wxXmlNode(sheet, wxXML_ELEMENT_NODE, "sheetFormatPr");
 format->AddAttribute(wxT("baseColWidth"),wxT("20"));
 format->AddAttribute(wxT("defaultRowHeight"),wxT("15"));
 wxXmlNode* views = new wxXmlNode(sheet, wxXML_ELEMENT_NODE, "sheetViews");
 wxXmlNode* view = new wxXmlNode(views, wxXML_ELEMENT_NODE, "sheetView");
 view->AddAttribute(wxT("tabSelected"),wxT("1"));
 view->AddAttribute(wxT("workbookViewId"),wxT("0"));
 wxXmlNode* sel = new wxXmlNode(view, wxXML_ELEMENT_NODE, "selection");
 wxXmlNode* dim = new wxXmlNode(sheet, wxXML_ELEMENT_NODE, "dimension");
 dim->AddAttribute(wxT("ref"),DimensionCell);
 //Export the file as a wxString
 wxStringOutputStream SheetStream;
 LocalXlsxSheet.Save(SheetStream);
 return SheetStream.GetString();
}

