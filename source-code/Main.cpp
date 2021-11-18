#include "Main.h"

wxBEGIN_EVENT_TABLE(Main, wxFrame)
EVT_MENU(ID_OPEN_C, Main::OnOpenC)
EVT_MENU(ID_OPEN_G, Main::OnOpenG)
EVT_MENU(ID_SAVE_C, Main::OnSaveC)
EVT_MENU(ID_SAVE_G, Main::OnSaveG)
EVT_MENU(ID_EXIT, Main::OnExit)

EVT_MENU(ID_RESET_C, Main::OnResetC)
EVT_MENU(ID_RESET_G, Main::OnResetG)

EVT_MENU(ID_DOCUMENTATION, Main::OnDocumentation)
wxEND_EVENT_TABLE()

Main::Main() : wxFrame(nullptr, wxID_ANY, "CellyGen", wxDefaultPosition, wxSize(1600, 900))
{
	SetIcon(wxICON(aaaIcon));
	
	Center();

	BuildMenuBar();

	SetShortcuts();

	BuildLayout();
}

Main::~Main()
{
	
}

void Main::BuildMenuBar()
{
	wxMenuBar* menuBar = new wxMenuBar();
	this->SetMenuBar(menuBar);

	wxMenu* menuFile = new wxMenu();
	wxMenu* menuOpen = new wxMenu();
	wxMenu* menuSave = new wxMenu();
	wxMenu* menuReset = new wxMenu();
	wxMenu* menuHelp = new wxMenu();

	menuOpen->Append(ID_OPEN_C, "&Cellular automaton configuration\tCtrl-O");
	menuOpen->Append(ID_OPEN_G, "&Genetic algorithm configuration\tCtrl-Shift-O");

	menuSave->Append(ID_SAVE_C, "&Cellular automaton configuration\tCtrl-S");
	menuSave->Append(ID_SAVE_G, "&Genetic algorithm configuration\tCtrl-Shift-S");

	menuFile->AppendSubMenu(menuOpen, "&Open");
	menuFile->AppendSubMenu(menuSave, "&Save");
	menuFile->AppendSeparator();
	menuFile->Append(ID_EXIT, "E&xit");

	menuReset->Append(ID_RESET_C, "Reset &cellular automaton\tCtrl-R");
	menuReset->Append(ID_RESET_G, "Reset &genetic algorithm\tCtrl-Shift-R");

	menuHelp->Append(ID_DOCUMENTATION, "&Documentation\tCtrl-D");

	menuBar->Append(menuFile, "&File");
	menuBar->Append(menuReset, "&Reset");
	menuBar->Append(menuHelp, "&Help");
}

void Main::SetShortcuts()
{
	wxAcceleratorEntry entries[7];
	entries[0].Set(wxACCEL_CTRL, (int)'O', ID_OPEN_C);
	entries[1].Set(wxACCEL_CTRL | wxACCEL_SHIFT, (int)'O', ID_OPEN_G);
	entries[2].Set(wxACCEL_CTRL, (int)'S', ID_SAVE_C);
	entries[3].Set(wxACCEL_CTRL | wxACCEL_SHIFT, (int)'S', ID_SAVE_G);
	entries[4].Set(wxACCEL_CTRL, (int)'R', ID_RESET_C);
	entries[5].Set(wxACCEL_CTRL | wxACCEL_SHIFT, (int)'R', ID_RESET_G);
	entries[6].Set(wxACCEL_CTRL, (int)'D', ID_DOCUMENTATION);
	

	wxAcceleratorTable accel(7, entries);
	this->SetAcceleratorTable(accel);
}

void Main::BuildLayout()
{
	wxBoxSizer* sizerPanels = new wxBoxSizer(wxHORIZONTAL);
	wxPanel* panelLeft = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize());
	wxPanel* panelRight = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize());

	//panelLeft->SetBackgroundColour(wxColor(100, 200, 100));
	//panelRight->SetBackgroundColour(wxColor(200, 100, 100));

	sizerPanels->Add(panelLeft, 1, wxEXPAND);
	sizerPanels->Add(panelRight, 2, wxEXPAND);

	wxPanel* panelTop = new wxPanel(panelLeft, wxID_ANY, wxDefaultPosition, wxSize());
	//panelTop->SetBackgroundColour(wxColor(0, 0, 100));
	wxPanel* panelMiddle = new wxPanel(panelLeft, wxID_ANY, wxDefaultPosition, wxSize());
	//panelMiddle->SetBackgroundColour(wxColor(100, 0, 0));
	wxPanel* panelBottom = new wxPanel(panelLeft, wxID_ANY, wxDefaultPosition, wxSize());
	//panelBottom->SetBackgroundColour(wxColor(0, 100, 0));

	wxBoxSizer* sizerPanelLeft = new wxBoxSizer(wxVERTICAL);
	sizerPanelLeft->Add(panelTop, 1, wxEXPAND);
	sizerPanelLeft->Add(panelMiddle, 1, wxEXPAND);
	sizerPanelLeft->Add(panelBottom, 1, wxEXPAND);

	panelLeft->SetSizer(sizerPanelLeft);

	wxStaticBoxSizer* sizerTop = new wxStaticBoxSizer(wxVERTICAL, panelTop, "States");
	wxStaticBoxSizer* sizerMiddle = new wxStaticBoxSizer(wxVERTICAL, panelMiddle, "Neighbourhood && Rules");
	wxStaticBoxSizer* sizerBottom = new wxStaticBoxSizer(wxVERTICAL, panelBottom, "Genetic Algorithm Parameters");

	panelTop->SetSizer(sizerTop);
	panelMiddle->SetSizer(sizerMiddle);
	panelBottom->SetSizer(sizerBottom);

	this->SetSizer(sizerPanels);
}

void Main::OnOpenC(wxCommandEvent& evt)
{
	wxMessageBox("This is a wxWidgets Hello World example", "OnOpenC", wxOK | wxICON_INFORMATION);
}

void Main::OnOpenG(wxCommandEvent& evt)
{
	wxMessageBox("This is a wxWidgets Hello World example", "OnOpenG", wxOK | wxICON_INFORMATION);
}

void Main::OnSaveC(wxCommandEvent& evt)
{
	wxMessageBox("This is a wxWidgets Hello World example", "OnSaveC", wxOK | wxICON_INFORMATION);
}

void Main::OnSaveG(wxCommandEvent& evt)
{
	wxMessageBox("This is a wxWidgets Hello World example", "OnSaveG", wxOK | wxICON_INFORMATION);
}

void Main::OnExit(wxCommandEvent& evt)
{
	Close();
	evt.Skip();
}

void Main::OnResetC(wxCommandEvent& evt)
{
	wxMessageBox("This is a wxWidgets Hello World example", "OnResetC", wxOK | wxICON_INFORMATION);
}

void Main::OnResetG(wxCommandEvent& evt)
{
	wxMessageBox("This is a wxWidgets Hello World example", "OnResetG", wxOK | wxICON_INFORMATION);
}

void Main::OnDocumentation(wxCommandEvent& evt)
{
	wxMessageBox("This is a wxWidgets Hello World example", "OnDocumentation", wxOK | wxICON_INFORMATION);
}
