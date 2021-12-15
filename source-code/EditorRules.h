#pragma once
#include "wx/wx.h"
#include "wx/stc/stc.h"
#include "wx/listctrl.h"

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

	wxStyledTextCtrl* m_TextCtrl = nullptr;

	void BuildMenuBar();
	void BuildInputPanel();

	wxDECLARE_EVENT_TABLE();
	void OnClose(wxCloseEvent& evt);
	void OnShow(wxShowEvent& evt);
	void OnSave(wxCommandEvent& evt);

	int FindRule(std::string rule);
};

