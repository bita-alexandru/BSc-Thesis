#include "EditorStates.h"

#include <algorithm>
#include <sstream>
#include <unordered_set>

wxBEGIN_EVENT_TABLE(EditorStates, wxFrame)
	EVT_CLOSE(EditorStates::OnCloseEvent)
	EVT_SHOW(EditorStates::OnShowEvent)
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
	std::string text = (std::string)m_TextCtrl->GetText().Upper();

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
		// to do, messagebox options: "mark&resolve", "ignore", "cancel"
		return std::vector<std::string>({ "FREE" });
	}

	return states;
}

void EditorStates::GoTo(std::string state)
{
	int result = m_TextCtrl->SearchNext(wxSTC_FIND_WHOLEWORD, state);

	// not found
	if (result == -1)
	{
		// to do, message box informing the text was not found
	}
	else
	{
		m_TextCtrl->ShowPosition(result);
		m_TextCtrl->SetSelection(result, result + state.size());

		Show();
		SetFocus();
	}
}

void EditorStates::DeleteState(std::string state)
{
	int result = m_TextCtrl->SearchNext(wxSTC_FIND_WHOLEWORD, state);

	if (result != -1) m_TextCtrl->LineDelete();
}

void EditorStates::BuildMenuBar()
{
	wxMenu* menuFile = new wxMenu();
	wxMenu* menuEdit = new wxMenu();

	menuFile->Append(Ids::ID_IMPORT_STATES, "&Import\tCtrl-I");
	menuFile->Append(Ids::ID_EXPORT_STATES, "E&xport\tCtrl-X");
	menuFile->AppendSeparator();
	menuFile->Append(Ids::ID_SAVE_STATES, "&Save\tCtrl-S");
	menuFile->Append(Ids::ID_SAVE_CLOSE_STATES, "Sa&ve && Close\tCtrl-Shift-S");
	menuFile->AppendSeparator();
	menuFile->Append(Ids::ID_CLOSE_STATES, "&Close\tAlt-F4");
	menuEdit->Append(Ids::ID_FIND_STATES, "&Find\tCtrl-F");
	menuEdit->Append(Ids::ID_REPLACE_STATES, "&Replace\tCtrl-H");
	menuEdit->AppendSeparator();
	menuEdit->Append(Ids::ID_MARK_PREV_STATES, "&Previous Mark\tCtrl-Q");
	menuEdit->Append(Ids::ID_MARK_NEXT_STATES, "&Next Mark\tCtrl-E");
	menuEdit->Append(Ids::ID_MARK_CLEAR_STATES, "&Clear Marks\tCtrl-B");
	menuEdit->AppendSeparator();
	menuEdit->Append(Ids::ID_FORMAT_STATES, "Forma&t\tCtrl-T");

	m_MenuBar = new wxMenuBar();
	m_MenuBar->Append(menuFile, "&File");
	m_MenuBar->Append(menuEdit, "&Edit");

	m_MenuBar->Enable(Ids::ID_MARK_PREV_STATES, false);
	m_MenuBar->Enable(Ids::ID_MARK_NEXT_STATES, false);
	m_MenuBar->Enable(Ids::ID_MARK_CLEAR_STATES, false);

	this->SetMenuBar(m_MenuBar);

	Bind(wxEVT_COMMAND_MENU_SELECTED, &EditorStates::OnImport, this, Ids::ID_IMPORT_STATES);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &EditorStates::OnExport, this, Ids::ID_EXPORT_STATES);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &EditorStates::OnClose, this, Ids::ID_CLOSE_STATES);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &EditorStates::OnSave, this, Ids::ID_SAVE_STATES);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &EditorStates::OnSaveClose, this, Ids::ID_SAVE_CLOSE_STATES);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &EditorStates::OnMenuFind, this, Ids::ID_FIND_STATES);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &EditorStates::OnMenuReplace, this, Ids::ID_REPLACE_STATES);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &EditorStates::OnPrevMark, this, Ids::ID_MARK_PREV_STATES);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &EditorStates::OnNextMark, this, Ids::ID_MARK_NEXT_STATES);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &EditorStates::OnClearMark, this, Ids::ID_MARK_CLEAR_STATES);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &EditorStates::OnFormat, this, Ids::ID_FORMAT_STATES);
}

