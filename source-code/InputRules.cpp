#include "InputRules.h"

InputRules::InputRules(wxScrolledWindow* parent, wxStaticBoxSizer* sizer)
{
	wxButton* button = new wxButton(parent, ID_EDIT_RULES, wxString("Edit Rules"));
	sizer->Add(button, 0, wxEXPAND);

	wxListView * list = new wxListView(parent, ID_LIST_RULES);
	sizer->Add(list, 1, wxEXPAND);

	list->AppendColumn("#", wxLIST_FORMAT_LEFT, 32);
	list->AppendColumn("Rule");
}

InputRules::~InputRules()
{
}

wxListView* InputRules::GetList()
{
    return m_List;
}
