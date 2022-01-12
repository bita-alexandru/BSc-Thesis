#pragma once
#include "wx/wx.h"
#include "wx/stc/stc.h"
#include "wx/listctrl.h"
#include "wx/fdrepdlg.h"

#include "Ids.h"
#include "Sizes.h"
#include "InputRules.h"
#include "Interpreter.h"
#include "Transition.h"

class InputRules;

class EditorRules: public wxFrame
{
public:
	EditorRules(wxFrame* parent);
	~EditorRules();

	void SetStates(std::unordered_map<std::string, std::string>& states);
	void SetNeighbors(std::unordered_set<std::string>& neighbors);
	void SetInputRules(InputRules* inputRules);
	vector<pair<string, Transition>> GetData();

	void GoTo(std::string rule);
	void DeleteRule(std::string rule);
	void ForceClose();
private:
	InputRules* m_InputRules = nullptr;
	Interpreter m_Interpreter;

	wxMenuBar* m_MenuBar = nullptr;
	wxFindReplaceData* m_FindData = nullptr;
	wxStyledTextCtrl* m_TextCtrl = nullptr;
	wxFindReplaceDialog* m_FindDialog = nullptr;

	bool m_DialogShown = false;
	bool m_ForceClose = false;

	wxString m_PrevText;
	int m_MarkLine = -1;
	bool m_InvalidInput = false;

	void BuildMenuBar();
	void BuildInputPanel();
	void BuildDialogFind(std::string title, long style);
	
	int FindRule(std::string rule);

	wxDECLARE_EVENT_TABLE();
	void OnCloseEvent(wxCloseEvent& evt);
	void OnFocusEvent(wxFocusEvent& evt);

	void OnClose(wxCommandEvent& evt);
	void OnSave(wxCommandEvent& evt);
	void OnSaveClose(wxCommandEvent& evt);

	void OnMenuFind(wxCommandEvent& evt);
	void OnMenuReplace(wxCommandEvent& evt);

	void OnFind(wxFindDialogEvent& evt);
	void OnFindNext(wxFindDialogEvent& evt);
	void OnReplace(wxFindDialogEvent& evt);
	void OnReplaceAll(wxFindDialogEvent& evt);

	void OnFormat(wxCommandEvent& evt);
	void OnPrevMark(wxCommandEvent& evt);
	void OnNextMark(wxCommandEvent& evt);

	void OnImport(wxCommandEvent& evt);
	void OnExport(wxCommandEvent& evt);

	void CloseEditor(bool save = false);
};

