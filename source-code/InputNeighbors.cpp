#include "InputNeighbors.h"

InputNeighbors::InputNeighbors(wxWindow* parent) : wxPanel(parent)
{
	wxPanel* row1 = new wxPanel(this);
	wxBoxSizer* sizer1 = new wxBoxSizer(wxHORIZONTAL);
	m_NW = new wxToggleButton(row1, wxID_ANY, "North-West");
	m_N = new wxToggleButton(row1, wxID_ANY, "North");
	m_NE = new wxToggleButton(row1, wxID_ANY, "North-East");
	sizer1->Add(m_NW, 1, wxEXPAND);
	sizer1->Add(m_N, 1, wxEXPAND);
	sizer1->Add(m_NE, 1, wxEXPAND);
	row1->SetSizer(sizer1);

	wxPanel* row2 = new wxPanel(this);
	wxBoxSizer* sizer2 = new wxBoxSizer(wxHORIZONTAL);
	m_W = new wxToggleButton(row2, wxID_ANY, "West");
	m_C = new wxToggleButton(row2, wxID_ANY, "Center");
	m_E = new wxToggleButton(row2, wxID_ANY, "East");
	sizer2->Add(m_W, 1, wxEXPAND);
	sizer2->Add(m_C, 1, wxEXPAND);
	sizer2->Add(m_E, 1, wxEXPAND);
	row2->SetSizer(sizer2);

	wxPanel* row3 = new wxPanel(this);
	wxBoxSizer* sizer3 = new wxBoxSizer(wxHORIZONTAL);
	m_SW = new wxToggleButton(row3, wxID_ANY, "South-West");
	m_S = new wxToggleButton(row3, wxID_ANY, "South");
	m_SE = new wxToggleButton(row3, wxID_ANY, "South-East");
	sizer3->Add(m_SW, 1, wxEXPAND);
	sizer3->Add(m_S, 1, wxEXPAND);
	sizer3->Add(m_SE, 1, wxEXPAND);
	row3->SetSizer(sizer3);

	wxStaticBoxSizer* sizer = new wxStaticBoxSizer(wxVERTICAL, this, "Neighbors");
	sizer->Add(row1, 1, wxEXPAND);
	sizer->Add(row2, 1, wxEXPAND);
	sizer->Add(row3, 1, wxEXPAND);

	this->SetSizer(sizer);
}

InputNeighbors::~InputNeighbors()
{
}
