/***************************************************************
 * Name:      ERYAPIXE.h
 * Purpose:   ERYA main class
 * Author:    Vasco Manteigas (vm.manteigas@campus.fct.utl.pt)
 * Created:   2015-12-01
 * Copyright: Grupo de Reacoes Nucleares - LIBPHYS
 * License:
 **************************************************************/

///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 29 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __ERYAPIXE_H__
#define __ERYAPIXE_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/html/htmlwin.h>
#include <wx/xml/xml.h>
#include <wx/filesys.h>
#include <wx/zipstrm.h>
#include <wx/wfstream.h>
#include <wx/fs_zip.h>
#include <wx/txtstrm.h>
#include <wx/sstream.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/stattext.h>
#include <wx/choice.h>
#include <wx/checkbox.h>
#include <wx/textctrl.h>
#include <wx/checklst.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/scrolwin.h>
#include <wx/statbmp.h>
#include <wx/notebook.h>
#include <wx/statline.h>
#include <wx/radiobox.h>
#include <wx/filepicker.h>
#include <wx/spinctrl.h>
#include <wx/button.h>
#include <wx/statusbr.h>
#include <wx/frame.h>
#include <wx/dialog.h>
#include <wx/grid.h>
#include <wx/msgdlg.h>
#include <wx/filedlg.h>
#include <wx/dirdlg.h>
#include <wx/dynarray.h>
#include <wx/wizard.h>
#include <wx/textfile.h>
#include <wx/stdpaths.h>
#include <wx/textdlg.h>
#include <wx/listbox.h>
#include <wx/clipbrd.h>
#include <wx/filefn.h>
#include <wx/gdicmn.h>
#include <wx/datetime.h>
#include <wx/busyinfo.h>
#include <wx/progdlg.h>
#include <wx/choicdlg.h>
#include "mathplot.h"
#include "ERYA.xpm"
#include "ERYA_Bulk.xpm"
#include "libphys.xpm"

WX_DEFINE_ARRAY_PTR( wxWizardPageSimple*, WizardPages );
WX_DECLARE_OBJARRAY( wxChoice*, ArrayElement);
WX_DECLARE_OBJARRAY( wxChoice*, ArrayGP);
WX_DECLARE_OBJARRAY( wxCheckBox*, ArrayFit);
WX_DECLARE_OBJARRAY( wxTextCtrl*, ArrayZ);
WX_DECLARE_OBJARRAY( wxTextCtrl*, ArrayCP);
WX_DECLARE_OBJARRAY( wxTextCtrl*, ArraySG);
WX_DECLARE_OBJARRAY( wxTextCtrl*, ArrayYS);
WX_DECLARE_OBJARRAY( wxTextCtrl*, ArrayYE);
WX_DECLARE_OBJARRAY( wxTextCtrl*, ArrayYF);
WX_DECLARE_OBJARRAY( wxTextCtrl*, ArraySF);
WX_DECLARE_OBJARRAY( wxTextCtrl*, ArraySM);
WX_DECLARE_OBJARRAY( wxTextCtrl*, ArraySE);

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class MainFrame
///////////////////////////////////////////////////////////////////////////////
class MainFrame : public wxFrame
{
	private:

