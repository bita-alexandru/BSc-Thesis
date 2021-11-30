#include "InputStates.h"

InputStates::InputStates(wxWindow* parent) : wxPanel(parent)
{
	wxButton* button = new wxButton(this, Ids::ID_EDIT_STATES, wxString("Edit States"));
	m_List = new wxListView(this, Ids::ID_LIST_STATES);

	m_List->AppendColumn("#", wxLIST_FORMAT_LEFT, 32);
	m_List->AppendColumn("State");

	wxStaticBoxSizer* sizer = new wxStaticBoxSizer(wxVERTICAL, this, "States");
	sizer->Add(button, 0, wxEXPAND);
	sizer->Add(m_List, 1, wxEXPAND);

	this->SetSizer(sizer);
}

InputStates::~InputStates()
{
	
}

wxListView* InputStates::GetList()
{
	return m_List;
}
