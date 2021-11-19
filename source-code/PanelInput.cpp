#include "PanelInput.h"

PanelInput::PanelInput(wxWindow* parent): wxScrolledWindow(parent)
{
	SetBackgroundColour(wxColor(250, 210, 225));
	wxStaticBoxSizer* sizer = new wxStaticBoxSizer(wxVERTICAL, this, wxString("States && Rules"));

	m_InputStates = new InputStates(this, sizer);
	sizer->Add(new wxStaticLine(this), 0, wxEXPAND | wxALL, 8);
	m_InputNeighbors = new InputNeighbors(this, sizer);
	sizer->Add(new wxStaticLine(this), 0, wxEXPAND | wxALL, 8);
	m_InputRules = new InputRules(this, sizer);

	this->SetScrollRate(4, 4);
	this->SetSizer(sizer);
}

PanelInput::~PanelInput()
{
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