	protected:
		wxMenuBar* mMainMenu;
		wxMenu* menuFile;
		wxMenu* menuDatabase;
		wxMenu* menuHelp;
        wxBoxSizer* sizerMainFrame;
		wxNotebook* tabbedOutput;
		wxPanel* tabElements;
		wxStaticText* labelElements;
		wxStaticText* labelGammaPeak;
		wxStaticText* labelFit;
		wxStaticText* labelZ;
        wxStaticText* labelElementCalibrationParameter;
		wxStaticText* labelStoichiometricGuess;
		wxStaticText* labelYieldSimulation;
		wxStaticText* labelYieldExperimental;
		wxStaticText* labelYieldFitted;
		wxStaticText* labelStoichiometricFitted;
		wxStaticText* labelStoichiometricMass;
		wxStaticText* labelStoichiometricError;
		wxBoxSizer* sizerElements;
		wxFlexGridSizer* sizerButtons;
        wxScrolledWindow* scrollButtons;
        wxArrayString choiceElementChoices,choiceGPChoices;
		ArrayElement choiceElement;
		ArrayGP choiceGP;
		ArrayFit checkFit;
		ArrayZ textZ;
        ArrayCP textCP;
		ArraySG textSG;
		ArrayYS textYS;
		ArrayYE textYE;
		ArrayYF textYF;
		ArraySF textSF;
		ArraySM textSM;
		ArraySE textSE;
		wxPanel* tabYieldSample;
		mpWindow* renderYield;
		wxPanel* tabYieldEnergy;
		mpWindow* renderStoichiometry;
		wxPanel* tabFittingError;
		mpWindow* renderError;
                wxPanel* tabDetectorFitting;
		mpWindow* renderDetectorFitting;
                wxPanel* tabLogProfiling;
                wxGrid* tableLogProfiling;
		wxStaticLine* separatorMainFrame;
		wxStaticText* labelNumberElements;
		wxSpinCtrl* spinNumberElements;
		wxStaticText* labelMinimumEnergy;
		wxTextCtrl* textMinimumEnergy;
		wxStaticText* labelMaximumEnergy;
		wxTextCtrl* textMaximumEnergy;
		wxStaticText* labelStepSize;
		wxTextCtrl* textStepSize;
        wxStaticText* labelDetectorAngle;
		wxTextCtrl* textDetectorAngle;
		wxStaticText* labelCharge;
		wxTextCtrl* textCharge;
		wxStaticText* labelThickness;
		wxTextCtrl* textThickness;
        wxButton* buttonMainNew;
        wxButton* buttonMainCheck;
        wxButton* buttonMainAdvanced;
        wxButton* buttonMainHelp;
        wxButton* buttonMainFit;
		wxButton* buttonMainStop;
		wxStatusBar* barMainStatus;

