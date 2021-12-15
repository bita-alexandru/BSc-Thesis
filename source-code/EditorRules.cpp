#include "EditorRules.h"

#include <algorithm>
#include <sstream>
#include <unordered_set>

wxBEGIN_EVENT_TABLE(EditorRules, wxFrame)
	EVT_CLOSE(EditorRules::OnClose)
	EVT_SHOW(EditorRules::OnShow)
wxEND_EVENT_TABLE()

EditorRules::EditorRules(wxFrame* parent) : wxFrame(parent, wxID_ANY, "CellyGen::Rules", wxDefaultPosition, wxSize(Sizes::EDITOR_WIDTH, Sizes::EDITOR_HEIGHT))
{
	SetIcon(wxICON(aaaIcon));

	Center();

    SetBackgroundColour(wxColour(255, 232, 214));

	BuildMenuBar();

	BuildInputPanel();
}

EditorRules::~EditorRules()
{
}

void EditorRules::SetInputRules(InputRules* inputRules)
{
	m_InputRules = inputRules;
}

std::vector<std::string> EditorRules::GetData()
{
	std::string text = (std::string)m_TextCtrl->GetText().Upper();

	// remove empty lines, white spaces and carriage symbols
	text.erase(remove(text.begin(), text.end(), ' '), text.end());
	text.erase(remove(text.begin(), text.end(), '\r'), text.end());
	text.erase(std::unique(text.begin(), text.end(), [](char a, char b) {return a == '\n' && b == '\n'; }), text.end());

	// count lines (rules) and mark duplicates/invalid rules
	std::unordered_set<std::string> setRules;
	std::vector<int> indexDuplicates;
	std::vector<int> indexInvalid;

	std::stringstream ssText(text);
	std::string rule;
	std::vector<std::string> rules;

	int cntLine = 0;
	while (std::getline(ssText, rule, '\n'))
	{
		cntLine++;

		if (rule.size() < 1) continue;

		// rule's name does not respect the character limits
		if (rule.size() < Sizes::CHARS_RULE_MIN || rule.size() > Sizes::CHARS_RULE_MAX)
		{
			indexInvalid.push_back(cntLine);
			continue;
		}

		// rule's name contains illegal characters
		if (std::find_if(rule.begin(), rule.end(), [](char c) { return !(isalnum(c) || (std::string("_->@()&<>+=#:").find(c)) != std::string::npos); }) != rule.end())
		{
			indexInvalid.push_back(cntLine);
			continue;
		}

		rules.push_back(rule);

		// duplicate found
		if (setRules.find(rule) != setRules.end())
		{
			indexDuplicates.push_back(cntLine);
		}
		else
		{
			setRules.insert(rule);
		}
	}

	if (rules.size() > Sizes::RULES_MAX || indexDuplicates.size() || indexInvalid.size())
	{
		// to do, messagebox options: mark&resolve, ignore, cancel
		return std::vector<std::string>();
	}
	
	return rules;
}

void EditorRules::GoTo(std::string rule)
{
	int result = FindRule(rule);

	// not found
	if (result == -1)
	{
		return;
	}

	int selectionEnd = m_TextCtrl->GetLineEndPosition(result);
	m_TextCtrl->ShowPosition(selectionEnd);

	m_TextCtrl->SetSelectionMode(wxSTC_SEL_LINES);
	m_TextCtrl->SetSelection(selectionEnd, selectionEnd);
	m_TextCtrl->SetSelectionMode(wxSTC_SEL_STREAM);

	Show();
	SetFocus();
}

void EditorRules::DeleteRule(std::string rule)
{	
	int result = FindRule(rule);

	if (result != -1)
	{
		int selectionEnd = m_TextCtrl->GetLineEndPosition(result);

		m_TextCtrl->SetSelectionMode(wxSTC_SEL_LINES);
		m_TextCtrl->SetSelection(selectionEnd, selectionEnd);
		m_TextCtrl->SetSelectionMode(wxSTC_SEL_STREAM);

		m_TextCtrl->LineDelete();
	}
}

void EditorRules::BuildMenuBar()
{
	wxMenu* menu = new wxMenu();

	menu->Append(Ids::ID_FIND_RULES, "&Find\tCtrl-F");
	menu->Append(Ids::ID_REPLACE_RULES, "&Replace\tCtrl-R");
	menu->AppendSeparator();
	menu->Append(Ids::ID_EXIT, "E&xit");

	wxMenuBar* menuBar = new wxMenuBar();
	menuBar->Append(menu, "&File");

	this->SetMenuBar(menuBar);
}

void EditorRules::BuildInputPanel()
{
	std::string labelHelp = "so basically yeah, plz no more than 256 Rules lol ok ? thx.";
	wxStaticText* help = new wxStaticText(this, wxID_ANY, labelHelp);

	m_TextCtrl = new wxStyledTextCtrl(this);
	m_TextCtrl->SetMarginWidth(wxSTC_MARGIN_NUMBER, 48);
	m_TextCtrl->SetMarginType(wxSTC_MARGINOPTION_SUBLINESELECT, wxSTC_MARGIN_NUMBER);
	m_TextCtrl->SetScrollWidth(1);

	wxFont font = wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false);
	m_TextCtrl->StyleSetFont(wxSTC_STYLE_DEFAULT, font);

	wxButton* save = new wxButton(this, Ids::ID_SAVE_RULES, wxString("Save"));
	save->Bind(wxEVT_BUTTON, &EditorRules::OnSave, this);

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(help, 0, wxALL, 6);
	sizer->Add(m_TextCtrl, 1, wxEXPAND, 6);
	sizer->Add(save, 0, wxALIGN_RIGHT | wxALL, 6);

	this->SetSizer(sizer);
}

void EditorRules::LoadData(wxListView* list)
{
	for (int i = 0; i < list->GetItemCount(); i++)
	{
		m_TextCtrl->WriteText(list->GetItemText(i, 1) + "\n");
	}
}

void EditorRules::OnClose(wxCloseEvent& evt)
{
	this->Hide();
}

void EditorRules::OnShow(wxShowEvent& evt)
{
	if (evt.IsShown())
	{
		m_TextCtrl->SetFocus();
	}
}

void EditorRules::OnSave(wxCommandEvent& evt)
{
	m_InputRules->SetRules(GetData());

	Hide();
}

int EditorRules::FindRule(std::string rule)
{
	for (int i = 0; i < m_TextCtrl->GetLineCount(); i++)
	{
		std::string line = std::string(m_TextCtrl->GetLine(i).Upper());
		line.erase(remove(line.begin(), line.end(), ' '), line.end());
		line.erase(remove(line.begin(), line.end(), '\r'), line.end());
		line.erase(remove(line.begin(), line.end(), '\n'), line.end());

		if (rule == line) return i;
	}

	return -1;
}
