#include "EditorRules.h"

#include <algorithm>
#include <sstream>
#include <unordered_set>

wxBEGIN_EVENT_TABLE(EditorRules, wxFrame)
	EVT_CLOSE(EditorRules::OnCloseEvent)
	EVT_SET_FOCUS(EditorRules::OnFocusEvent)
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
	wxDELETE(m_FindData);
	wxDELETE(m_FindDialog);
}

void EditorRules::SetStates(std::unordered_map<std::string, std::string>& states)
{
	m_Interpreter.SetStates(states);
}

void EditorRules::SetNeighbors(std::unordered_set<std::string>& neighbors)
{
	m_Interpreter.SetNeighbors(neighbors);
}

void EditorRules::SetInputRules(InputRules* inputRules)
{
	m_InputRules = inputRules;
}

std::vector<std::string> EditorRules::GetData()
{
	std::string text = (std::string)m_TextCtrl->GetText().Upper();

	wxLogDebug("[START]");
	m_Interpreter.Process(text);
	wxLogDebug("[END]");

	// remove empty lines, white spaces and carriage symbols
	text.erase(remove(text.begin(), text.end(), ' '), text.end());
	text.erase(remove(text.begin(), text.end(), '\t'), text.end());
	text.erase(remove(text.begin(), text.end(), '\r'), text.end());
	text.erase(std::unique(text.begin(), text.end(), [](char a, char b) {return a == '\n' && b == '\n'; }), text.end());

	// count lines (rules) and mark duplicates/invalid rules
	std::unordered_set<std::string> setRules;
	std::vector<int> indexInvalid;

	std::stringstream ssText(text);
	std::string rule;
	std::vector<std::string> rules;

	int cntLine = -1;
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

		// duplicate found
		if (setRules.find(rule) != setRules.end())
		{
			indexInvalid.push_back(cntLine);
			continue;
		}
		else
		{
			setRules.insert(rule);
		}

		rules.push_back(rule);
	}

	if (rules.size() > Sizes::RULES_MAX)
	{
		wxMessageDialog* dialog = new wxMessageDialog(
			this, "The maximum allowed number of rules has been surpassed.\nMake sure you're within the given limit before saving.", "Error",
			wxOK | wxICON_ERROR
		);
		dialog->SetExtendedMessage(
			"Limit: " + std::to_string(Sizes::RULES_MAX) + "\nCurrent number: " + std::to_string(rules.size())
		);
		int answer = dialog->ShowModal();

		m_InvalidInput = true;
		return {};
	}
	else if (indexInvalid.size())
	{
		wxMessageDialog* dialog = new wxMessageDialog(
			this, "Some of the rules appear to be invalid.", "Warning",
			wxYES_NO | wxCANCEL | wxICON_EXCLAMATION
		);
		dialog->SetYesNoLabels("Mark && Resolve", "Ignore");
		dialog->SetExtendedMessage(
			"Make sure you don't have any duplicates and that you're respecting the naming conventions."
		);
		int answer = dialog->ShowModal();

		if (answer == wxID_YES)
		{
			m_TextCtrl->MarkerDeleteAll(wxSTC_MARK_CIRCLE);

			for (int i = 0; i < indexInvalid.size(); i++)
			{
				m_TextCtrl->MarkerAdd(indexInvalid[i], wxSTC_MARK_CIRCLE);
			}
			m_MenuBar->Enable(Ids::ID_MARK_NEXT_RULES, true);
			m_MenuBar->Enable(Ids::ID_MARK_PREV_RULES, true);

			m_InvalidInput = true;
			return {};
		}
		if (answer == wxID_NO)
		{
			m_InvalidInput = false;
			return rules;
		}
		if (answer == wxID_CANCEL)
		{
			m_InvalidInput = true;
			return {};
		}
	}
	
	m_InvalidInput = false;
	return rules;
}

void EditorRules::GoTo(std::string rule)
{
	int result = FindRule(rule);

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

	int position = m_TextCtrl->PositionFromLine(result);
	m_TextCtrl->ShowPosition(position);

	m_TextCtrl->SetSelectionMode(wxSTC_SEL_LINES);
	m_TextCtrl->SetSelection(position, position);
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
		m_PrevText = m_TextCtrl->GetText();
	}
}