		// Virtual event handlers, overide them in your derived class
		virtual void OnFileNew( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnFileOpen( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnFileSave( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnFileSaveAs( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnFileQuit( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDatabaseDetector( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDatabaseElements( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDatabaseZiegler( wxCommandEvent& event ) { event.Skip(); }
        virtual void OnDatabaseSetup( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnHelpGuide( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnHelpAbout( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnElement( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnGammaPeak( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnNumberTrigger( wxSpinEvent& event ) { event.Skip(); }
        virtual void OnDirectNumberTrigger( wxCommandEvent& event ) { event.Skip(); }
        virtual void OnMainNew( wxCommandEvent& event ) { event.Skip(); }
        virtual void OnMainCheck( wxCommandEvent& event ) { event.Skip(); }
        virtual void OnMainAdvanced( wxCommandEvent& event ) { event.Skip(); }
        virtual void OnMainHelp( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMainFit( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMainStop( wxCommandEvent& event ) { event.Skip(); }


	public:

		MainFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("ERYA - Emission Radiation Yield Analysis "), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1920,1080 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~MainFrame();

};

///////////////////////////////////////////////////////////////////////////////
/// Class dialogDatabaseManager
///////////////////////////////////////////////////////////////////////////////
class dialogDatabaseManager : public wxDialog
{
	private:

	protected:
		wxStaticText* labelElementName;
		wxStaticText* labelGammaPeak;
		wxStaticText* labelAtomicNumber;
		wxStaticText* labelAbundance;
		wxStaticText* labelAtomicMass;
		wxStaticText* labelIsotopicMass;
		wxChoice* choiceElementName;
		wxChoice* choiceGammaPeak;
		wxTextCtrl* textAtomicNumber;
		wxTextCtrl* textAbundance;
		wxTextCtrl* textAtomicMass;
		wxTextCtrl* textIsotopicMass;
		wxStaticLine* separatorDisplay;
		wxGrid* tableElementViewer;
		mpWindow* renderElementViewer;
		wxStaticLine* separatorButtons;
		wxButton* buttonAdd;
		wxButton* buttonEdit;
		wxButton* buttonRemove;
		wxButton* buttonClear;
		wxButton* buttonLoad;
		wxButton* buttonSave;
		wxButton* buttonCancel;
		wxButton* buttonHelp;
		wxArrayString choiceElementNameChoices;
		wxArrayString choiceGammaPeakChoices;
        wxString MainDatabaseFile;

		// Virtual event handlers, overide them in your derived class
		virtual void OnElementName( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnGammaPeak( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDatabaseAdd( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDatabaseEdit( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDatabaseRemove( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDatabaseClear( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDatabaseLoad( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDatabaseSave( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDatabaseCancel( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDatabaseHelp( wxCommandEvent& event ) { event.Skip(); }


	public:
		dialogDatabaseManager( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("ERYA - Database Manager"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1200, 720 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
		~dialogDatabaseManager();

};

///////////////////////////////////////////////////////////////////////////////
/// Class dialogAddDatabase
///////////////////////////////////////////////////////////////////////////////
class dialogAddDatabase : public wxDialog
{
	private:

	protected:
		wxStaticText* labelEditElement;
		wxStaticText* labelEditGamma;
		wxStaticText* labelEditNumber;
		wxStaticText* labelEditAbundance;
		wxStaticText* labelEditAtomic;
		wxStaticText* labelEditIsotopic;
		wxTextCtrl* textEditElement;
		wxTextCtrl* textEditGamma;
		wxTextCtrl* textEditNumber;
		wxTextCtrl* textEditAbundance;
		wxTextCtrl* textEditAtomic;
		wxTextCtrl* textEditIsotopic;
		wxStaticLine* separatorEditor;
		wxGrid* tableDataEditor;
		wxStaticLine* separatorTable;
		wxButton* buttonImportR33;
		wxButton* buttonExportR33;
		wxButton* buttonEditCopy;
		wxButton* buttonEditPaste;
		wxButton* buttonEditClear;
		wxButton* buttonEditSave;
		wxButton* buttonEditCancel;
		wxButton* buttonEditHelp;
		wxString* dataClipboard;

		// Virtual event handlers, overide them in your derived class
		virtual void OnDatabaseSelect( wxGridRangeSelectEvent& event ) { event.Skip(); }
		virtual void OnDatabaseCopy( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDatabasePaste( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnImportR33( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnExportR33( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnEditClear( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnEditSave( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnEditCancel( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnEditHelp( wxCommandEvent& event ) { event.Skip(); }


	public:
		dialogAddDatabase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("ERYA - Add Element to Database"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1200, 720 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
		~dialogAddDatabase();

};


///////////////////////////////////////////////////////////////////////////////
/// Class dialogDetectorSetup
///////////////////////////////////////////////////////////////////////////////
class dialogDetectorSetup : public wxDialog
{
	private:

	protected:
		wxStaticText* labelFunctionEficiency;
		wxTextCtrl* textFunctionEficiency;
		wxStaticLine* separatorFunctionEficiency;
		wxStaticText* labelConstantsEficiency;
		wxGrid* tableConstantEficiency;
		wxStaticLine* separatorButtonEficiency;
		wxButton* buttonEficiencyLoad;
		wxButton* buttonEficiencySave;
        wxButton* buttonEficiencyCopy;
		wxButton* buttonEficiencyPaste;
		wxButton* buttonEficiencyClear;
		wxButton* buttonEficiencyOK;
		wxButton* buttonEficiencyCancel;
		wxButton* buttonEficiencyHelp;
       wxString* dataClipboard;

		// Virtual event handlers, overide them in your derived class
		virtual void OnEficiencySelect( wxGridRangeSelectEvent& event ) { event.Skip(); }
       virtual void OnEficiencyCopy( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnEficiencyPaste( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnEficiencyLoad( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnEficiencySave( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnEficiencyClear( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnEficiencyOK( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnEficiencyCancel( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnEficiencyHelp( wxCommandEvent& event ) { event.Skip(); }


	public:

		dialogDetectorSetup( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("ERYA - Detector Eficiency"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1200, 720 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
		~dialogDetectorSetup();

};

///////////////////////////////////////////////////////////////////////////////
/// Class dialogZieglerParameters
///////////////////////////////////////////////////////////////////////////////
class dialogZieglerParameters : public wxDialog
{
	private:

	protected:
		wxStaticText* labelZieglerParameters;
		wxStaticLine* lineZieglerVersion;
		wxStaticText* labelZieglerVersion;
		wxChoice* choiceZieglerVersion;
		wxStaticLine* lineZieglerTables;
		wxStaticText* labelZieglerFunction;
		wxTextCtrl* textZieglerFunction;
		wxNotebook* tabZieglerTables;
		wxPanel* tabZieglerEquations;
		wxGrid* tableZieglerParameters;
		wxPanel* tabZieglerSRIM;
		wxStaticText* labelZieglerSRIMCurrentElement;
		wxSpinCtrl* spinZieglerSRIMCurrentElement;
		wxButton* buttonZieglerSRIMUpdate;
		wxButton* buttonZieglerSRIMEraseAllData;
		wxGrid* tableZieglerSRIM;
		wxStaticLine* lineZieglerParameters;
		wxButton* buttonZieglerLoad;
		wxButton* buttonZieglerSave;
		wxButton* buttonZieglerCopy;
		wxButton* buttonZieglerPaste;
		wxButton* buttonZieglerClear;
		wxButton* buttonZieglerOK;
		wxButton* buttonZieglerCancel;
		wxButton* buttonZieglerHelp;
                wxString* dataClipboard;

		// Virtual event handlers, overide them in your derived class
		virtual void OnZieglerSelectElement( wxSpinEvent& event ) { event.Skip(); }
		virtual void OnZieglerSelectElementText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnZieglerSelectElementEnter( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnUpdateZieglerSRIM( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnEraseZieglerSRIM( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnZieglerLoad( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnZieglerSave( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnZieglerCopy( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnZieglerPaste( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnZieglerClear( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnZieglerOK( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnZieglerCancel( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnZieglerHelp( wxCommandEvent& event ) { event.Skip(); }


	public:

		dialogZieglerParameters( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("ERYA - Ziegler Parameters"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1280,720 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
		~dialogZieglerParameters();

};

///////////////////////////////////////////////////////////////////////////////
/// Class dialogSetup
///////////////////////////////////////////////////////////////////////////////
class dialogSetup : public wxDialog
{
	private:

	protected:
		wxStaticText* lineDefaultButtons;
		wxStaticText* labelSetupTip;
		wxRadioBox* radioDefaultConfig;
		wxStaticLine* lineDefaultFiles;
		wxStaticText* labelFilesInfo;
		wxStaticText* labelDefaultDetector;
		wxFilePickerCtrl* fileDefaultDetector;
		wxStaticText* labelDefaultDatabase;
		wxFilePickerCtrl* fileDefaultDatabase;
		wxStaticText* labelDefaultZiegler;
		wxFilePickerCtrl* fileDefaultZiegler;
		wxStaticLine* m_staticline1;
		wxButton* buttonDefaultSave;
		wxButton* buttonDefaultReset;
		wxButton* buttonDefaultDelete;
		wxButton* buttonDefaultQuit;

		// Virtual event handlers, overide them in your derived class
		virtual void OnDefaultSave( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDefaultReset( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDefaultDelete( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDefaultQuit( wxCommandEvent& event ) { event.Skip(); }


	public:

		dialogSetup( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Default Databases"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 640,480 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
		~dialogSetup();

};

///////////////////////////////////////////////////////////////////////////////
/// Class dialogHelp
///////////////////////////////////////////////////////////////////////////////
class dialogHelp : public wxDialog
{
	private:

	protected:
	    wxString helpMainPage;
		wxHtmlWindow* pageHelpViewer;
		wxStaticLine* lineHelpViewer;
		wxButton* buttonHelpMain;
		wxButton* buttonHelpIndex;
		wxButton* buttonHelpReadme;
		wxButton* buttonHelpBack;
		wxButton* buttonHelpForward;
		wxButton* buttonHelpClose;

		// Virtual event handlers, overide them in your derived class
		virtual void OnHelpMain( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnHelpIndex( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnHelpReadme( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnHelpBack( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnHelpForward( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnHelpClose( wxCommandEvent& event ) { event.Skip(); }


	public:

		dialogHelp( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("ERYA Help Viewer"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1200, 720 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
		~dialogHelp();

};

///////////////////////////////////////////////////////////////////////////////
/// Class wizardFirstRun
///////////////////////////////////////////////////////////////////////////////
class wizardFirstRun : public wxWizard
{
	private:

	protected:
		wxStaticText* labelFirstRun1;
		wxStaticText* labelFirstRun2;
		wxFilePickerCtrl* fileDetectorFirstRun;
		wxStaticText* labelFirstRun3;
		wxFilePickerCtrl* fileElementsFirstRun;
		wxStaticText* labelFirstRun4;
		wxFilePickerCtrl* fileZieglerFirstRun;
		wxStaticText* labelFirstRun5;
		wxRadioBox* radioSetupDefault;
		wxStaticText* labelFirstRun6;

        wxWizardPage* GetFirstPage(){return m_pages.Item(0);}; //required hack

		// Virtual event handlers, overide them in your derived class
		virtual void OnFirstRunCancel( wxWizardEvent& event ) { event.Skip(); }
		virtual void OnFirstRunFinish( wxWizardEvent& event ) { event.Skip(); }

	public:

		wizardFirstRun( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("ERYA Bulk Setup Wizard"), const wxBitmap& bitmap = wxBitmap(ERYA_Bulk_xpm), const wxPoint& pos = wxDefaultPosition, long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
		WizardPages m_pages;
        bool StartWizard(){return this->RunWizard(this->GetFirstPage());}; //Create a nice function to simplify the wizard loading
		~wizardFirstRun();

};


///////////////////////////////////////////////////////////////////////////////
/// Class dialogAdvancedFit
///////////////////////////////////////////////////////////////////////////////
class dialogAdvancedFit : public wxDialog
{
	private:

	protected:
		wxStaticText* labelAdvancedTip;
		wxStaticLine* lineAdvancedTip;
		wxStaticText* labelNumberIterations;
		wxSpinCtrl* spinNumberIterations;
		wxStaticText* labelDampingFactor;
		wxSpinCtrl* spinDampingFactor;
		wxStaticText* labelYieldConvergence;
		wxSpinCtrl* spinYieldConvergence;
		wxStaticText* labelStoichiometryConvergence;
		wxSpinCtrl* spinStoichiometryConvergence;
		wxStaticLine* lineAdvancedButtons;
		wxButton* buttonAdvancedOK;
		wxButton* buttonAdvancedCancel;
		wxButton* buttonAdvancedReset;
		wxButton* buttonAdvancedHelp;

		// Virtual event handlers, overide them in your derived class
		virtual void OnAdvancedOK( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAdvancedCancel( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAdvancedReset( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAdvancedHelp( wxCommandEvent& event ) { event.Skip(); }


	public:

		dialogAdvancedFit( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("ERYA Advanced Fitting Settings"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
		~dialogAdvancedFit();

};

///////////////////////////////////////////////////////////////////////////////
/// Class dialogR33DataImport
///////////////////////////////////////////////////////////////////////////////
class dialogR33DataImport : public wxDialog
{
	private:

	protected:
		wxStaticText* labelDataImportInfo;
		wxStaticLine* lineDataImportInfo;
		wxStaticText* labelDataImportUnits;
		wxRadioBox* radioDataImportUnits;
		wxRadioBox* radioDataImportAngle;
		wxCheckBox* checkIgnoreNonNumericalData;
		wxStaticLine* lineDataImportButtons;
		wxButton* buttonR33DataOK;
		wxButton* buttonR33DataCancel;
		wxButton* buttonR33DataReset;
		wxButton* buttonR33DataHelp;

		// Virtual event handlers, overide them in your derived class
		virtual void OnDataImportValidate( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDataImportAbort( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDataImportReset( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDataImportHelp( wxCommandEvent& event ) { event.Skip(); }


	public:

		dialogR33DataImport( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Additional Data Import Options"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 640,360 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~dialogR33DataImport();

};

///////////////////////////////////////////////////////////////////////////////
/// Class dialogXlsxDataImport
///////////////////////////////////////////////////////////////////////////////
class dialogXlsxDataImport : public wxDialog
{
	private:

	protected:
		wxStaticText* labelDataImportInfo;
		wxStaticLine* lineDataImportInfo;
		wxStaticText* labelDataImportUnits;
		wxRadioBox* radioDataImportUnits;
		wxRadioBox* radioDataImportAngle;
		wxRadioBox* radioDataImportColumn;
		wxStaticLine* lineDataImportButtons;
		wxButton* buttonXlsxDataOK;
		wxButton* buttonXlsxDataCancel;
		wxButton* buttonXlsxDataReset;
		wxButton* buttonXlsxDataHelp;

		// Virtual event handlers, overide them in your derived class
		virtual void OnDataImportValidate( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDataImportAbort( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDataImportReset( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDataImportHelp( wxCommandEvent& event ) { event.Skip(); }


	public:

		dialogXlsxDataImport( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Additional Excel File Data Import Options"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 700,450 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~dialogXlsxDataImport();

};


#endif //__ERYAPIXE_H__
