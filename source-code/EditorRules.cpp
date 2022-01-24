#include "EditorRules.h"
#include "Interpreter.h"

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

void EditorRules::SetInputRules(InputRules* inputRules)
{
	m_InputRules = inputRules;
}

std::vector<std::pair<std::string, Transition>> EditorRules::GetData()
{
	std::string text = (std::string)m_TextCtrl->GetText().Upper();

	Interpreter interpreter;

	std::vector<std::pair<int, std::string>> invalidPositions = interpreter.Process(text);
	vector<pair<string, Transition>> transitions = interpreter.GetTransitions();

	if (invalidPositions.empty())
	{
		m_InvalidInput = false;
		return transitions;
	}

	if (invalidPositions[0].first == -1)
	{
		wxMessageDialog dialog(
			this, "The maximum allowed number of rules has been surpassed.\nMake sure you're within the given limit before saving.", "Error",
			wxOK | wxICON_ERROR
		);
		dialog.SetExtendedMessage(
			"Limit: " + std::to_string(Sizes::RULES_MAX) + "\nCurrent number: " + std::to_string(transitions.size())
		);
		
		int answer = dialog.ShowModal();

		m_InvalidInput = true;
		return {};
	}
	
	wxMessageDialog dialog(
		this, "Some of the rules appear to be invalid.", "Warning",
		wxYES_NO | wxCANCEL | wxICON_EXCLAMATION
	);
	dialog.SetYesNoLabels("Mark && Resolve", "Ignore");
	/*dialog.SetExtendedMessage(
		"Make sure you don't have any duplicates and that you're respecting the naming conventions."
	);*/

	std::string extendedMessage = "";
	for (auto& it : invalidPositions)
	{
		int ncol = it.first;
		int nline = -1;
		int cnt = 0;

		//wxLogDebug("pos=%i", ncol);

		// map to real position (line, col)
		for (int i = 0; i < m_TextCtrl->GetLineCount(); i++)
		{
			// count linefeed whitespace
			wxString line = m_TextCtrl->GetLine(i);
			/*int lf = (line.size() > 0 && line[line.size() - 1] == '\n') ? 1 : 0;
			int cr = (line.size() > 1 && line[line.size() - 2] == '\r') ? 1 : 0;
			int ws = lf + cr;*/

			int ws = 0; // nvm, i dont need it

			cnt += (line.size() - ws);

			//wxLogDebug("LINE=%i SIZE=%i WS=%i CNT=%i", i, line.size(), ws, cnt);
			if (cnt >= ncol)
			{
				nline = i;
				ncol = (line.size() - ws) - (cnt - ncol);
				break;
			}
		}

		//wxLogDebug("cnt=%i, ncol=%i", cnt,ncol);

		std::string line = std::to_string(nline + 1);
		std::string col = std::to_string(ncol);

		extendedMessage += it.second + " at line " + line + ", after column " + col + "\n";
		//extendedMessage += it.second + " Line=" + line + ", Column=" + col + "\n";

		it.first = nline;
	}
	dialog.SetExtendedMessage(extendedMessage);

	int answer = dialog.ShowModal();

	if (answer == wxID_YES)
	{
		m_TextCtrl->MarkerDeleteAll(wxSTC_MARK_CIRCLE);
		m_TextCtrl->Refresh(false);

		for (int i = 0; i < invalidPositions.size(); i++)
		{
			int line = invalidPositions[i].first;
			m_TextCtrl->MarkerAdd(line, wxSTC_MARK_CIRCLE);
		}
		m_MenuBar->Enable(Ids::ID_MARK_NEXT_RULES, true);
		m_MenuBar->Enable(Ids::ID_MARK_PREV_RULES, true);

		m_InvalidInput = true;
		return {};
	}
	if (answer == wxID_NO)
	{
		m_InvalidInput = false;
		return transitions;
	}
	if (answer == wxID_CANCEL)
	{
		m_InvalidInput = true;
		return {};
	}

	return {};
}

void EditorRules::GoTo(std::string rule)
{
	std::pair<int, int> lines = FindRule(rule);
	int lineBegin = lines.first;
	int lineEnd = lines.second;

	// not found
	if (lineBegin == -1)
	{
		if (m_DialogShown) return;
		wxMessageDialog dialog (
			this, "No occurence found.", "Go To",
			wxOK | wxICON_INFORMATION
		);

		m_DialogShown = true;
		int answer = dialog.ShowModal();
		m_DialogShown = false;

		return;
	}

	int positionBegin = m_TextCtrl->PositionFromLine(lineBegin);
	int positionEnd = m_TextCtrl->GetLineEndPosition(lineEnd);
	m_TextCtrl->ShowPosition(positionBegin);

	m_TextCtrl->SetSelection(positionBegin, positionEnd);

	Show();
	SetFocus();
}

void EditorRules::DeleteRule(std::string rule)
{	
	std::pair<int, int> lines = FindRule(rule);
	int lineBegin = lines.first;
	int lineEnd = lines.second;

	if (lineBegin != -1)
	{
		int positionBegin = m_TextCtrl->PositionFromLine(lineBegin);
		int positionEnd = m_TextCtrl->GetLineEndPosition(lineEnd);
		m_TextCtrl->ShowPosition(positionBegin);

		m_TextCtrl->SetSelection(positionBegin, positionEnd);

		m_TextCtrl->DeleteBack();
		m_PrevText = m_TextCtrl->GetText();
	}
}

