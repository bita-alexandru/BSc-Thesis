#include "EditorNeighbors.h"

EditorNeighbors::EditorNeighbors(wxFrame* parent) : wxFrame(parent, wxID_ANY, "CellyGen::Neighbors", wxDefaultPosition, wxSize(Sizes::EDITOR_WIDTH, Sizes::EDITOR_HEIGHT))
{
	SetIcon(wxICON(aaaIcon));

	Center();

	SetBackgroundColour(wxColour(255, 232, 214));

	BuildInputPanel();

	LoadData();
}

EditorNeighbors::~EditorNeighbors()
{
}

std::vector<std::string> EditorNeighbors::GetData()
{
    return std::vector<std::string>();
}

void EditorNeighbors::BuildInputPanel()
{
	std::string labelHelp = "so basically yeah, plz no more than 256 states lol ok ? thx.";
	wxStaticText* help = new wxStaticText(this, wxID_ANY, labelHelp);

	wxPanel* buttons = new wxPanel(this);
	m_NW = new wxToggleButton(buttons, wxID_ANY, "North-West");
	m_N = new wxToggleButton(buttons, wxID_ANY, "North");
	m_NE = new wxToggleButton(buttons, wxID_ANY, "North-East");
	m_E = new wxToggleButton(buttons, wxID_ANY, "East");
	m_SE = new wxToggleButton(buttons, wxID_ANY, "South-East");
	m_S = new wxToggleButton(buttons, wxID_ANY, "South");
	m_SW = new wxToggleButton(buttons, wxID_ANY, "South-West");
	m_W = new wxToggleButton(buttons, wxID_ANY, "West");
	m_C = new wxToggleButton(buttons, wxID_ANY, "Center");

	wxGridSizer* grid = new wxGridSizer(3, 3, 24, 24);
	grid->Add(m_NW, 1, wxEXPAND); grid->Add(m_N, 1, wxEXPAND); grid->Add(m_NE, 1, wxEXPAND);
	grid->Add(m_W, 1, wxEXPAND); grid->Add(m_C, 1, wxEXPAND); grid->Add(m_E, 1, wxEXPAND);
	grid->Add(m_SW, 1, wxEXPAND); grid->Add(m_S, 1, wxEXPAND); grid->Add(m_SE, 1, wxEXPAND);

	buttons->SetSizerAndFit(grid);

	wxButton* close = new wxButton(this, Ids::ID_SAVE_NEIGHBORS, wxString("Close"));

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(help, 0, wxALL, 6);
	sizer->Add(buttons, 1, wxEXPAND | wxLEFT | wxRIGHT, 6);
	sizer->Add(close, 0, wxALIGN_RIGHT | wxALL, 6);

	this->SetSizer(sizer);
}

void EditorNeighbors::LoadData()
{
}
