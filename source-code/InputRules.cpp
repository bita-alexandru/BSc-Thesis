#include "InputRules.h"

InputRules::InputRules(wxScrolledWindow* parent, wxStaticBoxSizer* sizer)
{
	wxButton* button = new wxButton(parent, Ids::ID_EDIT_RULES, wxString("Edit Rules"));
	m_List = new wxListView(parent, Ids::ID_LIST_RULES);

	m_List->AppendColumn("#", wxLIST_FORMAT_LEFT, 32);
	m_List->AppendColumn("Rule");

	sizer->Add(button, 0, wxEXPAND);
	sizer->Add(m_List, 1, wxEXPAND);
}

InputRules::~InputRules()
{
}

wxListView* InputRules::GetList()
{
    return m_List;
}