void EditorRules::BuildMenuBar()
{
	wxMenu* menuFile = new wxMenu();
	wxMenu* menuEdit = new wxMenu();

	menuFile->Append(Ids::ID_IMPORT_RULES, "&Import\tCtrl-I");
	menuFile->Append(Ids::ID_EXPORT_RULES, "Ex&port\tCtrl-P");
	menuFile->AppendSeparator();
	menuFile->Append(Ids::ID_SAVE_RULES, "&Save\tCtrl-S");
	menuFile->Append(Ids::ID_SAVE_CLOSE_RULES, "Sa&ve && Close\tCtrl-Shift-S");
	menuFile->AppendSeparator();
	menuFile->Append(Ids::ID_CLOSE_RULES, "&Close\tAlt-F4");
	menuEdit->Append(Ids::ID_FIND_RULES, "&Find\tCtrl-F");
	menuEdit->Append(Ids::ID_REPLACE_RULES, "&Replace\tCtrl-H");
	menuEdit->AppendSeparator();
	menuEdit->Append(Ids::ID_MARK_NEXT_RULES, "&Next Mark\tCtrl-E");
	menuEdit->Append(Ids::ID_MARK_PREV_RULES, "&Previous Mark\tCtrl-Q");
	menuEdit->AppendSeparator();
	menuEdit->Append(Ids::ID_FORMAT_RULES, "Forma&t\tCtrl-T");

	m_MenuBar = new wxMenuBar();
	m_MenuBar->Append(menuFile, "&File");
	m_MenuBar->Append(menuEdit, "&Edit");

	m_MenuBar->Enable(Ids::ID_MARK_NEXT_RULES, false);
	m_MenuBar->Enable(Ids::ID_MARK_PREV_RULES, false);

	this->SetMenuBar(m_MenuBar);

	Bind(wxEVT_COMMAND_MENU_SELECTED, &EditorRules::OnImport, this, Ids::ID_IMPORT_RULES);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &EditorRules::OnExport, this, Ids::ID_EXPORT_RULES);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &EditorRules::OnClose, this, Ids::ID_CLOSE_RULES);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &EditorRules::OnSave, this, Ids::ID_SAVE_RULES);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &EditorRules::OnSaveClose, this, Ids::ID_SAVE_CLOSE_RULES);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &EditorRules::OnMenuFind, this, Ids::ID_FIND_RULES);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &EditorRules::OnMenuReplace, this, Ids::ID_REPLACE_RULES);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &EditorRules::OnPrevMark, this, Ids::ID_MARK_PREV_RULES);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &EditorRules::OnNextMark, this, Ids::ID_MARK_NEXT_RULES);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &EditorRules::OnFormat, this, Ids::ID_FORMAT_RULES);
}

void EditorRules::BuildInputPanel()
{
	std::string labelHelp = "so basically yeah, plz no more than 256 Rules lol ok ? thx.";
	wxStaticText* help = new wxStaticText(this, wxID_ANY, labelHelp);

	m_TextCtrl = new wxStyledTextCtrl(this);
	m_TextCtrl->SetMarginWidth(wxSTC_MARGIN_NUMBER, 80);
	m_TextCtrl->SetMarginType(wxSTC_MARGINOPTION_SUBLINESELECT, wxSTC_MARGIN_NUMBER);
	m_TextCtrl->SetScrollWidth(1);
	m_TextCtrl->MarkerSetBackground(wxSTC_MARK_CIRCLE, wxColour("red"));

	wxFont font = wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false);
	m_TextCtrl->StyleSetFont(wxSTC_STYLE_DEFAULT, font);

	wxPanel* panelButtons = new wxPanel(this);
	wxButton* save = new wxButton(panelButtons, Ids::ID_SAVE_RULES, wxString("Save"));
	wxButton* saveClose = new wxButton(panelButtons, Ids::ID_SAVE_CLOSE_RULES, wxString("Save && Close"));

	save->Bind(wxEVT_BUTTON, &EditorRules::OnSave, this);
	saveClose->Bind(wxEVT_BUTTON, &EditorRules::OnSaveClose, this);

	wxBoxSizer* sizerButtons = new wxBoxSizer(wxHORIZONTAL);
	panelButtons->SetSizerAndFit(new wxBoxSizer(wxHORIZONTAL));
	sizerButtons->Add(save, 0, wxLEFT, 6);
	sizerButtons->Add(saveClose, 0);

	panelButtons->SetSizerAndFit(sizerButtons);

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(help, 0, wxALL, 6);
	sizer->Add(m_TextCtrl, 1, wxEXPAND);
	sizer->Add(panelButtons, 0, wxALIGN_RIGHT | wxALL, 6);

	this->SetSizer(sizer);
}

void EditorRules::BuildDialogFind(std::string title, long style)
{
	wxDELETE(m_FindDialog);
	wxDELETE(m_FindData);

	m_FindData = new wxFindReplaceData(wxFR_DOWN);
	m_FindDialog = new wxFindReplaceDialog(this, m_FindData, title, style);

	m_FindDialog->Bind(wxEVT_FIND, &EditorRules::OnFind, this);
	m_FindDialog->Bind(wxEVT_FIND_NEXT, &EditorRules::OnFindNext, this);
	m_FindDialog->Bind(wxEVT_FIND_REPLACE, &EditorRules::OnReplace, this);
	m_FindDialog->Bind(wxEVT_FIND_REPLACE_ALL, &EditorRules::OnReplaceAll, this);
}

