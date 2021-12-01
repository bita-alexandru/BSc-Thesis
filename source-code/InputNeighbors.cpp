#include "InputNeighbors.h"

#define cout(x) wxMessageBox(x, "debug")

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

	wxToggleButton* m_NW = new wxToggleButton(row1, wxID_ANY, "North-West", wxDefaultPosition, wxSize(48, 48));
	wxToggleButton* m_N = new wxToggleButton(row1, wxID_ANY, "North", wxDefaultPosition, wxSize(48, 48));
	wxToggleButton* m_NE = new wxToggleButton(row1, wxID_ANY, "North-East", wxDefaultPosition, wxSize(48, 48));
	sizer1->Add(m_NW, 1, wxEXPAND);
	sizer1->Add(m_N, 1, wxEXPAND);
	sizer1->Add(m_NE, 1, wxEXPAND);
	row1->SetSizer(sizer1);

	wxPanel* row2 = new wxPanel(this);
	wxBoxSizer* sizer2 = new wxBoxSizer(wxHORIZONTAL);
	wxToggleButton* m_W = new wxToggleButton(row2, wxID_ANY, "West", wxDefaultPosition, wxSize(48, 48));
	wxToggleButton* m_C = new wxToggleButton(row2, wxID_ANY, "Center", wxDefaultPosition, wxSize(48, 48));
	wxToggleButton* m_E = new wxToggleButton(row2, wxID_ANY, "East", wxDefaultPosition, wxSize(48, 48));
	sizer2->Add(m_W, 1, wxEXPAND);
	sizer2->Add(m_C, 1, wxEXPAND);
	sizer2->Add(m_E, 1, wxEXPAND);
	row2->SetSizer(sizer2);

	wxPanel* row3 = new wxPanel(this);
	wxBoxSizer* sizer3 = new wxBoxSizer(wxHORIZONTAL);
	wxToggleButton* m_SW = new wxToggleButton(row3, wxID_ANY, "South-West", wxDefaultPosition, wxSize(48, 48));
	wxToggleButton* m_S = new wxToggleButton(row3, wxID_ANY, "South", wxDefaultPosition, wxSize(48, 48));
	wxToggleButton* m_SE = new wxToggleButton(row3, wxID_ANY, "South-East", wxDefaultPosition, wxSize(48, 48));
	sizer3->Add(m_SW, 1, wxEXPAND);
	sizer3->Add(m_S, 1, wxEXPAND);
	sizer3->Add(m_SE, 1, wxEXPAND);
	row3->SetSizer(sizer3);

	wxStaticBoxSizer* sizer = new wxStaticBoxSizer(wxVERTICAL, this, "Neighbors");
	sizer->Add(row1, 1, wxEXPAND);
	sizer->Add(row2, 1, wxEXPAND);
	sizer->Add(row3, 1, wxEXPAND);

	m_Buttons = {
		{"NW", m_NW}, {"N", m_N}, {"NE", m_NE},
		{"W", m_W}, {"C", m_C}, {"E", m_E},
		{"SW", m_SW}, {"S", m_S}, {"SE", m_SE}
	};

	this->SetSizer(sizer);
}
