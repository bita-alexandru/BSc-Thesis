#include "InputRules.h"

#define cout(x) wxMessageBox(x, "debug")

InputRules::InputRules(wxWindow* parent) : wxPanel(parent)
{
	BuildInterface();
}

InputRules::~InputRules()
{
}

ListRules* InputRules::GetList()
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
        int id = i;
        std::string rule = rules[i];

        if (i > nOfItems - 1)
        {
            wxColour bgColorA("white"), bgColorB("white");
            wxColour txtColorA("black"), txtColorB("black");
            // come back later to this [TO DO]
            if (m_States.find(rule) != m_States.end())
            {
                bgColorA = wxColour(m_States[rule]); bgColorB = wxColour(m_States[rule]);
                txtColorA = (bgColorA.Red() * 0.299 + bgColorA.Green() * 0.587 + bgColorA.Blue() * 0.114) > 186.0 ? wxColour("black") : wxColour("white");
                txtColorB = (bgColorB.Red() * 0.299 + bgColorB.Green() * 0.587 + bgColorB.Blue() * 0.114) > 186.0 ? wxColour("black") : wxColour("white");
            }

            m_List->PushBack({ id, rule, "", "" }, { bgColorA, txtColorA }, { bgColorB, txtColorB });
            m_List->ChangeItemStateA(i, rule);

            continue;
        }

        wxString itmId = m_List->GetItemText(i, 0);
        wxString itmRule = m_List->GetItemText(i, 1);
        wxColour itmColor = m_List->GetItemBackgroundColour(i);

        if (itmId != id)
        {
            m_List->ChangeItemId(i, id);
        }
        if (itmRule != rule)
        {
            m_List->ChangeItemStateA(i, rule);
        }
    }

    while (i < nOfItems--)
    {
        m_List->DeleteItem(i);
    }

    m_List->RefreshAfterUpdate();
}

void InputRules::SetStates(std::unordered_map<std::string, std::string>& states)
{
    m_States = states;
}

void InputRules::BuildInterface()
{
	wxButton* button = new wxButton(this, Ids::ID_EDIT_RULES, wxString("Edit Rules"));
	m_List = new ListRules(this);

	wxStaticBoxSizer* sizer = new wxStaticBoxSizer(wxVERTICAL, this, "Rules");
	sizer->Add(button, 0, wxEXPAND);
	sizer->Add(m_List, 1, wxEXPAND);

	this->SetSizer(sizer);
}