void EditorRules::OnCloseEvent(wxCloseEvent& evt)
{
	// changes unsaved -> show dialog
	if (m_PrevText != m_TextCtrl->GetText())
	{
		wxMessageDialog* dialog = new wxMessageDialog(
			this, "Do you want to save the changes?", "Save",
			wxYES_NO | wxCANCEL | wxICON_INFORMATION
		);
		int answer = dialog->ShowModal();

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

void EditorRules::OnFocusEvent(wxFocusEvent& evt)
{
	m_TextCtrl->SetFocus();
}

void EditorRules::OnClose(wxCommandEvent& evt)
{
	// changes unsaved -> show dialog
	if (m_PrevText != m_TextCtrl->GetText())
	{
		wxMessageDialog* dialog = new wxMessageDialog(
			this, "Do you want to save the changes?", "Save",
			wxYES_NO | wxCANCEL | wxICON_INFORMATION
		);
		int answer = dialog->ShowModal();

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

void EditorRules::OnSave(wxCommandEvent& evt)
{
	std::vector<std::string> data = GetData();
	if (m_InvalidInput) return;

	m_TextCtrl->MarkerDeleteAll(wxSTC_MARK_CIRCLE);
	m_MenuBar->Enable(Ids::ID_MARK_NEXT_RULES, false);
	m_MenuBar->Enable(Ids::ID_MARK_PREV_RULES, false);

	m_PrevText = m_TextCtrl->GetText();
	m_InputRules->SetRules(data);
}

void EditorRules::OnSaveClose(wxCommandEvent& evt)
{
	CloseEditor(true);
}

void EditorRules::OnMenuFind(wxCommandEvent& evt)
{
	BuildDialogFind("Find", 0);

	m_FindDialog->Show();
}

void EditorRules::OnMenuReplace(wxCommandEvent& evt)
{
	BuildDialogFind("Find & Replace", wxFR_REPLACEDIALOG);

	m_FindDialog->Show();
}

void EditorRules::OnFind(wxFindDialogEvent& evt)
{
	wxString find = m_FindData->GetFindString();

	int flags = m_FindData->GetFlags();
	int result = m_TextCtrl->FindText(0, m_TextCtrl->GetLastPosition(), find, flags);

	if (result == -1)
	{
		wxMessageDialog* dialog = new wxMessageDialog(
			this, "No occurences found.", "Find",
			wxOK | wxICON_INFORMATION
		);
		int answer = dialog->ShowModal();
		return;
	}

	m_TextCtrl->ShowPosition(result);
	m_TextCtrl->SetSelection(result, result + find.size());
}

void EditorRules::OnFindNext(wxFindDialogEvent& evt)
{
	wxString find = m_FindData->GetFindString();

	int flags = m_FindData->GetFlags();
	int result;

	if (flags & wxFR_DOWN) result = m_TextCtrl->FindText(m_TextCtrl->GetAnchor(), m_TextCtrl->GetLastPosition(), find, flags);
	else result = m_TextCtrl->FindText(m_TextCtrl->GetAnchor(), 0, find, flags);

	if (result == -1)
	{
		wxMessageDialog* dialog = new wxMessageDialog(
			this, "No more occurences found.", "Find",
			wxOK | wxICON_INFORMATION
		);
		int answer = dialog->ShowModal();
		return;
	}

	m_TextCtrl->ShowPosition(result);
	m_TextCtrl->SetSelection(result, result + find.size());
}

void EditorRules::OnReplace(wxFindDialogEvent& evt)
{
	wxString replace = m_FindData->GetReplaceString();
	wxString find = m_FindData->GetFindString();

	if (m_TextCtrl->GetSelectedText() != find) return;

	m_TextCtrl->ReplaceSelection(replace);
}

void EditorRules::OnReplaceAll(wxFindDialogEvent& evt)
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
	wxMessageDialog* dialog = new wxMessageDialog(
		this, message, "Replace All",
		wxOK | wxICON_INFORMATION
	);
	int answer = dialog->ShowModal();
}

void EditorRules::OnFormat(wxCommandEvent& evt)
{
	std::string text = (std::string)m_TextCtrl->GetText().Upper();

	// remove empty lines, white spaces and carriage symbols
	text.erase(std::unique(text.begin(), text.end(), [](char a, char b) {return a == ' ' && b == ' '; }), text.end());
	text.erase(remove(text.begin(), text.end(), '\r'), text.end());
	text.erase(std::unique(text.begin(), text.end(), [](char a, char b) {return a == '\n' && b == '\n'; }), text.end());

	m_TextCtrl->SetText(text);
}

void EditorRules::OnPrevMark(wxCommandEvent& evt)
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

void EditorRules::OnNextMark(wxCommandEvent& evt)
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

void EditorRules::OnImport(wxCommandEvent& evt)
{
}

void EditorRules::OnExport(wxCommandEvent& evt)
{
}

void EditorRules::CloseEditor(bool save)
{
	wxDELETE(m_FindData); wxDELETE(m_FindDialog);
	
	if (save)
	{
		std::vector<std::string> data = GetData();
		if (m_InvalidInput) return;

		m_TextCtrl->MarkerDeleteAll(wxSTC_MARK_CIRCLE);
		m_MenuBar->Enable(Ids::ID_MARK_NEXT_RULES, false);
		m_MenuBar->Enable(Ids::ID_MARK_PREV_RULES, false);

		m_PrevText = m_TextCtrl->GetText();
		m_InputRules->SetRules(data);
	}
	else m_TextCtrl->SetText(m_PrevText);

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