void EditorStates::BuildInputPanel()
{
	std::string labelHelp = "so basically yeah, plz no more than 256 states lol ok ? thx.";
	wxStaticText* help = new wxStaticText(this, wxID_ANY, labelHelp);

	m_TextCtrl = new wxStyledTextCtrl(this, 7777);
	m_TextCtrl->SetMarginWidth(wxSTC_MARGIN_NUMBER, 80);
	m_TextCtrl->SetMarginType(wxSTC_MARGINOPTION_SUBLINESELECT, wxSTC_MARGIN_NUMBER);
	m_TextCtrl->SetScrollWidth(1);
	m_TextCtrl->SetMarginSensitive(wxSTC_MARGIN_NUMBER, true);
	m_TextCtrl->MarkerSetBackground(wxSTC_MARK_CIRCLE, wxColour("blue"));

	m_TextCtrl->Bind(wxEVT_STC_MARGINCLICK, &EditorStates::AddMarker, this);

	wxFont font = wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false);
	m_TextCtrl->StyleSetFont(wxSTC_STYLE_DEFAULT, font);

	wxPanel* panelButtons = new wxPanel(this);
	wxButton* close = new wxButton(panelButtons, Ids::ID_CLOSE_STATES, wxString("Close"));
	wxButton* save = new wxButton(panelButtons, Ids::ID_SAVE_STATES, wxString("Save"));
	wxButton* saveClose = new wxButton(panelButtons, Ids::ID_SAVE_CLOSE_STATES, wxString("Save && Close"));

	close->Bind(wxEVT_BUTTON, &EditorStates::OnClose, this);
	save->Bind(wxEVT_BUTTON, &EditorStates::OnSave, this);
	saveClose->Bind(wxEVT_BUTTON, &EditorStates::OnSaveClose, this);

	wxBoxSizer* sizerButtons = new wxBoxSizer(wxHORIZONTAL);
	panelButtons->SetSizerAndFit(new wxBoxSizer(wxHORIZONTAL));
	sizerButtons->Add(close, 0);
	sizerButtons->Add(save, 0, wxLEFT, 6);
	sizerButtons->Add(saveClose, 0);

	panelButtons->SetSizerAndFit(sizerButtons);

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(help, 0, wxALL, 6);
	sizer->Add(m_TextCtrl, 1, wxEXPAND);
	sizer->Add(panelButtons, 0, wxALIGN_RIGHT | wxALL, 6);

	this->SetSizer(sizer);
}

void EditorStates::BuildDialogFind(std::string title, long style)
{
	wxDELETE(m_FindDialog);
	wxDELETE(m_FindData);

	m_FindData = new wxFindReplaceData(wxFR_DOWN);
	m_FindDialog = new wxFindReplaceDialog(this, m_FindData, title, style);

	m_FindDialog->Bind(wxEVT_FIND, &EditorStates::OnFind, this);
	m_FindDialog->Bind(wxEVT_FIND_NEXT, &EditorStates::OnFindNext, this);
	m_FindDialog->Bind(wxEVT_FIND_REPLACE, &EditorStates::OnReplace, this);
	m_FindDialog->Bind(wxEVT_FIND_REPLACE_ALL, &EditorStates::OnReplaceAll, this);
}

void EditorStates::OnCloseEvent(wxCloseEvent& evt)
{
	wxDELETE(m_FindData); wxDELETE(m_FindDialog);

	// undo modifications
	m_TextCtrl->SetText(m_PrevText);

	Hide();
}

void EditorStates::OnShowEvent(wxShowEvent& evt)
{
	if (!IsShown())
	{
		m_PrevText = m_TextCtrl->GetText();
	}

	m_TextCtrl->SetFocus();
}

void EditorStates::OnClose(wxCommandEvent& evt)
{
	wxDELETE(m_FindData); wxDELETE(m_FindDialog);

	// undo modifications
	m_TextCtrl->SetText(m_PrevText);

	Hide();
}

void EditorStates::OnSave(wxCommandEvent& evt)
{
	m_InputStates->SetStates(GetData());
}

