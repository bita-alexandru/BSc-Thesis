#include "PanelInput.h"

PanelInput::PanelInput(wxWindow* parent) : wxScrolledWindow(parent)
{
	SetBackgroundColour(wxColor(250, 210, 225));

	m_InputStates = new InputStates(this);
	m_InputNeighbors = new InputNeighbors(this);
	m_InputRules = new InputRules(this);

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(m_InputStates, 1, wxEXPAND | wxALL, 6);
	sizer->Add(m_InputNeighbors, 1, wxEXPAND | wxLEFT | wxRIGHT, 6);
	sizer->Add(m_InputRules, 1, wxEXPAND | wxALL, 6);

	this->SetSizer(sizer);
	this->SetScrollRate(4, 4);
}

PanelInput::~PanelInput()
{
	wxDELETE(m_InputStates);
	wxDELETE(m_InputNeighbors);
	wxDELETE(m_InputRules);
}

InputStates* PanelInput::GetInputStates()
{
	return m_InputStates;
}

InputNeighbors* PanelInput::GetInputNeighbors()
{
	return m_InputNeighbors;
}

InputRules* PanelInput::GetInputRules()
{
	return m_InputRules;
}


//void xd()
//{
//	wxButton* buttonStates = new wxButton(panel, ID_EDIT_STATES, wxString("Edit States"));
//	sizer->Add(buttonStates, 0, wxEXPAND);
//
//	wxListView* listStates = new wxListView(panel, ID_LIST_STATES);
//	listStates->AppendColumn(wxString("#"));
//	listStates->AppendColumn(wxString("State"));
//	listStates->AppendColumn(wxString("Color"));
//	sizer->Add(listStates, 1, wxEXPAND);
//
//	sizer->Add(new wxStaticLine(panel), 0, wxEXPAND | wxALL, 16);
//
//	wxButton* buttonNeighbors = new wxButton(panel, ID_EDIT_NEIGHBORS, wxString("Edit Neighbors"));
//	sizer->Add(buttonNeighbors, 0, wxEXPAND);
//
//	wxListView* listNeighbors = new wxListView(panel, ID_LIST_NEIGHBORS);
//	listNeighbors->AppendColumn(wxString("#"));
//	listNeighbors->AppendColumn(wxString("Direction"));
//	listNeighbors->AppendColumn(wxString("Alias"));
//	sizer->Add(listNeighbors, 1, wxEXPAND);
//
//	sizer->Add(new wxStaticLine(panel), 0, wxEXPAND | wxALL, 16);
//
//	wxButton* buttonRules = new wxButton(panel, ID_EDIT_RULES, wxString("Edit Rules"));
//	sizer->Add(buttonRules, 0, wxEXPAND);
//
//	wxListView* listRules = new wxListView(panel, ID_LIST_RULES);
//	listRules->AppendColumn(wxString("#"));
//	listRules->AppendColumn(wxString("Rule"));
//	sizer->Add(listRules, 1, wxEXPAND);
//}