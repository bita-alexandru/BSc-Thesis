#include "InputRules.h"

#include <unordered_set>

InputRules::InputRules(wxWindow* parent) : wxPanel(parent)
{
	BuildInterface();

    BuildMenu();
}

InputRules::~InputRules()
{
    wxDELETE(m_Menu);
}

ListRules* InputRules::GetList()
{
    return m_List;
}

std::unordered_map<std::string, std::unordered_map<std::string, std::any>>& InputRules::GetRules()
{
	return m_Rules;
}

void InputRules::SetEditorRules(EditorRules* editorRules)
{
    m_EditorRules = editorRules;
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
    std::unordered_set<std::string> alreadyUpdated;
    for (i = 0; i < rules.size(); i++)
    {
        int id = i + 1;
        std::string rule = rules[i];

        if (alreadyUpdated.find(rule) != alreadyUpdated.end()) continue;

        if (i > nOfItems - 1)
        {
            wxColour bgColorA("white"), bgColorB("white");
            wxColour txtColorA("black"), txtColorB("black");
            // TO DO, actually separate the <stateA> <stateB> <condition>
            if (m_States.find(rule) != m_States.end())
            {
                bgColorA = wxColour(m_States[rule]); bgColorB = wxColour(m_States[rule]);
                txtColorA = (bgColorA.Red() * 0.299 + bgColorA.Green() * 0.587 + bgColorA.Blue() * 0.114) > 186.0 ? wxColour("black") : wxColour("white");
                txtColorB = (bgColorB.Red() * 0.299 + bgColorB.Green() * 0.587 + bgColorB.Blue() * 0.114) > 186.0 ? wxColour("black") : wxColour("white");
            }

            m_List->PushBack({ id, rule, "", "" }, { bgColorA, txtColorA }, { bgColorB, txtColorB });
            m_List->ChangeItemState1(i, rule);

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
            m_List->ChangeItemState1(i, rule);
        }

        alreadyUpdated.insert(rule);
    }

    while (i < nOfItems--)
    {
        std::string state1 = m_List->GetState1(i);
        std::string state2 = m_List->GetState2(i);
        std::string cond = m_List->GetCond(i);

        // to do, come back when items are inserted properly
        std::string rule = state1;// +"->" + state2;
        if (!cond.empty()) rule += ":" + cond;

        m_List->Erase(i);
        if (alreadyUpdated.find(rule) != alreadyUpdated.end()) continue;
    }

    m_List->RefreshAfterUpdate();
}

void InputRules::SetStates(std::unordered_map<std::string, std::string>& states)
{
    m_States = states;
}

void InputRules::BuildInterface()
{
	wxButton* edit = new wxButton(this, Ids::ID_EDIT_RULES, wxString("Edit Rules"));
    edit->Bind(wxEVT_BUTTON, &InputRules::OnEdit, this);

    m_Search = new wxSearchCtrl(this, wxID_ANY);
    m_Search->Bind(wxEVT_TEXT, &InputRules::Search, this);
    m_Search->Bind(wxEVT_SEARCHCTRL_SEARCH_BTN, &InputRules::SearchEnter, this);

	m_List = new ListRules(this);
    m_List->Bind(wxEVT_CONTEXT_MENU, &InputRules::OnItemRightClick, this);

    wxButton* focusSearch = new wxButton(this, Ids::ID_SEARCH_RULES);
    focusSearch->Hide();
    focusSearch->Bind(wxEVT_BUTTON, &InputRules::FocusSearch, this);

	wxStaticBoxSizer* sizer = new wxStaticBoxSizer(wxVERTICAL, this, "Rules");
	sizer->Add(edit, 0, wxEXPAND);
    sizer->Add(m_Search, 0, wxEXPAND);
	sizer->Add(m_List, 1, wxEXPAND);

	this->SetSizer(sizer);
}

void InputRules::Search(wxCommandEvent& evt)
{
    std::string query = std::string(evt.GetString().MakeUpper());

    // unselect previous found queries
    int selection = m_List->GetFirstSelected();
    while (selection != -1)
    {
        m_List->Select(selection, false);
        selection = m_List->GetNextSelected(selection);
    }

    if (query.size() < 1) return;

    for (int i = 0; i < m_List->GetItemCount(); i++)
    {
        // to do, come back here when the items are inserted properly
        std::string row = m_List->GetState1(i) + "-" + m_List->GetState2(i) + "-" + m_List->GetCond(i);

        if (row.find(query) != row.npos)
        {
            m_List->Select(i);
            m_List->EnsureVisible(i);

            return;
        }
    }
}

