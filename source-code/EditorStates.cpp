#include "EditorStates.h"

#include <algorithm>
#include <sstream>
#include <unordered_set>

#define cout(x) wxMessageBox(x, "debug")

wxBEGIN_EVENT_TABLE(EditorStates, wxFrame)
	EVT_CLOSE(EditorStates::OnClose)
	EVT_SHOW(EditorStates::OnShow)
wxEND_EVENT_TABLE()

EditorStates::EditorStates(wxFrame* parent) : wxFrame(parent, wxID_ANY, "CellyGen::States", wxDefaultPosition, wxSize(Sizes::EDITOR_WIDTH, Sizes::EDITOR_HEIGHT))
{
	SetIcon(wxICON(aaaIcon));

	Center();

	SetBackgroundColour(wxColour(255, 232, 214));

	BuildMenuBar();

	BuildInputPanel();
}

EditorStates::~EditorStates()
{
}

std::vector<std::string> EditorStates::GetData()
{
	std::string text = (std::string)m_TextCtrl->GetText();

	// remove empty lines, white spaces and carriage symbols
	text.erase(remove(text.begin(), text.end(), ' '), text.end());
	text.erase(remove(text.begin(), text.end(), '\r'), text.end());
	text.erase(std::unique(text.begin(), text.end(), [](char a, char b) {return a == '\n' && b == '\n'; }), text.end());

	// count lines (states) and mark duplicates/invalid states
	std::unordered_set<std::string> setStates;
	std::vector<int> indexDuplicates;
	std::vector<int> indexInvalid;

	std::stringstream ssText(text);
	std::string state;
	std::vector<std::string> states({ "FREE" });

	int cntLine = 0;
	while (std::getline(ssText, state, '\n'))
	{
		cntLine++;

		// state's name does not respect the character limits
		if (state.size() < Sizes::CHARS_STATE_MIN || state.size() > Sizes::CHARS_STATE_MAX)
		{
			indexInvalid.push_back(cntLine);
			continue;
		}

		// state's name contains illegal characters
		if (std::find_if(state.begin(), state.end(), [](char c) { return !(isalnum(c) || (c == '_')); }) != state.end())
		{
			indexInvalid.push_back(cntLine);
			continue;
		}

		states.push_back(state);

		// duplicate found
		if (setStates.find(state) != setStates.end())
		{
			indexDuplicates.push_back(cntLine);
		}
		else
		{
			setStates.insert(state);
		}
	}

	if (states.size() > Sizes::STATES_MAX || indexDuplicates.size() || indexInvalid.size())
	{
		// error
		cout(text);
		return std::vector<std::string>({ "FREE" });
	}

	return states;
}

void EditorStates::LoadData(wxListView* list)
{
	for (int i = 1; i < list->GetItemCount(); i++)
	{
		m_TextCtrl->WriteText(list->GetItemText(i, 1) + "\n");
	}
}

void EditorStates::BuildMenuBar()
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

void EditorStates::BuildInputPanel()
{
	std::string labelHelp = "so basically yeah, plz no more than 256 states lol ok ? thx.";
	wxStaticText* help = new wxStaticText(this, wxID_ANY, labelHelp);

	m_TextCtrl = new wxStyledTextCtrl(this);
	m_TextCtrl->SetMarginWidth(wxSTC_MARGIN_NUMBER, 32);
	m_TextCtrl->SetMarginType(wxSTC_MARGINOPTION_SUBLINESELECT, wxSTC_MARGIN_NUMBER);
	m_TextCtrl->SetScrollWidth(1);

	wxFont font = wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false);
	m_TextCtrl->StyleSetFont(wxSTC_STYLE_DEFAULT, font);

	wxButton* save = new wxButton(this, Ids::ID_SAVE_STATES, wxString("Save"));

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(help, 0, wxALL, 6);
	sizer->Add(m_TextCtrl, 1, wxEXPAND | wxLEFT | wxRIGHT, 6);
	sizer->Add(save, 0, wxALIGN_RIGHT | wxALL, 6);

	this->SetSizer(sizer);
}

void EditorStates::OnClose(wxCloseEvent& evt)
{
	this->Show(false);
}

void EditorStates::OnShow(wxShowEvent& evt)
{
	m_TextCtrl->SetFocus();
}
