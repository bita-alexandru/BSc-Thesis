#include "EditorStates.h"

#include <algorithm>
#include <sstream>
#include <unordered_set>
#include "wx/app.h"

wxBEGIN_EVENT_TABLE(EditorStates, wxFrame)
	EVT_CLOSE(EditorStates::OnCloseEvent)
	EVT_SET_FOCUS(EditorStates::OnFocusEvent)
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
	text.erase(remove(text.begin(), text.end(), '\t'), text.end());

	// count lines (states) and mark duplicates/invalid states
	std::unordered_set<std::string> setStates;
	std::vector<int> indexInvalid;

	std::stringstream ssText(text);
	std::string state;
	std::vector<std::string> states({ "FREE" });

	int cntLine = -1;
	while (std::getline(ssText, state, '\n'))
	{
		cntLine++;

		if (state.size() < 1) continue;

		// separate line comment
		if (state.size() > 1 && state.substr(0, 2) == "//") continue;

		// inline comment, ignore it but continue with the state before it
		if (state.find("//") != state.npos) state = state.substr(0, state.find("//"));

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

		// duplicate found
		if (setStates.find(state) != setStates.end())
		{
			indexInvalid.push_back(cntLine);
			continue;
		}
		else
		{
			setStates.insert(state);
		}

		states.push_back(state);
	}

	if (states.size() > Sizes::STATES_MAX)
	{
		wxMessageDialog dialog(
			this, "The maximum allowed number of states has been surpassed.\nMake sure you're within the given limit before saving.", "Error",
			wxOK | wxICON_ERROR
		);
		dialog.SetExtendedMessage(
			"Limit: " + std::to_string(Sizes::STATES_MAX - 1) + "\nCurrent number: " + std::to_string(states.size() - 1)
		);

		int answer = dialog.ShowModal();

		m_InvalidInput = true;
		return {};
	}
	else if (indexInvalid.size())
	{
		wxMessageDialog dialog(
			this, "Some of the states appear to be invalid.", "Warning",
			wxYES_NO | wxCANCEL | wxICON_EXCLAMATION
		);
		dialog.SetYesNoLabels("Mark && Resolve", "Ignore");
		dialog.SetExtendedMessage(
			"Make sure you don't have any duplicates and that you're respecting the naming conventions."
		);

		int answer = dialog.ShowModal();

		if (answer == wxID_YES)
		{
			m_TextCtrl->MarkerDeleteAll(wxSTC_MARK_CIRCLE);

			for (int i = 0; i < indexInvalid.size(); i++)
			{
				m_TextCtrl->MarkerAdd(indexInvalid[i], wxSTC_MARK_CIRCLE);
			}
			m_MenuBar->Enable(Ids::ID_MARK_NEXT_STATES, true);
			m_MenuBar->Enable(Ids::ID_MARK_PREV_STATES, true);

			m_InvalidInput = true;
			return {};
		}
		if (answer == wxID_NO)
		{
			m_InvalidInput = false;
			return states;
		}
		if (answer == wxID_CANCEL)
		{
			m_InvalidInput = true;
			return {};
		}
	}

	m_InvalidInput = false;
	return states;
}

