#include "InputStates.h"

InputStates::InputStates(wxScrolledWindow* parent, wxStaticBoxSizer* sizer)
{
	wxButton* button = new wxButton(parent, Ids::ID_EDIT_STATES, wxString("Edit States"));
	m_List = new wxListView(parent, Ids::ID_LIST_STATES);

	m_List->AppendColumn("#", wxLIST_FORMAT_LEFT, 32);
	m_List->AppendColumn("State");

	sizer->Add(button, 0, wxEXPAND);
	sizer->Add(m_List, 1, wxEXPAND);
}

InputStates::~InputStates()
{
	
}

wxListView* InputStates::GetList()
{
	return m_List;
}
