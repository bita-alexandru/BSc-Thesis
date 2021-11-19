#include "Main.h"

wxBEGIN_EVENT_TABLE(Main, wxFrame)
	EVT_MENU(MenuBar::ID_OPEN_C, Main::OnOpenAutomaton)
	EVT_MENU(MenuBar::ID_OPEN_G, Main::OnOpenAlgorithm)
	EVT_MENU(MenuBar::ID_SAVE_C, Main::OnSaveAutomaton)
	EVT_MENU(MenuBar::ID_SAVE_G, Main::OnSaveAlgorithm)
	EVT_MENU(MenuBar::ID_EXIT, Main::OnExit)
	EVT_MENU(MenuBar::ID_RESET_C, Main::OnResetAutomaton)
	EVT_MENU(MenuBar::ID_RESET_G, Main::OnResetAlgorithm)
	EVT_MENU(MenuBar::ID_DOCUMENTATION, Main::OnDocumentation)
wxEND_EVENT_TABLE()

Main::Main() : wxFrame(nullptr, wxID_ANY, "CellyGen", wxDefaultPosition, wxSize(1600, 900))
{
	SetIcon(wxICON(aaaIcon));
	
	Center();

	SetBackgroundColour(wxColour(255, 255, 255));

	BuildInterface();

	SetShortcuts();
}

Main::~Main()
{
	
}

void Main::BuildInterface()
{
	m_MenuBar = new MenuBar();
	this->SetMenuBar((wxMenuBar*) m_MenuBar);

	m_PanelInput = new PanelInput(this);

	wxPanel* panelRight = new wxPanel(this);
	m_PanelGrid = new PanelGrid(panelRight);
	m_PanelAlgorithm = new PanelAlgorithm(panelRight);

	wxBoxSizer* sizerRight = new wxBoxSizer(wxVERTICAL);
	sizerRight->Add((wxPanel*)m_PanelGrid, 2, wxEXPAND | wxBOTTOM, 6);
	sizerRight->Add((wxScrolledWindow*)m_PanelAlgorithm, 1, wxEXPAND);

	panelRight->SetSizer(sizerRight);

	wxBoxSizer* sizerMain = new wxBoxSizer(wxHORIZONTAL);
	sizerMain->Add((wxScrolledWindow*)m_PanelInput, 1, wxEXPAND | wxALL, 6);
	sizerMain->Add(panelRight, 3, wxEXPAND | wxTOP | wxRIGHT | wxBOTTOM, 6);
	this->SetSizer(sizerMain);
}

void Main::SetShortcuts()
{
	wxAcceleratorEntry entries[7];
	entries[0].Set(wxACCEL_CTRL, (int)'O', MenuBar::ID_OPEN_C);
	entries[1].Set(wxACCEL_CTRL | wxACCEL_SHIFT, (int)'O', MenuBar::ID_OPEN_G);
	entries[2].Set(wxACCEL_CTRL, (int)'S', MenuBar::ID_SAVE_C);
	entries[3].Set(wxACCEL_CTRL | wxACCEL_SHIFT, (int)'S', MenuBar::ID_SAVE_G);
	entries[4].Set(wxACCEL_CTRL, (int)'R', MenuBar::ID_RESET_C);
	entries[5].Set(wxACCEL_CTRL | wxACCEL_SHIFT, (int)'R', MenuBar::ID_RESET_G);
	entries[6].Set(wxACCEL_CTRL, (int)'D', MenuBar::ID_DOCUMENTATION);


	wxAcceleratorTable accel(7, entries);
	this->SetAcceleratorTable(accel);
}

void Main::OnOpenAutomaton(wxCommandEvent& evt)
{
	wxMessageBox("This is a wxWidgets Hello World example", "OnOpenAutomata", wxOK | wxICON_INFORMATION);
}

void Main::OnOpenAlgorithm(wxCommandEvent& evt)
{
	wxMessageBox("This is a wxWidgets Hello World example", "OnOpenAlgorithm", wxOK | wxICON_INFORMATION);
}

void Main::OnSaveAutomaton(wxCommandEvent& evt)
{
	wxMessageBox("This is a wxWidgets Hello World example", "OnSaveAutomata", wxOK | wxICON_INFORMATION);
}

void Main::OnSaveAlgorithm(wxCommandEvent& evt)
{
	wxMessageBox("This is a wxWidgets Hello World example", "OnSaveAlgorithm", wxOK | wxICON_INFORMATION);
}

void Main::OnExit(wxCommandEvent& evt)
{
	Close();
	evt.Skip();
}

void Main::OnResetAutomaton(wxCommandEvent& evt)
{
	wxMessageBox("This is a wxWidgets Hello World example", "OnResetAutomata", wxOK | wxICON_INFORMATION);
}

void Main::OnResetAlgorithm(wxCommandEvent& evt)
{
	wxMessageBox("This is a wxWidgets Hello World example", "OnResetAlgorithm", wxOK | wxICON_INFORMATION);
}

void Main::OnDocumentation(wxCommandEvent& evt)
{
	wxMessageBox("This is a wxWidgets Hello World example", "OnDocumentation", wxOK | wxICON_INFORMATION);
}