void EditorStates::OnSaveClose(wxCommandEvent& evt)
{
	wxDELETE(m_FindData); wxDELETE(m_FindDialog);

	m_InputStates->SetStates(GetData());

	Hide();
}

void EditorStates::OnMenuFind(wxCommandEvent& evt)
{
	BuildDialogFind("Find", 0);

	m_FindDialog->Show();
}

void EditorStates::OnMenuReplace(wxCommandEvent& evt)
{
	BuildDialogFind("Find & Replace", wxFR_REPLACEDIALOG);

	m_FindDialog->Show();
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

void EditorStates::OnFormat(wxCommandEvent& evt)
{
	std::string text = (std::string)m_TextCtrl->GetText().Upper();

	// remove empty lines, white spaces and carriage symbols
	text.erase(remove(text.begin(), text.end(), ' '), text.end());
	text.erase(remove(text.begin(), text.end(), '\r'), text.end());
	text.erase(std::unique(text.begin(), text.end(), [](char a, char b) {return a == '\n' && b == '\n'; }), text.end());

	m_TextCtrl->SetText(text);
}

void EditorStates::OnPrevMark(wxCommandEvent& evt)
{
	int line = m_TextCtrl->MarkerPrevious(--m_MarkLine, 1);

	if (line == -1) line = m_TextCtrl->MarkerPrevious(m_TextCtrl->GetLineCount(), 1);
	m_MarkLine = line;// -1;

	int position = m_TextCtrl->PositionFromLine(line);
	m_TextCtrl->ShowPosition(position);

	m_TextCtrl->SetSelectionMode(wxSTC_SEL_LINES);
	m_TextCtrl->SetSelection(position, position);
	m_TextCtrl->SetSelectionMode(wxSTC_SEL_STREAM);
}

void EditorStates::OnNextMark(wxCommandEvent& evt)
{
	int line = m_TextCtrl->MarkerNext(++m_MarkLine, 1);

	if (line == -1) line = m_TextCtrl->MarkerNext(0, 1);
	m_MarkLine = line;// +1;

	int position = m_TextCtrl->PositionFromLine(line);
	m_TextCtrl->ShowPosition(position);

	m_TextCtrl->SetSelectionMode(wxSTC_SEL_LINES);
	m_TextCtrl->SetSelection(position, position);
	m_TextCtrl->SetSelectionMode(wxSTC_SEL_STREAM);
}

void EditorStates::OnClearMark(wxCommandEvent& evt)
{
	m_TextCtrl->MarkerDeleteAll(wxSTC_MARK_CIRCLE);

	m_MarkLine = -1;

	m_MenuBar->Enable(Ids::ID_MARK_PREV_STATES, false);
	m_MenuBar->Enable(Ids::ID_MARK_NEXT_STATES, false);
	m_MenuBar->Enable(Ids::ID_MARK_CLEAR_STATES, false);
}

void EditorStates::OnImport(wxCommandEvent& evt)
{
}

void EditorStates::OnExport(wxCommandEvent& evt)
{
}

void EditorStates::AddMarker(wxStyledTextEvent& evt)
{
	int line = m_TextCtrl->LineFromPosition(evt.GetPosition());

	if (m_TextCtrl->MarkerGet(line))
	{
		m_TextCtrl->MarkerDelete(line, wxSTC_MARK_CIRCLE);

		if (m_TextCtrl->MarkerNext(0, 1)  == -1)
		{
			m_MenuBar->Enable(Ids::ID_MARK_PREV_STATES, false);
			m_MenuBar->Enable(Ids::ID_MARK_NEXT_STATES, false);
			m_MenuBar->Enable(Ids::ID_MARK_CLEAR_STATES, false);
		}
	}
	else
	{
		m_TextCtrl->MarkerAdd(line, wxSTC_MARK_CIRCLE);

		m_MenuBar->Enable(Ids::ID_MARK_PREV_STATES, true);
		m_MenuBar->Enable(Ids::ID_MARK_NEXT_STATES, true);
		m_MenuBar->Enable(Ids::ID_MARK_CLEAR_STATES, true);
	}
}