void EditorRules::ForceClose()
{
	m_ForceClose = true;

	//Close();
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

	m_TextCtrl->Bind(wxEVT_KEY_UP, &EditorRules::UpdateLineColKey, this);
	m_TextCtrl->Bind(wxEVT_LEFT_UP, &EditorRules::UpdateLineColMouse, this);

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

	CreateStatusBar(); GetStatusBar()->SetStatusText("Line=1\tColumn=1");

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

void EditorRules::OnFocusEvent(wxFocusEvent& evt)
{
	m_TextCtrl->SetFocus();
}

void EditorRules::OnClose(wxCommandEvent& evt)
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

void EditorRules::OnSave(wxCommandEvent& evt)
{
	std::vector<std::pair<std::string, Transition>> data = GetData();
	if (m_InvalidInput) return;

	m_TextCtrl->MarkerDeleteAll(wxSTC_MARK_CIRCLE);
	m_TextCtrl->Refresh(false);
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

void EditorRules::OnFindNext(wxFindDialogEvent& evt)
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

	if (m_DialogShown) return;
	wxMessageDialog dialog(
		this, message, "Replace All",
		wxOK | wxICON_INFORMATION
	);

	m_DialogShown = true;
	int answer = dialog.ShowModal();
	m_DialogShown = false;
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

	if (m_ForceClose)
	{
		Close();
		return;
	}
	
	if (save)
	{
		std::vector<std::pair<std::string, Transition>> data = GetData();
		if (m_InvalidInput) return;

		m_TextCtrl->MarkerDeleteAll(wxSTC_MARK_CIRCLE);
		m_TextCtrl->Refresh(false);
		m_MenuBar->Enable(Ids::ID_MARK_NEXT_RULES, false);
		m_MenuBar->Enable(Ids::ID_MARK_PREV_RULES, false);

		m_PrevText = m_TextCtrl->GetText();
		m_InputRules->SetRules(data);
	}
	else m_TextCtrl->SetText(m_PrevText);

	Hide();
}

void EditorRules::UpdateLineColMouse(wxMouseEvent& evt)
{
	long line = 0;
	long col = 0;
	long pos = m_TextCtrl->GetInsertionPoint();

	//if (pos == m_TextCtrl->GetLastPosition()) pos = (pos - 1 > 1) ? pos - 1 : 1;

	if (!m_TextCtrl->PositionToXY(pos, &col, &line))
	{
		line = m_TextCtrl->GetLineCount() - 1;
		col = m_TextCtrl->GetLine(line).size() - m_TextCtrl->GetWhitespaceSize() + 1;
	}

	std::string message = "Line=" + std::to_string(line+1) + "\tColumn=" + std::to_string(col+1);

	GetStatusBar()->SetStatusText(message);

	evt.Skip();
}

void EditorRules::UpdateLineColKey(wxKeyEvent& evt)
{
	long line = 0;
	long col = 0;
	int pos = m_TextCtrl->GetInsertionPoint();

	//if (pos == m_TextCtrl->GetLastPosition()) pos = (pos - 1 > 1) ? pos - 1 : 1;

	if (!m_TextCtrl->PositionToXY(pos, &col, &line))
	{
		line = m_TextCtrl->GetLineCount() - 1;
		col = m_TextCtrl->GetLine(line).size() - m_TextCtrl->GetWhitespaceSize() + 1;
	}

	std::string message = "Line=" + std::to_string(line+1) + "\tColumn=" + std::to_string(col+1);

	GetStatusBar()->SetStatusText(message);

	evt.Skip();
}

std::pair<int, int> EditorRules::FindRule(std::string rule)
{
	int lineBegin = 0;
	int n = rule.size();
	int cnt = 0;
	std::string s = "";

	for (int i = 0; i < m_TextCtrl->GetLineCount(); i++)
	{
		if (cnt == 0) lineBegin = i;

		wxString line = m_TextCtrl->GetLine(i);

		// inline comment, ignore it but continue with the state before it
		if (line.find("!") != line.npos) line = line.substr(0, line.find("!"));

		if (line.empty()) continue;
		line.MakeUpper();

		//wxLogDebug("line=<%s>", line);
		
		bool substr = true;
		// see if the line is a substring of the given rule
		for (int j = 0; j < line.size(); j++)
		{
			if (std::isspace(line[j])) continue;

			if (cnt == n)
			{
				substr = false;
				break;
			}

			if (line[j] == rule[cnt])
			{
				cnt++;
				s.push_back(line[j]);
			}
			else
			{
				substr = false;
				break;
			}
		}

		//wxLogDebug("s=<%s> substr=%i pos=%i,%i", s, substr, lineBegin, i);

		if (!substr)
		{
			s = "";
			cnt = 0;
			lineBegin = i + 1;
		}
		// we found stacking lines that produce the given rule
		else if (cnt == n) return { lineBegin, i };
	}

	return { -1,-1 };
}

