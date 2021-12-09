#include "EditorRules.h"

#include <algorithm>
#include <sstream>
#include <unordered_set>

#define cout(x) wxLogDebug(x)

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

std::vector<std::string> EditorRules::GetData()
{
	std::string text = (std::string)m_TextCtrl->GetText().MakeUpper();

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
		// error
		//cout(text);
		return std::vector<std::string>();
	}
	
	return rules;
}

void EditorRules::BuildMenuBar()
{
	wxMenu* menu = new wxMenu();

	menu->Append(Ids::ID_FIND, "&Find\tCtrl-F");
	menu->Append(Ids::ID_REPLACE, "&Replace\tCtrl-R");
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
	m_TextCtrl->SetMarginWidth(wxSTC_MARGIN_NUMBER, 32);
	m_TextCtrl->SetMarginType(wxSTC_MARGINOPTION_SUBLINESELECT, wxSTC_MARGIN_NUMBER);
	m_TextCtrl->SetScrollWidth(1);

	wxFont font = wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false);
	m_TextCtrl->StyleSetFont(wxSTC_STYLE_DEFAULT, font);

	wxButton* close = new wxButton(this, Ids::ID_SAVE_RULES, wxString("Close"));

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(help, 0, wxALL, 6);
	sizer->Add(m_TextCtrl, 1, wxEXPAND | wxLEFT | wxRIGHT, 6);
	sizer->Add(close, 0, wxALIGN_RIGHT | wxALL, 6);

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
