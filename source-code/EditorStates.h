#pragma once
#include "wx/wx.h"
#include "wx/stc/stc.h"
#include "wx/listctrl.h"
#include "wx/fdrepdlg.h"

#include "Ids.h"
#include "Sizes.h"
#include "InputStates.h"

class InputStates;

class EditorStates: public wxFrame
{
public:
	EditorStates(wxFrame* parent);
	~EditorStates();

	void SetInputStates(InputStates* inputStates);

	std::vector<std::string> GetData();
	void LoadData();

	void GoTo(std::string state);
	void DeleteState(std::string state);
private:
	InputStates* m_InputStates = nullptr;

	wxStyledTextCtrl* m_TextCtrl = nullptr;
	wxFindReplaceData* m_FindData = nullptr;
	wxFindReplaceDialog* m_FindDialog = nullptr;

	void BuildMenuBar();
	void BuildInputPanel();
	
	wxDECLARE_EVENT_TABLE();
	void OnClose(wxCloseEvent& evt);
	void OnShow(wxShowEvent& evt);
	void OnSave(wxCommandEvent& evt);
	void OnMenuFind(wxCommandEvent& evt);
	void OnMenuReplace(wxCommandEvent& evt);
	void OnFind(wxFindDialogEvent& evt);
	void OnFindNext(wxFindDialogEvent& evt);
	void OnReplace(wxFindDialogEvent& evt);
	void OnReplaceAll(wxFindDialogEvent& evt);
};

