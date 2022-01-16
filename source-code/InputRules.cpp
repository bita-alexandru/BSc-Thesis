#include "InputRules.h"
#include "Interpreter.h"

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

std::unordered_multimap<std::string, Transition>& InputRules::GetRules()
{
	return m_Rules;
}

InputStates* InputRules::GetInputStates()
{
    return m_InputStates;
}

InputNeighbors* InputRules::GetInputNeighbors()
{
    return m_InputNeighbors;
}

void InputRules::SetEditorRules(EditorRules* editorRules)
{
    m_EditorRules = editorRules;
}

void InputRules::SetInputStates(InputStates* inputStates)
{
    m_InputStates = inputStates;
}

void InputRules::SetInputNeighbors(InputNeighbors* inputNeighbors)
{
    m_InputNeighbors = inputNeighbors;
}

void InputRules::SetRules(std::vector<std::pair<std::string, Transition>> rules)
{
    // rules appear in our list but not in the given list -> they got recently deleted
    /*for (auto it1 = m_Rules.begin(); it1 != m_Rules.end();)
    {
        bool erased = false;

        for (int i = 0; i < rules.size(); i++)
            if (it1->first == rules[i].first)
            {
                if (it1->second.orRules == rules[i].second.orRules)
                {
                    it1 = m_Rules.erase(it1);
                    erased = true;
                }
                else
                {
                    auto er = m_Rules.equal_range(it1->first);
                    for (auto it2 = er.first; it2 != er.second; it2++)
                    {
                        if (it2->second.orRules == rules[i].second.orRules)
                        {
                            m_Rules.erase(it2);
                            break;
                        }
                    }
                }
            }

        if (!erased) it1++;
    }*/

    // rules appear in the given list but not in our map -> they got recently introduced
    for (int i = 0; i < rules.size(); i++)
    {
        if (m_Rules.find(rules[i].first) == m_Rules.end())
        {
            m_Rules.insert(rules[i]);
            //wxLogDebug("INSERT-1");
        }
        else if (m_Rules.find(rules[i].first) != m_Rules.end())
        {
            auto er = m_Rules.equal_range(rules[i].first);
            bool present = false;

            for (auto& it = er.first; it != er.second; it++)
            {
                if (rules[i].second.orRules == it->second.orRules)
                {
                    present = true;
                    break;
                }
            }

            if (!present) m_Rules.insert(rules[i]);
        }
    }

    m_States = m_InputStates->GetStates();

    // update list display
    int nOfItems = m_List->GetItemCount();
    int i = 0;
    std::unordered_set<std::string> alreadyUpdated;
    for (i = 0; i < rules.size(); i++)
    {
        int id = i + 1;
        std::string state1 = rules[i].first;
        std::string state2 = rules[i].second.state;
        std::string condition = rules[i].second.condition;

        if (alreadyUpdated.find(state1 + "-" + state2 + "-" + condition) != alreadyUpdated.end()) continue;

        if (i > nOfItems - 1)
        {
            wxColour bgColorA("white"), bgColorB("white");
            wxColour txtColorA("black"), txtColorB("black");
            // TO DO, actually separate the <stateA> <stateB> <condition>
            if (m_States.find(state1) != m_States.end() && m_States.find(state2) != m_States.end())
            {
                bgColorA = wxColour(m_States[state1]); bgColorB = wxColour(m_States[state2]);
                txtColorA = (bgColorA.Red() * 0.299 + bgColorA.Green() * 0.587 + bgColorA.Blue() * 0.114) > 186.0 ? wxColour("black") : wxColour("white");
                txtColorB = (bgColorB.Red() * 0.299 + bgColorB.Green() * 0.587 + bgColorB.Blue() * 0.114) > 186.0 ? wxColour("black") : wxColour("white");
            }

            m_List->PushBack({ id, state1, state2, condition }, { bgColorA, txtColorA }, { bgColorB, txtColorB });

            continue;
        }

        wxString itmId = m_List->GetItemText(i, 0);
        wxString itmState1 = m_List->GetState1(i);
        wxString itmState2 = m_List->GetState2(i);
        wxString itmCondition = m_List->GetCond(i);
        wxColour itmColor = m_List->GetItemBackgroundColour(i);

        if (itmId != id)
        {
            m_List->ChangeItemId(i, id);
        }
        if (itmState1 != state1)
        {
            m_List->ChangeItemState1(i, state1);
        }
        if (itmState2 != state2)
        {
            m_List->ChangeItemState2(i, state2);
        }
        if (itmCondition != condition)
        {
            m_List->ChangeItemCond(i, condition);
        }

        alreadyUpdated.insert(state1 + "-" + state2 + "-" + condition);
    }

    while (i < nOfItems--)
    {
        std::string state1 = m_List->GetState1(i);
        std::string state2 = m_List->GetState2(i);
        std::string condition = m_List->GetCond(i);

        // to do, come back when items are inserted properly
        //std::string rule = state1;// +"->" + state2;
        //if (!cond.empty()) rule += ":" + cond;

        m_List->Erase(i);
        if (alreadyUpdated.find(state1 + "-" + state2 + "-" + condition) != alreadyUpdated.end()) continue;
    }

    m_List->RefreshAfterUpdate();
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

    std::string rule = state1 +"/" + state2;
    if (!cond.empty()) rule += ":" + cond;
    rule += ";";

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

        std::string rule = state1 +"/" + state2;
        if (!cond.empty()) rule += ":" + cond;
        rule += ";";

        selection = m_List->GetNextSelected(selection);

        toBeDeleted.insert(rule);

        m_EditorRules->DeleteRule(rule);
    }

    std::string rules = "";
    for (int i = 0; i < m_List->GetItemCount(); i++)
    {
        std::string state1 = m_List->GetState1(i);
        std::string state2 = m_List->GetState2(i);
        std::string cond = m_List->GetCond(i);

        std::string rule = state1 + "/" + state2;
        if (!cond.empty()) rule += ":" + cond;
        rule += ";";

        if (toBeDeleted.find(rule) == toBeDeleted.end()) rules += rule + "\n";
    }

    //wxLogDebug("RULES=\n<%s>", rules);

    // to do, update m_Rules
    Interpreter interpreter;
    interpreter.SetStates(m_InputStates->GetStates());
    interpreter.SetNeighbors(m_InputNeighbors->GetNeighbors());
    interpreter.Process(rules);
    
    SetRules(interpreter.GetTransitions());
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
