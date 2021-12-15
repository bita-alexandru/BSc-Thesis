#include "InputNeighbors.h"

InputNeighbors::InputNeighbors(wxWindow* parent) : wxPanel(parent)
{
	BuildInterface();
}

InputNeighbors::~InputNeighbors()
{
}

std::unordered_set<std::string>& InputNeighbors::GetNeighbors()
{
	return m_Neighbors;
}

void InputNeighbors::SetNeighbors(std::vector<std::string> neighbors)
{
}

void InputNeighbors::BuildInterface()
{
	wxPanel* row1 = new wxPanel(this);
	wxBoxSizer* sizer1 = new wxBoxSizer(wxHORIZONTAL);

	wxToggleButton* m_NW = new wxToggleButton(row1, Ids::ID_NEIGHBOR_NW, "North-West", wxDefaultPosition, wxSize(48, 48));
	wxToggleButton* m_N = new wxToggleButton(row1, Ids::ID_NEIGHBOR_N, "North", wxDefaultPosition, wxSize(48, 48));
	wxToggleButton* m_NE = new wxToggleButton(row1, Ids::ID_NEIGHBOR_NE, "North-East", wxDefaultPosition, wxSize(48, 48));
	sizer1->Add(m_NW, 1, wxEXPAND);
	sizer1->Add(m_N, 1, wxEXPAND);
	sizer1->Add(m_NE, 1, wxEXPAND);
	row1->SetSizer(sizer1);

	wxPanel* row2 = new wxPanel(this);
	wxBoxSizer* sizer2 = new wxBoxSizer(wxHORIZONTAL);
	wxToggleButton* m_W = new wxToggleButton(row2, Ids::ID_NEIGHBOR_W, "West", wxDefaultPosition, wxSize(48, 48));
	wxToggleButton* m_C = new wxToggleButton(row2, Ids::ID_NEIGHBOR_C, "Center", wxDefaultPosition, wxSize(48, 48));
	wxToggleButton* m_E = new wxToggleButton(row2, Ids::ID_NEIGHBOR_E, "East", wxDefaultPosition, wxSize(48, 48));
	sizer2->Add(m_W, 1, wxEXPAND);
	sizer2->Add(m_C, 1, wxEXPAND);
	sizer2->Add(m_E, 1, wxEXPAND);
	row2->SetSizer(sizer2);

	wxPanel* row3 = new wxPanel(this);
	wxBoxSizer* sizer3 = new wxBoxSizer(wxHORIZONTAL);
	wxToggleButton* m_SW = new wxToggleButton(row3, Ids::ID_NEIGHBOR_SW, "South-West", wxDefaultPosition, wxSize(48, 48));
	wxToggleButton* m_S = new wxToggleButton(row3, Ids::ID_NEIGHBOR_S, "South", wxDefaultPosition, wxSize(48, 48));
	wxToggleButton* m_SE = new wxToggleButton(row3, Ids::ID_NEIGHBOR_SE, "South-East", wxDefaultPosition, wxSize(48, 48));
	sizer3->Add(m_SW, 1, wxEXPAND);
	sizer3->Add(m_S, 1, wxEXPAND);
	sizer3->Add(m_SE, 1, wxEXPAND);
	row3->SetSizer(sizer3);

	wxStaticBoxSizer* sizer = new wxStaticBoxSizer(wxVERTICAL, this, "Neighbors");
	sizer->Add(row1, 1, wxEXPAND);
	sizer->Add(row2, 1, wxEXPAND);
	sizer->Add(row3, 1, wxEXPAND);

	this->SetSizer(sizer);

	m_Buttons = {
		{"NW", m_NW}, {"N", m_N}, {"NE", m_NE},
		{"W", m_W}, {"C", m_C}, {"E", m_E},
		{"SW", m_SW}, {"S", m_S}, {"SE", m_SE}
	};

	m_Ids = {
		{Ids::ID_NEIGHBOR_NW, "NW"}, {Ids::ID_NEIGHBOR_N, "N"}, {Ids::ID_NEIGHBOR_NE, "NE"},
		{Ids::ID_NEIGHBOR_W, "W"}, {Ids::ID_NEIGHBOR_C, "C"}, {Ids::ID_NEIGHBOR_E, "E"},
		{Ids::ID_NEIGHBOR_SW, "SW"}, {Ids::ID_NEIGHBOR_S, "S"}, {Ids::ID_NEIGHBOR_SE, "SE"}
	};

	for (auto it : m_Ids)
	{
		m_Buttons[it.second]->Bind(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, &InputNeighbors::OnToggleButton, this);
	}
}

void InputNeighbors::OnToggleButton(wxCommandEvent& evt)
{
	std::string neighbor = m_Ids[evt.GetId()];

	if (m_Neighbors.find(neighbor) != m_Neighbors.end())
	{
		// neighbor already in our list -> remove it
		m_Neighbors.erase(neighbor);
	}
	else
	{
		// neighbor not in list -> add it
		m_Neighbors.insert(neighbor);
	}
}