void EditorStates::GoTo(std::string state)
{
	int result = m_TextCtrl->SearchNext(wxSTC_FIND_WHOLEWORD, state);

	// not found
	if (result == -1)
	{
		wxMessageDialog* dialog = new wxMessageDialog(
			this, "No occurence found.", "Go To",
			wxOK | wxICON_INFORMATION
		);

		int answer = dialog->ShowModal();

		return;
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

	if (result != -1)
	{
		m_TextCtrl->LineDelete();
		m_PrevText = m_TextCtrl->GetText();
	}
}

void EditorStates::ForceClose()
{
	m_ForceClose = true;

	Close();
}

void EditorStates::BuildMenuBar()
{
	wxMenu* menuFile = new wxMenu();
	wxMenu* menuEdit = new wxMenu();

	menuFile->Append(Ids::ID_IMPORT_STATES, "&Import\tCtrl-I");
	menuFile->Append(Ids::ID_EXPORT_STATES, "Ex&port\tCtrl-P");
	menuFile->AppendSeparator();
	menuFile->Append(Ids::ID_SAVE_STATES, "&Save\tCtrl-S");
	menuFile->Append(Ids::ID_SAVE_CLOSE_STATES, "Sa&ve && Close\tCtrl-Shift-S");
	menuFile->AppendSeparator();
	menuFile->Append(Ids::ID_CLOSE_STATES, "&Close\tAlt-F4");
	menuEdit->Append(Ids::ID_FIND_STATES, "&Find\tCtrl-F");
	menuEdit->Append(Ids::ID_REPLACE_STATES, "&Replace\tCtrl-H");
	menuEdit->AppendSeparator();
	menuEdit->Append(Ids::ID_MARK_NEXT_STATES, "&Next Mark\tCtrl-E");
	menuEdit->Append(Ids::ID_MARK_PREV_STATES, "&Previous Mark\tCtrl-Q");
	menuEdit->AppendSeparator();
	menuEdit->Append(Ids::ID_FORMAT_STATES, "Forma&t\tCtrl-T");

	m_MenuBar = new wxMenuBar();
	m_MenuBar->Append(menuFile, "&File");
	m_MenuBar->Append(menuEdit, "&Edit");

	m_MenuBar->Enable(Ids::ID_MARK_NEXT_STATES, false);
	m_MenuBar->Enable(Ids::ID_MARK_PREV_STATES, false);

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
	Bind(wxEVT_COMMAND_MENU_SELECTED, &EditorStates::OnFormat, this, Ids::ID_FORMAT_STATES);
}

void EditorStates::BuildInputPanel()
{
	std::string labelHelp = "so basically yeah, plz no more than 256 states lol ok ? thx.";
	wxStaticText* help = new wxStaticText(this, wxID_ANY, labelHelp);

	m_TextCtrl = new wxStyledTextCtrl(this);
	m_TextCtrl->SetMarginWidth(wxSTC_MARGIN_NUMBER, 80);
	m_TextCtrl->SetMarginType(wxSTC_MARGINOPTION_SUBLINESELECT, wxSTC_MARGIN_NUMBER);
	m_TextCtrl->SetScrollWidth(1);
	m_TextCtrl->MarkerSetBackground(wxSTC_MARK_CIRCLE, wxColour("red"));

	wxFont font = wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false);
	m_TextCtrl->StyleSetFont(wxSTC_STYLE_DEFAULT, font);

	wxPanel* panelButtons = new wxPanel(this);
	wxButton* save = new wxButton(panelButtons, Ids::ID_SAVE_STATES, wxString("Save"));
	wxButton* saveClose = new wxButton(panelButtons, Ids::ID_SAVE_CLOSE_STATES, wxString("Save && Close"));

	save->Bind(wxEVT_BUTTON, &EditorStates::OnSave, this);
	saveClose->Bind(wxEVT_BUTTON, &EditorStates::OnSaveClose, this);

	wxBoxSizer* sizerButtons = new wxBoxSizer(wxHORIZONTAL);
	panelButtons->SetSizerAndFit(new wxBoxSizer(wxHORIZONTAL));
	sizerButtons->Add(save, 0);
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
	if (m_ForceClose)
	{
		evt.Skip();
		return;
	}

	// changes unsaved -> show dialog
	if (m_PrevText != m_TextCtrl->GetText())
	{
		wxMessageDialog dialog(
			this, "Do you want to save the changes?", "Save",
			wxYES_NO | wxCANCEL | wxICON_INFORMATION
		);

		//dialog.ShowWindowModal();

		int answer = dialog.ShowModal();

	/*	dlg = new wxMessageDialog(this, "save boss?", "save", wxYES_NO | wxCANCEL | wxICON_INFORMATION);
		dlg->ShowWindowModal();
		int answer = dlg->GetReturnCode();*/

		if (answer == wxID_YES)
		{
			CloseEditor(true);
			return;
		}
		if (answer == wxID_NO)
		{
			CloseEditor(false);
			return;
		}
		if (answer == wxID_CANCEL) return;
	}
	else CloseEditor(false);
}

void EditorStates::OnFocusEvent(wxFocusEvent& evt)
{
	m_TextCtrl->SetFocus();
}

