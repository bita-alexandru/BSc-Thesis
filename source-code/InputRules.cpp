#include "InputRules.h"

#define cout(x) wxMessageBox(x, "debug")

InputRules::InputRules(wxWindow* parent) : wxPanel(parent)
{
	BuildInterface();
}

InputRules::~InputRules()
{
}

wxListView* InputRules::GetList()
{
    return m_List;
}

std::unordered_map<std::string, std::unordered_map<std::string, std::any>>& InputRules::GetRules()
{
	return m_Rules;
}

void InputRules::SetRules(std::vector<std::string> rules)
{
    // rules appear in our list but not in the given list -> they got recently deleted
    for (auto it = m_Rules.begin(); it != m_Rules.end();)
    {
        if (std::find(rules.begin(), rules.end(), it->first) == rules.end())
        {
            //cout("deleted");
            it = m_Rules.erase(it);
        }
        else
        {
            it++;
        }
    }

    // rules appear in the given list but not in our map -> they got recently introduced
    for (int i = 0; i < rules.size(); i++)
    {
        if (m_Rules.find(rules[i]) == m_Rules.end())
        {
            m_Rules.insert({ rules[i], { } });
        }
    }

    // update list display
    int nOfItems = m_List->GetItemCount();
    int i = 0;
    for (i = 0; i < rules.size(); i++)
    {
        wxString id = std::to_string(i);
        wxString rule = rules[i];

        if (i > nOfItems - 1)
        {
            m_List->InsertItem(i, id);
            m_List->SetItem(i, 1, rule);

            continue;
        }

        wxString itmId = m_List->GetItemText(i, 0);
        wxString itmRule = m_List->GetItemText(i, 1);
        wxColour itmColor = m_List->GetItemBackgroundColour(i);

        if (itmId != id)
        {
            m_List->SetItem(i, 0, id);
        }
        if (itmRule != rule)
        {
            m_List->SetItem(i, 1, rule);
        }
    }

    while (i < nOfItems--)
    {
        m_List->DeleteItem(i);
    }
}

void InputRules::BuildInterface()
{
	wxButton* button = new wxButton(this, Ids::ID_EDIT_RULES, wxString("Edit Rules"));
	m_List = new wxListView(this, Ids::ID_LIST_RULES, wxDefaultPosition, wxSize(128, 128));

	m_List->AppendColumn("#", wxLIST_FORMAT_LEFT, 32);
	m_List->AppendColumn("Rule");

	wxStaticBoxSizer* sizer = new wxStaticBoxSizer(wxVERTICAL, this, "Rules");
	sizer->Add(button, 0, wxEXPAND);
	sizer->Add(m_List, 1, wxEXPAND);

	this->SetSizer(sizer);
}
