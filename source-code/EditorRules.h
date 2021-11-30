#pragma once
#include "wx/wx.h"
#include "wx/stc/stc.h"
#include "wx/listctrl.h"

#include <unordered_map>
#include <any>

#include "Ids.h"
#include "Sizes.h"

class EditorRules: public wxFrame
{
public:
	EditorRules(wxFrame* parent);
	~EditorRules();

	std::vector<std::unordered_map<std::string, std::any>> GetData();
private:
	wxStyledTextCtrl* m_TextCtrl = nullptr;

	void BuildMenuBar();
	void BuildInputPanel();
	void LoadData(wxListView* list);

	void OnClose(wxCloseEvent& evt);
	void OnShow(wxShowEvent& evt);

	wxDECLARE_EVENT_TABLE();
};

