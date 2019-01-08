/***************************************************************
 * Name:      ERYAPIXE.cpp
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

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif //WX_PRECOMP

#include "ERYAPIXE.h"
#include <wx/arrimpl.cpp>
WX_DEFINE_OBJARRAY( ArrayElement);
WX_DEFINE_OBJARRAY( ArrayGP);
WX_DEFINE_OBJARRAY( ArrayFit);
WX_DEFINE_OBJARRAY( ArrayZ);
WX_DEFINE_OBJARRAY( ArrayCP);
WX_DEFINE_OBJARRAY( ArraySG);
WX_DEFINE_OBJARRAY( ArrayYS);
WX_DEFINE_OBJARRAY( ArrayYE);
WX_DEFINE_OBJARRAY( ArrayYF);
WX_DEFINE_OBJARRAY( ArraySF);
WX_DEFINE_OBJARRAY( ArraySM);
WX_DEFINE_OBJARRAY( ArraySE);



///////////////////////////////////////////////////////////////////////////

MainFrame::MainFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	mMainMenu = new wxMenuBar( 0 );
	menuFile = new wxMenu();
	wxMenuItem* menuFileNew;
	menuFileNew = new wxMenuItem( menuFile, wxID_ANY, wxString( wxT("New") ) , wxEmptyString, wxITEM_NORMAL );
	menuFile->Append( menuFileNew );

	wxMenuItem* menuFileOpen;
	menuFileOpen = new wxMenuItem( menuFile, wxID_ANY, wxString( wxT("Open") ) , wxEmptyString, wxITEM_NORMAL );
	menuFile->Append( menuFileOpen );

	wxMenuItem* menuFileSave;
	menuFileSave = new wxMenuItem( menuFile, wxID_ANY, wxString( wxT("Save") ) , wxEmptyString, wxITEM_NORMAL );
	menuFile->Append( menuFileSave );

	wxMenuItem* menuFileSaveAs;
	menuFileSaveAs = new wxMenuItem( menuFile, wxID_ANY, wxString( wxT("Save As") ) , wxEmptyString, wxITEM_NORMAL );
	menuFile->Append( menuFileSaveAs );

	menuFile->AppendSeparator();

	wxMenuItem* menuFileQuit;
	menuFileQuit = new wxMenuItem( menuFile, wxID_ANY, wxString( wxT("Quit") ) , wxEmptyString, wxITEM_NORMAL );
	menuFile->Append( menuFileQuit );

	mMainMenu->Append( menuFile, wxT("File") );

	menuDatabase = new wxMenu();
	wxMenuItem* menuDatabaseDetector;
	menuDatabaseDetector = new wxMenuItem( menuDatabase, wxID_ANY, wxString( wxT("Detector Eficiency") ) , wxEmptyString, wxITEM_NORMAL );
	menuDatabase->Append( menuDatabaseDetector );

	wxMenuItem* menuDatabaseElements;
	menuDatabaseElements = new wxMenuItem( menuDatabase, wxID_ANY, wxString( wxT("Elements") ) , wxEmptyString, wxITEM_NORMAL );
	menuDatabase->Append( menuDatabaseElements );

	wxMenuItem* menuDatabaseZiegler;
	menuDatabaseZiegler = new wxMenuItem( menuDatabase, wxID_ANY, wxString( wxT("Ziegler Parameters") ) , wxEmptyString, wxITEM_NORMAL );
	menuDatabase->Append( menuDatabaseZiegler );

    menuDatabase->AppendSeparator();

    wxMenuItem* menuDatabaseSetup;
	menuDatabaseSetup = new wxMenuItem( menuDatabase, wxID_ANY, wxString( wxT("Setup Default Databases") ) , wxEmptyString, wxITEM_NORMAL );
	menuDatabase->Append( menuDatabaseSetup );

	mMainMenu->Append( menuDatabase, wxT("Database") );

	menuHelp = new wxMenu();
	wxMenuItem* menuHelpGuide;
	menuHelpGuide = new wxMenuItem( menuHelp, wxID_ANY, wxString( wxT("User Guide") ) , wxEmptyString, wxITEM_NORMAL );
	menuHelp->Append( menuHelpGuide );

	wxMenuItem* menuHelpAbout;
	menuHelpAbout = new wxMenuItem( menuHelp, wxID_ANY, wxString( wxT("About ERYA") ) , wxEmptyString, wxITEM_NORMAL );
	menuHelp->Append( menuHelpAbout );

	mMainMenu->Append( menuHelp, wxT("Help") );

	this->SetMenuBar( mMainMenu );

	sizerMainFrame = new wxBoxSizer( wxVERTICAL );

	tabbedOutput = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	tabElements = new wxPanel( tabbedOutput, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );

	sizerElements = new wxBoxSizer( wxVERTICAL );

    scrollButtons = new wxScrolledWindow( tabElements, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	scrollButtons->SetScrollRate( 5, 5 );

	sizerButtons = new wxFlexGridSizer( 0, 12, 30, 30 );

	wxFont TableFont(70, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString, wxFONTENCODING_DEFAULT);

    labelElements = new wxStaticText( scrollButtons, wxID_ANY, wxT("Element"), wxDefaultPosition, wxDefaultSize, 0 );
	labelElements->Wrap( -1 );
    labelElements->SetFont(TableFont);
	sizerButtons->Add( labelElements, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	labelGammaPeak = new wxStaticText( scrollButtons, wxID_ANY, wxT("Gamma Peak (keV)"), wxDefaultPosition, wxDefaultSize, 0 );
	labelGammaPeak->Wrap( -1 );
    labelGammaPeak->SetFont(TableFont);
	sizerButtons->Add( labelGammaPeak, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	labelFit = new wxStaticText( scrollButtons, wxID_ANY, wxT("Fit"), wxDefaultPosition, wxDefaultSize, 0 );
	labelFit->Wrap( -1 );
    labelFit->SetFont(TableFont);
	sizerButtons->Add( labelFit, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	labelZ = new wxStaticText( scrollButtons, wxID_ANY, wxT("Fixed Ratio\nGroup Number"), wxDefaultPosition, wxDefaultSize, 0 );
	labelZ->Wrap( -1 );
    labelZ->SetFont(TableFont);
	sizerButtons->Add( labelZ, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

        labelElementCalibrationParameter = new wxStaticText( scrollButtons, wxID_ANY, wxT("Cross-Section\nCalibration Parameter"), wxDefaultPosition, wxDefaultSize, 0 );
	labelElementCalibrationParameter->Wrap( -1 );
        labelElementCalibrationParameter->SetFont(TableFont);
	sizerButtons->Add( labelElementCalibrationParameter, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	labelStoichiometricGuess = new wxStaticText( scrollButtons, wxID_ANY, wxT("Stoichiometric\nInitial Guess"), wxDefaultPosition, wxDefaultSize, 0 );
	labelStoichiometricGuess->Wrap( -1 );
    labelStoichiometricGuess->SetFont(TableFont);
	sizerButtons->Add( labelStoichiometricGuess, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	labelYieldSimulation = new wxStaticText(scrollButtons, wxID_ANY, wxT("Yield\nInitial Guess"), wxDefaultPosition, wxDefaultSize, 0 );
	labelYieldSimulation->Wrap( -1 );
    labelYieldSimulation->SetFont(TableFont);
	sizerButtons->Add( labelYieldSimulation, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	labelYieldExperimental = new wxStaticText( scrollButtons, wxID_ANY, wxT("Experimental\nYield"), wxDefaultPosition, wxDefaultSize, 0 );
	labelYieldExperimental->Wrap( -1 );
    labelYieldExperimental->SetFont(TableFont);
	sizerButtons->Add( labelYieldExperimental, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	labelYieldFitted = new wxStaticText( scrollButtons, wxID_ANY, wxT("Fitted\nYield"), wxDefaultPosition, wxDefaultSize, 0 );
	labelYieldFitted->Wrap( -1 );
    labelYieldFitted->SetFont(TableFont);
	sizerButtons->Add( labelYieldFitted, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	labelStoichiometricFitted = new wxStaticText( scrollButtons, wxID_ANY, wxT("Fitted\nStoichiometry"), wxDefaultPosition, wxDefaultSize, 0 );
	labelStoichiometricFitted->Wrap( -1 );
    labelStoichiometricFitted->SetFont(TableFont);
	sizerButtons->Add( labelStoichiometricFitted, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	labelStoichiometricMass = new wxStaticText( scrollButtons, wxID_ANY, wxT("Stoichiometric\nFitted Mass"), wxDefaultPosition, wxDefaultSize, 0 );
	labelStoichiometricMass->Wrap( -1 );
    labelStoichiometricMass->SetFont(TableFont);
	sizerButtons->Add( labelStoichiometricMass, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	labelStoichiometricError = new wxStaticText( scrollButtons, wxID_ANY, wxT("Fitting\nError"), wxDefaultPosition, wxDefaultSize, 0 );
	labelStoichiometricError->Wrap( -1 );
    labelStoichiometricError->SetFont(TableFont);
	sizerButtons->Add( labelStoichiometricError, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );


    choiceElement.Add( new wxChoice( scrollButtons, wxID_ANY, wxDefaultPosition, wxSize(120,-1), choiceElementChoices, 0 ));
    choiceElement.Last()->SetSelection( 0 );
    sizerButtons->Add( choiceElement.Last(), 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

    choiceGP.Add(new wxChoice( scrollButtons, wxID_ANY, wxDefaultPosition, wxSize(120,-1), choiceGPChoices, 0 ));
    choiceGP.Last()->SetSelection( 0 );
    sizerButtons->Add( choiceGP.Last(), 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

    checkFit.Add(new wxCheckBox( scrollButtons, wxID_ANY, wxT("Fit ?"), wxDefaultPosition, wxDefaultSize, 0 ));
    sizerButtons->Add( checkFit.Last(), 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

    textZ.Add(new wxTextCtrl( scrollButtons, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(120,-1), 0 ));
    sizerButtons->Add( textZ.Last(), 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

    textCP.Add(new wxTextCtrl( scrollButtons, wxID_ANY, wxT("1"), wxDefaultPosition, wxSize(120,-1), 0 ));
    sizerButtons->Add( textCP.Last(), 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

    textSG.Add(new wxTextCtrl( scrollButtons, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(120,-1), 0 ));
    sizerButtons->Add( textSG.Last(), 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

    textYS.Add(new wxTextCtrl( scrollButtons, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(120,-1), wxTE_READONLY ));
    sizerButtons->Add( textYS.Last(), 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

    textYE.Add(new wxTextCtrl( scrollButtons, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(120,-1), 0 ));
    sizerButtons->Add( textYE.Last(), 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

    textYF.Add(new wxTextCtrl( scrollButtons, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(120,-1), wxTE_READONLY ));
    sizerButtons->Add( textYF.Last(), 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

    textSF.Add(new wxTextCtrl( scrollButtons, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(120,-1), wxTE_READONLY ));
    sizerButtons->Add( textSF.Last(), 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

    textSM.Add(new wxTextCtrl( scrollButtons, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(120,-1), wxTE_READONLY ));
    sizerButtons->Add( textSM.Last(), 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

    textSE.Add(new wxTextCtrl( scrollButtons, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(120,-1), wxTE_READONLY ));
    sizerButtons->Add( textSE.Last(), 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

    scrollButtons->SetSizer( sizerButtons );
	scrollButtons->Layout();
    sizerButtons->Fit( scrollButtons );
	sizerElements->Add( scrollButtons, 1, wxEXPAND, 5 );
    scrollButtons->Centre(wxBOTH);

	tabElements->SetSizer( sizerElements );
	tabElements->Layout();
	sizerElements->Fit( tabElements );
	tabbedOutput->AddPage( tabElements, wxT("Elements"), true );
	tabYieldSample = new wxPanel( tabbedOutput, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* sizerYieldSample;
	sizerYieldSample = new wxBoxSizer( wxVERTICAL );

	renderYield = new mpWindow( tabYieldSample, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	sizerYieldSample->Add( renderYield, 1, wxALL|wxEXPAND, 5 );

    tabDetectorFitting = new wxPanel( tabbedOutput, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* sizerDetectorFitting;
	sizerDetectorFitting = new wxBoxSizer( wxVERTICAL );
    renderDetectorFitting = new mpWindow( tabDetectorFitting, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	sizerDetectorFitting->Add( renderDetectorFitting, 1, wxALL|wxEXPAND, 5 );

    tabDetectorFitting->SetSizer( sizerDetectorFitting );
	tabDetectorFitting->Layout();
	sizerDetectorFitting->Fit( tabDetectorFitting );
	tabbedOutput->AddPage( tabDetectorFitting, wxT("Detector"), false );


	tabYieldSample->SetSizer( sizerYieldSample );
	tabYieldSample->Layout();
	sizerYieldSample->Fit( tabYieldSample );
	tabbedOutput->AddPage( tabYieldSample, wxT("Yields"), false );
	tabYieldEnergy = new wxPanel( tabbedOutput, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* sizerYieldEnergy;
	sizerYieldEnergy = new wxBoxSizer( wxVERTICAL );

	renderStoichiometry = new mpWindow( tabYieldEnergy, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	sizerYieldEnergy->Add( renderStoichiometry, 1, wxALL|wxEXPAND, 5 );


	tabYieldEnergy->SetSizer( sizerYieldEnergy );
	tabYieldEnergy->Layout();
	sizerYieldEnergy->Fit( tabYieldEnergy );
	tabbedOutput->AddPage( tabYieldEnergy, wxT("Stoichiometry"), false );
	tabFittingError = new wxPanel( tabbedOutput, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* sizerFittingError;
	sizerFittingError = new wxBoxSizer( wxVERTICAL );

	renderError = new mpWindow( tabFittingError, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	sizerFittingError->Add( renderError, 1, wxALL|wxEXPAND, 5 );


	tabFittingError->SetSizer( sizerFittingError );
	tabFittingError->Layout();
	sizerFittingError->Fit( tabFittingError );
	tabbedOutput->AddPage( tabFittingError, wxT("Fitting Error"), false );
        tabLogProfiling = new wxPanel( tabbedOutput, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* sizerLogProfiling;
	sizerLogProfiling = new wxBoxSizer( wxVERTICAL );

	tableLogProfiling = new wxGrid( tabLogProfiling, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );

	// Grid
	tableLogProfiling->CreateGrid( 1, 1 );
	tableLogProfiling->EnableEditing( false );
	tableLogProfiling->EnableGridLines( true );
	tableLogProfiling->EnableDragGridSize( false );
	tableLogProfiling->SetMargins( 0, 0 );

	// Columns
	tableLogProfiling->SetColSize( 0, 300 );
	tableLogProfiling->EnableDragColMove( true );
	tableLogProfiling->EnableDragColSize( true );
	tableLogProfiling->SetColLabelSize( 30 );
	tableLogProfiling->SetColLabelValue( 0, wxT("Dummy...") );
	tableLogProfiling->SetColLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );

	// Rows
	tableLogProfiling->EnableDragRowSize( true );
	tableLogProfiling->SetRowLabelSize( 80 );
	tableLogProfiling->SetRowLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );

	// Label Appearance

	// Cell Defaults
	tableLogProfiling->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_TOP );
	sizerLogProfiling->Add( tableLogProfiling, 1, wxEXPAND|wxALL, 5 );

	tabLogProfiling->SetSizer( sizerLogProfiling );
	tabLogProfiling->Layout();
	sizerLogProfiling->Fit( tabLogProfiling );
	tabbedOutput->AddPage( tabLogProfiling, wxT("Energy Profiling"), false );

	sizerMainFrame->Add( tabbedOutput, 1, wxEXPAND | wxALL, 5 );

	separatorMainFrame = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	sizerMainFrame->Add( separatorMainFrame, 0, wxEXPAND | wxALL, 5 );

	wxGridSizer* sizerMainButtons;
	sizerMainButtons = new wxGridSizer( 2, 11, 10, 10 );

    wxImage LibphysLogo = wxImage(libphys_xpm);
    wxStaticBitmap* pictureLibphysLogo = new wxStaticBitmap( this, wxID_ANY, wxBitmap(LibphysLogo), wxDefaultPosition, wxDefaultSize, 0 );
	sizerMainButtons->Add( pictureLibphysLogo, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );

    labelNumberElements = new wxStaticText( this, wxID_ANY, wxT("Number\nElements"), wxDefaultPosition, wxDefaultSize, 0 );
    labelNumberElements->SetFont(TableFont);
    labelNumberElements->SetForegroundColour(wxColour(64,64,128,wxALPHA_OPAQUE));
	labelNumberElements->Wrap( -1 );
	sizerMainButtons->Add( labelNumberElements, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );

	spinNumberElements = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 255, 1 );
	sizerMainButtons->Add( spinNumberElements, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	labelMinimumEnergy = new wxStaticText( this, wxID_ANY, wxT("Minimum\nEnergy(keV)"), wxDefaultPosition, wxDefaultSize, 0 );
    labelMinimumEnergy->SetFont(TableFont);
    labelMinimumEnergy->SetForegroundColour(wxColour(64,64,128,wxALPHA_OPAQUE));
	labelMinimumEnergy->Wrap( -1 );
	sizerMainButtons->Add( labelMinimumEnergy, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	textMinimumEnergy = new wxTextCtrl( this, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerMainButtons->Add( textMinimumEnergy, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	labelMaximumEnergy = new wxStaticText( this, wxID_ANY, wxT("Maximum\nEnergy(keV)"), wxDefaultPosition, wxDefaultSize, 0 );
    labelMaximumEnergy->SetFont(TableFont);
    labelMaximumEnergy->SetForegroundColour(wxColour(64,64,128,wxALPHA_OPAQUE));
	labelMaximumEnergy->Wrap( -1 );
	sizerMainButtons->Add( labelMaximumEnergy, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	textMaximumEnergy = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	sizerMainButtons->Add( textMaximumEnergy, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	labelStepSize = new wxStaticText( this, wxID_ANY, wxT("Step Size\n(keV)"), wxDefaultPosition, wxDefaultSize, 0 );
    labelStepSize->SetFont(TableFont);
    labelStepSize->SetForegroundColour(wxColour(64,64,128,wxALPHA_OPAQUE));
	labelStepSize->Wrap( -1 );
	sizerMainButtons->Add( labelStepSize, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	textStepSize = new wxTextCtrl( this, wxID_ANY, wxT("1"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerMainButtons->Add( textStepSize, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

    labelDetectorAngle = new wxStaticText( this, wxID_ANY, wxT("Profiling Step\n (keV)"), wxDefaultPosition, wxDefaultSize, 0 );
    labelDetectorAngle->SetFont(TableFont);
    labelDetectorAngle->SetForegroundColour(wxColour(64,64,128,wxALPHA_OPAQUE));
	labelDetectorAngle->Wrap( -1 );
	sizerMainButtons->Add( labelDetectorAngle, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	textDetectorAngle = new wxTextCtrl( this, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerMainButtons->Add( textDetectorAngle, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

    wxImage ERYALogo = wxImage(ERYA_Bulk_xpm);
    wxStaticBitmap* pictureERYALogo = new wxStaticBitmap( this, wxID_ANY, wxBitmap(ERYALogo), wxDefaultPosition, wxDefaultSize, 0 );
	sizerMainButtons->Add( pictureERYALogo, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );

	labelCharge = new wxStaticText( this, wxID_ANY, wxT("Charge\n(uC)"), wxDefaultPosition, wxDefaultSize, 0 );
    labelCharge->SetFont(TableFont);
    labelCharge->SetForegroundColour(wxColour(64,64,128,wxALPHA_OPAQUE));
	labelCharge->Wrap( -1 );
	sizerMainButtons->Add( labelCharge, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	textCharge = new wxTextCtrl( this, wxID_ANY, wxT("1"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerMainButtons->Add( textCharge, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	labelThickness = new wxStaticText( this, wxID_ANY, wxT("Thickness\n(ug/cm2)"), wxDefaultPosition, wxDefaultSize, 0 );
    labelThickness->SetFont(TableFont);
    labelThickness->SetForegroundColour(wxColour(64,64,128,wxALPHA_OPAQUE));
	labelThickness->Wrap( -1 );
	sizerMainButtons->Add( labelThickness, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	textThickness = new wxTextCtrl( this, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerMainButtons->Add( textThickness, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

    buttonMainNew = new wxButton( this, wxID_ANY, wxT("Clear Table"), wxDefaultPosition, wxDefaultSize, 0 );
    buttonMainNew->SetFont(TableFont);
    buttonMainNew->SetBackgroundColour(wxColour(96,128,176,wxALPHA_OPAQUE));
	sizerMainButtons->Add( buttonMainNew, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

    buttonMainCheck = new wxButton( this, wxID_ANY, wxT("Check Table"), wxDefaultPosition, wxDefaultSize, 0 );
    buttonMainCheck->SetFont(TableFont);
    buttonMainCheck->SetBackgroundColour(wxColour(96,128,176,wxALPHA_OPAQUE));
	sizerMainButtons->Add( buttonMainCheck, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

    buttonMainAdvanced = new wxButton( this, wxID_ANY, wxT("Advanced"), wxDefaultPosition, wxDefaultSize, 0 );
    buttonMainAdvanced->SetFont(TableFont);
    buttonMainAdvanced->SetBackgroundColour(wxColour(96,128,176,wxALPHA_OPAQUE));
	sizerMainButtons->Add( buttonMainAdvanced, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

    buttonMainHelp = new wxButton( this, wxID_ANY, wxT("Help"), wxDefaultPosition, wxDefaultSize, 0 );
    buttonMainHelp->SetFont(TableFont);
    buttonMainHelp->SetBackgroundColour(wxColour(96,128,176,wxALPHA_OPAQUE));
	sizerMainButtons->Add( buttonMainHelp, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	buttonMainFit = new wxButton( this, wxID_ANY, wxT("Run"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonMainFit->SetFont(TableFont);
    buttonMainFit->SetBackgroundColour(wxColour(96,128,176,wxALPHA_OPAQUE));
	sizerMainButtons->Add( buttonMainFit, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	buttonMainStop = new wxButton( this, wxID_ANY, wxT("Export Table"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonMainStop->SetFont(TableFont);
    buttonMainStop->SetBackgroundColour(wxColour(96,128,176,wxALPHA_OPAQUE));
	sizerMainButtons->Add( buttonMainStop, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

    sizerMainButtons->Fit(this);
	sizerMainFrame->Add( sizerMainButtons, 0, wxALIGN_CENTER_HORIZONTAL, 5 );


	this->SetSizer( sizerMainFrame );
	this->Layout();
	barMainStatus = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );

	this->Centre( wxBOTH );

    this->SetIcon(ERYA_xpm);
    this->SetBackgroundColour(wxColour(*wxWHITE));

	// Connect Events
	this->Connect( menuFileNew->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnFileNew ) );
	this->Connect( menuFileOpen->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnFileOpen ) );
	this->Connect( menuFileSave->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnFileSave ) );
	this->Connect( menuFileSaveAs->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnFileSaveAs ) );
	this->Connect( menuFileQuit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnFileQuit ) );
	this->Connect( menuDatabaseDetector->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnDatabaseDetector ) );
	this->Connect( menuDatabaseElements->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnDatabaseElements ) );
	this->Connect( menuDatabaseZiegler->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnDatabaseZiegler ) );
    this->Connect( menuDatabaseSetup->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnDatabaseSetup ) );
	this->Connect( menuHelpGuide->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnHelpGuide ) );
	this->Connect( menuHelpAbout->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnHelpAbout ) );
	choiceElement.Last()->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( MainFrame::OnElement ), NULL, this );
	choiceGP.Last()->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( MainFrame::OnGammaPeak ), NULL, this );
	spinNumberElements->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( MainFrame::OnNumberTrigger ), NULL, this );
    spinNumberElements->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( MainFrame::OnDirectNumberTrigger ), NULL, this );
    buttonMainNew->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnMainNew ), NULL, this );
    buttonMainCheck->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnMainCheck ), NULL, this );
    buttonMainAdvanced->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnMainAdvanced ), NULL, this );
    buttonMainHelp->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnMainHelp ), NULL, this );
	buttonMainFit->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnMainFit ), NULL, this );
	buttonMainStop->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnMainStop ), NULL, this );
}

MainFrame::~MainFrame()
{
	// Disconnect Events
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnFileNew ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnFileOpen ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnFileSave ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnFileSaveAs ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnFileQuit ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnDatabaseDetector ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnDatabaseElements ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnDatabaseZiegler ) );
    this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnDatabaseSetup ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnHelpGuide ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnHelpAbout ) );
	choiceElement.Last()->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( MainFrame::OnElement ), NULL, this );
	choiceGP.Last()->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( MainFrame::OnGammaPeak ), NULL, this );
	spinNumberElements->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( MainFrame::OnNumberTrigger ), NULL, this );
    spinNumberElements->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( MainFrame::OnDirectNumberTrigger ), NULL, this );
    buttonMainAdvanced->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnMainAdvanced ), NULL, this );
    buttonMainHelp->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnMainHelp ), NULL, this );
	buttonMainFit->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnMainFit ), NULL, this );
	buttonMainStop->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnMainStop ), NULL, this );

}

dialogDatabaseManager::dialogDatabaseManager( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* sizerDatabaseManager;
	sizerDatabaseManager = new wxBoxSizer( wxVERTICAL );

	wxGridSizer* sizerElements;
	sizerElements = new wxGridSizer( 2, 6, 0, 0 );

	labelElementName = new wxStaticText( this, wxID_ANY, wxT("Element Name"), wxDefaultPosition, wxDefaultSize, 0 );
	labelElementName->Wrap( -1 );
	sizerElements->Add( labelElementName, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	labelGammaPeak = new wxStaticText( this, wxID_ANY, wxT("Gamma Peak (keV)"), wxDefaultPosition, wxDefaultSize, 0 );
	labelGammaPeak->Wrap( -1 );
	sizerElements->Add( labelGammaPeak, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	labelAtomicNumber = new wxStaticText( this, wxID_ANY, wxT("Atomic Number"), wxDefaultPosition, wxDefaultSize, 0 );
	labelAtomicNumber->Wrap( -1 );
	sizerElements->Add( labelAtomicNumber, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	labelAbundance = new wxStaticText( this, wxID_ANY, wxT("Abundance"), wxDefaultPosition, wxDefaultSize, 0 );
	labelAbundance->Wrap( -1 );
	sizerElements->Add( labelAbundance, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	labelAtomicMass = new wxStaticText( this, wxID_ANY, wxT("Atomic Mass"), wxDefaultPosition, wxDefaultSize, 0 );
	labelAtomicMass->Wrap( -1 );
	sizerElements->Add( labelAtomicMass, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	labelIsotopicMass = new wxStaticText( this, wxID_ANY, wxT("Isotopic Mass"), wxDefaultPosition, wxDefaultSize, 0 );
	labelIsotopicMass->Wrap( -1 );
	sizerElements->Add( labelIsotopicMass, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	choiceElementName = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxSize(120,-1), choiceElementNameChoices, 0 );
	choiceElementName->SetSelection( 0 );
	sizerElements->Add( choiceElementName, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	choiceGammaPeak = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxSize(120,-1), choiceGammaPeakChoices, 0 );
	choiceGammaPeak->SetSelection( 0 );
	sizerElements->Add( choiceGammaPeak, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	textAtomicNumber = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	sizerElements->Add( textAtomicNumber, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	textAbundance = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	sizerElements->Add( textAbundance, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	textAtomicMass = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	sizerElements->Add( textAtomicMass, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	textIsotopicMass = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	sizerElements->Add( textIsotopicMass, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );


    sizerDatabaseManager->Add( sizerElements, 0, wxEXPAND, 5 );

	separatorDisplay = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	sizerDatabaseManager->Add( separatorDisplay, 0, wxEXPAND | wxALL, 5 );

	wxFlexGridSizer* sizerElementDisplay;
	sizerElementDisplay = new wxFlexGridSizer( 1, 2, 0, 0 );

	sizerElementDisplay->AddGrowableCol(1);

    wxBoxSizer* sizerElementGrid;
	sizerElementGrid = new wxBoxSizer( wxVERTICAL );

    wxBoxSizer* sizerElementGraph;
	sizerElementGraph = new wxBoxSizer( wxVERTICAL );

	tableElementViewer = new wxGrid( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );

	// Grid
	tableElementViewer->CreateGrid( 1000, 2 );
	tableElementViewer->EnableEditing( false );
	tableElementViewer->EnableGridLines( true );
	tableElementViewer->EnableDragGridSize( false );
	tableElementViewer->SetMargins( 0, 0 );

	// Columns
	tableElementViewer->SetColSize( 0, 200 );
	tableElementViewer->SetColSize( 1, 200 );
	tableElementViewer->EnableDragColMove( false );
	tableElementViewer->EnableDragColSize( true );
	tableElementViewer->SetColLabelSize( 30 );
	tableElementViewer->SetColLabelValue( 0, wxT("Energy (keV)") );
	tableElementViewer->SetColLabelValue( 1, wxT("Cross Section (mbarn)") );
	tableElementViewer->SetColLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );

	// Rows
	tableElementViewer->EnableDragRowSize( true );
	tableElementViewer->SetRowLabelSize( 80 );
	tableElementViewer->SetRowLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );

	// Label Appearance

	// Cell Defaults
	tableElementViewer->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_TOP );
	sizerElementGrid->Add( tableElementViewer, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

    sizerElementDisplay->Add( sizerElementGrid, 1, wxEXPAND, 5 );

	renderElementViewer = new mpWindow( this, wxID_ANY,  wxDefaultPosition, wxDefaultSize, 0 );
	sizerElementGraph->Add( renderElementViewer, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );

    sizerElementDisplay->Add( sizerElementGraph, 1, wxEXPAND, 5 );

	sizerDatabaseManager->Add( sizerElementDisplay, 1, wxEXPAND, 5 );

	separatorButtons = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	sizerDatabaseManager->Add( separatorButtons, 0, wxEXPAND | wxALL, 5 );


	wxGridSizer* sizerButtons;
	sizerButtons = new wxGridSizer( 1, 8, 0, 0 );

	buttonAdd = new wxButton( this, wxID_ANY, wxT("Add"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerButtons->Add( buttonAdd, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	buttonEdit = new wxButton( this, wxID_ANY, wxT("Edit"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerButtons->Add( buttonEdit, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	buttonRemove = new wxButton( this, wxID_ANY, wxT("Remove"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerButtons->Add( buttonRemove, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	buttonClear = new wxButton( this, wxID_ANY, wxT("Clear"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerButtons->Add( buttonClear, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	buttonLoad = new wxButton( this, wxID_ANY, wxT("Load"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerButtons->Add( buttonLoad, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	buttonSave = new wxButton( this, wxID_ANY, wxT("Save"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerButtons->Add( buttonSave, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	buttonCancel = new wxButton( this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerButtons->Add( buttonCancel, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	buttonHelp = new wxButton( this, wxID_ANY, wxT("Help"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerButtons->Add( buttonHelp, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );


	sizerDatabaseManager->Add( sizerButtons, 0, wxEXPAND, 5 );

	this->SetSizer( sizerDatabaseManager );
	this->Layout();

	this->Centre( wxBOTH );

    this->SetIcon(ERYA_xpm);

	// Connect Events
	choiceElementName->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( dialogDatabaseManager::OnElementName ), NULL, this );
	choiceGammaPeak->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( dialogDatabaseManager::OnGammaPeak ), NULL, this );
	buttonAdd->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogDatabaseManager::OnDatabaseAdd ), NULL, this );
	buttonEdit->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogDatabaseManager::OnDatabaseEdit ), NULL, this );
	buttonRemove->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogDatabaseManager::OnDatabaseRemove ), NULL, this );
	buttonClear->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogDatabaseManager::OnDatabaseClear ), NULL, this );
	buttonLoad->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogDatabaseManager::OnDatabaseLoad ), NULL, this );
	buttonSave->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogDatabaseManager::OnDatabaseSave ), NULL, this );
	buttonCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogDatabaseManager::OnDatabaseCancel ), NULL, this );
	buttonHelp->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogDatabaseManager::OnDatabaseHelp ), NULL, this );
}

dialogDatabaseManager::~dialogDatabaseManager()
{
	// Disconnect Events
	choiceElementName->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( dialogDatabaseManager::OnElementName ), NULL, this );
	choiceGammaPeak->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( dialogDatabaseManager::OnGammaPeak ), NULL, this );
	buttonAdd->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogDatabaseManager::OnDatabaseAdd ), NULL, this );
	buttonEdit->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogDatabaseManager::OnDatabaseEdit ), NULL, this );
	buttonRemove->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogDatabaseManager::OnDatabaseRemove ), NULL, this );
	buttonClear->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogDatabaseManager::OnDatabaseClear ), NULL, this );
	buttonLoad->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogDatabaseManager::OnDatabaseLoad ), NULL, this );
	buttonSave->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogDatabaseManager::OnDatabaseSave ), NULL, this );
	buttonCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogDatabaseManager::OnDatabaseCancel ), NULL, this );
	buttonHelp->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogDatabaseManager::OnDatabaseHelp ), NULL, this );

}

dialogAddDatabase::dialogAddDatabase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* sizerDatabaseEditor;
	sizerDatabaseEditor = new wxBoxSizer( wxVERTICAL );

	wxGridSizer* sizerEditor;
	sizerEditor = new wxGridSizer( 2, 6, 0, 0 );

	labelEditElement = new wxStaticText( this, wxID_ANY, wxT("Element Name"), wxDefaultPosition, wxDefaultSize, 0 );
	labelEditElement->Wrap( -1 );
	sizerEditor->Add( labelEditElement, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	labelEditGamma = new wxStaticText( this, wxID_ANY, wxT("Gamma Peak (keV)"), wxDefaultPosition, wxDefaultSize, 0 );
	labelEditGamma->Wrap( -1 );
	sizerEditor->Add( labelEditGamma, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	labelEditNumber = new wxStaticText( this, wxID_ANY, wxT("Atomic Number"), wxDefaultPosition, wxDefaultSize, 0 );
	labelEditNumber->Wrap( -1 );
	sizerEditor->Add( labelEditNumber, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	labelEditAbundance = new wxStaticText( this, wxID_ANY, wxT("Abundance"), wxDefaultPosition, wxDefaultSize, 0 );
	labelEditAbundance->Wrap( -1 );
	sizerEditor->Add( labelEditAbundance, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	labelEditAtomic = new wxStaticText( this, wxID_ANY, wxT("Atomic Mass"), wxDefaultPosition, wxDefaultSize, 0 );
	labelEditAtomic->Wrap( -1 );
	sizerEditor->Add( labelEditAtomic, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	labelEditIsotopic = new wxStaticText( this, wxID_ANY, wxT("Isotopic Mass"), wxDefaultPosition, wxDefaultSize, 0 );
	labelEditIsotopic->Wrap( -1 );
	sizerEditor->Add( labelEditIsotopic, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	textEditElement = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	sizerEditor->Add( textEditElement, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	textEditGamma = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	sizerEditor->Add( textEditGamma, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	textEditNumber = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	sizerEditor->Add( textEditNumber, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	textEditAbundance = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	sizerEditor->Add( textEditAbundance, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	textEditAtomic = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	sizerEditor->Add( textEditAtomic, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	textEditIsotopic = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	sizerEditor->Add( textEditIsotopic, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );


	sizerDatabaseEditor->Add( sizerEditor, 0, wxEXPAND, 5 );

	separatorEditor = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	sizerDatabaseEditor->Add( separatorEditor, 0, wxEXPAND | wxALL, 5 );

	wxGridSizer* sizerTable;
	sizerTable = new wxGridSizer( 1, 1, 0, 0 );

	tableDataEditor = new wxGrid( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );

	// Grid
	tableDataEditor->CreateGrid( 2000, 4 );
	tableDataEditor->EnableEditing( true );
	tableDataEditor->EnableGridLines( true );
	tableDataEditor->EnableDragGridSize( false );
	tableDataEditor->SetMargins( 0, 0 );

	// Columns
	tableDataEditor->SetColSize( 0, 300 );
	tableDataEditor->SetColSize( 1, 300 );
	tableDataEditor->SetColSize( 2, 300 );
	tableDataEditor->SetColSize( 3, 300 );
	tableDataEditor->EnableDragColMove( true );
	tableDataEditor->EnableDragColSize( true );
	tableDataEditor->SetColLabelSize( 30 );
	tableDataEditor->SetColLabelValue( 0, wxT("Energy") );
	tableDataEditor->SetColLabelValue( 1, wxT("Energy Error") );
	tableDataEditor->SetColLabelValue( 2, wxT("Cross Section") );
	tableDataEditor->SetColLabelValue( 3, wxT("Cross Section Error") );
	tableDataEditor->SetColLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );

	// Rows
	tableDataEditor->EnableDragRowSize( true );
	tableDataEditor->SetRowLabelSize( 80 );
	tableDataEditor->SetRowLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );

	// Label Appearance

	// Cell Defaults
	tableDataEditor->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_TOP );
	sizerTable->Add( tableDataEditor, 0, wxALL|wxEXPAND, 5 );


	sizerDatabaseEditor->Add( sizerTable, 1, wxEXPAND, 5 );

	separatorTable = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	sizerDatabaseEditor->Add( separatorTable, 0, wxEXPAND | wxALL, 5 );

	wxGridSizer* sizerEditButtons;
	sizerEditButtons = new wxGridSizer( 1, 8, 0, 0 );

	buttonImportR33 = new wxButton( this, wxID_ANY, wxT("Import R33"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerEditButtons->Add( buttonImportR33, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	buttonExportR33 = new wxButton( this, wxID_ANY, wxT("Export R33"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerEditButtons->Add( buttonExportR33, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

    buttonEditCopy = new wxButton( this, wxID_ANY, wxT("Copy"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerEditButtons->Add( buttonEditCopy, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	buttonEditPaste = new wxButton( this, wxID_ANY, wxT("Paste"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerEditButtons->Add( buttonEditPaste, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	buttonEditClear = new wxButton( this, wxID_ANY, wxT("Clear"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerEditButtons->Add( buttonEditClear, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	buttonEditSave = new wxButton( this, wxID_ANY, wxT("Save"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerEditButtons->Add( buttonEditSave, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	buttonEditCancel = new wxButton( this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerEditButtons->Add( buttonEditCancel, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	buttonEditHelp = new wxButton( this, wxID_ANY, wxT("Help"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerEditButtons->Add( buttonEditHelp, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );


	sizerDatabaseEditor->Add( sizerEditButtons, 0, wxEXPAND, 5 );


	this->SetSizer( sizerDatabaseEditor );
	this->Layout();

	this->Centre( wxBOTH );

    // Clipboard text variable
    dataClipboard = new wxString();

    this->SetIcon(ERYA_xpm);

	// Connect Events
	tableDataEditor->Connect( wxEVT_GRID_RANGE_SELECT, wxGridRangeSelectEventHandler( dialogAddDatabase::OnDatabaseSelect ), NULL, this );
	buttonImportR33->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogAddDatabase::OnImportR33 ), NULL, this );
	buttonExportR33->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogAddDatabase::OnExportR33 ), NULL, this );
	buttonEditCopy->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogAddDatabase::OnDatabaseCopy ), NULL, this );
	buttonEditPaste->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogAddDatabase::OnDatabasePaste ), NULL, this );
	buttonEditClear->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogAddDatabase::OnEditClear ), NULL, this );
	buttonEditSave->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogAddDatabase::OnEditSave ), NULL, this );
	buttonEditCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogAddDatabase::OnEditCancel ), NULL, this );
	buttonEditHelp->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogAddDatabase::OnEditHelp ), NULL, this );
}

dialogAddDatabase::~dialogAddDatabase()
{
	// Disconnect Events
	tableDataEditor->Disconnect( wxEVT_GRID_RANGE_SELECT, wxGridRangeSelectEventHandler( dialogAddDatabase::OnDatabaseSelect ), NULL, this );
	buttonImportR33->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogAddDatabase::OnImportR33 ), NULL, this );
	buttonExportR33->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogAddDatabase::OnExportR33 ), NULL, this );
	buttonEditCopy->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogAddDatabase::OnDatabaseCopy ), NULL, this );
	buttonEditPaste->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogAddDatabase::OnDatabasePaste ), NULL, this );
	buttonEditClear->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogAddDatabase::OnEditClear ), NULL, this );
	buttonEditSave->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogAddDatabase::OnEditSave ), NULL, this );
	buttonEditCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogAddDatabase::OnEditCancel ), NULL, this );
	buttonEditHelp->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogAddDatabase::OnEditHelp ), NULL, this );

}

dialogDetectorSetup::dialogDetectorSetup( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* sizerDetectorSetup;
	sizerDetectorSetup = new wxBoxSizer( wxVERTICAL );

	labelFunctionEficiency = new wxStaticText( this, wxID_ANY, wxT("Input the desired algebraic function that models your detector efficiency:"), wxDefaultPosition, wxDefaultSize, 0 );
	labelFunctionEficiency->Wrap( -1 );
	sizerDetectorSetup->Add( labelFunctionEficiency, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxEXPAND, 5 );

	textFunctionEficiency = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	sizerDetectorSetup->Add( textFunctionEficiency, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 5 );

	separatorFunctionEficiency = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	sizerDetectorSetup->Add( separatorFunctionEficiency, 0, wxEXPAND | wxALL, 5 );

	labelConstantsEficiency = new wxStaticText( this, wxID_ANY, wxT("Or export a file, and edit the following table:"), wxDefaultPosition, wxDefaultSize, 0 );
	labelConstantsEficiency->Wrap( -1 );
	sizerDetectorSetup->Add( labelConstantsEficiency, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxEXPAND, 5 );

	tableConstantEficiency = new wxGrid( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );

	// Grid
	tableConstantEficiency->CreateGrid( 2000, 2 );
	tableConstantEficiency->EnableEditing( true );
	tableConstantEficiency->EnableGridLines( true );
	tableConstantEficiency->EnableDragGridSize( false );
	tableConstantEficiency->SetMargins( 0, 0 );

	// Columns
	tableConstantEficiency->SetColSize( 0, 550 );
	tableConstantEficiency->SetColSize( 1, 550 );
	tableConstantEficiency->EnableDragColMove( false );
	tableConstantEficiency->EnableDragColSize( true );
	tableConstantEficiency->SetColLabelSize( 30 );
	tableConstantEficiency->SetColLabelValue( 0, wxT("Energy (keV)") );
	tableConstantEficiency->SetColLabelValue( 1, wxT("Absolute Efficiency") );
	tableConstantEficiency->SetColLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );

	// Rows
	tableConstantEficiency->EnableDragRowSize( true );
	tableConstantEficiency->SetRowLabelSize( 80 );
	tableConstantEficiency->SetRowLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );

	// Label Appearance

	// Cell Defaults
	tableConstantEficiency->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_TOP );
	sizerDetectorSetup->Add( tableConstantEficiency, 1, wxALL, 5 );

	separatorButtonEficiency = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	sizerDetectorSetup->Add( separatorButtonEficiency, 0, wxEXPAND | wxALL, 5 );

	wxGridSizer* sizerButtonSetup;
	sizerButtonSetup = new wxGridSizer( 1, 8, 0, 0 );

	buttonEficiencyLoad = new wxButton( this, wxID_ANY, wxT("Import"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerButtonSetup->Add( buttonEficiencyLoad, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	buttonEficiencySave = new wxButton( this, wxID_ANY, wxT("Export"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerButtonSetup->Add( buttonEficiencySave, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

    buttonEficiencyCopy = new wxButton( this, wxID_ANY, wxT("Copy"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerButtonSetup->Add( buttonEficiencyCopy, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	buttonEficiencyPaste = new wxButton( this, wxID_ANY, wxT("Paste"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerButtonSetup->Add( buttonEficiencyPaste, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	buttonEficiencyClear = new wxButton( this, wxID_ANY, wxT("Clear"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerButtonSetup->Add( buttonEficiencyClear, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	buttonEficiencyOK = new wxButton( this, wxID_ANY, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerButtonSetup->Add( buttonEficiencyOK, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	buttonEficiencyCancel = new wxButton( this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerButtonSetup->Add( buttonEficiencyCancel, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	buttonEficiencyHelp = new wxButton( this, wxID_ANY, wxT("Help"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerButtonSetup->Add( buttonEficiencyHelp, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );


	sizerDetectorSetup->Add( sizerButtonSetup, 0, wxEXPAND, 5 );


	this->SetSizer( sizerDetectorSetup );
	this->Layout();

	this->Centre( wxBOTH );

    // Clipboard text variable
    dataClipboard = new wxString();

    this->SetIcon(ERYA_xpm);

	// Connect Events
	tableConstantEficiency->Connect( wxEVT_GRID_RANGE_SELECT, wxGridRangeSelectEventHandler( dialogDetectorSetup::OnEficiencySelect ), NULL, this );
    buttonEficiencyCopy->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogDetectorSetup::OnEficiencyCopy ), NULL, this );
	buttonEficiencyPaste->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogDetectorSetup::OnEficiencyPaste ), NULL, this );
	buttonEficiencyLoad->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogDetectorSetup::OnEficiencyLoad ), NULL, this );
	buttonEficiencySave->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogDetectorSetup::OnEficiencySave ), NULL, this );
	buttonEficiencyClear->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogDetectorSetup::OnEficiencyClear ), NULL, this );
	buttonEficiencyOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogDetectorSetup::OnEficiencyOK ), NULL, this );
	buttonEficiencyCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogDetectorSetup::OnEficiencyCancel ), NULL, this );
	buttonEficiencyHelp->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogDetectorSetup::OnEficiencyHelp ), NULL, this );
}

dialogDetectorSetup::~dialogDetectorSetup()
{
	// Disconnect Events
	tableConstantEficiency->Disconnect( wxEVT_GRID_RANGE_SELECT, wxGridRangeSelectEventHandler( dialogDetectorSetup::OnEficiencySelect ), NULL, this );
    buttonEficiencyCopy->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogDetectorSetup::OnEficiencyCopy ), NULL, this );
	buttonEficiencyPaste->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogDetectorSetup::OnEficiencyPaste ), NULL, this );
	buttonEficiencyLoad->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogDetectorSetup::OnEficiencyLoad ), NULL, this );
	buttonEficiencySave->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogDetectorSetup::OnEficiencySave ), NULL, this );
	buttonEficiencyClear->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogDetectorSetup::OnEficiencyClear ), NULL, this );
	buttonEficiencyOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogDetectorSetup::OnEficiencyOK ), NULL, this );
	buttonEficiencyCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogDetectorSetup::OnEficiencyCancel ), NULL, this );
	buttonEficiencyHelp->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogDetectorSetup::OnEficiencyHelp ), NULL, this );

}

dialogZieglerParameters::dialogZieglerParameters( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* sizerZieglerParameters;
	sizerZieglerParameters = new wxBoxSizer( wxVERTICAL );

	labelZieglerParameters = new wxStaticText( this, wxID_ANY, wxT("It's necessary to fill manually the following spreadsheets or import the necessary files, to model the Stopping-Power (Click \"Help\" for more detail.)"), wxDefaultPosition, wxDefaultSize, 0 );
	labelZieglerParameters->Wrap( -1 );
	sizerZieglerParameters->Add( labelZieglerParameters, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 5 );

	lineZieglerVersion = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	sizerZieglerParameters->Add( lineZieglerVersion, 0, wxEXPAND | wxALL, 5 );

	wxFlexGridSizer* selectorZieglerVersion;
	selectorZieglerVersion = new wxFlexGridSizer( 1, 2, 0, 0 );
	selectorZieglerVersion->AddGrowableCol( 0 );
	selectorZieglerVersion->SetFlexibleDirection( wxBOTH );
	selectorZieglerVersion->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	labelZieglerVersion = new wxStaticText( this, wxID_ANY, wxT("Select the correct option from the following list options, to model the Ziegler's or SRIM parameters."), wxDefaultPosition, wxDefaultSize, 0 );
	labelZieglerVersion->Wrap( -1 );
	selectorZieglerVersion->Add( labelZieglerVersion, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );

	wxString choiceZieglerVersionChoices[] = { wxT("1977/1983 Version (12 Parameters)"), wxT("1991/2000 Version (8 Parameters)"), wxT("Custom Interpolation Formula (Algebraic)"), wxT("SRIM Experimental Data (Interpolation)"), wxEmptyString };
	int choiceZieglerVersionNChoices = sizeof( choiceZieglerVersionChoices ) / sizeof( wxString );
	choiceZieglerVersion = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, choiceZieglerVersionNChoices, choiceZieglerVersionChoices, 0 );
	choiceZieglerVersion->SetSelection( 0 );
	selectorZieglerVersion->Add( choiceZieglerVersion, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );


	sizerZieglerParameters->Add( selectorZieglerVersion, 0, wxEXPAND, 5 );

	lineZieglerTables = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	sizerZieglerParameters->Add( lineZieglerTables, 0, wxEXPAND | wxALL, 5 );

	labelZieglerFunction = new wxStaticText( this, wxID_ANY, wxT("To override the built-in Ziegler Functions and SRIM tables, while reading the tables values, you can insert on the box blow, a custom ERYA macro function (See \"Help\" for more details)."), wxDefaultPosition, wxDefaultSize, 0 );
	labelZieglerFunction->Wrap( -1 );
	sizerZieglerParameters->Add( labelZieglerFunction, 1, wxALL|wxEXPAND, 5 );

	textZieglerFunction = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	sizerZieglerParameters->Add( textZieglerFunction, 0, wxALL|wxEXPAND, 5 );

	tabZieglerTables = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	tabZieglerEquations = new wxPanel( tabZieglerTables, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* sizerZieglerEquationsTable;
	sizerZieglerEquationsTable = new wxBoxSizer( wxVERTICAL );

	tableZieglerParameters = new wxGrid( tabZieglerEquations, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );

	// Grid
	tableZieglerParameters->CreateGrid( 200, 14 );
	tableZieglerParameters->EnableEditing( true );
	tableZieglerParameters->EnableGridLines( true );
	tableZieglerParameters->EnableDragGridSize( false );
	tableZieglerParameters->SetMargins( 0, 0 );

	// Columns
	tableZieglerParameters->SetColSize( 0, 180 );
	tableZieglerParameters->SetColSize( 1, 180 );
	tableZieglerParameters->SetColSize( 2, 180 );
	tableZieglerParameters->SetColSize( 3, 180 );
	tableZieglerParameters->SetColSize( 4, 180 );
	tableZieglerParameters->SetColSize( 5, 180 );
	tableZieglerParameters->SetColSize( 6, 180 );
	tableZieglerParameters->SetColSize( 7, 180 );
	tableZieglerParameters->SetColSize( 8, 180 );
	tableZieglerParameters->SetColSize( 9, 180 );
	tableZieglerParameters->SetColSize( 10, 180 );
	tableZieglerParameters->SetColSize( 11, 180 );
	tableZieglerParameters->SetColSize( 12, 180 );
	tableZieglerParameters->SetColSize( 13, 180 );
	tableZieglerParameters->EnableDragColMove( false );
	tableZieglerParameters->EnableDragColSize( true );
	tableZieglerParameters->SetColLabelSize( 30 );
	tableZieglerParameters->SetColLabelValue( 0, wxT("Element") );
	tableZieglerParameters->SetColLabelValue( 1, wxT("A-1") );
	tableZieglerParameters->SetColLabelValue( 2, wxT("A-2") );
	tableZieglerParameters->SetColLabelValue( 3, wxT("A-3") );
	tableZieglerParameters->SetColLabelValue( 4, wxT("A-4") );
	tableZieglerParameters->SetColLabelValue( 5, wxT("A-5") );
	tableZieglerParameters->SetColLabelValue( 6, wxT("A-6") );
	tableZieglerParameters->SetColLabelValue( 7, wxT("A-7") );
	tableZieglerParameters->SetColLabelValue( 8, wxT("A-8") );
	tableZieglerParameters->SetColLabelValue( 9, wxT("A-9") );
	tableZieglerParameters->SetColLabelValue( 10, wxT("A-10") );
	tableZieglerParameters->SetColLabelValue( 11, wxT("A-11") );
	tableZieglerParameters->SetColLabelValue( 12, wxT("A-12") );
	tableZieglerParameters->SetColLabelValue( 13, wxT("Atomic Mass") );
	tableZieglerParameters->SetColLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );

	// Rows
	tableZieglerParameters->EnableDragRowSize( true );
	tableZieglerParameters->SetRowLabelSize( 80 );
	tableZieglerParameters->SetRowLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );

	// Label Appearance

	// Cell Defaults
	tableZieglerParameters->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_TOP );
	sizerZieglerEquationsTable->Add( tableZieglerParameters, 1, wxALL|wxEXPAND, 5 );


	tabZieglerEquations->SetSizer( sizerZieglerEquationsTable );
	tabZieglerEquations->Layout();
	sizerZieglerEquationsTable->Fit( tabZieglerEquations );
	tabZieglerTables->AddPage( tabZieglerEquations, wxT("Ziegler Equations Parameters"), false );
	tabZieglerSRIM = new wxPanel( tabZieglerTables, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* sizerZieglerSRIMTable;
	sizerZieglerSRIMTable = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* sizerZieglerSRIMSelector;
	sizerZieglerSRIMSelector = new wxFlexGridSizer( 1, 4, 0, 0 );
	sizerZieglerSRIMSelector->AddGrowableCol( 0 );
	sizerZieglerSRIMSelector->SetFlexibleDirection( wxBOTH );
	sizerZieglerSRIMSelector->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	labelZieglerSRIMCurrentElement = new wxStaticText( tabZieglerSRIM, wxID_ANY, wxT("The SRIM data related to Atomic Number Element selected, are displayed here:"), wxDefaultPosition, wxDefaultSize, 0 );
	labelZieglerSRIMCurrentElement->Wrap( -1 );
	sizerZieglerSRIMSelector->Add( labelZieglerSRIMCurrentElement, 0, wxALL|wxEXPAND, 5 );

	spinZieglerSRIMCurrentElement = new wxSpinCtrl( tabZieglerSRIM, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 200, 1 );
	sizerZieglerSRIMSelector->Add( spinZieglerSRIMCurrentElement, 0, wxALL, 5 );

	buttonZieglerSRIMUpdate = new wxButton( tabZieglerSRIM, wxID_ANY, wxT("Update Element"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerZieglerSRIMSelector->Add( buttonZieglerSRIMUpdate, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );

	buttonZieglerSRIMEraseAllData = new wxButton( tabZieglerSRIM, wxID_ANY, wxT("Erase All Tables"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerZieglerSRIMSelector->Add( buttonZieglerSRIMEraseAllData, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );


	sizerZieglerSRIMTable->Add( sizerZieglerSRIMSelector, 0, wxEXPAND, 5 );

	tableZieglerSRIM = new wxGrid( tabZieglerSRIM, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );

	// Grid
	tableZieglerSRIM->CreateGrid( 1000, 2 );
	tableZieglerSRIM->EnableEditing( true );
	tableZieglerSRIM->EnableGridLines( true );
	tableZieglerSRIM->EnableDragGridSize( false );
	tableZieglerSRIM->SetMargins( 0, 0 );

	// Columns
	tableZieglerSRIM->SetColSize( 0, 600 );
	tableZieglerSRIM->SetColSize( 1, 600 );
	tableZieglerSRIM->EnableDragColMove( false );
	tableZieglerSRIM->EnableDragColSize( true );
	tableZieglerSRIM->SetColLabelSize( 30 );
	tableZieglerSRIM->SetColLabelValue( 0, wxT("Energy (keV)") );
	tableZieglerSRIM->SetColLabelValue( 1, wxT("Stopping Power (ev*10^15 atm/cm^2)") );
	tableZieglerSRIM->SetColLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );

	// Rows
	tableZieglerSRIM->EnableDragRowSize( true );
	tableZieglerSRIM->SetRowLabelSize( 80 );
	tableZieglerSRIM->SetRowLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );

	// Label Appearance

	// Cell Defaults
	tableZieglerSRIM->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_TOP );
	sizerZieglerSRIMTable->Add( tableZieglerSRIM, 1, wxALL|wxEXPAND, 5 );


	tabZieglerSRIM->SetSizer( sizerZieglerSRIMTable );
	tabZieglerSRIM->Layout();
	sizerZieglerSRIMTable->Fit( tabZieglerSRIM );
	tabZieglerTables->AddPage( tabZieglerSRIM, wxT("SRIM Stopping Power Tables"), true );

	sizerZieglerParameters->Add( tabZieglerTables, 1, wxALL, 5 );

	lineZieglerParameters = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	sizerZieglerParameters->Add( lineZieglerParameters, 0, wxALL|wxEXPAND, 5 );

	wxGridSizer* sizerZieglerButtons;
	sizerZieglerButtons = new wxGridSizer( 1, 8, 0, 0 );

	buttonZieglerLoad = new wxButton( this, wxID_ANY, wxT("Import"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerZieglerButtons->Add( buttonZieglerLoad, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	buttonZieglerSave = new wxButton( this, wxID_ANY, wxT("Export"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerZieglerButtons->Add( buttonZieglerSave, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	buttonZieglerCopy = new wxButton( this, wxID_ANY, wxT("Copy"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerZieglerButtons->Add( buttonZieglerCopy, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	buttonZieglerPaste = new wxButton( this, wxID_ANY, wxT("Paste"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerZieglerButtons->Add( buttonZieglerPaste, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	buttonZieglerClear = new wxButton( this, wxID_ANY, wxT("Clear"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerZieglerButtons->Add( buttonZieglerClear, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	buttonZieglerOK = new wxButton( this, wxID_ANY, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerZieglerButtons->Add( buttonZieglerOK, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	buttonZieglerCancel = new wxButton( this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerZieglerButtons->Add( buttonZieglerCancel, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	buttonZieglerHelp = new wxButton( this, wxID_ANY, wxT("Help"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerZieglerButtons->Add( buttonZieglerHelp, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );


	sizerZieglerParameters->Add( sizerZieglerButtons, 0, wxEXPAND, 5 );


	this->SetSizer( sizerZieglerParameters );
	this->Layout();

	this->Centre( wxBOTH );

        // Clipboard text variable
        dataClipboard = new wxString();

	// Connect Events
	spinZieglerSRIMCurrentElement->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( dialogZieglerParameters::OnZieglerSelectElement ), NULL, this );
	spinZieglerSRIMCurrentElement->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( dialogZieglerParameters::OnZieglerSelectElementText ), NULL, this );
	spinZieglerSRIMCurrentElement->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( dialogZieglerParameters::OnZieglerSelectElementEnter ), NULL, this );
        buttonZieglerSRIMUpdate->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogZieglerParameters::OnUpdateZieglerSRIM ), NULL, this );
	buttonZieglerSRIMEraseAllData->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogZieglerParameters::OnEraseZieglerSRIM ), NULL, this );
	buttonZieglerLoad->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogZieglerParameters::OnZieglerLoad ), NULL, this );
	buttonZieglerSave->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogZieglerParameters::OnZieglerSave ), NULL, this );
	buttonZieglerCopy->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogZieglerParameters::OnZieglerCopy ), NULL, this );
	buttonZieglerPaste->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogZieglerParameters::OnZieglerPaste ), NULL, this );
	buttonZieglerClear->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogZieglerParameters::OnZieglerClear ), NULL, this );
	buttonZieglerOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogZieglerParameters::OnZieglerOK ), NULL, this );
	buttonZieglerCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogZieglerParameters::OnZieglerCancel ), NULL, this );
	buttonZieglerHelp->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogZieglerParameters::OnZieglerHelp ), NULL, this );
}

dialogZieglerParameters::~dialogZieglerParameters()
{
	// Disconnect Events
	spinZieglerSRIMCurrentElement->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( dialogZieglerParameters::OnZieglerSelectElement ), NULL, this );
	spinZieglerSRIMCurrentElement->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( dialogZieglerParameters::OnZieglerSelectElementText ), NULL, this );
	spinZieglerSRIMCurrentElement->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( dialogZieglerParameters::OnZieglerSelectElementEnter ), NULL, this );
        buttonZieglerSRIMUpdate->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogZieglerParameters::OnUpdateZieglerSRIM ), NULL, this );
	buttonZieglerSRIMEraseAllData->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogZieglerParameters::OnEraseZieglerSRIM ), NULL, this );
	buttonZieglerLoad->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogZieglerParameters::OnZieglerLoad ), NULL, this );
	buttonZieglerSave->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogZieglerParameters::OnZieglerSave ), NULL, this );
	buttonZieglerCopy->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogZieglerParameters::OnZieglerCopy ), NULL, this );
	buttonZieglerPaste->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogZieglerParameters::OnZieglerPaste ), NULL, this );
	buttonZieglerClear->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogZieglerParameters::OnZieglerClear ), NULL, this );
	buttonZieglerOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogZieglerParameters::OnZieglerOK ), NULL, this );
	buttonZieglerCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogZieglerParameters::OnZieglerCancel ), NULL, this );
	buttonZieglerHelp->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogZieglerParameters::OnZieglerHelp ), NULL, this );

}


dialogSetup::dialogSetup( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* sizerSetup;
	sizerSetup = new wxBoxSizer( wxVERTICAL );

	lineDefaultButtons = new wxStaticText( this, wxID_ANY, wxT("The Default Databases loaded during ERYA initialization are displayed here."), wxDefaultPosition, wxDefaultSize, 0 );
	lineDefaultButtons->Wrap( -1 );
	sizerSetup->Add( lineDefaultButtons, 0, wxALL|wxEXPAND, 5 );

	labelSetupTip = new wxStaticText( this, wxID_ANY, wxT("Use this tool to change the setup file, and reset if necessary."), wxDefaultPosition, wxDefaultSize, 0 );
	labelSetupTip->Wrap( -1 );
	sizerSetup->Add( labelSetupTip, 0, wxALL|wxEXPAND, 5 );

	wxString radioDefaultConfigChoices[] = { wxT("The default operating system profile folder"), wxT("At the same root program folder.") };
	int radioDefaultConfigNChoices = sizeof( radioDefaultConfigChoices ) / sizeof( wxString );
	radioDefaultConfig = new wxRadioBox( this, wxID_ANY, wxT("Store the ERYA configuration file at..."), wxDefaultPosition, wxDefaultSize, radioDefaultConfigNChoices, radioDefaultConfigChoices, 1, wxRA_SPECIFY_COLS );
	radioDefaultConfig->SetSelection( 0 );
	sizerSetup->Add( radioDefaultConfig, 0, wxALL|wxEXPAND, 5 );

	lineDefaultFiles = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	sizerSetup->Add( lineDefaultFiles, 0, wxEXPAND | wxALL, 5 );

	labelFilesInfo = new wxStaticText( this, wxID_ANY, wxT("Select the Default Databases"), wxDefaultPosition, wxDefaultSize, 0 );
	labelFilesInfo->Wrap( -1 );
	sizerSetup->Add( labelFilesInfo, 0, wxALL|wxEXPAND, 5 );

	wxGridSizer* sizerDefaultDatabase;
	sizerDefaultDatabase = new wxGridSizer( 3, 2, 0, 0 );

	labelDefaultDetector = new wxStaticText( this, wxID_ANY, wxT("Default Detector Setup File"), wxDefaultPosition, wxDefaultSize, 0 );
	labelDefaultDetector->Wrap( -1 );
	sizerDefaultDatabase->Add( labelDefaultDetector, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	fileDefaultDetector = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, wxT("ERYA Detector Setup files"), wxT("*.epsd"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE );
	sizerDefaultDatabase->Add( fileDefaultDetector, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	labelDefaultDatabase = new wxStaticText( this, wxID_ANY, wxT("Default Elements Database File"), wxDefaultPosition, wxDefaultSize, 0 );
	labelDefaultDatabase->Wrap( -1 );
	sizerDefaultDatabase->Add( labelDefaultDatabase, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	fileDefaultDatabase = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, wxT("ERYA Database Files"), wxT("*.epd"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE );
	sizerDefaultDatabase->Add( fileDefaultDatabase, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	labelDefaultZiegler = new wxStaticText( this, wxID_ANY, wxT("Default Ziegler Parameters File"), wxDefaultPosition, wxDefaultSize, 0 );
	labelDefaultZiegler->Wrap( -1 );
	sizerDefaultDatabase->Add( labelDefaultZiegler, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	fileDefaultZiegler = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, wxT("ERYA Ziegler Parameters file"), wxT("*.epsz"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE );
	sizerDefaultDatabase->Add( fileDefaultZiegler, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );


	sizerSetup->Add( sizerDefaultDatabase, 1, wxEXPAND, 5 );

	m_staticline1 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	sizerSetup->Add( m_staticline1, 0, wxEXPAND|wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	wxGridSizer* sizerSetupButtons;
	sizerSetupButtons = new wxGridSizer( 1, 4, 0, 0 );

	buttonDefaultSave = new wxButton( this, wxID_ANY, wxT("Save"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerSetupButtons->Add( buttonDefaultSave, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	buttonDefaultReset = new wxButton( this, wxID_ANY, wxT("Reset"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerSetupButtons->Add( buttonDefaultReset, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	buttonDefaultDelete = new wxButton( this, wxID_ANY, wxT("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerSetupButtons->Add( buttonDefaultDelete, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	buttonDefaultQuit = new wxButton( this, wxID_ANY, wxT("Quit"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerSetupButtons->Add( buttonDefaultQuit, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );


	sizerSetup->Add( sizerSetupButtons, 0, wxEXPAND, 5 );


	this->SetSizer( sizerSetup );
	this->Layout();

	this->Centre( wxBOTH );

    this->SetIcon(ERYA_xpm);

	// Connect Events
	buttonDefaultSave->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogSetup::OnDefaultSave ), NULL, this );
	buttonDefaultReset->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogSetup::OnDefaultReset ), NULL, this );
	buttonDefaultDelete->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogSetup::OnDefaultDelete ), NULL, this );
	buttonDefaultQuit->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogSetup::OnDefaultQuit ), NULL, this );
}

dialogSetup::~dialogSetup()
{
	// Disconnect Events
	buttonDefaultSave->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogSetup::OnDefaultSave ), NULL, this );
	buttonDefaultReset->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogSetup::OnDefaultReset ), NULL, this );
	buttonDefaultDelete->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogSetup::OnDefaultDelete ), NULL, this );
	buttonDefaultQuit->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogSetup::OnDefaultQuit ), NULL, this );

}


dialogHelp::dialogHelp( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* sizerHelpViewer;
	sizerHelpViewer = new wxBoxSizer( wxVERTICAL );

	pageHelpViewer = new wxHtmlWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHW_SCROLLBAR_AUTO );
	sizerHelpViewer->Add( pageHelpViewer, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxEXPAND, 5 );

	lineHelpViewer = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	sizerHelpViewer->Add( lineHelpViewer, 0, wxEXPAND | wxALL, 5 );

	wxGridSizer* sizerHelpButtons;
	sizerHelpButtons = new wxGridSizer( 1, 6, 0, 0 );

	buttonHelpMain = new wxButton( this, wxID_ANY, wxT("Main Page"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerHelpButtons->Add( buttonHelpMain, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	buttonHelpIndex = new wxButton( this, wxID_ANY, wxT("Index"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerHelpButtons->Add( buttonHelpIndex, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	buttonHelpReadme = new wxButton( this, wxID_ANY, wxT("Read Me"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerHelpButtons->Add( buttonHelpReadme, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	buttonHelpBack = new wxButton( this, wxID_ANY, wxT("Back"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerHelpButtons->Add( buttonHelpBack, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	buttonHelpForward = new wxButton( this, wxID_ANY, wxT("Forward"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerHelpButtons->Add( buttonHelpForward, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	buttonHelpClose = new wxButton( this, wxID_ANY, wxT("Close"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerHelpButtons->Add( buttonHelpClose, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );


	sizerHelpViewer->Add( sizerHelpButtons, 0, wxEXPAND, 5 );


	this->SetSizer( sizerHelpViewer );
	this->Layout();

	this->Centre( wxBOTH );

    this->SetIcon(ERYA_xpm);

	// Connect Events
	buttonHelpMain->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogHelp::OnHelpMain ), NULL, this );
	buttonHelpIndex->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogHelp::OnHelpIndex ), NULL, this );
	buttonHelpReadme->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogHelp::OnHelpReadme ), NULL, this );
	buttonHelpBack->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogHelp::OnHelpBack ), NULL, this );
	buttonHelpForward->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogHelp::OnHelpForward ), NULL, this );
	buttonHelpClose->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogHelp::OnHelpClose ), NULL, this );
}

dialogHelp::~dialogHelp()
{
	// Disconnect Events
	buttonHelpMain->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogHelp::OnHelpMain ), NULL, this );
	buttonHelpIndex->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogHelp::OnHelpIndex ), NULL, this );
	buttonHelpReadme->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogHelp::OnHelpReadme ), NULL, this );
	buttonHelpBack->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogHelp::OnHelpBack ), NULL, this );
	buttonHelpForward->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogHelp::OnHelpForward ), NULL, this );
	buttonHelpClose->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogHelp::OnHelpClose ), NULL, this );

}

wizardFirstRun::wizardFirstRun( wxWindow* parent, wxWindowID id, const wxString& title, const wxBitmap& bitmap, const wxPoint& pos, long style )
{
	this->Create( parent, id, title, bitmap, pos, style );
	this->SetSizeHints( wxDefaultSize, wxDefaultSize);

	wxWizardPageSimple* wizardFirstRun1 = new wxWizardPageSimple( this );
	m_pages.Add( wizardFirstRun1 );

	wxBoxSizer* sizerFirstRun1;
	sizerFirstRun1 = new wxBoxSizer( wxVERTICAL );

	labelFirstRun1 = new wxStaticText( wizardFirstRun1, wxID_ANY, wxT("Welcome to ERYA Bulk!\n\nBefore use the program, \nERYA requires some additional informations to write a Configuration File,\nwhere the main Database files will be loaded when ERYA starts.\n\nPlease follow the instructions."), wxDefaultPosition, wxDefaultSize, 0 );
	labelFirstRun1->Wrap( -1 );
	sizerFirstRun1->Add( labelFirstRun1, 1, wxALL|wxEXPAND, 5 );


	wizardFirstRun1->SetSizer( sizerFirstRun1 );
	wizardFirstRun1->Layout();
	sizerFirstRun1->Fit( wizardFirstRun1 );
	wxWizardPageSimple* wizardFirstRun2 = new wxWizardPageSimple( this );
	m_pages.Add( wizardFirstRun2 );

	wxBoxSizer* sizerFirstRun2;
	sizerFirstRun2 = new wxBoxSizer( wxVERTICAL );

	labelFirstRun2 = new wxStaticText( wizardFirstRun2, wxID_ANY, wxT("Select the Detector Efficiency Profile file, from the widget below.\n\nNote: It will only support native ERYA Bulk files.\nIf you had another file formats, \nplease use the Database Management tools to convert them first.\n\nIf necessary, read the User Guide before proceding."), wxDefaultPosition, wxDefaultSize, 0 );
	labelFirstRun2->Wrap( -1 );
	sizerFirstRun2->Add( labelFirstRun2, 1, wxALL|wxEXPAND, 5 );

	fileDetectorFirstRun = new wxFilePickerCtrl( wizardFirstRun2, wxID_ANY, wxEmptyString, wxT("Select a Compatible File for the ERYA Detector Efficiency"), wxT("ERYA Profiling Detector Efficiency (*.epsd)|*.epsd"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE );
	sizerFirstRun2->Add( fileDetectorFirstRun, 0, wxALL|wxEXPAND, 5 );


	wizardFirstRun2->SetSizer( sizerFirstRun2 );
	wizardFirstRun2->Layout();
	sizerFirstRun2->Fit( wizardFirstRun2 );
	wxWizardPageSimple* wizardFirstRun3 = new wxWizardPageSimple( this );
	m_pages.Add( wizardFirstRun3 );

	wxBoxSizer* sizerFirstRun3;
	sizerFirstRun3 = new wxBoxSizer( wxVERTICAL );

	labelFirstRun3 = new wxStaticText( wizardFirstRun3, wxID_ANY, wxT("Select the Elements Database compatible with ERYA Bulk, from the widget below.\n\nNote: If you have Databases from LabView ERYA,\nplease use the \"Import LabView ERYA\" wizard first to convert them.\n\nIf necessary, read the User Guide for further details."), wxDefaultPosition, wxDefaultSize, 0 );
	labelFirstRun3->Wrap( -1 );
	sizerFirstRun3->Add( labelFirstRun3, 1, wxALL|wxEXPAND, 5 );

	fileElementsFirstRun = new wxFilePickerCtrl( wizardFirstRun3, wxID_ANY, wxEmptyString, wxT("Select a Compatible File for the ERYA Element Database"), wxT("ERYA Element Database (*.epd)|*.epd"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE );
	sizerFirstRun3->Add( fileElementsFirstRun, 0, wxALL|wxEXPAND, 5 );


	wizardFirstRun3->SetSizer( sizerFirstRun3 );
	wizardFirstRun3->Layout();
	sizerFirstRun3->Fit( wizardFirstRun3 );
	wxWizardPageSimple* wizardFirstRun4 = new wxWizardPageSimple( this );
	m_pages.Add( wizardFirstRun4 );

	wxBoxSizer* sizerFirstRun4;
	sizerFirstRun4 = new wxBoxSizer( wxVERTICAL );

	labelFirstRun4 = new wxStaticText( wizardFirstRun4, wxID_ANY, wxT("Select a compatible ERYA Ziegler Parameters file,\nfrom the widget below.\n\nNote: Use the Database Management tools to convert the LabView ERYA file formats if necessary, before continue.\n\nPlease read the User Guide, before continue."), wxDefaultPosition, wxDefaultSize, 0 );
	labelFirstRun4->Wrap( -1 );
	sizerFirstRun4->Add( labelFirstRun4, 1, wxALL|wxEXPAND, 5 );

	fileZieglerFirstRun = new wxFilePickerCtrl( wizardFirstRun4, wxID_ANY, wxEmptyString, wxT("Select a Compatible File for the ERYA Ziegler Parameters"), wxT("ERYA Profiling Ziegler Parameters (*.epsz)|*.epsz"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE );
	sizerFirstRun4->Add( fileZieglerFirstRun, 0, wxALL|wxEXPAND, 5 );


	wizardFirstRun4->SetSizer( sizerFirstRun4 );
	wizardFirstRun4->Layout();
	sizerFirstRun4->Fit( wizardFirstRun4 );
	wxWizardPageSimple* wizardFirstRun5 = new wxWizardPageSimple( this );
	m_pages.Add( wizardFirstRun5 );

	wxBoxSizer* sizerFirstRun5;
	sizerFirstRun5 = new wxBoxSizer( wxVERTICAL );

	labelFirstRun5 = new wxStaticText( wizardFirstRun5, wxID_ANY, wxT("Select where the ERYA Bulk Configuration file should be placed.\n\nThe default Databases will also be placed at the same Configuration directory.\n\nThe recomended setting is to choose the default folder defined by the operating system itself.\n\nRead the manual for more information."), wxDefaultPosition, wxDefaultSize, 0 );
	labelFirstRun5->Wrap( -1 );
	sizerFirstRun5->Add( labelFirstRun5, 1, wxALL|wxEXPAND, 5 );

	wxString radioSetupDefaultChoices[] = { wxT("At the User Profile Directory"), wxT("At the ERYA's Root Directory") };
	int radioSetupDefaultNChoices = sizeof( radioSetupDefaultChoices ) / sizeof( wxString );
	radioSetupDefault = new wxRadioBox( wizardFirstRun5, wxID_ANY, wxT("Place the Setup Profile..."), wxDefaultPosition, wxDefaultSize, radioSetupDefaultNChoices, radioSetupDefaultChoices, 2, wxRA_SPECIFY_ROWS );
	radioSetupDefault->SetSelection( 0 );
	sizerFirstRun5->Add( radioSetupDefault, 0, wxALL|wxEXPAND, 5 );


	wizardFirstRun5->SetSizer( sizerFirstRun5 );
	wizardFirstRun5->Layout();
	sizerFirstRun5->Fit( wizardFirstRun5 );
	wxWizardPageSimple* wizardFirstRun6 = new wxWizardPageSimple( this );
	m_pages.Add( wizardFirstRun6 );

	wxBoxSizer* sizerFirstRun6;
	sizerFirstRun6 = new wxBoxSizer( wxVERTICAL );

	labelFirstRun6 = new wxStaticText( wizardFirstRun6, wxID_ANY, wxT("ERYA Bulk Setup Complete!\n\nThe default configuration file was stored, and the default Databases will be loaded automatically at next start-ups.\n\nYou can reset the configurations using the \"Reset Settings\" option menu, and it will repeat the same process again."), wxDefaultPosition, wxDefaultSize, 0 );
	labelFirstRun6->Wrap( -1 );
	sizerFirstRun6->Add( labelFirstRun6, 1, wxALL|wxEXPAND, 5 );


	wizardFirstRun6->SetSizer( sizerFirstRun6 );
	wizardFirstRun6->Layout();
	sizerFirstRun6->Fit( wizardFirstRun6 );

	this->Centre( wxBOTH );

	for ( unsigned int i = 1; i < m_pages.GetCount(); i++ )
	{
		m_pages.Item( i )->SetPrev( m_pages.Item( i - 1 ) );
		m_pages.Item( i - 1 )->SetNext( m_pages.Item( i ) );
	}

	// Connect Events
	this->Connect( wxID_ANY, wxEVT_WIZARD_CANCEL, wxWizardEventHandler( wizardFirstRun::OnFirstRunCancel ) );
	this->Connect( wxID_ANY, wxEVT_WIZARD_FINISHED, wxWizardEventHandler( wizardFirstRun::OnFirstRunFinish ) );
}

wizardFirstRun::~wizardFirstRun()
{
	// Disconnect Events
	this->Disconnect( wxID_ANY, wxEVT_WIZARD_CANCEL, wxWizardEventHandler( wizardFirstRun::OnFirstRunCancel ) );
	this->Disconnect( wxID_ANY, wxEVT_WIZARD_FINISHED, wxWizardEventHandler( wizardFirstRun::OnFirstRunFinish ) );

	m_pages.Clear();
}


dialogAdvancedFit::dialogAdvancedFit( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 640,480 ), wxDefaultSize );

	wxBoxSizer* sizerAdvanced;
	sizerAdvanced = new wxBoxSizer( wxVERTICAL );

	labelAdvancedTip = new wxStaticText( this, wxID_ANY, wxT("You can change here the convergence criteria, using this tool."), wxDefaultPosition, wxDefaultSize, 0 );
	labelAdvancedTip->Wrap( -1 );
	sizerAdvanced->Add( labelAdvancedTip, 0, wxALL|wxEXPAND, 5 );

	lineAdvancedTip = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	sizerAdvanced->Add( lineAdvancedTip, 0, wxEXPAND | wxALL, 5 );

	wxFlexGridSizer* sizerAdvancedOptions;
	sizerAdvancedOptions = new wxFlexGridSizer( 4, 2, 60, 0 );
	sizerAdvancedOptions->AddGrowableCol( 0 );
	sizerAdvancedOptions->SetFlexibleDirection( wxBOTH );
	sizerAdvancedOptions->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	labelNumberIterations = new wxStaticText( this, wxID_ANY, wxT("Maximum Number of Iterations"), wxDefaultPosition, wxDefaultSize, 0 );
	labelNumberIterations->Wrap( -1 );
	sizerAdvancedOptions->Add( labelNumberIterations, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	spinNumberIterations = new wxSpinCtrl( this, wxID_ANY, wxT("100"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0 );
	sizerAdvancedOptions->Add( spinNumberIterations, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );

	labelDampingFactor = new wxStaticText( this, wxID_ANY, wxT("Initial damping factor, in number of Significant Digits"), wxDefaultPosition, wxDefaultSize, 0 );
	labelDampingFactor->Wrap( -1 );
	sizerAdvancedOptions->Add( labelDampingFactor, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	spinDampingFactor = new wxSpinCtrl( this, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 3, 0 );
	sizerAdvancedOptions->Add( spinDampingFactor, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );

	labelYieldConvergence = new wxStaticText( this, wxID_ANY, wxT("Yield Convergence Criteria, in number of Significant Digits"), wxDefaultPosition, wxDefaultSize, 0 );
	labelYieldConvergence->Wrap( -1 );
	sizerAdvancedOptions->Add( labelYieldConvergence, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	spinYieldConvergence = new wxSpinCtrl( this, wxID_ANY, wxT("3"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 6, 3 );
	sizerAdvancedOptions->Add( spinYieldConvergence, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	labelStoichiometryConvergence = new wxStaticText( this, wxID_ANY, wxT("Stoichiometry Convergence Criteria, in Number of Significant Digits"), wxDefaultPosition, wxDefaultSize, 0 );
	labelStoichiometryConvergence->Wrap( -1 );
	sizerAdvancedOptions->Add( labelStoichiometryConvergence, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	spinStoichiometryConvergence = new wxSpinCtrl( this, wxID_ANY, wxT("3"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 6, 0 );
	sizerAdvancedOptions->Add( spinStoichiometryConvergence, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );


	sizerAdvanced->Add( sizerAdvancedOptions, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 5 );

	lineAdvancedButtons = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	sizerAdvanced->Add( lineAdvancedButtons, 0, wxALL|wxEXPAND, 5 );

	wxGridSizer* sizerAdvancedButtons;
	sizerAdvancedButtons = new wxGridSizer( 1, 4, 0, 0 );

	buttonAdvancedOK = new wxButton( this, wxID_ANY, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerAdvancedButtons->Add( buttonAdvancedOK, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	buttonAdvancedCancel = new wxButton( this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerAdvancedButtons->Add( buttonAdvancedCancel, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	buttonAdvancedReset = new wxButton( this, wxID_ANY, wxT("Default"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerAdvancedButtons->Add( buttonAdvancedReset, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	buttonAdvancedHelp = new wxButton( this, wxID_ANY, wxT("Help"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerAdvancedButtons->Add( buttonAdvancedHelp, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	sizerAdvanced->Add( sizerAdvancedButtons, 0, wxEXPAND, 5 );


	this->SetSizer( sizerAdvanced );
	this->Layout();
	sizerAdvanced->Fit( this );

	this->Centre( wxBOTH );

    this->SetIcon(ERYA_xpm);

	// Connect Events
	buttonAdvancedOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogAdvancedFit::OnAdvancedOK ), NULL, this );
	buttonAdvancedCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogAdvancedFit::OnAdvancedCancel ), NULL, this );
	buttonAdvancedReset->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogAdvancedFit::OnAdvancedReset ), NULL, this );
	buttonAdvancedHelp->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogAdvancedFit::OnAdvancedHelp ), NULL, this );
}

dialogAdvancedFit::~dialogAdvancedFit()
{
	// Disconnect Events
	buttonAdvancedOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogAdvancedFit::OnAdvancedOK ), NULL, this );
	buttonAdvancedCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogAdvancedFit::OnAdvancedCancel ), NULL, this );
	buttonAdvancedReset->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogAdvancedFit::OnAdvancedReset ), NULL, this );
	buttonAdvancedHelp->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogAdvancedFit::OnAdvancedHelp ), NULL, this );

}

dialogR33DataImport::dialogR33DataImport( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* sizerDataImport;
	sizerDataImport = new wxBoxSizer( wxVERTICAL );

	labelDataImportInfo = new wxStaticText( this, wxID_ANY, wxT("The imported file require some additional information to export correctly."), wxDefaultPosition, wxDefaultSize, 0 );
	labelDataImportInfo->Wrap( -1 );
	sizerDataImport->Add( labelDataImportInfo, 0, wxALL|wxEXPAND, 5 );

	lineDataImportInfo = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	sizerDataImport->Add( lineDataImportInfo, 0, wxEXPAND | wxALL, 5 );

	labelDataImportUnits = new wxStaticText( this, wxID_ANY, wxT("Select from the options below, the original units from the source file."), wxDefaultPosition, wxDefaultSize, 0 );
	labelDataImportUnits->Wrap( -1 );
	sizerDataImport->Add( labelDataImportUnits, 0, wxALL|wxEXPAND, 5 );

	wxString radioDataImportUnitsChoices[] = { wxT("MiliBarn (mb)"), wxT("Barn (b)") };
	int radioDataImportUnitsNChoices = sizeof( radioDataImportUnitsChoices ) / sizeof( wxString );
	radioDataImportUnits = new wxRadioBox( this, wxID_ANY, wxT("Select the Cross-Section Units"), wxDefaultPosition, wxDefaultSize, radioDataImportUnitsNChoices, radioDataImportUnitsChoices, 1, wxRA_SPECIFY_COLS );
	radioDataImportUnits->SetSelection( 0 );
	sizerDataImport->Add( radioDataImportUnits, 0, wxALL|wxEXPAND, 5 );

	wxString radioDataImportAngleChoices[] = { wxT("Esterradian (sr)"), wxT("Total Angle (tot)") };
	int radioDataImportAngleNChoices = sizeof( radioDataImportAngleChoices ) / sizeof( wxString );
	radioDataImportAngle = new wxRadioBox( this, wxID_ANY, wxT("Select the Cross-Section Angle Unit"), wxDefaultPosition, wxDefaultSize, radioDataImportAngleNChoices, radioDataImportAngleChoices, 1, wxRA_SPECIFY_COLS );
	radioDataImportAngle->SetSelection( 0 );
	sizerDataImport->Add( radioDataImportAngle, 0, wxALL|wxEXPAND, 5 );

	checkIgnoreNonNumericalData = new wxCheckBox( this, wxID_ANY, wxT("Ignore all lines with labels, or other non-numerical data columns."), wxDefaultPosition, wxDefaultSize, 0 );
	sizerDataImport->Add( checkIgnoreNonNumericalData, 0, wxALL|wxEXPAND, 5 );

	lineDataImportButtons = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	sizerDataImport->Add( lineDataImportButtons, 0, wxEXPAND | wxALL, 5 );

    wxGridSizer* sizerR33DataButtons;
	sizerR33DataButtons = new wxGridSizer( 1, 4, 0, 0 );

	buttonR33DataOK = new wxButton( this, wxID_ANY, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerR33DataButtons->Add( buttonR33DataOK, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	buttonR33DataCancel = new wxButton( this, wxID_ANY, wxT("Default"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerR33DataButtons->Add( buttonR33DataCancel, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	buttonR33DataReset = new wxButton( this, wxID_ANY, wxT("Reset"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerR33DataButtons->Add( buttonR33DataReset, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	buttonR33DataHelp = new wxButton( this, wxID_ANY, wxT("Help"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerR33DataButtons->Add( buttonR33DataHelp, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	sizerDataImport->Add( sizerR33DataButtons, 0, wxEXPAND, 5 );


	this->SetSizer( sizerDataImport );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	buttonR33DataOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogR33DataImport::OnDataImportValidate ), NULL, this );
	buttonR33DataCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogR33DataImport::OnDataImportAbort ), NULL, this );
	buttonR33DataReset->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogR33DataImport::OnDataImportReset ), NULL, this );
	buttonR33DataHelp->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogR33DataImport::OnDataImportHelp ), NULL, this );

}

dialogR33DataImport::~dialogR33DataImport()
{
	// Disconnect Events
	buttonR33DataOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogR33DataImport::OnDataImportValidate ), NULL, this );
	buttonR33DataCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogR33DataImport::OnDataImportAbort ), NULL, this );
	buttonR33DataReset->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogR33DataImport::OnDataImportReset ), NULL, this );
	buttonR33DataHelp->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogR33DataImport::OnDataImportHelp ), NULL, this );

}

dialogXlsxDataImport::dialogXlsxDataImport( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* sizerDataImport;
	sizerDataImport = new wxBoxSizer( wxVERTICAL );

	labelDataImportInfo = new wxStaticText( this, wxID_ANY, wxT("The imported file require some additional information to import correctly."), wxDefaultPosition, wxDefaultSize, 0 );
	labelDataImportInfo->Wrap( -1 );
	sizerDataImport->Add( labelDataImportInfo, 0, wxALL|wxEXPAND, 5 );

	lineDataImportInfo = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	sizerDataImport->Add( lineDataImportInfo, 0, wxEXPAND | wxALL, 5 );

	labelDataImportUnits = new wxStaticText( this, wxID_ANY, wxT("Select from the options below, the original units from the source file."), wxDefaultPosition, wxDefaultSize, 0 );
	labelDataImportUnits->Wrap( -1 );
	sizerDataImport->Add( labelDataImportUnits, 0, wxALL|wxEXPAND, 5 );

	wxString radioDataImportUnitsChoices[] = { wxT("MiliBarn (mb)"), wxT("Barn (b)") };
	int radioDataImportUnitsNChoices = sizeof( radioDataImportUnitsChoices ) / sizeof( wxString );
	radioDataImportUnits = new wxRadioBox( this, wxID_ANY, wxT("Select the Cross-Section Units"), wxDefaultPosition, wxDefaultSize, radioDataImportUnitsNChoices, radioDataImportUnitsChoices, 1, wxRA_SPECIFY_COLS );
	radioDataImportUnits->SetSelection( 0 );
	sizerDataImport->Add( radioDataImportUnits, 0, wxALL|wxEXPAND, 5 );

	wxString radioDataImportAngleChoices[] = { wxT("Esterradian (sr)"), wxT("Total Angle (tot)") };
	int radioDataImportAngleNChoices = sizeof( radioDataImportAngleChoices ) / sizeof( wxString );
	radioDataImportAngle = new wxRadioBox( this, wxID_ANY, wxT("Select the Cross-Section Angle Unit"), wxDefaultPosition, wxDefaultSize, radioDataImportAngleNChoices, radioDataImportAngleChoices, 1, wxRA_SPECIFY_COLS );
	radioDataImportAngle->SetSelection( 0 );
	sizerDataImport->Add( radioDataImportAngle, 0, wxALL|wxEXPAND, 5 );

	wxString radioDataImportColumnChoices[] = { wxT("Automatic (Use the default 2 or 4-columns profile by program own guess)"), wxT("Override the first column for Energy, and second column for Cross-section"), wxT("Override the first column for Energy, and third column for Cross-section") };
	int radioDataImportColumnNChoices = sizeof( radioDataImportColumnChoices ) / sizeof( wxString );
	radioDataImportColumn = new wxRadioBox( this, wxID_ANY, wxT("Select the Importation Profile "), wxDefaultPosition, wxDefaultSize, radioDataImportColumnNChoices, radioDataImportColumnChoices, 1, wxRA_SPECIFY_COLS );
	radioDataImportColumn->SetSelection( 0 );
	sizerDataImport->Add( radioDataImportColumn, 0, wxALL|wxEXPAND, 5 );

	lineDataImportButtons = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	sizerDataImport->Add( lineDataImportButtons, 0, wxEXPAND | wxALL, 5 );

	wxGridSizer* sizerXlsxDataButtons;
	sizerXlsxDataButtons = new wxGridSizer( 1, 4, 0, 0 );

	buttonXlsxDataOK = new wxButton( this, wxID_ANY, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerXlsxDataButtons->Add( buttonXlsxDataOK, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	buttonXlsxDataCancel = new wxButton( this, wxID_ANY, wxT("Default"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerXlsxDataButtons->Add( buttonXlsxDataCancel, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	buttonXlsxDataReset = new wxButton( this, wxID_ANY, wxT("Reset"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerXlsxDataButtons->Add( buttonXlsxDataReset, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	buttonXlsxDataHelp = new wxButton( this, wxID_ANY, wxT("Help"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerXlsxDataButtons->Add( buttonXlsxDataHelp, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	sizerDataImport->Add( sizerXlsxDataButtons, 0, wxEXPAND, 5 );


	this->SetSizer( sizerDataImport );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	buttonXlsxDataOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogXlsxDataImport::OnDataImportValidate ), NULL, this );
	buttonXlsxDataCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogXlsxDataImport::OnDataImportAbort ), NULL, this );
	buttonXlsxDataReset->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogXlsxDataImport::OnDataImportReset ), NULL, this );
	buttonXlsxDataHelp->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogXlsxDataImport::OnDataImportHelp ), NULL, this );
}

dialogXlsxDataImport::~dialogXlsxDataImport()
{
	// Disconnect Events
	buttonXlsxDataOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogXlsxDataImport::OnDataImportValidate ), NULL, this );
	buttonXlsxDataCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogXlsxDataImport::OnDataImportAbort ), NULL, this );
	buttonXlsxDataReset->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogXlsxDataImport::OnDataImportReset ), NULL, this );
	buttonXlsxDataHelp->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dialogXlsxDataImport::OnDataImportHelp ), NULL, this );

}

