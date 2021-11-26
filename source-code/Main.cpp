#include "Main.h"

wxBEGIN_EVENT_TABLE(Main, wxFrame)
	// MenuBar
	EVT_MENU(Constants::ID_OPEN_C, Main::OnOpenAutomaton)
	EVT_MENU(Constants::ID_OPEN_G, Main::OnOpenAlgorithm)
	EVT_MENU(Constants::ID_SAVE_C, Main::OnSaveAutomaton)
	EVT_MENU(Constants::ID_SAVE_G, Main::OnSaveAlgorithm)
	EVT_MENU(Constants::ID_EXIT, Main::OnExit)
	EVT_MENU(Constants::ID_RESET_C, Main::OnResetAutomaton)
	EVT_MENU(Constants::ID_RESET_G, Main::OnResetAlgorithm)
	EVT_MENU(Constants::ID_DOCUMENTATION, Main::OnDocumentation)

	// ToolZoom
	EVT_BUTTON(Constants::ID_ZOOM_IN, Main::OnZoomIn)
	EVT_BUTTON(Constants::ID_ZOOM_OUT, Main::OnZoomOut)

	// Editors
	EVT_BUTTON(Constants::ID_EDIT_STATES, Main::EditStates)
	EVT_BUTTON(Constants::ID_EDIT_NEIGHBORS, Main::EditNeighbors)
	EVT_BUTTON(Constants::ID_EDIT_RULES, Main::EditRules)

	// Save Buttons
	EVT_BUTTON(Constants::ID_SAVE_STATES, Main::SaveStates)
wxEND_EVENT_TABLE()

Main::Main() : wxFrame(nullptr, wxID_ANY, "CellyGen", wxDefaultPosition, wxSize(Constants::MAIN_WIDTH, Constants::MAIN_HEIGHT))
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
	this->SetMenuBar(m_MenuBar);

	m_PanelInput = new PanelInput(this);

	wxPanel* panelGridAlgorithm = new wxPanel(this);
	m_PanelGrid = new PanelGrid(panelGridAlgorithm);
	m_PanelAlgorithm = new PanelAlgorithm(panelGridAlgorithm);

	wxBoxSizer* sizerGridAlgorithm = new wxBoxSizer(wxVERTICAL);
	sizerGridAlgorithm->Add(m_PanelGrid, 3, wxEXPAND | wxBOTTOM, 6);
	sizerGridAlgorithm->Add(m_PanelAlgorithm, 1, wxEXPAND);

	panelGridAlgorithm->SetSizer(sizerGridAlgorithm);

	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(m_PanelInput, 1, wxEXPAND | wxALL, 6);
	sizer->Add(panelGridAlgorithm, 3, wxEXPAND | wxTOP | wxRIGHT | wxBOTTOM, 6);
	this->SetSizer(sizer);
}

void Main::SetShortcuts()
{
	wxAcceleratorEntry entries[7];
	entries[0].Set(wxACCEL_CTRL, (int)'O', Constants::ID_OPEN_C);
	entries[1].Set(wxACCEL_CTRL | wxACCEL_SHIFT, (int)'O', Constants::ID_OPEN_G);
	entries[2].Set(wxACCEL_CTRL, (int)'S', Constants::ID_SAVE_C);
	entries[3].Set(wxACCEL_CTRL | wxACCEL_SHIFT, (int)'S', Constants::ID_SAVE_G);
	entries[4].Set(wxACCEL_CTRL, (int)'R', Constants::ID_RESET_C);
	entries[5].Set(wxACCEL_CTRL | wxACCEL_SHIFT, (int)'R', Constants::ID_RESET_G);
	entries[6].Set(wxACCEL_CTRL, (int)'D', Constants::ID_DOCUMENTATION);

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

void Main::OnZoomIn(wxCommandEvent& evt)
{
	m_PanelGrid->GetGridTools()->GetToolZoom()->SetSize('+', m_PanelGrid->GetGrid());
}

void Main::OnZoomOut(wxCommandEvent& evt)
{
	m_PanelGrid->GetGridTools()->GetToolZoom()->SetSize('-', m_PanelGrid->GetGrid());
}

void Main::EditStates(wxCommandEvent& evt)
{
	if (m_EditorStates == nullptr)
	{
		m_EditorStates = new EditorStates(this);
		m_EditorStates->Show();
	}
	else
	{
		m_EditorStates->SetFocus();
	}
}

void Main::EditNeighbors(wxCommandEvent& evt)
{
	if (m_EditorNeighbors == nullptr)
	{
		m_EditorNeighbors = new EditorNeighbors(this);
		m_EditorNeighbors->Show();
	}
	else
	{
		m_EditorNeighbors->SetFocus();
	}
}

void Main::EditRules(wxCommandEvent& evt)
{
	if (m_EditorRules == nullptr)
	{
		m_EditorRules = new EditorRules(this);
		m_EditorRules->Show();
	}
	else
	{
		m_EditorRules->SetFocus();
	}
}

void Main::SaveStates(wxCommandEvent& evt)
{
	wxMessageBox("uite ca a mers", "ce mirare", wxOK | wxICON_INFORMATION);
}
