#include "ERYAPIXEdialogRemark.h"
#include "ERYAPIXEdialogAddDatabase.h"
#include "ERYAPIXEdialogDatabaseManager.h"
#include "ERYAPIXEdialogDetectorSetup.h"
#include "ERYAPIXEdialogZieglerParameters.h"

ERYAPIXEdialogRemark::ERYAPIXEdialogRemark( wxWindow* parent )
:
dialogRemark( parent )
{

}

ERYAPIXEdialogRemark::ERYAPIXEdialogRemark( wxWindow* parent, wxString info, int n )
:
dialogRemark( parent )
{
 textRemark->SetValue(info);
 FlagValue = n;
}

void ERYAPIXEdialogRemark::OnRemarkSave( wxCommandEvent& event )
{
 // Call the parent frame
if(FlagValue==0)
{
 ERYAPIXEdialogAddDatabase *Parent = (ERYAPIXEdialogAddDatabase *) GetParent();
 wxString data = textRemark->GetValue();
 Parent->SetRemark(data);
 Close();
}
else if(FlagValue==1)
{
 ERYAPIXEdialogDatabaseManager *Parent = (ERYAPIXEdialogDatabaseManager *) GetParent();
 wxString data = textRemark->GetValue();
 Parent->SetRemark(data);
 Close();
}
else if(FlagValue==2)
{
 ERYAPIXEdialogDetectorSetup *Parent = (ERYAPIXEdialogDetectorSetup *) GetParent();
 wxString data = textRemark->GetValue();
 Parent->SetRemark(data);
 Close();
}
else if(FlagValue==3)
{
 ERYAPIXEdialogZieglerParameters *Parent = (ERYAPIXEdialogZieglerParameters *) GetParent();
 wxString data = textRemark->GetValue();
 Parent->SetRemark(data);
 Close();
}
else
{
 Close();
}
}

void ERYAPIXEdialogRemark::OnRemarkClear( wxCommandEvent& event )
{
 textRemark->Clear();
}

void ERYAPIXEdialogRemark::OnRemarkQuit( wxCommandEvent& event )
{
 Close();
}
