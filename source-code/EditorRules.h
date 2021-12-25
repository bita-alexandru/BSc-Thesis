#pragma once
#include "wx/wx.h"
#include "wx/stc/stc.h"
#include "wx/listctrl.h"
#include "wx/fdrepdlg.h"

#include "Ids.h"
#include "Sizes.h"
#include "InputRules.h"

class InputRules;

class EditorRules: public wxFrame
{
public:
	EditorRules(wxFrame* parent);
	~EditorRules();

	void SetInputRules(InputRules* inputRules);
	std::vector<std::string> GetData();

	void GoTo(std::string rule);
	void DeleteRule(std::string rule);
private:
	InputRules* m_InputRules = nullptr;

	wxMenuBar* m_MenuBar = nullptr;
	wxFindReplaceData* m_FindData = nullptr;
	wxStyledTextCtrl* m_TextCtrl = nullptr;
	wxFindReplaceDialog* m_FindDialog = nullptr;

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

