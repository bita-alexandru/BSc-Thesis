#include "InputStates.h"

InputStates::InputStates(wxScrolledWindow* parent, wxStaticBoxSizer* sizer)
{
	wxButton* button = new wxButton(parent, ID_EDIT_STATES, wxString("Edit States"));
	sizer->Add(button, 0, wxEXPAND);

	wxListView* list = new wxListView(parent, ID_LIST_STATES);
	sizer->Add(list, 1, wxEXPAND);

	list->AppendColumn("#", wxLIST_FORMAT_LEFT, 32);
	list->AppendColumn("State");
	list->AppendColumn("Color");
}

InputStates::~InputStates()
{
}

wxListView* InputStates::GetList()
{
	return m_List;
}