void EditorStates::OnClose(wxCommandEvent& evt)
{
	// changes unsaved -> show dialog
	if (m_PrevText != m_TextCtrl->GetText())
	{
		wxMessageDialog dialog(
			this, "Do you want to save the changes?", "Save",
			wxYES_NO | wxCANCEL | wxICON_INFORMATION
		);

		int answer = dialog.ShowModal();

		if (answer == wxID_YES)
		{
			CloseEditor(true);
			return;
		}
		if (answer == wxID_NO)
		{
			CloseEditor(false);
			return;
		}
		if (answer == wxID_CANCEL) return;
	}
	else CloseEditor(false);
}

void EditorStates::OnSave(wxCommandEvent& evt)
{
	std::vector<std::string> data = GetData();
	if (m_InvalidInput) return;

	m_TextCtrl->MarkerDeleteAll(wxSTC_MARK_CIRCLE);
	m_MenuBar->Enable(Ids::ID_MARK_NEXT_STATES, false);
	m_MenuBar->Enable(Ids::ID_MARK_PREV_STATES, false);

	m_PrevText = m_TextCtrl->GetText();
	m_InputStates->SetStates(data);
}

void EditorStates::OnSaveClose(wxCommandEvent& evt)
{
	CloseEditor(true);
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
	int result;

	if (flags & wxFR_DOWN) result = m_TextCtrl->FindText(0, m_TextCtrl->GetLastPosition(), find, flags);
	else result = m_TextCtrl->FindText(m_TextCtrl->GetLastPosition(), 0, find, flags);

	if (result == -1)
	{
		if (m_DialogShown) return;

		wxMessageDialog dialog(
			this, "No occurences found.", "Find",
			wxOK | wxICON_INFORMATION
		);

		m_DialogShown = true;
		int answer = dialog.ShowModal();
		m_DialogShown = false;

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
		if (m_DialogShown) return;

		wxMessageDialog dialog(
			this, "No more occurences found.", "Find",
			wxOK | wxICON_INFORMATION
		);

		m_DialogShown = true;
		int answer = dialog.ShowModal();
		m_DialogShown = false;

		return;
	}

	m_TextCtrl->ShowPosition(result);
	m_TextCtrl->SetSelection(result, result + find.size());
}

void EditorStates::OnReplace(wxFindDialogEvent& evt)
{
	wxString replace = m_FindData->GetReplaceString();
	wxString find = m_FindData->GetFindString();

	if (m_TextCtrl->GetSelectedText() != find) return;

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

	std::string message = std::to_string(occurences);
	message += (occurences == 1) ? " occurence has been replaced." : " occurences have been replaced.";

	if (m_DialogShown) return;
	wxMessageDialog dialog(
		this, message, "Replace All",
		wxOK | wxICON_INFORMATION
	);

	m_DialogShown = true;
	int answer = dialog.ShowModal();
	m_DialogShown = false;
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
	m_MarkLine = line;

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
	m_MarkLine = line;

	int position = m_TextCtrl->PositionFromLine(line);
	m_TextCtrl->ShowPosition(position);

	m_TextCtrl->SetSelectionMode(wxSTC_SEL_LINES);
	m_TextCtrl->SetSelection(position, position);
	m_TextCtrl->SetSelectionMode(wxSTC_SEL_STREAM);
}

void EditorStates::OnImport(wxCommandEvent& evt)
{
}

void EditorStates::OnExport(wxCommandEvent& evt)
{
}

void EditorStates::CloseEditor(bool save)
{
	wxDELETE(m_FindData); wxDELETE(m_FindDialog);

	if (save)
	{
		std::vector<std::string> data = GetData();
		if (m_InvalidInput) return;

		m_TextCtrl->MarkerDeleteAll(wxSTC_MARK_CIRCLE);
		m_MenuBar->Enable(Ids::ID_MARK_NEXT_STATES, false);
		m_MenuBar->Enable(Ids::ID_MARK_PREV_STATES, false);

		m_PrevText = m_TextCtrl->GetText();
		m_InputStates->SetStates(data);
	}
	else m_TextCtrl->SetText(m_PrevText);

	Hide();
}
