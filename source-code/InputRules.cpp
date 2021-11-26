#include "InputRules.h"

InputRules::InputRules(wxScrolledWindow* parent, wxStaticBoxSizer* sizer)
{
	wxButton* button = new wxButton(parent, Ids::ID_EDIT_RULES, wxString("Edit Rules"));
	wxListView * list = new wxListView(parent, Ids::ID_LIST_RULES);

	list->AppendColumn("#", wxLIST_FORMAT_LEFT, 32);
	list->AppendColumn("Rule");

	sizer->Add(button, 0, wxEXPAND);
	sizer->Add(list, 1, wxEXPAND);
}

InputRules::~InputRules()
{
}

wxListView* InputRules::GetList()
{
    return m_List;
}
