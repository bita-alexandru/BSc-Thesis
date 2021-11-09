#include "Main.h"

wxBEGIN_EVENT_TABLE(Main, wxFrame)
EVT_MENU(ID_IMPORT_C, Main::OnImportC)
EVT_MENU(ID_IMPORT_G, Main::OnImportG)
EVT_MENU(ID_EXPORT_C, Main::OnExportC)
EVT_MENU(ID_EXPORT_G, Main::OnExportG)
EVT_MENU(ID_EXIT, Main::OnExit)

EVT_MENU(ID_RESET_C, Main::OnResetC)
EVT_MENU(ID_RESET_G, Main::OnResetG)

EVT_MENU(ID_DOCUMENTATION, Main::OnDocumentation)
wxEND_EVENT_TABLE()

Main::Main() : wxFrame(nullptr, wxID_ANY, "CellyGen", wxDefaultPosition, wxSize(1600, 900))
{
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
	wxMenu* menuImport = new wxMenu();
	wxMenu* menuExport = new wxMenu();
	wxMenu* menuReset = new wxMenu();
	wxMenu* menuHelp = new wxMenu();

	menuImport->Append(ID_IMPORT_C, "&Cellular automaton configuration");
	menuImport->Append(ID_IMPORT_G, "&Genetic algorithm configuration");

	menuExport->Append(ID_EXPORT_C, "&Cellular automaton configuration");
	menuExport->Append(ID_EXPORT_G, "&Genetic algorithm configuration");

	menuFile->AppendSubMenu(menuImport, "&Import");
	menuFile->AppendSubMenu(menuExport, "&Export");
	menuFile->AppendSeparator();
	menuFile->Append(ID_EXIT, "E&xit");

	menuReset->Append(ID_RESET_C, "Reset &cellular automaton");
	menuReset->Append(ID_RESET_G, "Reset &genetic algorithm");

	menuHelp->Append(ID_DOCUMENTATION, "&Documentation");

	m_MenuBar->Append(menuFile, "&File");
	m_MenuBar->Append(menuReset, "&Reset");
	m_MenuBar->Append(menuHelp, "&Help");
}

void Main::OnImportC(wxCommandEvent& evt)
{
	wxMessageBox("This is a wxWidgets Hello World example", "OnImportC", wxOK | wxICON_INFORMATION);
}

void Main::OnImportG(wxCommandEvent& evt)
{
	wxMessageBox("This is a wxWidgets Hello World example", "OnImportG", wxOK | wxICON_INFORMATION);
}

void Main::OnExportC(wxCommandEvent& evt)
{
	wxMessageBox("This is a wxWidgets Hello World example", "OnExportC", wxOK | wxICON_INFORMATION);
}

void Main::OnExportG(wxCommandEvent& evt)
{
	wxMessageBox("This is a wxWidgets Hello World example", "OnExportG", wxOK | wxICON_INFORMATION);
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
