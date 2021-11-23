#include "InputStates.h"

InputStates::InputStates(wxScrolledWindow* parent, wxStaticBoxSizer* sizer)
{
	wxButton* button = new wxButton(parent, IDs::ID_EDIT_STATES, wxString("Edit States"));
	wxListView* list = new wxListView(parent, IDs::ID_LIST_STATES);

	list->AppendColumn("#", wxLIST_FORMAT_LEFT, 32);
	list->AppendColumn("State");
	list->AppendColumn("Color");

	sizer->Add(button, 0, wxEXPAND);
	sizer->Add(list, 1, wxEXPAND);
}

InputStates::~InputStates()
{
}

wxListView* InputStates::GetList()
{
	return m_List;
}
