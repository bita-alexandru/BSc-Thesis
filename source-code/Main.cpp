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
}

Main::~Main()
{
	
}

void Main::BuildMenuBar()
{
	m_MenuBar = new wxMenuBar();
	this->SetMenuBar(m_MenuBar);

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

	menuHelp->Append(ID_DOCUMENTATION, "&Documentation\tCtrl-H");

	m_MenuBar->Append(menuFile, "&File");
	m_MenuBar->Append(menuReset, "&Reset");
	m_MenuBar->Append(menuHelp, "&Help");
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