void InputRules::SearchEnter(wxCommandEvent& evt)
{
    std::string query = std::string(evt.GetString().MakeUpper());

    int selection = m_List->GetFirstSelected();

    if (query.size() < 1) return;

    // continue searching
    if (wxGetKeyState(WXK_SHIFT))
    {
        // search upwards
        for (int i = selection - 1; i > -1; i--)
        {
            std::string row = m_List->GetState1(i) + "-" + m_List->GetState2(i) + "-" + m_List->GetCond(i);

            if (row.find(query) != row.npos)
            {
                m_List->Select(selection, false);
                m_List->Select(i);
                m_List->EnsureVisible(i);

                return;
            }
        }

        return;
    }

    // search downwards
    for (int i = selection + 1; i < m_List->GetItemCount(); i++)
    {
        std::string row = m_List->GetState1(i) + "-" + m_List->GetState2(i) + "-" + m_List->GetCond(i);

        if (row.find(query) != row.npos)
        {
            m_List->Select(selection, false);
            m_List->Select(i);
            m_List->EnsureVisible(i);

            return;
        }
    }
}

void InputRules::BuildMenu()
{
    m_Menu = new wxMenu();

    m_Menu->Append(Ids::ID_GOTO_RULE, "Go To");
    m_Menu->AppendSeparator();
    m_Menu->Append(Ids::ID_DELETE_RULE, "Delete");

    m_Menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &InputRules::OnMenuSelected, this);
}

void InputRules::OnItemRightClick(wxContextMenuEvent& evt)
{
    int itemCount = m_List->GetSelectedItemCount();
    if (itemCount != 1)
    {
        m_Menu->Enable(Ids::ID_GOTO_RULE, false);

        if (itemCount) m_Menu->Enable(Ids::ID_DELETE_RULE, true);
        else m_Menu->Enable(Ids::ID_DELETE_RULE, false);
    }
    else
    {
        m_Menu->Enable(Ids::ID_GOTO_RULE, true);
        m_Menu->Enable(Ids::ID_DELETE_RULE, true);
    }

    PopupMenu(m_Menu);
}

void InputRules::OnMenuSelected(wxCommandEvent& evt)
{
    switch (evt.GetId())
    {
    case Ids::ID_GOTO_RULE:
        RuleGoTo();
        break;
    case Ids::ID_DELETE_RULE:
        RuleDelete();
        break;
    default:
        break;
    }
}

void InputRules::RuleGoTo()
{
    int selection = m_List->GetFirstSelected();
    std::string state1 = m_List->GetState1(selection);
    std::string state2 = m_List->GetState2(selection);
    std::string cond = m_List->GetCond(selection);

    // to do, come back when items are inserted properly
    std::string rule = state1;// +"->" + state2;
    if (!cond.empty()) rule += ":" + cond;

    m_EditorRules->GoTo(rule);
}

void InputRules::RuleDelete()
{
    int selection = m_List->GetFirstSelected();
    std::unordered_set<std::string> toBeDeleted;

    while (selection != -1)
    {
        std::string state1 = m_List->GetState1(selection);
        std::string state2 = m_List->GetState2(selection);
        std::string cond = m_List->GetCond(selection);

        // to do, come back when items are inserted properly
        std::string rule = state1;// +"->" + state2;
        if (!cond.empty()) rule += ":" + cond;

        selection = m_List->GetNextSelected(selection);

        toBeDeleted.insert(rule);

        m_EditorRules->DeleteRule(rule);
    }

    std::vector<std::string> rules;
    for (int i = 0; i < m_List->GetItemCount(); i++)
    {
        std::string state1 = m_List->GetState1(i);
        std::string state2 = m_List->GetState2(i);
        std::string cond = m_List->GetCond(i);

        // to do, come back when items are inserted properly
        std::string rule = state1;// +"->" + state2;
        if (!cond.empty()) rule += ":" + cond;

        if (toBeDeleted.find(rule) == toBeDeleted.end()) rules.push_back(rule);
    }

    SetRules(rules);

    // TO DO, update the editors textbox
}

void InputRules::OnEdit(wxCommandEvent& evt)
{
    m_List->SetFocus();

    m_EditorRules->Show();
    m_EditorRules->SetFocus();
}

void InputRules::FocusSearch(wxCommandEvent& evt)
{
    m_Search->SetFocus();
}
