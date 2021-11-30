#include "InputRules.h"

InputRules::InputRules(wxWindow* parent) : wxPanel(parent)
{
	wxButton* button = new wxButton(this, Ids::ID_EDIT_RULES, wxString("Edit Rules"));
	m_List = new wxListView(this, Ids::ID_LIST_RULES);

	m_List->AppendColumn("#", wxLIST_FORMAT_LEFT, 32);
	m_List->AppendColumn("Rule");

	wxStaticBoxSizer* sizer = new wxStaticBoxSizer(wxVERTICAL, this, "Rules");
	sizer->Add(button, 0, wxEXPAND);
	sizer->Add(m_List, 1, wxEXPAND);

	this->SetSizer(sizer);
}

InputRules::~InputRules()
{
}

wxListView* InputRules::GetList()
{
    return m_List;
}
