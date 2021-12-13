#include "EditorStates.h"

#include <algorithm>
#include <sstream>
#include <unordered_set>

#define cout(x) wxLogDebug(x)

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
	wxDELETE(m_FindData);
	wxDELETE(m_FindDialog);
}

void EditorStates::SetInputStates(InputStates* inputStates)
{
	m_InputStates = inputStates;
}

std::vector<std::string> EditorStates::GetData()
{
	std::string text = (std::string)m_TextCtrl->GetText().MakeUpper();

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

		if (state.size() < 1) continue;

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
		//cout(text);
		return std::vector<std::string>({ "FREE" });
	}

	return states;
}

void EditorStates::LoadData()
{
	ListStates* list = m_InputStates->GetList();

	for (int i = 1; i < list->GetItemCount(); i++)
	{
		m_TextCtrl->WriteText(list->GetItemText(i, 1) + "\n");
	}
}

void EditorStates::GoTo(std::string row)
{
	int result = m_TextCtrl->SearchNext(wxSTC_FIND_WHOLEWORD, row);

	// not found
	if (result == -1)
	{
		// to do, message box informing the text was not found
	}
	else
	{
		m_TextCtrl->ShowPosition(result);
		m_TextCtrl->SetSelection(result, result + row.size());

		Show();
		SetFocus();
	}
}

void EditorStates::BuildMenuBar()
{
	wxMenu* menuFile = new wxMenu();
	wxMenu* menuEdit = new wxMenu();

	menuFile->Append(Ids::ID_IMPORT_STATES, "&Import\tCtrl-O");
	menuFile->Append(Ids::ID_EXPORT_STATES, "&Export\tCtrl-S");
	menuFile->AppendSeparator();
	menuFile->Append(Ids::ID_EXIT, "E&xit");
	menuEdit->Append(Ids::ID_FIND_STATES, "&Find\tCtrl-F");
	menuEdit->Append(Ids::ID_REPLACE_STATES, "&Replace\tCtrl-H");

	wxMenuBar* menuBar = new wxMenuBar();
	menuBar->Append(menuFile, "&File");
	menuBar->Append(menuEdit, "&Edit");

	this->SetMenuBar(menuBar);

	Bind(wxEVT_COMMAND_MENU_SELECTED, &EditorStates::OnMenuFind, this, Ids::ID_FIND_STATES);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &EditorStates::OnMenuReplace, this, Ids::ID_REPLACE_STATES);
}

void EditorStates::BuildInputPanel()
{
	std::string labelHelp = "so basically yeah, plz no more than 256 states lol ok ? thx.";
	wxStaticText* help = new wxStaticText(this, wxID_ANY, labelHelp);

	m_TextCtrl = new wxStyledTextCtrl(this);
	m_TextCtrl->SetMarginWidth(wxSTC_MARGIN_NUMBER, 48);
	m_TextCtrl->SetMarginType(wxSTC_MARGINOPTION_SUBLINESELECT, wxSTC_MARGIN_NUMBER);
	m_TextCtrl->SetScrollWidth(1);

	wxFont font = wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false);
	m_TextCtrl->StyleSetFont(wxSTC_STYLE_DEFAULT, font);

	wxButton* save = new wxButton(this, Ids::ID_SAVE_STATES, wxString("Save"));
	save->Bind(wxEVT_BUTTON, &EditorStates::OnSave, this);

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(help, 0, wxALL, 6);
	sizer->Add(m_TextCtrl, 1, wxEXPAND, 6);
	sizer->Add(save, 0, wxALIGN_RIGHT | wxALL, 6);

	this->SetSizer(sizer);
}

void EditorStates::OnClose(wxCloseEvent& evt)
{
	m_InputStates->SetStates(GetData());

	Hide();
}

void EditorStates::OnShow(wxShowEvent& evt)
{
	m_TextCtrl->SetFocus();
}

void EditorStates::OnSave(wxCommandEvent& evt)
{
	m_InputStates->SetStates(GetData());

	Hide();
}

void EditorStates::OnMenuFind(wxCommandEvent& evt)
{
	wxDELETE(m_FindDialog);
	wxDELETE(m_FindData);

	m_FindData = new wxFindReplaceData(wxFR_DOWN);
	m_FindDialog = new wxFindReplaceDialog(this, m_FindData, "Find");

	m_FindDialog->Show();

	m_FindDialog->Bind(wxEVT_FIND, &EditorStates::OnFind, this);
	m_FindDialog->Bind(wxEVT_FIND_NEXT, &EditorStates::OnFindNext, this);
	m_FindDialog->Bind(wxEVT_FIND_REPLACE, &EditorStates::OnReplace, this);
	m_FindDialog->Bind(wxEVT_FIND_REPLACE_ALL, &EditorStates::OnReplaceAll, this);
}

void EditorStates::OnMenuReplace(wxCommandEvent& evt)
{
	wxDELETE(m_FindDialog);
	wxDELETE(m_FindData);

	m_FindData = new wxFindReplaceData(wxFR_DOWN);
	m_FindDialog = new wxFindReplaceDialog(this, m_FindData, "Find & Replace", wxFR_REPLACEDIALOG);

	m_FindDialog->Show();

	m_FindDialog->Bind(wxEVT_FIND, &EditorStates::OnFind, this);
	m_FindDialog->Bind(wxEVT_FIND_NEXT, &EditorStates::OnFindNext, this);
	m_FindDialog->Bind(wxEVT_FIND_REPLACE, &EditorStates::OnReplace, this);
	m_FindDialog->Bind(wxEVT_FIND_REPLACE_ALL, &EditorStates::OnReplaceAll, this);
}

void EditorStates::OnFind(wxFindDialogEvent& evt)
{
	wxString find = m_FindData->GetFindString();

	int flags = m_FindData->GetFlags();
	int result = m_TextCtrl->FindText(0, m_TextCtrl->GetLastPosition(), find, flags);

	if (result == -1)
	{
		// to do, messagebox
		return;
	}

	m_TextCtrl->ShowPosition(result);
	m_TextCtrl->SetSelection(result, result + find.size());
}

void EditorStates::OnFindNext(wxFindDialogEvent& evt)
{
	wxString find = m_FindData->GetFindString();

	int flags = m_FindData->GetFlags();
	int result;

	if (flags & wxFR_DOWN) result = m_TextCtrl->FindText(m_TextCtrl->GetAnchor() + 1, m_TextCtrl->GetLastPosition(), find, flags);
	else result = m_TextCtrl->FindText(m_TextCtrl->GetAnchor(), 0, find, flags);

	if (result == -1)
	{
		// to do, messagebox
		return;
	}

	m_TextCtrl->ShowPosition(result);
	m_TextCtrl->SetSelection(result, result + find.size());
}

void EditorStates::OnReplace(wxFindDialogEvent& evt)
{
	wxString replace = m_FindData->GetReplaceString();
	m_TextCtrl->ReplaceSelection(replace);
}

void EditorStates::OnReplaceAll(wxFindDialogEvent& evt)
{
	wxString find = m_FindData->GetFindString();
	wxString replace = m_FindData->GetReplaceString();

	int flags = m_FindData->GetFlags();

	int occurences = 0;
	int result;
	while ((result = m_TextCtrl->FindText(0, m_TextCtrl->GetLastPosition(), find, flags)) != -1)
	{
		occurences++;

		m_TextCtrl->SetSelection(result, result + find.size());
		m_TextCtrl->ReplaceSelection(replace);
	}

	// to do, messagebox occurences
}
